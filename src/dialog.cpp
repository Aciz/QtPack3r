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

#include "dialog.h"

#include <QGridLayout>
#include <QLabel>

// dialog.cpp - static functions to set up dialogs needed by the application

void Dialog::setupMessageBox(QMessageBox &messageBox, const MessageBox type) {
  switch (type) {
  case INVALID_MAP_PATH:
    setupInvalidMapPathMessageBox(messageBox);
    break;
  case OVERWRITE:
    setupOverwriteMessageBox(messageBox);
    break;
  case SAVE_MAPS_PATH:
    setupSaveMapsPathMessageBox(messageBox);
    break;
  case PACK3R_RUN_ERROR:
    setupPack3rRunErrorMessageBox(messageBox);
    break;
  case RESET_PREFERENCES:
    setupResetPreferencesMessageBox(messageBox);
    break;
  default:
    break;
  }
}

void Dialog::setupInvalidMapPathMessageBox(QMessageBox &messageBox) {
  messageBox.setWindowTitle(tr("Invalid map path"));
  messageBox.setText(tr("Invalid location for a map file!"));
  messageBox.setInformativeText(
      tr("The selected file does not appear to be located in a valid mapping "
         "installation path. Pack3r won't be able to resolve assets correctly, "
         "please move the map file to a valid mapping installation path."));
  messageBox.setIcon(QMessageBox::Critical);
  messageBox.setWindowModality(Qt::ApplicationModal);
}

void Dialog::setupOverwriteMessageBox(QMessageBox &messageBox) {
  messageBox.setWindowTitle(tr("Overwrite existing file?"));
  messageBox.setInformativeText("Do you want to overwrite it?");
  messageBox.setIcon(QMessageBox::Question);
  messageBox.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
  messageBox.setWindowModality(Qt::ApplicationModal);
}

void Dialog::setupSaveMapsPathMessageBox(QMessageBox &messageBox) {
  messageBox.setWindowTitle(tr("Save mapping directory as default?"));
  messageBox.setText(tr("Do you want to save this path as the default mapping "
                        "installation location?"));
  messageBox.setInformativeText(
      tr("You can change this later in preferences."));
  messageBox.setIcon(QMessageBox::Question);
  messageBox.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
  messageBox.setWindowModality(Qt::ApplicationModal);
}

void Dialog::setupPack3rRunErrorMessageBox(QMessageBox &messageBox) {
  messageBox.setWindowTitle(tr("Unable to execute Pack3r command"));
  messageBox.setIcon(QMessageBox::Critical);
  messageBox.setWindowModality(Qt::ApplicationModal);
}

void Dialog::setupResetPreferencesMessageBox(QMessageBox &messageBox) {
  messageBox.setWindowTitle(tr("Reset preferences to defaults?"));
  messageBox.setIcon(QMessageBox::Question);
  messageBox.setText(tr("This will reset all preferences to default values."));
  messageBox.setInformativeText(tr("Are you sure you want to continue?"));
  messageBox.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
  messageBox.setWindowModality(Qt::ApplicationModal);
}
