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

#include "preferences.h"
#include "qtpack3r_widget.h"

#include <QApplication>
#include <QDir>
#include <QStandardPaths>

// global preferences instance
Preferences preferences;

void Preferences::init() {
  const auto locations = PREFERENCES_PATH;

  // this should never happen but just in case of some weirdness,
  // use the users home directory as a fallback location0
  if (locations.empty()) {
    preferencesFile = QDir::homePath() + NATIVE_PATHSEP +
                      QApplication::applicationName() + NATIVE_PATHSEP +
                      PREFERENCES_FILENAME;
  } else {
    preferencesFile = locations.first() + NATIVE_PATHSEP + PREFERENCES_FILENAME;
  }

  writeDefaults(false);
}

QVariant Preferences::readSetting(const Settings &setting) {
  const QSettings s(preferencesFile, QSettings::IniFormat);
  return s.value(settingsMap[setting].first);
}

void Preferences::writeSetting(const Settings &setting, const QVariant &value) {
  QSettings s(preferencesFile, QSettings::IniFormat);
  s.setValue(settingsMap[setting].first, value);
}

void Preferences::writeDefaults(const bool restoreDefaults) {
  QSettings s(preferencesFile, QSettings::IniFormat);

  for (int i = 0; i < NUM_SETTINGS; i++) {
    const auto setting = static_cast<Settings>(i);

    switch (setting) {
    // we want to always write the correct application version and schema
    case APPLICATION_VERSION:
    case APPLICATION_SCHEMA:
      s.setValue(settingsMap[setting].first, settingsMap[setting].second);
      break;
    default:
      if (restoreDefaults) {
        s.setValue(settingsMap[setting].first, settingsMap[setting].second);
      } else {
        s.setValue(
            settingsMap[setting].first,
            s.value(settingsMap[setting].first, settingsMap[setting].second));
      }
      break;
    }
  }
}

void PreferencesDialog::buildPreferencesDialog() {
  preferencesDialog = new QDialog(this);
  preferencesDialog->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  preferencesDialog->setMinimumSize(700, 500);
  preferencesDialog->setMaximumSize(preferencesDialog->minimumSize());

  preferencesDialog->setWindowTitle(tr("Preferences"));
  preferencesDialog->setWindowModality(Qt::ApplicationModal);
  preferencesDialog->setAttribute(Qt::WA_DeleteOnClose);

  pageList = new QListWidget(preferencesDialog);
  interfaceItem = new QListWidgetItem(tr("Interface"), pageList);
  pathsItem = new QListWidgetItem(tr("Paths"), pageList);

  pageList->addItem(interfaceItem);
  pageList->addItem(pathsItem);

  pages = new QStackedWidget(preferencesDialog);

  buildInterfacePage();
  buildPathsPage();

  pages->insertWidget(0, interfacePage.widget);
  pages->insertWidget(1, pathsPage.widget);

  resetDefaultsButton =
      new QPushButton(tr("Reset to defaults"), preferencesDialog);
  closeButton = new QPushButton(
      QApplication::style()->standardIcon(QStyle::SP_DialogCloseButton),
      tr("Close"), preferencesDialog);

  buttonLayout = new QHBoxLayout;
  buttonLayout->addWidget(resetDefaultsButton);
  buttonLayout->addStretch(1);
  buttonLayout->addWidget(closeButton);

  layout = new QGridLayout;
  layout->addWidget(pageList, 0, 0, 2, 1);
  layout->addWidget(pages, 0, 1, 1, 1);
  layout->addLayout(buttonLayout, 1, 1, 1, 1);
  layout->setColumnStretch(0, 1);
  layout->setColumnStretch(1, 4);

  preferencesDialog->setLayout(layout);

  setupConnections();
}

void PreferencesDialog::buildInterfacePage() {
  interfacePage.widget = new QWidget(preferencesDialog);
  interfacePage.groupBox = new QGroupBox(tr("Interface"), interfacePage.widget);

  interfacePage.windowSizeCheckbox =
      new QCheckBox(tr("Remember window size"), interfacePage.groupBox);
  interfacePage.windowSizeCheckbox->setChecked(
      preferences.readSetting(Preferences::Settings::WINDOW_REMEMBER_SIZE)
          .toBool());
  interfacePage.windowSizeCheckbox->setToolTip(
      tr("Restore window size from previous session on startup"));

  interfacePage.itemLayout = new QGridLayout(interfacePage.groupBox);
  interfacePage.itemLayout->addWidget(interfacePage.windowSizeCheckbox);
  interfacePage.itemLayout->setAlignment(Qt::AlignTop | Qt::AlignHCenter);

  interfacePage.widgetLayout = new QVBoxLayout(interfacePage.widget);
  interfacePage.widgetLayout->addWidget(interfacePage.groupBox);
}

void PreferencesDialog::buildPathsPage() {
  pathsPage.widget = new QWidget(preferencesDialog);
  pathsPage.groupBox = new QGroupBox(tr("Paths"), pathsPage.widget);

  const QString pack3rPathTooltip = tr("Location of Pack3r executable");

  pathsPage.pack3rPathLabel = new QLabel(tr("Pack3r location"));
  pathsPage.pack3rPathLabel->setToolTip(pack3rPathTooltip);

  pathsPage.pack3rPathField = new QLineEdit(pathsPage.groupBox);
  pathsPage.pack3rPathField->setToolTip(pack3rPathTooltip);

  pathsPage.pack3rPathAction = new QAction(pathsPage.groupBox);
  pathsPage.pack3rPathAction = pathsPage.pack3rPathField->addAction(
      QApplication::style()->standardIcon(QStyle::SP_DialogOpenButton),
      QLineEdit::ActionPosition::TrailingPosition);
  const QString pack3rPathStr =
      preferences.readSetting(Preferences::Settings::PACK3R_PATH).toString();

  if (!pack3rPathStr.isEmpty()) {
    pathsPage.pack3rPathField->setText(pack3rPathStr);
  }

  const QString mapsPathTooltip = tr("Default location to discover maps from");
  pathsPage.mapsPathLabel = new QLabel(tr("Mapping path"));
  pathsPage.mapsPathLabel->setToolTip(mapsPathTooltip);

  pathsPage.mapsPathField = new QLineEdit(pathsPage.groupBox);
  pathsPage.mapsPathField->setToolTip(mapsPathTooltip);

  pathsPage.mapsPathAction = new QAction(pathsPage.groupBox);
  pathsPage.mapsPathAction = pathsPage.mapsPathField->addAction(
      QApplication::style()->standardIcon(QStyle::SP_DialogOpenButton),
      QLineEdit::ActionPosition::TrailingPosition);
  const QString mapsPathStr =
      preferences.readSetting(Preferences::Settings::MAPS_PATH).toString();

  if (!mapsPathStr.isEmpty()) {
    pathsPage.mapsPathField->setText(mapsPathStr);
  }

  pathsPage.itemLayout = new QGridLayout(pathsPage.groupBox);

  pathsPage.itemLayout->addWidget(pathsPage.pack3rPathLabel, 0, 0);
  pathsPage.itemLayout->addWidget(pathsPage.pack3rPathField, 0, 1);
  pathsPage.itemLayout->addWidget(pathsPage.mapsPathLabel, 1, 0);
  pathsPage.itemLayout->addWidget(pathsPage.mapsPathField, 1, 1);
  pathsPage.itemLayout->setColumnStretch(0, 1);
  pathsPage.itemLayout->setColumnStretch(1, 4);
  pathsPage.itemLayout->setAlignment(Qt::AlignTop);

  pathsPage.widgetLayout = new QVBoxLayout(pathsPage.widget);
  pathsPage.widgetLayout->addWidget(pathsPage.groupBox);
}

void PreferencesDialog::setupConnections() {
  connect(pageList, &QListWidget::currentRowChanged, this,
          [&] { pages->setCurrentIndex(pageList->currentRow()); });

  connect(closeButton, &QPushButton::released, this,
          [&] { preferencesDialog->close(); });

  connect(resetDefaultsButton, &QPushButton::released, this, [&] {
    QMessageBox dialog{};
    Dialog::setupMessageBox(dialog, Dialog::RESET_PREFERENCES);

    const int ret = dialog.exec();

    if (ret == QMessageBox::Yes) {
      preferences.writeDefaults(true);
    }
  });

  setupInterfacePageConnections();
  setupPathsPageConnections();
}

void PreferencesDialog::setupInterfacePageConnections() {
  connect(interfacePage.windowSizeCheckbox, &QCheckBox::toggled, this, [&] {
    preferences.writeSetting(Preferences::Settings::WINDOW_REMEMBER_SIZE,
                             interfacePage.windowSizeCheckbox->isChecked());
  });
}

void PreferencesDialog::setupPathsPageConnections() {
  connect(pathsPage.mapsPathAction, &QAction::triggered, this, [&] {
    const QString currentPath =
        preferences.readSetting(Preferences::Settings::MAPS_PATH).toString();
    const QString mapsPath =
        !currentPath.isEmpty() ? currentPath : QDir::homePath();

    const QString newPath =
        NATIVE_GETDIR(this, tr("Choose mapping install location"), mapsPath,
                      QFileDialog::ShowDirsOnly);

    if (!newPath.isEmpty()) {
      preferences.writeSetting(Preferences::Settings::MAPS_PATH, newPath);
      pathsPage.mapsPathField->setText(newPath);
    }
  });

  connect(pathsPage.mapsPathField, &QLineEdit::editingFinished, this, [&] {
    preferences.writeSetting(Preferences::Settings::MAPS_PATH,
                             pathsPage.mapsPathField->text());
  });

  connect(pathsPage.pack3rPathAction, &QAction::triggered, this, [&] {
    const QString currentPath =
        preferences.readSetting(Preferences::Settings::PACK3R_PATH).toString();
    const QString pack3rPath =
        !currentPath.isEmpty() ? currentPath : QDir::homePath();

    const QString newPath = NATIVE_GETFILE(this, tr("Find Pack3r executable"),
                                           pack3rPath, QString());

    if (!newPath.isEmpty()) {
      preferences.writeSetting(Preferences::Settings::PACK3R_PATH, newPath);
      pathsPage.pack3rPathField->setText(newPath);
    }
  });

  connect(pathsPage.pack3rPathField, &QLineEdit::editingFinished, this, [&] {
    preferences.writeSetting(Preferences::Settings::PACK3R_PATH,
                             pathsPage.pack3rPathField->text());
  });
}
