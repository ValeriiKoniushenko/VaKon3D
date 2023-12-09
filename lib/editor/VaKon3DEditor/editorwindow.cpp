#include "editorwindow.h"

#include "./ui_editorwindow.h"
#include "EditorIntegration.h"
#include "Wsa.h"

#include <fstream>
#include <iostream>

EditorWindow::EditorWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::EditorWindow), consoleConnector(acceptedClient)
{
	ui->setupUi(this);
	fillUpTabTree();
	ui->treeWidget->expandAll();

	consoleConnector.onDeserializeOneCommand.subscribe(
		[this](const std::string& command)
		{
			if (ui)
			{
				addConsoleCommandToScreen(command.c_str(), false);
			}
		});
	consoleConnector.deserializeConsoleCommands();
	consoleConnector.onAddCommand.subscribe(
		[this](const std::string& command)
		{
			addConsoleCommandToScreen(command.c_str(), false);
			ui->lineEditConsole->clear();
		});
	consoleConnector.onClearScreen.subscribe(
		[this]()
		{
			ui->plainTextEditConsole->clear();
			ui->lineEditConsole->clear();
		});
	consoleConnector.onResponse.subscribe(
		[this](const std::string& response) { addConsoleCommandToScreen(response.c_str(), true); });

	Wsa::instance().initialize(1, 1);
	// onConnectToServer(false);

	connect(ui->treeWidget, &QTreeWidget::itemClicked, this, &EditorWindow::onTabClicked);
	connect(ui->pushButtonConnect, &QPushButton::clicked, this, &EditorWindow::onConnectToServer);
	connect(ui->lineEditConsole, &QLineEdit::returnPressed, this, &EditorWindow::onEnterDataToConsole);
	connect(ui->pushButtonConsoleEnter, &QPushButton::pressed, this, &EditorWindow::onEnterDataToConsole);
	ui->lineEditConsole->installEventFilter(this);
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
		ui->stackedWidget->setCurrentIndex(0);
	}
	else if (item->text(column) == Tabs::General::title)
	{
		ui->stackedWidget->setCurrentIndex(1);
	}
	else if (item->text(column) == "Scene Object Collector")
	{
		ui->stackedWidget->setCurrentIndex(2);
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
	consoleConnector.executeConsoleCommand(command.toStdString());
}

void EditorWindow::closeEvent(QCloseEvent* event)
{
	consoleConnector.serializeConsoleCommands();
	QWidget::closeEvent(event);
}

void EditorWindow::addConsoleCommandToScreen(const QString& command, bool isIn)
{
	ui->plainTextEditConsole->appendPlainText((isIn ? "> " : "< ") + command);
}

bool EditorWindow::eventFilter(QObject* obj, QEvent* event)
{
	if (obj == ui->lineEditConsole)
	{
		if (event->type() == QEvent::KeyPress)
		{
			auto* keyEvent = static_cast<QKeyEvent*>(event);
			if (keyEvent->key() == Qt::Key_Up)
			{
				if (auto command = consoleConnector.previousCommand(); !command.empty())
				{
					if (ui)
					{
						ui->lineEditConsole->setText(command.c_str());
					}
				}
				return true;
			}
			else if (keyEvent->key() == Qt::Key_Down)
			{
				if (auto command = consoleConnector.nextCommand(); !command.empty())
				{
					if (ui)
					{
						ui->lineEditConsole->setText(command.c_str());
					}
				}
				return true;
			}
		}
		return false;
	}
	return QMainWindow::eventFilter(obj, event);
}
