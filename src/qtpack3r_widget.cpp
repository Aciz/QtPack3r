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

/*
 * packer_widget.cpp
 * Top level widget which manages all the user-facing functionality of the app
 */

#include "qtpack3r_widget.h"
#include "dialog.h"
#include "filesystem.h"
#include "preferences.h"

#include <QMimeData>
#include <QSignalBlocker>

QtPack3rWidget::QtPack3rWidget(
    QWidget *parent, const QPointer<PreferencesDialog> &preferencesDialogPtr)
    : QWidget(parent), preferencesDialog(preferencesDialogPtr) {
  setupCommands();

  outputParser = new Pack3rOutputParser(this);
  processHandler = new Pack3rProcessHandler(this, outputParser);
  clipboard = QApplication::clipboard();

  setLayout(buildUI());
  setupConnections();

  parseOptions();
  setDefaults();
  updateCommandPreview();

  checkPack3rVersion();

  setAcceptDrops(true);
}

void QtPack3rWidget::setupCommands() {
  for (int i = 0; i < NUM_PACK3R_OPTIONS; i++) {
    Option opt{};
    opt.command = pack3rOptionStrings[i].first;
    opt.hasValue = pack3rOptionStrings[i].second;

    pack3rCommands.append(qMakePair(false, opt));
  }
}

void QtPack3rWidget::parseOptions() const {
  QVariant setting =
      preferences.readSetting(Preferences::Settings::PACK3R_PATH);

  if (!setting.toString().isEmpty()) {
    ui.paths.pack3rPathField->setText(setting.toString());
  }

  setting = preferences.readSetting(Preferences::Settings::WRAP_OUTPUT_LINES);
  ui.output.wrapCheckbox->setChecked(setting.toBool());
}

void QtPack3rWidget::setDefaults() {
  pack3rCommands[NOSCAN].second.value = ui.options.noScanField->text();
  pack3rCommands[NOPACK].second.value = ui.options.noPackField->text();
  pack3rCommands[VERBOSITY].second.value =
      ui.debug.verbosityCombobox->currentText().toLower();
}

void QtPack3rWidget::checkPack3rVersion() const {
  const QString pack3rBinary = ui.paths.pack3rPathField->text();

  // technically this should not be possible because we don't allow
  // selecting a non-executable file, but if this somehow happens,
  // it's better to not just silently error
#ifdef Q_OS_LINUX
  const QFileInfo fileInfo(pack3rBinary);

  if (!fileInfo.isExecutable()) {
    ui.paths.pack3rVersionLabel->setText("-");
    return;
  }
#endif

  if (!pack3rBinary.isEmpty() &&
      pack3rBinary.endsWith(QDir::toNativeSeparators(PACK3R_EXECUTABLE),
                            Qt::CaseInsensitive)) {
    processHandler->spawnProcess({pack3rBinary, {"--version"}}, "");
  }
}

void QtPack3rWidget::dragEnterEvent(QDragEnterEvent *event) {
  // map files are text/plain mime type
  if (event->mimeData()->hasText() && event->mimeData()->hasUrls()) {
    event->acceptProposedAction();
  }
}

void QtPack3rWidget::dropEvent(QDropEvent *event) {
  // toLocalFile() always returns forward slashes
  const QString file =
      QDir::toNativeSeparators(event->mimeData()->urls().first().toLocalFile());

  if (file.endsWith(".map") || file.endsWith(".reg")) {
    if (!isValidMapPath(file)) {
      QMessageBox dialog{};
      Dialog::setupMessageBox(dialog, Dialog::INVALID_MAP_PATH);
      dialog.exec();
      return;
    }

    ui.paths.mapPathField->setText(file);
    autoFillOutputPath(file);
    updateCommandPreview();
  }
}

bool QtPack3rWidget::canRunPack3r() const {
  QMessageBox dialog{};
  Dialog::setupMessageBox(dialog, Dialog::PACK3R_RUN_ERROR);

  bool canRun = true;

  if (ui.paths.pack3rPathField->text().isEmpty()) {
    dialog.setText(tr("Unable to find Pack3r executable!"));
    dialog.setInformativeText(
        tr("Make sure path to Pack3r executable is set."));
    canRun = false;
  } else if (ui.paths.mapPathField->text().isEmpty()) {
    dialog.setText(tr("No map file specified!"));
    dialog.setInformativeText(tr("Please select a valid map file to process."));
    canRun = false;
  } else if (ui.paths.outputPathField->text().isEmpty()) {
    dialog.setText(tr("No output file specified!"));
    dialog.setInformativeText(tr("Please specify a valid output file."));
    canRun = false;
  } else {
    // check file permissions on Linux, so we don't silently error
#ifdef Q_OS_LINUX
    const QFileInfo fileInfo(ui.paths.pack3rPathField->text());

    if (!fileInfo.isExecutable()) {
      QMessageBox::critical(
          nullptr, "Invalid file permissions",
          QString("File '%1' is not executable. Check the file permissions.")
              .arg(ui.paths.pack3rPathField->text()));
      // return here so we don't get double dialogs
      return false;
    }
#endif
  }

  if (!canRun) {
    dialog.exec();
  }

  return canRun;
}

void QtPack3rWidget::copyFieldToClipboard(const QPlainTextEdit *field) const {
  Q_ASSERT(clipboard != nullptr);
  clipboard->setText(field->toPlainText());
}

// FIXME: TODO: this is disgusting, see if we can make this less verbose
void QtPack3rWidget::resetWidgetState() {
  for (int i = 0; i < NUM_PACK3R_OPTIONS; i++) {
    pack3rCommands[i].first = false;
    pack3rCommands[i].second.value.clear();
  }

  ui.paths.pack3rPathField->clear();
  ui.paths.mapPathField->clear();

  QSignalBlocker outputCheckboxBlocker(ui.paths.outputCheckbox);
  QSignalBlocker outputPathFieldBlocker(ui.paths.outputPathField);
  ui.paths.outputCheckbox->setChecked(false);
  ui.paths.outputPathField->clear();

  QSignalBlocker dryRunCheckboxBlocker(ui.options.dryRunCheckbox);
  QSignalBlocker overwriteCheckboxBlocker(ui.options.overwriteCheckbox);
  QSignalBlocker looseCheckboxBlocker(ui.options.looseCheckbox);
  QSignalBlocker sourceCheckboxBlocker(ui.options.sourceCheckbox);
  QSignalBlocker includePk3CheckboxBlocker(ui.options.includePk3Checkbox);
  ui.options.dryRunCheckbox->setChecked(false);
  ui.options.overwriteCheckbox->setChecked(false);
  ui.options.looseCheckbox->setChecked(false);
  ui.options.sourceCheckbox->setChecked(false);
  ui.options.includePk3Checkbox->setChecked(false);

  QSignalBlocker renameCheckboxBlocker(ui.options.renameCheckbox);
  QSignalBlocker renameFieldBlocker(ui.options.renameField);
  ui.options.renameCheckbox->setChecked(false);
  ui.options.renameField->clear();

  QSignalBlocker noScanCheckboxBlocker(ui.options.noScanCheckbox);
  QSignalBlocker noScanFieldBlocker(ui.options.noScanField);
  ui.options.noScanCheckbox->setChecked(false);
  ui.options.noScanField->setText(noScanDefaultStr);

  QSignalBlocker noPackCheckboxBlocker(ui.options.noPackCheckbox);
  QSignalBlocker noPackFieldBlocker(ui.options.noPackField);
  ui.options.noPackCheckbox->setChecked(false);
  ui.options.noPackField->setText(noPackDefaultStr);

  QSignalBlocker modsCheckboxBlocker(ui.options.modsCheckbox);
  QSignalBlocker modsFieldBlocker(ui.options.modsField);
  ui.options.modsCheckbox->setChecked(false);
  ui.options.modsField->clear();

  QSignalBlocker verbosityComboboxBlocker(ui.debug.verbosityCombobox);
  QSignalBlocker shaderDebugCheckboxBlocker(ui.debug.shaderDebugCheckbox);
  QSignalBlocker referenceDebugCheckboxBlocker(ui.debug.referenceDebugCheckbox);
  ui.debug.verbosityCombobox->setCurrentIndex(
      ui.debug.verbosityCombobox->findText(tr("Default"), Qt::MatchContains));
  ui.debug.shaderDebugCheckbox->setChecked(false);
  ui.debug.referenceDebugCheckbox->setChecked(false);

  ui.commandPreview.commandPreviewField->clear();

  parseOptions();
  setDefaults();
  updateCommandPreview();
}

void QtPack3rWidget::updatePack3rOutput(const QByteArray &data) const {
  ui.output.outputField->appendPlainText(data);
}

void QtPack3rWidget::updatePack3rPath(const QString &newPath) {
  ui.paths.pack3rPathField->setText(newPath);
  updateCommandPreview();
  checkPack3rVersion();
}

void QtPack3rWidget::setPack3rVersionString(const QString &version) const {
  ui.paths.pack3rVersionLabel->setText(version);
}
