#include "editorwindow.h"

#include "./ui_editorwindow.h"

#include <iostream>

EditorWindow::EditorWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::EditorWindow)
{
	ui->setupUi(this);
	fillUpTabTree();
	ui->treeWidget->expandAll();

	connect(ui->treeWidget, &QTreeWidget::itemClicked, this, &EditorWindow::onTabClicked);
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
	else
	{
		ui->stackedWidget->setCurrentIndex(1);
	}
}
