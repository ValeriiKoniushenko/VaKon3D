// MIT License
//
// Copyright (c) 2023 Valerii Koniushenko
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

#include "ConsoleConnector.h"
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

	EditorWindow(QWidget* parent = nullptr);
	~EditorWindow();

protected:
	void closeEvent(QCloseEvent* event) override;

private:
	void fillUpTabTree();
	void onConnectToServer(bool checked);
	void executeConsoleCommand(const QString& command);
	void addConsoleCommandToScreen(const QString& command, bool isIn);
	bool eventFilter(QObject* obj, QEvent* event) override;

	// slots
	void onTabClicked(QTreeWidgetItem* item, int column);
	void onEnterDataToConsole();

private:
	ConsoleConnector consoleConnector;
	Ui::EditorWindow* ui;
	TCPServerSocket serverSocket;
	TCPClientSocket acceptedClient;
	bool isConnected = false;
};
