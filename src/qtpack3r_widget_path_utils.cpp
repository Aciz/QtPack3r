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

// pack3r_widget_filesystem.cpp - filesystem interactions

#include "qtpack3r_widget.h"

#include "dialog.h"
#include "filesystem.h"
#include "preferences.h"

void QtPack3rWidget::findPack3r() {
  const QString path = FileSystem::getPack3rPath(
      preferences.readSetting(Preferences::Settings::PACK3R_PATH).toString());

  if (!path.isEmpty()) {
    ui.paths.pack3rPathField->setText(path);
    preferences.writeSetting(Preferences::Settings::PACK3R_PATH,
                             ui.paths.pack3rPathField->text());
    updateCommandPreview();
    checkPack3rVersion();
  }
}

void QtPack3rWidget::openMap() {
  const QString defaultPath =
      preferences.readSetting(Preferences::Settings::MAPS_PATH).toString();
  ui.paths.defaultMapPathSet = !defaultPath.isEmpty();

  const QString path = FileSystem::getMapPath(defaultPath);

  if (path.isEmpty()) {
    return;
  }

  if (!isValidMapPath(path)) {
    QMessageBox dialog{};
    Dialog::setupMessageBox(dialog, Dialog::INVALID_MAP_PATH);
    dialog.exec();
    return;
  }

  ui.paths.mapPathField->setText(path);
  autoFillOutputPath(path);

  if (!ui.paths.defaultMapPathSet) {
    QMessageBox dialog{};
    Dialog::setupMessageBox(dialog, Dialog::SAVE_MAPS_PATH);

    const int ret = dialog.exec();

    if (ret == QMessageBox::Yes) {
      const bool isPk3Dir = ui.paths.mapPathField->text().contains(
          QDir::toNativeSeparators(".pk3dir/maps/"), Qt::CaseInsensitive);
      auto splits = ui.paths.mapPathField->text().split(NATIVE_PATHSEP);

      // we want the path to end in 'etmain', so strip the ending
      // ../etmain/maps/mapname.map or ../etmain/foo.pk3dir/maps/mapname.map
      for (int i = 0; i < (isPk3Dir ? 3 : 2); i++) {
        splits.removeLast();
      }

      preferences.writeSetting(Preferences::Settings::MAPS_PATH,
                               splits.join(NATIVE_PATHSEP) + NATIVE_PATHSEP);
    }
  }

  updateCommandPreview();
}

void QtPack3rWidget::setOutput() {
  QString path = FileSystem::getOutputPath(
      preferences.readSetting(Preferences::Settings::MAPS_PATH).toString());

  if (path.isEmpty()) {
    return;
  }

  // if we have not selected a map file yet, fill the output field
  if (ui.paths.mapPathField->text().isEmpty()) {
    ui.paths.outputPathField->setText(path);
  } else { // append the default output naming to the directory string
    const auto splits = ui.paths.mapPathField->text().split(NATIVE_PATHSEP);

    path.append(NATIVE_PATHSEP + splits.last());
    replaceMapFileExtension(path);
    ui.paths.outputPathField->setText(path);
  }

  updateCommandPreview();
}

bool QtPack3rWidget::isValidMapPath(const QString &path) {
  const QString mapsDir = QDir::toNativeSeparators("/maps/");
  return path.contains(mapsDir, Qt::CaseInsensitive);
}

void QtPack3rWidget::autoFillOutputPath(const QString &file) const {
  const QString mapsDir = QDir::toNativeSeparators("maps/");
  QString outputPath = file;

  outputPath.remove(mapsDir, Qt::CaseInsensitive);
  replaceMapFileExtension(outputPath);
  ui.paths.outputPathField->setText(outputPath);
}

void QtPack3rWidget::replaceMapFileExtension(QString &str) const {
  const QString ext = ui.options.sourceCheckbox->isChecked() ? ".zip" : ".pk3";

  if (str.endsWith(".map", Qt::CaseInsensitive)) {
    str.replace(".map", ext);
  } else if (str.endsWith(".reg", Qt::CaseInsensitive)) {
    str.replace(".reg", ext);
  }
}

void QtPack3rWidget::updateOutputExtension() const {
  const QString ext = ui.options.sourceCheckbox->isChecked() ? ".zip" : ".pk3";

  // output isn't set yet, or we're pointing to a directory
  if (!ui.paths.outputPathField->text().endsWith(".pk3", Qt::CaseInsensitive) &&
      !ui.paths.outputPathField->text().endsWith(".zip", Qt::CaseInsensitive)) {
    return;
  }

  // call replace() only to the last 4 characters to only change the extension,
  // otherwise we mess up filepath when the output points to a .pk3dir
  QString tmp = ui.paths.outputPathField->text();
  tmp.replace(ui.paths.outputPathField->text().length() - 4, 4, ext);
  ui.paths.outputPathField->setText(tmp);
}
