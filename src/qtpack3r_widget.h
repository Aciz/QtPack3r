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

#include "pack3r_output_parser.h"
#include "pack3r_process_handler.h"

#include <QApplication>
#include <QButtonGroup>
#include <QCheckBox>
#include <QClipboard>
#include <QComboBox>
#include <QDebug>
#include <QFileDialog>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QMenuBar>
#include <QMessageBox>
#include <QPlainTextEdit>
#include <QPointer>
#include <QPushButton>
#include <QScrollBar>

// Linux users will very likely have a system-wide monospace font set to one
// they prefer, but Windows default is 'Courier New', which looks awful here.
// Use 'Consolas' as default on Windows as it's always available.
// TODO: make this configurable in settings?
#ifdef Q_OS_LINUX
#define MONOSPACE_FONT QFontDatabase::systemFont(QFontDatabase::FixedFont)
#else
#define MONOSPACE_FONT QFont("Consolas")
#endif

class QtPack3rWidget : public QWidget {
  Q_OBJECT

public:
  explicit QtPack3rWidget(QWidget *parent);

public slots:
  void findPack3r();
  void openMap();
  void setOutput();

private:
  enum Pack3rOptions {
    OUTPUT,
    DRYRUN,
    RENAME,
    VERBOSITY,
    LOOSE,
    SOURCE,
    SHADERDEBUG,
    REFDEBUG,
    OVERWRITE,
    INCLUDEPK3,
    NOSCAN,
    NOPACK,
    MODS,

    NUM_PACK3R_OPTIONS // endcap
  };

  // .second indicates whether this option takes in a value
  const QList<QPair<QString, bool>> pack3rOptionStrings = {
      {"-o", true},  {"-d", false}, {"-r", true},   {"-v", true},
      {"-l", false}, {"-s", false}, {"-sd", false}, {"-rd", false},
      {"-f", false}, {"-p", false}, {"-ns", true},  {"-np", true},
      {"-m", true},
  };

  // from least to most verbose
  const QStringList verbosityLevels = {
      "None", "Fatal", "Error", "Warn", "Info (Default)", "Debug", "Trace",
  };

  // user interface building
  QGridLayout *buildUI();
  void setupPathsGroupBox();
  void setupOptionsGroupBox();
  void setupDebugGroupBox();
  void setupCommandPreviewGroupBox();
  void setupOutputGroupBox();

  // connections
  void setupConnections();
  void setupPathsConnections();
  void setupOptionsConnections();
  void setupDebugConnections();
  void setupCommandPreviewConnections();
  void setupOutputConnections();

  // state management
  void updateCommandPreview();
  void updateCheckbox(Pack3rOptions option, bool isChecked,
                      QLineEdit *field = nullptr);
  void updateOptionValue(Pack3rOptions option, const QString &value);
  void updateComboboxValue(Pack3rOptions option, const QString &value);

  void setupCommands();
  void parseOptions() const;
  void setDefaults();

  bool canRunPack3r() const;
  void autoFillOutputPath(const QString &file) const;
  static bool isValidMapPath(const QString &path);
  void replaceMapFileExtension(QString &str) const;
  void updateOutputExtension() const;

  void dragEnterEvent(QDragEnterEvent *event) override;
  void dropEvent(QDropEvent *event) override;

  const QString noScanDefaultStr = "pak1.pk3 pak2.pk3 mp_bin.pk3";
  const QString noPackDefaultStr =
      "pak0.pk3 pak0.pk3dir lights.pk3 sd-mapobjects.pk3 common.pk3 "
      "astro-skies.pk3";

  QClipboard *clipboard{};
  Pack3rProcessHandler *processHandler;
  QPointer<Pack3rOutputParser> outputParser;

  QGridLayout *layout{};

  struct Option {
    QString command;
    bool hasValue{};
    QString value;
  };

  // .first is true if the option is selected
  QList<QPair<bool, Option>> pack3rCommands{};

  // path to Pack3r, options
  QPair<QString, QStringList> currentCmd{};

  struct UIPaths {
    QGroupBox *groupBox{};
    QGridLayout *layout{};

    QLabel *pack3rPathLabel{};
    QLabel *mapPathLabel{};

    QLineEdit *pack3rPathField{};
    QAction *pack3rPathAction{};

    bool defaultMapPathSet{};
    QLineEdit *mapPathField{};
    QAction *mapPathAction{};

    QCheckBox *outputCheckbox{};
    QLineEdit *outputPathField{};
    QAction *outputPathAction{};
  };

  struct UIOptions {
    QGroupBox *groupBox{};
    QGridLayout *checkboxLayout{};
    QGridLayout *mainLayout{};

    QCheckBox *dryRunCheckbox{};
    QCheckBox *looseCheckbox{};
    QCheckBox *sourceCheckbox{};
    QCheckBox *overwriteCheckbox{};
    QCheckBox *includePk3Checkbox{};

    QCheckBox *renameCheckbox{};
    QLineEdit *renameField{};

    QCheckBox *noScanCheckbox{};
    QLineEdit *noScanField{};

    QCheckBox *noPackCheckbox{};
    QLineEdit *noPackField{};

    QCheckBox *modsCheckbox{};
    QLineEdit *modsField{};
  };

  struct UIDebug {
    QGroupBox *groupBox{};
    QGridLayout *verbosityLayout{};
    QGridLayout *mainLayout{};

    QLabel *verbosityLabel{};
    QComboBox *verbosityCombobox{};
    QCheckBox *shaderDebugCheckbox{};
    QCheckBox *referenceDebugCheckbox{};
  };

  struct UICommandPreview {
    QGroupBox *groupBox{};
    QGridLayout *layout{};

    QPlainTextEdit *commandPreviewField{};

    QHBoxLayout *buttonLayout{};

    QPushButton *runButton{};
    QPushButton *copyButton{};
    QPushButton *resetButton{};
  };

  struct UIOutput {
    QGroupBox *groupBox{};
    QGridLayout *layout{};

    QPlainTextEdit *outputField{};

    QHBoxLayout *buttonLayout{};

    QCheckBox *wrapCheckbox{};
    QPushButton *copyButton{};
    QPushButton *clearButton{};
  };

  struct UI {
    UIPaths paths;
    UIOptions options;
    UIDebug debug;
    UICommandPreview commandPreview;
    UIOutput output;
  };

  UI ui{};

private slots:
  void updatePack3rOutput(const QByteArray &data) const;
  void copyFieldToClipboard(const QPlainTextEdit *field) const;
  void resetWidgetState();
};
