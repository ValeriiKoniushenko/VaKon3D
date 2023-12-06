#include "editorwindow.h"

#include "./ui_editorwindow.h"
#include "EditorIntegration.h"
#include "Wsa.h"

#include <iostream>

EditorWindow::EditorWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::EditorWindow)
{
	ui->setupUi(this);
	fillUpTabTree();
	ui->treeWidget->expandAll();

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

void EditorWindow::paintEvent(QPaintEvent* event)
{
	QWidget::paintEvent(event);
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
	{
		EditorNetworkProtocol::Body body;
		body.content = ui->lineEditConsole->text().toStdString();
		body.action = "console";
		const auto bodyString = EditorNetworkProtocol::Body::generate(body);
		const auto headerString = EditorNetworkProtocol::Header::generate(bodyString.size());

		std::vector<char> headerData(headerString.begin(), headerString.end());
		headerData.resize(EditorNetworkProtocol::Header::length);

		acceptedClient.send(headerData);
		acceptedClient.send(bodyString);
	}

	ui->plainTextEditConsole->appendPlainText(ui->lineEditConsole->text());
	ui->lineEditConsole->clear();

	{
		auto header = nlohmann::json::parse(acceptedClient.receiveAsString(EditorNetworkProtocol::Header::length));

		const std::size_t bodyLength = header[EditorNetworkProtocol::Header::lengthPropertyName];
		auto body = nlohmann::json::parse(acceptedClient.receiveAsString(bodyLength + 1ull));

		if (body[EditorNetworkProtocol::Body::possibleActionsPropertyName] == "response-to-console")
		{
			ui->plainTextEditConsole->appendPlainText(
				QString(body[EditorNetworkProtocol::Body::contentPropertyName].get<std::string*>()->c_str()));
		}
	}
}
