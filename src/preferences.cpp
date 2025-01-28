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

#include "filesystem.h"
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

PreferencesDialog::PreferencesDialog(QWidget *parent) : QWidget(parent) {}

void PreferencesDialog::buildPreferencesDialog() {
  dialog = new QDialog(this);
  dialog->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  dialog->setMinimumSize(700, 500);
  dialog->setMaximumSize(dialog->minimumSize());

  dialog->setWindowTitle(tr("Preferences"));
  dialog->setWindowModality(Qt::ApplicationModal);

  pageList = new QListWidget(this);
  interfaceItem = new QListWidgetItem(tr("Interface"), pageList);
  pathsItem = new QListWidgetItem(tr("Paths"), pageList);

  pageList->addItem(interfaceItem);
  pageList->addItem(pathsItem);

  pages = new QStackedWidget(this);

  buildInterfacePage();
  buildPathsPage();

  pages->insertWidget(0, interfacePage.widget);
  pages->insertWidget(1, pathsPage.widget);

  resetDefaultsButton = new QPushButton(tr("Reset to defaults"), this);
  closeButton = new QPushButton(
      QApplication::style()->standardIcon(QStyle::SP_DialogCloseButton),
      tr("Close"), this);
  closeButton->setDefault(true);

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

  dialog->setLayout(layout);

  setupConnections();
}
void PreferencesDialog::setInitialState() {
  parseSettingsFile();

  pageList->setCurrentRow(0);
  pages->setCurrentIndex(pageList->currentRow());
  pathsPage.oldPack3rPath = pathsPage.pack3rPathField->text();
}

void PreferencesDialog::buildInterfacePage() {
  interfacePage.widget = new QWidget(dialog);
  interfacePage.groupBox = new QGroupBox(tr("Interface"), interfacePage.widget);

  interfacePage.windowSizeCheckbox =
      new QCheckBox(tr("Remember window size"), interfacePage.groupBox);
  interfacePage.windowSizeCheckbox->setToolTip(
      tr("Restore window size from previous session on startup"));

  interfacePage.itemLayout = new QGridLayout(interfacePage.groupBox);
  interfacePage.itemLayout->addWidget(interfacePage.windowSizeCheckbox);
  interfacePage.itemLayout->setAlignment(Qt::AlignTop | Qt::AlignHCenter);

  interfacePage.widgetLayout = new QVBoxLayout(interfacePage.widget);
  interfacePage.widgetLayout->addWidget(interfacePage.groupBox);
}

void PreferencesDialog::buildPathsPage() {
  pathsPage.widget = new QWidget(dialog);
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

  const QString mapsPathTooltip = tr("Default location to discover maps from");
  pathsPage.mapsPathLabel = new QLabel(tr("Mapping path"));
  pathsPage.mapsPathLabel->setToolTip(mapsPathTooltip);

  pathsPage.mapsPathField = new QLineEdit(pathsPage.groupBox);
  pathsPage.mapsPathField->setToolTip(mapsPathTooltip);

  pathsPage.mapsPathAction = new QAction(pathsPage.groupBox);
  pathsPage.mapsPathAction = pathsPage.mapsPathField->addAction(
      QApplication::style()->standardIcon(QStyle::SP_DialogOpenButton),
      QLineEdit::ActionPosition::TrailingPosition);

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

  connect(closeButton, &QPushButton::released, this, [&] { dialog->close(); });

  connect(resetDefaultsButton, &QPushButton::released, this,
          &PreferencesDialog::restoreDefaults);

  connect(dialog, &QDialog::finished, this, [&] {
    if (pathsPage.oldPack3rPath != pathsPage.pack3rPathField->text()) {
      emit pack3rPathChanged(pathsPage.pack3rPathField->text());
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
    const QString path = FileSystem::getMappingPath(
        preferences.readSetting(Preferences::Settings::MAPS_PATH).toString());

    if (!path.isEmpty()) {
      preferences.writeSetting(Preferences::Settings::MAPS_PATH, path);
      pathsPage.mapsPathField->setText(path);
    }
  });

  connect(pathsPage.mapsPathField, &QLineEdit::editingFinished, this, [&] {
    preferences.writeSetting(Preferences::Settings::MAPS_PATH,
                             pathsPage.mapsPathField->text());
  });

  connect(pathsPage.pack3rPathAction, &QAction::triggered, this, [&] {
    const QString path = FileSystem::getPack3rPath(
        preferences.readSetting(Preferences::Settings::PACK3R_PATH).toString());

    if (!path.isEmpty()) {
      preferences.writeSetting(Preferences::Settings::PACK3R_PATH, path);
      pathsPage.pack3rPathField->setText(path);
    }
  });

  connect(pathsPage.pack3rPathField, &QLineEdit::editingFinished, this, [&] {
    if (FileSystem::isValidPack3rBinary(pathsPage.pack3rPathField->text())) {
      preferences.writeSetting(Preferences::Settings::PACK3R_PATH,
                               pathsPage.pack3rPathField->text());
    } else {
      // try to restore from settings
      const QString oldFile =
          preferences.readSetting(Preferences::Settings::PACK3R_PATH)
              .toString();

      if (FileSystem::isValidPack3rBinary(oldFile, true)) {
        pathsPage.pack3rPathField->setText(oldFile);
      } else {
        // fallback, just clear the field and remove the bad entry from settings
        preferences.writeSetting(Preferences::Settings::PACK3R_PATH, "");
        pathsPage.pack3rPathField->clear();
      }
    }
  });
}

void PreferencesDialog::parseSettingsFile() {
  interfacePage.windowSizeCheckbox->setChecked(
      preferences.readSetting(Preferences::Settings::WINDOW_REMEMBER_SIZE)
          .toBool());

  pathsPage.pack3rPathField->setText(
      preferences.readSetting(Preferences::Settings::PACK3R_PATH).toString());
  pathsPage.mapsPathField->setText(
      preferences.readSetting(Preferences::Settings::MAPS_PATH).toString());
}

// TODO: once this dialog is part of the Preferences class,
//  we should read values from 'settingsMap' to grab these defaults.
//  For now, this is just hardcoded which is kinda bleh but whatever,
//  this is fine for now with the amount of settings we have
void PreferencesDialog::resetPreferencesDialogWidget() const {
  interfacePage.windowSizeCheckbox->setChecked(true);
  pathsPage.pack3rPathField->clear();
  pathsPage.mapsPathField->clear();
}

void PreferencesDialog::restoreDefaults() const {
  QMessageBox dialog{};
  Dialog::setupMessageBox(dialog, Dialog::RESET_PREFERENCES);

  const int ret = dialog.exec();

  if (ret == QMessageBox::Yes) {
    preferences.writeDefaults(true);
    resetPreferencesDialogWidget();
  }
}
