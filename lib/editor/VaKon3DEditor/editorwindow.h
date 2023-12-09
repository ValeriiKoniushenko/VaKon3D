#pragma once

#include "ServerSocket.h"

#include <QMainWindow>
#include <QTreeWidgetItem>
#include <filesystem>

QT_BEGIN_NAMESPACE

namespace Ui
{
class EditorWindow;
}

QT_END_NAMESPACE

class EditorWindow : public QMainWindow
{
	Q_OBJECT
public:
	struct Tabs
	{
		Tabs() = delete;

		inline static const char* title = "Tabs";

		struct General
		{
			General() = delete;
			inline static const char* title = "General";
		};

		struct GameProcess
		{
			GameProcess() = delete;

			inline static const char* title = "Game process";
			inline static std::vector<const char*> tabs = {"World manager", "Game State Manager", "Game Mode manager"};
		};

		struct Scene
		{
			Scene() = delete;

			inline static const char* title = "Scene";
			inline static std::vector<const char*> tabs = {"Scene Object Collector", "Scene manager", "Cube map manager"};
		};

		struct Shaders
		{
			Shaders() = delete;

			inline static const char* title = "Shaders";
			inline static std::vector<const char*> tabs = {"Editor"};
		};

		struct Console
		{
			Console() = delete;

			inline static const char* title = "Console";
		};

		struct Preferences
		{
			Preferences() = delete;

			inline static const char* title = "Preferences";
		};
	};

	inline static const std::filesystem::path pathToHistory = "history.txt";

	EditorWindow(QWidget* parent = nullptr);
	~EditorWindow();

protected:
	void closeEvent(QCloseEvent* event) override;

private:
	void fillUpTabTree();
	void onConnectToServer(bool checked);
	void executeConsoleCommand(const QString& command);
	void addConsoleCommandToScreen(const QString& command, bool isIn);
	void deserializeConsoleCommands();
	void serializeConsoleCommands();

	// slots
	void onTabClicked(QTreeWidgetItem* item, int column);
	void onEnterDataToConsole();

private:
	std::vector<QString> commands_;
	Ui::EditorWindow* ui;
	TCPServerSocket serverSocket;
	TCPClientSocket acceptedClient;
	bool isConnected = false;
};
