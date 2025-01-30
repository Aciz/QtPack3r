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

#include "qtpack3r_widget.h"

QGridLayout *QtPack3rWidget::buildUI() {
  layout = new QGridLayout(this);

  setupPathsGroupBox();
  setupOptionsGroupBox();
  setupDebugGroupBox();
  setupCommandPreviewGroupBox();
  setupOutputGroupBox();
  setupStatusBar();

  layout->addWidget(ui.paths.groupBox);
  layout->addWidget(ui.options.groupBox);
  layout->addWidget(ui.debug.groupBox);
  layout->addWidget(ui.commandPreview.groupBox);
  layout->addWidget(ui.output.groupBox);
  layout->addWidget(ui.statusBar.bar);

  return layout;
}

void QtPack3rWidget::setupPathsGroupBox() {
  ui.paths.groupBox = new QGroupBox(tr("Paths"), this);

  const QString pack3rPathToolTip = tr("Location of Pack3r executable");
  ui.paths.pack3rPathLabel = new QLabel(tr("Path to Pack3r"), this);
  ui.paths.pack3rPathLabel->setToolTip(pack3rPathToolTip);

  ui.paths.pack3rPathField = new QLineEdit(this);
  ui.paths.pack3rPathField->setToolTip(pack3rPathToolTip);
  ui.paths.pack3rPathField->setReadOnly(true);
  ui.paths.pack3rPathAction = ui.paths.pack3rPathField->addAction(
      QApplication::style()->standardIcon(QStyle::SP_DialogOpenButton),
      QLineEdit::ActionPosition::TrailingPosition);

  const QString mapPathToolTip = tr("Location of map file to process");
  ui.paths.mapPathLabel = new QLabel(tr("Path to map"), this);
  ui.paths.mapPathLabel->setToolTip(mapPathToolTip);

  ui.paths.mapPathField = new QLineEdit(this);
  ui.paths.mapPathField->setToolTip(mapPathToolTip);
  ui.paths.mapPathField->setReadOnly(true);
  ui.paths.mapPathAction = ui.paths.mapPathField->addAction(
      QApplication::style()->standardIcon(QStyle::SP_DialogOpenButton),
      QLineEdit::ActionPosition::TrailingPosition);

  const QString outputToolTip =
      tr("Path to destination pk3/zip (or directory where it will be written)");
  ui.paths.outputCheckbox = new QCheckBox(tr("Custom output"), this);
  ui.paths.outputCheckbox->setToolTip(outputToolTip);

  ui.paths.outputPathField = new QLineEdit(this);
  ui.paths.outputPathField->setToolTip(outputToolTip);
  ui.paths.outputPathField->setReadOnly(true);
  ui.paths.outputPathAction = ui.paths.outputPathField->addAction(
      QApplication::style()->standardIcon(QStyle::SP_DialogOpenButton),
      QLineEdit::ActionPosition::TrailingPosition);

  ui.paths.layout = new QGridLayout;

  ui.paths.layout->addWidget(ui.paths.pack3rPathLabel, 0, 0);
  ui.paths.layout->addWidget(ui.paths.pack3rPathField, 0, 1);

  ui.paths.layout->addWidget(ui.paths.mapPathLabel, 1, 0);
  ui.paths.layout->addWidget(ui.paths.mapPathField, 1, 1);

  ui.paths.layout->addWidget(ui.paths.outputCheckbox, 2, 0);
  ui.paths.layout->addWidget(ui.paths.outputPathField, 2, 1);

  ui.paths.groupBox->setLayout(ui.paths.layout);
}

void QtPack3rWidget::setupOptionsGroupBox() {
  ui.options.groupBox = new QGroupBox(tr("Options"), this);

  ui.options.dryRunCheckbox = new QCheckBox(tr("Dry run"), this);
  ui.options.looseCheckbox = new QCheckBox(tr("Loose"), this);
  ui.options.sourceCheckbox = new QCheckBox(tr("Source"), this);
  ui.options.overwriteCheckbox = new QCheckBox(tr("Overwrite"), this);
  ui.options.includePk3Checkbox = new QCheckBox(tr("Include pk3(dir)s"), this);

  ui.options.dryRunCheckbox->setToolTip(tr(
      "Discover packed files and estimate file size without creating a pk3"));
  ui.options.looseCheckbox->setToolTip(
      tr("Complete packing even if some files are missing"));
  ui.options.sourceCheckbox->setToolTip(
      tr("Pack only source files (.map, editorimages, misc_models) without "
         "packing BSP & lightmaps"));
  ui.options.overwriteCheckbox->setToolTip(
      tr("Overwrite existing files in the output path with impunity"));
  ui.options.includePk3Checkbox->setToolTip(
      tr("Include pk3 files and pk3dirs in etmain when indexing files"));

  const QString renameToolTip = tr(
      "Name of the map after packing (renames bsp, lightmaps, mapscript etc.)");
  ui.options.renameCheckbox = new QCheckBox(tr("Rename"), this);
  ui.options.renameCheckbox->setToolTip(renameToolTip);

  ui.options.renameField = new QLineEdit(this);
  ui.options.renameField->setToolTip(renameToolTip);
  ui.options.renameField->setReadOnly(true);

  const QString noScanToolTip =
      tr("Don't scan pk3/pk3dirs for assets\nIf this isn't set, the default "
         "files/directories to ignore are 'pak1.pk3', 'pak2.pk3' and "
         "'mp_bin.pk3'");
  ui.options.noScanCheckbox = new QCheckBox(tr("Skip scanning"), this);
  ui.options.noScanCheckbox->setToolTip(noScanToolTip);

  ui.options.noScanField = new QLineEdit(this);
  ui.options.noScanField->setToolTip(noScanToolTip);
  ui.options.noScanField->setText(noScanDefaultStr);
  ui.options.noScanField->setReadOnly(true);

  const QString noPackToolTip = tr(
      "Scan some pk3s/pk3dirs but don't pack their contents\nIf this isn't "
      "set, the defaults files/directories are 'pak0.pk3', 'pak0.pk3dir', "
      "'lights.pk3', 'sd-mapobjects.pk3', 'common.pk3' and 'astro-skies.pk3'");
  ui.options.noPackCheckbox = new QCheckBox(tr("Skip packing"), this);
  ui.options.noPackCheckbox->setToolTip(noPackToolTip);

  ui.options.noPackField = new QLineEdit(this);
  ui.options.noPackField->setToolTip(noPackToolTip);
  ui.options.noPackField->setText(noPackDefaultStr);
  ui.options.noPackField->setReadOnly(true);

  const QString modsToolTip = tr("Add all pk3s in specified mod directories to "
                                 "scanning, but don't pack their contents");
  ui.options.modsCheckbox = new QCheckBox(tr("Include mods"), this);
  ui.options.modsCheckbox->setToolTip(modsToolTip);

  ui.options.modsField = new QLineEdit(this);
  ui.options.modsField->setToolTip(modsToolTip);
  ui.options.modsField->setReadOnly(true);

  ui.options.checkboxLayout = new QGridLayout;

  ui.options.checkboxLayout->addWidget(ui.options.dryRunCheckbox, 0, 0);
  ui.options.checkboxLayout->addWidget(ui.options.overwriteCheckbox, 0, 1);
  ui.options.checkboxLayout->addWidget(ui.options.looseCheckbox, 0, 2);
  ui.options.checkboxLayout->addWidget(ui.options.sourceCheckbox, 0, 3);
  ui.options.checkboxLayout->addWidget(ui.options.includePk3Checkbox, 0, 4);

  ui.options.mainLayout = new QGridLayout;

  ui.options.mainLayout->addLayout(ui.options.checkboxLayout, 0, 0, 1, 2);

  ui.options.mainLayout->addWidget(ui.options.renameCheckbox, 1, 0);
  ui.options.mainLayout->addWidget(ui.options.renameField, 1, 1);

  ui.options.mainLayout->addWidget(ui.options.noScanCheckbox, 2, 0);
  ui.options.mainLayout->addWidget(ui.options.noScanField, 2, 1);

  ui.options.mainLayout->addWidget(ui.options.noPackCheckbox, 3, 0);
  ui.options.mainLayout->addWidget(ui.options.noPackField, 3, 1);

  ui.options.mainLayout->addWidget(ui.options.modsCheckbox, 4, 0);
  ui.options.mainLayout->addWidget(ui.options.modsField, 4, 1);

  ui.options.groupBox->setLayout(ui.options.mainLayout);
}

void QtPack3rWidget::setupDebugGroupBox() {
  ui.debug.groupBox = new QGroupBox(tr("Debug"), this);

  const QString verbosityToolTip =
      tr("Log severity threshold, from least (None) to most (Trace) verbose");
  ui.debug.verbosityLabel = new QLabel(tr("Verbosity"), this);
  ui.debug.verbosityLabel->setToolTip(verbosityToolTip);

  ui.debug.verbosityCombobox = new QComboBox(this);
  ui.debug.verbosityCombobox->setToolTip(verbosityToolTip);
  ui.debug.verbosityCombobox->setMaximumWidth(200);

  for (const auto &level : verbosityLevels) {
    ui.debug.verbosityCombobox->addItem(tr("%1").arg(level));
  }

  ui.debug.verbosityCombobox->setCurrentIndex(
      ui.debug.verbosityCombobox->findText(tr("Default"), Qt::MatchContains));

  ui.debug.shaderDebugCheckbox = new QCheckBox(tr("Shader debug"), this);
  ui.debug.shaderDebugCheckbox->setToolTip(
      tr("Print shader resolution details (Debug verbosity needed)"));

  ui.debug.referenceDebugCheckbox = new QCheckBox(tr("Reference debug"), this);
  ui.debug.referenceDebugCheckbox->setToolTip(
      tr("Print asset resolution details (Info verbosity needed)"));

  ui.debug.verbosityLayout = new QGridLayout;
  ui.debug.verbosityLayout->addWidget(ui.debug.verbosityLabel, 0, 0, 1, 1);
  ui.debug.verbosityLayout->addWidget(ui.debug.verbosityCombobox, 0, 1, 1, 2);

  ui.debug.mainLayout = new QGridLayout;
  ui.debug.mainLayout->addLayout(ui.debug.verbosityLayout, 0, 0, 1, 2);
  ui.debug.mainLayout->addWidget(ui.debug.shaderDebugCheckbox, 0, 2);
  ui.debug.mainLayout->addWidget(ui.debug.referenceDebugCheckbox, 0, 3);

  ui.debug.groupBox->setLayout(ui.debug.mainLayout);
}

void QtPack3rWidget::setupCommandPreviewGroupBox() {
  ui.commandPreview.groupBox = new QGroupBox(tr("Command preview"), this);
  ui.commandPreview.groupBox->setFixedHeight(140);

  ui.commandPreview.commandPreviewField = new QPlainTextEdit(this);
  ui.commandPreview.commandPreviewField->setToolTip(
      tr("Pack3r command to be executed"));
  ui.commandPreview.commandPreviewField->setReadOnly(true);
  ui.commandPreview.commandPreviewField->setFont(MONOSPACE_FONT);
  ui.commandPreview.commandPreviewField->setMaximumHeight(100);

  Q_ASSERT(processHandler != nullptr);

  ui.commandPreview.runButton = new QPushButton(tr("Run Pack3r"), this);
  ui.commandPreview.runButton->setToolTip(tr("Run command with Pack3r"));

  ui.commandPreview.copyButton = new QPushButton(tr("Copy"), this);
  ui.commandPreview.copyButton->setToolTip(
      tr("Copy the current command to clipboard"));

  ui.commandPreview.resetButton = new QPushButton(tr("Reset"), this);
  ui.commandPreview.resetButton->setToolTip(tr("Reset options to defaults"));

  ui.commandPreview.buttonLayout = new QHBoxLayout;
  ui.commandPreview.buttonLayout->addWidget(ui.commandPreview.runButton);
  ui.commandPreview.buttonLayout->addStretch(1);
  ui.commandPreview.buttonLayout->addWidget(ui.commandPreview.copyButton);
  ui.commandPreview.buttonLayout->addWidget(ui.commandPreview.resetButton);

  ui.commandPreview.layout = new QGridLayout;
  ui.commandPreview.layout->addWidget(ui.commandPreview.commandPreviewField, 0,
                                      0);
  ui.commandPreview.layout->addLayout(ui.commandPreview.buttonLayout, 1, 0);

  ui.commandPreview.groupBox->setLayout(ui.commandPreview.layout);
}

void QtPack3rWidget::setupOutputGroupBox() {
  ui.output.groupBox = new QGroupBox(tr("Output"), this);

  ui.output.outputField = new QPlainTextEdit(this);
  ui.output.outputField->setReadOnly(true);
  ui.output.outputField->setFont(MONOSPACE_FONT);
  ui.output.outputField->setWordWrapMode(QTextOption::NoWrap);

  ui.output.wrapCheckbox = new QCheckBox(tr("Wrap lines"), this);
  ui.output.wrapCheckbox->setToolTip(
      tr("Toggle line wrapping for the output display"));

  ui.output.copyButton = new QPushButton(tr("Copy"), this);
  ui.output.copyButton->setToolTip(tr("Copy current output to clipboard"));

  ui.output.clearButton = new QPushButton(tr("Clear"), this);
  ui.output.clearButton->setToolTip(tr("Clear current output"));

  ui.output.buttonLayout = new QHBoxLayout;
  ui.output.buttonLayout->addWidget(ui.output.wrapCheckbox);
  ui.output.buttonLayout->addStretch(1);
  ui.output.buttonLayout->addWidget(ui.output.copyButton);
  ui.output.buttonLayout->addWidget(ui.output.clearButton);

  ui.output.layout = new QGridLayout;
  ui.output.layout->addWidget(ui.output.outputField, 0, 0);
  ui.output.layout->addLayout(ui.output.buttonLayout, 1, 0);

  ui.output.groupBox->setLayout(ui.output.layout);
}

void QtPack3rWidget::setupStatusBar() {
  ui.statusBar.bar = new QStatusBar(this);

  ui.statusBar.statusBarMessage = new QLabel(this);

  ui.statusBar.qtPack3rVersion = new QLabel(PROJECT_VERSION, this);
  ui.statusBar.qtPack3rVersion->setToolTip(tr("QtPack3r version"));
  ui.statusBar.qtPack3rVersion->setMinimumWidth(80);
  ui.statusBar.qtPack3rVersion->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

  ui.statusBar.pack3rVersion = new QLabel("-", this);
  ui.statusBar.pack3rVersion->setToolTip(tr("Pack3r version"));
  ui.statusBar.pack3rVersion->setMinimumWidth(80);
  ui.statusBar.pack3rVersion->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

  ui.statusBar.bar->addWidget(ui.statusBar.statusBarMessage, 1);
  ui.statusBar.bar->addPermanentWidget(ui.statusBar.pack3rVersion);
  ui.statusBar.bar->addPermanentWidget(ui.statusBar.qtPack3rVersion);
}

void QtPack3rWidget::updateCheckbox(const Pack3rOptions option,
                                    const bool isChecked, QLineEdit *field) {
  pack3rCommands[option].first = isChecked;

  // handle output field if 'source' is set
  if (option == SOURCE) {
    updateOutputExtension();
  }

  // if field is passed in, toggle read-only state
  if (field) {
    field->setReadOnly(!isChecked);
  }

  updateCommandPreview();
}

void QtPack3rWidget::updateOptionValue(const Pack3rOptions option,
                                       const QString &value) {
  pack3rCommands[option].second.value = value;
  updateCommandPreview();
}

void QtPack3rWidget::updateComboboxValue(Pack3rOptions option,
                                         const QString &value) {
  pack3rCommands[option].first =
      !value.contains(tr("Default"), Qt::CaseInsensitive);
  pack3rCommands[option].second.value = value;
  updateCommandPreview();
}

// FIXME: this isn't great
void QtPack3rWidget::updateCommandPreview() {
  currentCmd.first.clear();
  currentCmd.second.clear();

  if (!ui.paths.pack3rPathField->text().isEmpty()) {
    currentCmd.first.append(ui.paths.pack3rPathField->text());
  }

  if (!ui.paths.mapPathField->text().isEmpty()) {
    currentCmd.second.append(ui.paths.mapPathField->text());
  }

  for (int i = 0; i < NUM_PACK3R_OPTIONS; i++) {
    const auto cmdkvp = pack3rCommands[i];

    if (!cmdkvp.first) {
      continue;
    }

    currentCmd.second.append(cmdkvp.second.command);

    if (cmdkvp.second.hasValue) {
      currentCmd.second.append(cmdkvp.second.value);
    }
  }

  ui.commandPreview.commandPreviewField->setPlainText(
      currentCmd.first + " " + currentCmd.second.join(" "));
}
