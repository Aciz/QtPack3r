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

#ifndef PROJECT_NAME
#define PROJECT_NAME "QtPack3r"
#endif

#ifndef PROJECT_VERSION
#define PROJECT_VERSION "unversioned"
#endif

#ifndef GIT_COMMIT_HASH
#define GIT_COMMIT_HASH "n/a"
#endif

#ifndef GIT_COMMIT_HASH_SHORT
#define GIT_COMMIT_HASH_SHORT "n/a"
#endif

#include "preferences.h"
#include "qtpack3r_widget.h"

#include <QApplication>
#include <QMainWindow>

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow();
  ~MainWindow() override;

private:
  void setupGeometry();
  void setupMenuBar();

  void setupFileMenu();
  void setupEditMenu();
  void setupHelpMenu();

  void buildAboutDialog();

  QtPack3rWidget *qtPack3rwidget{};
  PreferencesDialog *preferencesDialog{};

  const QString pack3rLink = "https://github.com/ovska/Pack3r/releases/latest";
  const QString qtPack3rLink = "https://github.com/Aciz/QtPack3r";
  const QString qtPack3rBugReportLink =
      "https://github.com/Aciz/QtPack3r/issues";
  const QString logoPath = ":images/QtPack3r.png";

  QMenu *fileMenu{};
  QMenu *editMenu{};
  QMenu *helpMenu{};

  QAction *openAction{};
  QAction *quitAction{};

  QAction *preferencesAction{};

  QAction *aboutAction{};
  QAction *bugReportAction{};
  QAction *linkToPack3rAction{};

  QDialog *aboutDialog{};

private slots:
  void openPreferences() const;
  void openAboutWindow();
  void openPack3rLink() const;
  void openBugReportLink() const;
};
