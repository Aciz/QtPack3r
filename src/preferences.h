/*
 * MIT License
 *
 * Copyright (c) 2025 Aciz
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once

#include "dialog.h"

#include <QBoxLayout>
#include <QCheckBox>
#include <QComboBox>
#include <QDialog>
#include <QGroupBox>
#include <QLabel>
#include <QListWidget>
#include <QSettings>
#include <QStackedWidget>

#ifdef Q_OS_WINDOWS
#define PREFERENCES_PATH                                                       \
  QStandardPaths::standardLocations(QStandardPaths::AppDataLocation)
#define PREFERENCES_FILENAME "preferences.ini"
#else
#define PREFERENCES_PATH                                                       \
  QStandardPaths::standardLocations(QStandardPaths::AppConfigLocation)
#define PREFERENCES_FILENAME "preferences.conf"
#endif

#define SETTINGS_SCHEMA "v1"

class Preferences : public QObject {
  Q_OBJECT

public:
  enum Settings {
    APPLICATION_VERSION,
    APPLICATION_SCHEMA,
    WINDOW_REMEMBER_SIZE,
    WINDOW_WIDTH,
    WINDOW_HEIGHT,
    PACK3R_PATH,
    MAPS_PATH,
    WRAP_OUTPUT_LINES,

    NUM_SETTINGS // endcap
  };

  void init();
  QVariant readSetting(const Settings &setting);
  void writeSetting(const Settings &setting, const QVariant &value);
  void writeDefaults(bool restoreDefaults);

private:
  QHash<Settings, QPair<QString, QVariant>> settingsMap = {
      {APPLICATION_VERSION, {"Application/Version", PROJECT_VERSION}},
      {APPLICATION_SCHEMA, {"Application/Schema", SETTINGS_SCHEMA}},
      {WINDOW_REMEMBER_SIZE, {"Window/RememberSize", true}},
      {WINDOW_WIDTH, {"Window/Width", -1}},
      {WINDOW_HEIGHT, {"Window/Height", -1}},
      {PACK3R_PATH, {"Paths/Pack3rPath", ""}},
      {MAPS_PATH, {"Paths/MapsPath", ""}},
      {WRAP_OUTPUT_LINES, {"Interface/WrapOutputLines", false}}};

  QString preferencesFile;
};

extern Preferences preferences;

// this is a separate class because we must read settings before the application
// window is created in order to restore the window size, but we cannot create
// a class which inherits QWidget before the application window is created
// TODO: merge this with Preferences class by taking the window size
//   restoration out of the MainWindow constructor,
//   and just resizing the window after it has been created
class PreferencesDialog : public QWidget {
  Q_OBJECT

public:
  void buildPreferencesDialog();
  QDialog *preferencesDialog{};

private:
  void buildInterfacePage();
  void buildPathsPage();

  void setupConnections();
  void setupInterfacePageConnections();
  void setupPathsPageConnections();

  struct InterfacePage {
    QWidget *widget{};
    QVBoxLayout *widgetLayout{};

    QGroupBox *groupBox{};
    QGridLayout *itemLayout{};

    QCheckBox *windowSizeCheckbox{};
  };

  struct PathsPage {
    QWidget *widget{};
    QVBoxLayout *widgetLayout{};

    QGroupBox *groupBox{};
    QGridLayout *itemLayout{};

    QLabel *pack3rPathLabel{};
    QLineEdit *pack3rPathField{};
    QAction *pack3rPathAction{};

    QLabel *mapsPathLabel{};
    QLineEdit *mapsPathField{};
    QAction *mapsPathAction{};
  };

  InterfacePage interfacePage{};
  PathsPage pathsPage{};

  QListWidget *pageList{};
  QListWidgetItem *interfaceItem{};
  QListWidgetItem *pathsItem{};

  QStackedWidget *pages{};

  QHBoxLayout *buttonLayout{};
  QPushButton *resetDefaultsButton{};
  QPushButton *closeButton{};

  QGridLayout *layout{};
};
