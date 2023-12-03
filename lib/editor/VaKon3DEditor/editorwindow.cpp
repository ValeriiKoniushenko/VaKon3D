#include "editorwindow.h"

#include "./ui_editorwindow.h"
#include "Wsa.h"

#include <iostream>

EditorWindow::EditorWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::EditorWindow)
{
	ui->setupUi(this);
	fillUpTabTree();
	ui->treeWidget->expandAll();

	Wsa::instance().initialize(1, 1);

	connect(ui->treeWidget, &QTreeWidget::itemClicked, this, &EditorWindow::onTabClicked);
	connect(ui->pushButtonConnect, &QPushButton::clicked, this, &EditorWindow::onConnectToServer);
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
		clientSocket = serverSocket.accept();
		ui->pushButtonConnect->setText("Disconnect");
		isConnected = true;
	}
	else
	{
		serverSocket.close();
		ui->pushButtonConnect->setText("Accept");
		clientSocket = {};
		isConnected = false;
	}
}
