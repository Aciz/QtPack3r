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

#include "mainwindow.h"
#include "preferences.h"

#include <QDesktopServices>
#include <QDialogButtonBox>
#include <QGraphicsScene>
#include <QScreen>
#include <QSettings>

inline constexpr int DEFAULT_WIDTH = 800;
inline constexpr int DEFAULT_HEIGHT = 800;

MainWindow::MainWindow() : qtPack3rwidget(new QtPack3rWidget(this)) {
  setupGeometry();
  setupMenuBar();
  setCentralWidget(qtPack3rwidget);
  setWindowTitle(PROJECT_NAME);
  setWindowIcon(QIcon(logoPath));
}

MainWindow::~MainWindow() {
  preferences.writeSetting(Preferences::Settings::WINDOW_WIDTH, width());
  preferences.writeSetting(Preferences::Settings::WINDOW_HEIGHT, height());
}

void MainWindow::setupGeometry() {
  const QPoint cursor = QCursor::pos();
  const QScreen *screen = QApplication::screenAt(cursor);

  if (!screen) {
    screen = QApplication::primaryScreen();
  }

  int w, h;
  const QRect screenGeometry = screen->availableGeometry();
  const bool restoreSize =
      preferences.readSetting(Preferences::Settings::WINDOW_REMEMBER_SIZE)
          .toBool();

  if (restoreSize) {
    const int prefW =
        preferences.readSetting(Preferences::Settings::WINDOW_WIDTH).toInt();
    const int prefH =
        preferences.readSetting(Preferences::Settings::WINDOW_HEIGHT).toInt();

    if (prefW > 0 && prefH > 0) {
      w = std::min(prefW, screenGeometry.width());
      h = std::min(prefH, screenGeometry.height());
    } else {
      w = std::min(DEFAULT_WIDTH, screenGeometry.width());
      h = std::min(DEFAULT_HEIGHT, screenGeometry.height());
    }
  } else {
    w = std::min(DEFAULT_WIDTH, screenGeometry.width());
    h = std::min(DEFAULT_HEIGHT, screenGeometry.height());
  }

  resize(w, h);

  const int x = (screenGeometry.width() - width()) / 2 + screenGeometry.x();
  const int y = (screenGeometry.height() - height()) / 2 + screenGeometry.y();

  move(x, y);
}

void MainWindow::setupMenuBar() {
  setupFileMenu();
  setupEditMenu();
  setupHelpMenu();
}

void MainWindow::setupFileMenu() {
  fileMenu = menuBar()->addMenu(tr("&File"));

  openAction = new QAction(tr("&Open map..."), this);
  openAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_O));
  openAction->setIcon(
      QApplication::style()->standardIcon(QStyle::SP_DialogOpenButton));
  fileMenu->addAction(openAction);
  connect(openAction, &QAction::triggered, qtPack3rwidget,
          &QtPack3rWidget::openMap);

  fileMenu->addSeparator();

  quitAction = new QAction(tr("&Quit"), this);
  quitAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Q));
  quitAction->setIcon(
      QApplication::style()->standardIcon(QStyle::SP_TitleBarCloseButton));
  fileMenu->addAction(quitAction);
  connect(quitAction, &QAction::triggered, qApp, &QApplication::quit,
          Qt::QueuedConnection);
}

void MainWindow::setupEditMenu() {
  editMenu = menuBar()->addMenu(tr("&Edit"));

  preferencesAction = new QAction(tr("&Preferences"), this);
  preferencesAction->setShortcut(QKeySequence(Qt::Key_P));
  editMenu->addAction(preferencesAction);
  connect(preferencesAction, &QAction::triggered, this,
          &MainWindow::openPreferences);
}

void MainWindow::setupHelpMenu() {
  helpMenu = menuBar()->addMenu(tr("&Help"));

  bugReportAction = new QAction(tr("&Report a bug"), this);
  bugReportAction->setIcon(
      QApplication::style()->standardIcon(QStyle::SP_CommandLink));
  helpMenu->addAction(bugReportAction);
  connect(bugReportAction, &QAction::triggered, this,
          &MainWindow::openBugReportLink);

  linkToPack3rAction = new QAction(tr("&Download Pack3r (GitHub)"), this);
  linkToPack3rAction->setIcon(
      QApplication::style()->standardIcon(QStyle::SP_CommandLink));
  helpMenu->addAction(linkToPack3rAction);
  connect(linkToPack3rAction, &QAction::triggered, this,
          &MainWindow::openPack3rLink);

  helpMenu->addSeparator();

  aboutAction = new QAction(tr("&About %1").arg(PROJECT_NAME), this);
  aboutAction->setIcon(
      QApplication::style()->standardIcon(QStyle::SP_MessageBoxInformation));
  helpMenu->addAction(aboutAction);
  connect(aboutAction, &QAction::triggered, this, &MainWindow::openAboutWindow);
}

// TODO: make this part of Dialog class
void MainWindow::buildAboutDialog() {
  aboutDialog = new QDialog(this);
  aboutDialog->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  aboutDialog->setMinimumSize(500, 500);
  aboutDialog->setMaximumSize(aboutDialog->minimumSize());

  // cleanup on close
  aboutDialog->setAttribute(Qt::WA_DeleteOnClose);
  aboutDialog->setWindowTitle(tr("About %1").arg(PROJECT_NAME));

  // make the pointer null when the object gets destroyed,
  connect(aboutDialog, &QObject::destroyed, this,
          [&] { aboutDialog = nullptr; });

  auto *layout = new QVBoxLayout(aboutDialog);
  auto *header = new QLabel(QString("%1").arg(PROJECT_NAME), aboutDialog);
  auto *version = new QLabel(aboutDialog);
  auto *logoLabel = new QLabel(aboutDialog);
  auto *body = new QLabel(aboutDialog);
  auto *buttonBox = new QDialogButtonBox(aboutDialog);

  header->setAlignment(Qt::AlignCenter);
  QFont font = header->font();
  font.setBold(true);
  font.setPointSize(16);
  header->setFont(font);

  const QString versionStr =
      tr("Version %1\nBuild date %2\nBuild commit %3")
          .arg(PROJECT_VERSION, __DATE__, GIT_COMMIT_HASH);
  version->setText(versionStr);
  version->setAlignment(Qt::AlignCenter);
  version->setTextInteractionFlags(Qt::TextSelectableByMouse);

  const QPixmap logo(logoPath);
  logoLabel->setFixedSize(100, 100);
  logoLabel->setPixmap(logo.scaled(logoLabel->width(), logoLabel->height(),
                                   Qt::KeepAspectRatio));

  const QString year = QString("%1").arg(__DATE__).split(" ").back();
  body->setTextFormat(Qt::RichText);
  body->setTextInteractionFlags(Qt::TextBrowserInteraction);
  body->setOpenExternalLinks(true);
  body->setAlignment(Qt::AlignCenter);
  body->setWordWrap(true);

  body->setText(QString(
      tr("Copyright (c) %1 Aciz<br><br>This software is licensed under the MIT "
         "License. You are free to use, copy, modify, and distribute this "
         "software with proper attribution.<br<br>For details, see the <a "
         "href='https://opensource.org/license/mit'>MIT License</a><br><br><a "
         "href='%2'>%4 on GitHub</a><br><br><a "
         "href='https://www.flaticon.com/free-icons/zip-format'>Icon adapted "
         "from juicy_fish - Flaticon</a>")
          .arg(year, qtPack3rLink, PROJECT_NAME)));

  const auto closeButton = buttonBox->addButton(QDialogButtonBox::Close);
  connect(closeButton, &QPushButton::released, this,
          [&] { aboutDialog->close(); });

  const auto qtButton =
      buttonBox->addButton("About Qt", QDialogButtonBox::ButtonRole::NoRole);
  connect(qtButton, &QPushButton::clicked, &QApplication::aboutQt);

  auto *buttonLayout = new QHBoxLayout;
  buttonLayout->addWidget(qtButton);
  buttonLayout->addStretch(1);
  buttonLayout->addWidget(closeButton);

  layout->addWidget(header, 0, Qt::AlignCenter);
  layout->addWidget(version, 0, Qt::AlignCenter);
  layout->addWidget(logoLabel, 1, Qt::AlignCenter);
  layout->addWidget(body, 1, Qt::AlignCenter);
  layout->addLayout(buttonLayout, 0);
}

void MainWindow::openPreferences() {
  PreferencesDialog preferencesDialog{};
  preferencesDialog.buildPreferencesDialog();
  preferencesDialog.preferencesDialog->exec();
}

void MainWindow::openAboutWindow() {
  if (aboutDialog) {
    aboutDialog->raise();
    aboutDialog->activateWindow();
  } else {
    buildAboutDialog();
    aboutDialog->show();
  }
}

void MainWindow::openPack3rLink() const {
  QDesktopServices::openUrl(QUrl(pack3rLink));
}

void MainWindow::openBugReportLink() const {
  QDesktopServices::openUrl(QUrl(qtPack3rBugReportLink));
}
