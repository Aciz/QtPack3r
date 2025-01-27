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

#include <QMessageBox>

class Dialog : public QWidget {
  Q_OBJECT

public:
  enum MessageBox {
    INVALID_MAP_PATH,
    OVERWRITE, // does NOT call setText()
    SAVE_MAPS_PATH,
    PACK3R_RUN_ERROR, // does NOT call setText() nor setInformativeText()
    RESET_PREFERENCES,
    INVALID_PACK3R_BINARY,
  };

  static void setupMessageBox(QMessageBox &messageBox, MessageBox type);

private:
  static void setupInvalidMapPathMessageBox(QMessageBox &messageBox);
  static void setupOverwriteMessageBox(QMessageBox &messageBox);
  static void setupSaveMapsPathMessageBox(QMessageBox &messageBox);
  static void setupPack3rRunErrorMessageBox(QMessageBox &messageBox);
  static void setupResetPreferencesMessageBox(QMessageBox &messageBox);
  static void setupInvalidPack3rBinaryMessageBox(QMessageBox &messageBox);
};
