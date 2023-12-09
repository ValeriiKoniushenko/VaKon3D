#include "editorwindow.h"

#include "./ui_editorwindow.h"
#include "EditorIntegration.h"
#include "Wsa.h"

#include <fstream>
#include <iostream>

EditorWindow::EditorWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::EditorWindow)
{
	ui->setupUi(this);
	fillUpTabTree();
	ui->treeWidget->expandAll();
	deserializeConsoleCommands();

	Wsa::instance().initialize(1, 1);
	onConnectToServer(false);

	connect(ui->treeWidget, &QTreeWidget::itemClicked, this, &EditorWindow::onTabClicked);
	connect(ui->pushButtonConnect, &QPushButton::clicked, this, &EditorWindow::onConnectToServer);
	connect(ui->lineEditConsole, &QLineEdit::returnPressed, this, &EditorWindow::onEnterDataToConsole);
	connect(ui->pushButtonConsoleEnter, &QPushButton::pressed, this, &EditorWindow::onEnterDataToConsole);
}

EditorWindow::~EditorWindow()
{
	delete ui;
}

void EditorWindow::fillUpTabTree()
{
	ui->treeWidget->setHeaderLabel(Tabs::title);

	{
		QTreeWidgetItem* topLevel = new QTreeWidgetItem();
		topLevel->setText(0, Tabs::General::title);
		ui->treeWidget->addTopLevelItem(topLevel);
	}
	{
		QTreeWidgetItem* topLevel = new QTreeWidgetItem();
		topLevel->setText(0, Tabs::GameProcess::title);

		for (const char* tab : Tabs::GameProcess::tabs)
		{
			QTreeWidgetItem* item = new QTreeWidgetItem();
			item->setText(0, tab);
			topLevel->addChild(item);
		}

		ui->treeWidget->addTopLevelItem(topLevel);
	}
	{
		QTreeWidgetItem* topLevel = new QTreeWidgetItem();
		topLevel->setText(0, Tabs::Scene::title);

		for (const char* tab : Tabs::Scene::tabs)
		{
			QTreeWidgetItem* item = new QTreeWidgetItem();
			item->setText(0, tab);
			topLevel->addChild(item);
		}
		ui->treeWidget->addTopLevelItem(topLevel);
	}
	{
		QTreeWidgetItem* topLevel = new QTreeWidgetItem();
		topLevel->setText(0, Tabs::Shaders::title);

		for (const char* tab : Tabs::Shaders::tabs)
		{
			QTreeWidgetItem* item = new QTreeWidgetItem();
			item->setText(0, tab);
			topLevel->addChild(item);
		}

		ui->treeWidget->addTopLevelItem(topLevel);
	}
	{
		QTreeWidgetItem* topLevel = new QTreeWidgetItem();
		topLevel->setText(0, Tabs::Console::title);
		ui->treeWidget->addTopLevelItem(topLevel);
	}
	{
		QTreeWidgetItem* topLevel = new QTreeWidgetItem();
		topLevel->setText(0, Tabs::Preferences::title);
		ui->treeWidget->addTopLevelItem(topLevel);
	}
}

void EditorWindow::onTabClicked(QTreeWidgetItem* item, int column)
{
	if (item->text(column) == Tabs::Console::title)
	{
		int i = ui->stackedWidget->count();
		ui->stackedWidget->setCurrentIndex(0);
	}
	else if (item->text(column) == Tabs::General::title)
	{
		int i = ui->stackedWidget->count();
		ui->stackedWidget->setCurrentIndex(1);
	}
}

void EditorWindow::onConnectToServer(bool checked)
{
	if (!isConnected)
	{
		serverSocket.open(AddressFamily::Inet);
		serverSocket.bind(SocketAddress(ui->lineEditIp->text().toStdString(), ui->lineEditPort->text().toInt()));
		serverSocket.listen();
		acceptedClient = serverSocket.accept();

		ui->pushButtonConnect->setText("Disconnect");
		isConnected = true;
	}
	else
	{
		serverSocket.close();
		ui->pushButtonConnect->setText("Accept");
		acceptedClient = {};
		isConnected = false;
	}
}

void EditorWindow::onEnterDataToConsole()
{
	if (!isConnected)
	{
		ui->plainTextEditConsole->appendPlainText("The connection wasn't established");
		return;
	}

	executeConsoleCommand(ui->lineEditConsole->text());
}

void EditorWindow::executeConsoleCommand(const QString& command)
{
	if (command == "cls")
	{
		ui->plainTextEditConsole->clear();
		ui->lineEditConsole->clear();
		return;
	}

	{
		EditorNetworkProtocol::Body body;
		body.content = command.toStdString();
		body.action = "console";
		const auto bodyString = EditorNetworkProtocol::Body::generate(body);
		const auto headerString = EditorNetworkProtocol::Header::generate(bodyString.size());

		std::vector<char> headerData(headerString.begin(), headerString.end());
		headerData.resize(EditorNetworkProtocol::Header::length);

		acceptedClient.send(headerData);
		acceptedClient.send(bodyString);
	}

	addConsoleCommandToScreen(command, false);
	ui->lineEditConsole->clear();

	{
		auto header = nlohmann::json::parse(acceptedClient.receiveAsString(EditorNetworkProtocol::Header::length));

		const std::size_t bodyLength = header[EditorNetworkProtocol::Header::lengthPropertyName];
		auto body = nlohmann::json::parse(acceptedClient.receiveAsString(bodyLength + 1ull));

		if (body[EditorNetworkProtocol::Body::possibleActionsPropertyName] == "response-to-console")
		{
			addConsoleCommandToScreen(body[EditorNetworkProtocol::Body::contentPropertyName].get<std::string*>()->c_str(), true);
		}
	}

	commands_.push_back(command);
}

void EditorWindow::closeEvent(QCloseEvent* event)
{
	serializeConsoleCommands();
	QWidget::closeEvent(event);
}

void EditorWindow::serializeConsoleCommands()
{
	std::ofstream file(pathToHistory);
	if (file.is_open())
	{
		for (auto& command : commands_)
		{
			file << command.toStdString() << std::endl;
		}
		file.close();
	}
}

void EditorWindow::deserializeConsoleCommands()
{
	std::ifstream file(pathToHistory);
	if (file.is_open())
	{
		while (!file.eof())
		{
			std::string line;
			std::getline(file, line);
			if (!line.empty())
			{
				if (ui)
				{
					addConsoleCommandToScreen(line.c_str(), false);
				}
				commands_.push_back(line.c_str());
			}
		}

		file.close();
	}
}

void EditorWindow::addConsoleCommandToScreen(const QString& command, bool isIn)
{
	ui->plainTextEditConsole->appendPlainText((isIn ? "> " : "< ") + command);
}
