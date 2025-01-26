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

void QtPack3rWidget::setupConnections() {
  setupPathsConnections();
  setupOptionsConnections();
  setupDebugConnections();
  setupCommandPreviewConnections();
  setupOutputConnections();
}

void QtPack3rWidget::setupPathsConnections() {
  connect(ui.paths.pack3rPathAction, &QAction::triggered, this,
          &QtPack3rWidget::findPack3r);

  connect(ui.paths.mapPathAction, &QAction::triggered, this,
          &QtPack3rWidget::openMap);

  connect(ui.paths.outputCheckbox, &QCheckBox::toggled, this, [&] {
    updateCheckbox(OUTPUT, ui.paths.outputCheckbox->isChecked(),
                   ui.paths.outputPathField);
  });
  connect(ui.paths.outputPathAction, &QAction::triggered, this,
          &QtPack3rWidget::setOutput);
  connect(ui.paths.outputPathField, &QLineEdit::textChanged, this,
          [&] { updateOptionValue(OUTPUT, ui.paths.outputPathField->text()); });
}

void QtPack3rWidget::setupOptionsConnections() {
  connect(ui.options.dryRunCheckbox, &QCheckBox::toggled, this, [&] {
    updateCheckbox(DRYRUN, ui.options.dryRunCheckbox->isChecked());
  });
  connect(ui.options.looseCheckbox, &QCheckBox::toggled, this, [&] {
    updateCheckbox(LOOSE, ui.options.looseCheckbox->isChecked());
  });
  connect(ui.options.sourceCheckbox, &QCheckBox::toggled, this, [&] {
    updateCheckbox(SOURCE, ui.options.sourceCheckbox->isChecked());
  });
  connect(ui.options.overwriteCheckbox, &QCheckBox::toggled, this, [&] {
    updateCheckbox(OVERWRITE, ui.options.overwriteCheckbox->isChecked());
  });
  connect(ui.options.includePk3Checkbox, &QCheckBox::toggled, this, [&] {
    updateCheckbox(INCLUDEPK3, ui.options.includePk3Checkbox->isChecked());
  });

  connect(ui.options.renameCheckbox, &QCheckBox::toggled, this, [&] {
    updateCheckbox(RENAME, ui.options.renameCheckbox->isChecked(),
                   ui.options.renameField);
  });
  connect(ui.options.renameField, &QLineEdit::textChanged, this,
          [&] { updateOptionValue(RENAME, ui.options.renameField->text()); });

  connect(ui.options.noScanCheckbox, &QCheckBox::toggled, this, [&] {
    updateCheckbox(NOSCAN, ui.options.noScanCheckbox->isChecked(),
                   ui.options.noScanField);
  });
  connect(ui.options.noScanField, &QLineEdit::textChanged, this,
          [&] { updateOptionValue(NOSCAN, ui.options.noScanField->text()); });

  connect(ui.options.noPackCheckbox, &QCheckBox::toggled, this, [&] {
    updateCheckbox(NOPACK, ui.options.noPackCheckbox->isChecked(),
                   ui.options.noPackField);
  });
  connect(ui.options.noPackField, &QLineEdit::textChanged, this,
          [&] { updateOptionValue(NOPACK, ui.options.noPackField->text()); });

  connect(ui.options.modsCheckbox, &QCheckBox::toggled, this, [&] {
    updateCheckbox(MODS, ui.options.modsCheckbox->isChecked(),
                   ui.options.modsField);
  });
  connect(ui.options.modsField, &QLineEdit::textChanged, this,
          [&] { updateOptionValue(MODS, ui.options.modsField->text()); });
}

void QtPack3rWidget::setupDebugConnections() {
  connect(ui.debug.verbosityCombobox, &QComboBox::currentTextChanged, this,
          [&] {
            updateComboboxValue(
                VERBOSITY, ui.debug.verbosityCombobox->currentText().toLower());
          });
  connect(ui.debug.shaderDebugCheckbox, &QCheckBox::toggled, this, [&] {
    updateCheckbox(SHADERDEBUG, ui.debug.shaderDebugCheckbox->isChecked());
  });
  connect(ui.debug.referenceDebugCheckbox, &QCheckBox::toggled, this, [&] {
    updateCheckbox(REFDEBUG, ui.debug.referenceDebugCheckbox->isChecked());
  });
}

void QtPack3rWidget::setupCommandPreviewConnections() {
  connect(ui.commandPreview.runButton, &QPushButton::released, processHandler,
          [&] {
            if (!canRunPack3r()) {
              return;
            }

            ui.output.outputField->clear();
            processHandler->spawnProcess(currentCmd,
                                         ui.paths.outputPathField->text());
          });

  connect(ui.commandPreview.copyButton, &QPushButton::released, this,
          [&] { copyFieldToClipboard(ui.commandPreview.commandPreviewField); });

  connect(ui.commandPreview.resetButton, &QPushButton::released, this,
          &QtPack3rWidget::resetWidgetState);
}

void QtPack3rWidget::setupOutputConnections() {
  connect(outputParser, &Pack3rOutputParser::pack3rOutputProcessed, this,
          &QtPack3rWidget::updatePack3rOutput);

  connect(ui.output.wrapCheckbox, &QCheckBox::toggled, this, [&] {
    const auto wrapMode = ui.output.wrapCheckbox->isChecked()
                              ? QTextOption::WrapAtWordBoundaryOrAnywhere
                              : QTextOption::NoWrap;
    ui.output.outputField->setWordWrapMode(wrapMode);
    preferences.writeSetting(Preferences::Settings::WRAP_OUTPUT_LINES,
                             ui.output.wrapCheckbox->isChecked());
  });

  connect(ui.output.copyButton, &QPushButton::released, this,
          [&] { copyFieldToClipboard(ui.output.outputField); });

  connect(ui.output.clearButton, &QPushButton::released, this,
          [&] { ui.output.outputField->clear(); });
}
