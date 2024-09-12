#include "qeditorwindow.h"
#include "ui_qeditorwindow.h"
#include "codeeditor.h"
#include "renamewindow.h"
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QLabel>
#if defined(QT_PRINTSUPPORT_LIB)
#include <QtPrintSupport/qtprintsupportglobal.h>
#if QT_CONFIG(printer)
#if QT_CONFIG(printdialog)
#include <QPrintDialog>
#endif // QT_CONFIG(printdialog)
#include <QPrinter>
#endif // QT_CONFIG(printer)
#endif // QT_PRINTSUPPORT_LIB
#include <QFont>
#include <QFontDialog>
#include <QFormLayout>

QEditorWindow::QEditorWindow(QWidget *parent) : QMainWindow(parent) , ui(new Ui::QEditorWindow), renameWindow(new RenameWindow(this, parent))
{
    ui->setupUi(this);
    editor = new CodeEditor(ui->formLayoutWidget);
    ui->formLayout->setWidget(1, QFormLayout::SpanningRole, editor);
    editor->setGeometry(QRect(0, 20, 800, 361));
    connect(ui->actionNew, &QAction::triggered, this, &QEditorWindow::newDocument);
    connect(ui->actionOpen, &QAction::triggered, this, &QEditorWindow::open);
    connect(ui->actionSave, &QAction::triggered, this, &QEditorWindow::save);
    connect(ui->actionSave_As, &QAction::triggered, this, &QEditorWindow::saveAs);
    connect(ui->actionRename, &QAction::triggered, this, &QEditorWindow::rename);
    connect(ui->actionDelete, &QAction::triggered, this, &QEditorWindow::deleteFile);
    connect(ui->actionClose, &QAction::triggered, this, &QEditorWindow::close);
    connect(ui->actionUndo, &QAction::triggered, this, &QEditorWindow::undo);
    connect(ui->actionRedo, &QAction::triggered, this, &QEditorWindow::redo);
    connect(ui->actionCopy, &QAction::triggered, this, &QEditorWindow::copy);
    connect(ui->actionCut, &QAction::triggered, this, &QEditorWindow::cut);
    connect(ui->actionPaste, &QAction::triggered, this, &QEditorWindow::paste);
    connect(ui->actionPrint, &QAction::triggered, this, &QEditorWindow::print);
    connect(ui->actionFont, &QAction::triggered, this, &QEditorWindow::font);
    connect(ui->actionBold, &QAction::triggered, this, &QEditorWindow::boldFont);
    connect(ui->actionItalic, &QAction::triggered, this, &QEditorWindow::italicFont);
    connect(ui->actionUnderline, &QAction::triggered, this, &QEditorWindow::underlineFont);
    connect(ui->actionZoom_In, &QAction::triggered, this, &QEditorWindow::zoomIn);
    connect(ui->actionZoom_Out, &QAction::triggered, this, &QEditorWindow::zoomOut);

#if !defined(QT_PRINTSUPPORT_LIB) || !QT_CONFIG(printer)
    ui->actionPrint->setEnabled(false);
#endif
}

QEditorWindow::~QEditorWindow()
{
    delete ui;
    delete editor;
    delete renameWindow;
}

void QEditorWindow::open()
{
    QString filename = QFileDialog::getOpenFileName(this, "Open the file");
    QFile file(filename);
    currentFile = filename;
    if(!file.open(QIODevice::ReadOnly | QFile::Text))
    {
      QMessageBox::warning(this, "Warning", "Can not open file: " + file.errorString());
      return;
    }
    ui->fileNameLabel->setText(filename.section('/', -1));
    ui->fileNameLabel->setAlignment(Qt::AlignCenter);
    QTextStream in(&file);
    editor->setPlainText(in.readAll());
    ui->actionRename->setEnabled(true);
    ui->actionDelete->setEnabled(true);
    file.close();
}

void QEditorWindow::newDocument()
{
    currentFile.clear();
    ui->fileNameLabel->setText("Untitled");
    ui->fileNameLabel->setAlignment(Qt::AlignCenter);
    editor->setPlainText(QString());
    ui->actionRename->setEnabled(false);
    ui->actionDelete->setEnabled(false);
}

void QEditorWindow::save()
{
    QString filename;
    if(currentFile.isEmpty())
    {
       filename = QFileDialog::getSaveFileName(this, "Save the file");
       currentFile = filename;
    }
    else{
        filename = currentFile;
    }
    QFile file(filename);
    if(!file.open(QIODevice::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this, "Warning", "Can not save the file" + file.errorString());
        return;
    }
    ui->fileNameLabel->setText(filename.section('/', -1));
    ui->fileNameLabel->setAlignment(Qt::AlignCenter);
    QTextStream out(&file);
    out << editor->toPlainText();
    ui->actionRename->setEnabled(true);
    ui->actionDelete->setEnabled(true);

    file.close();
}

void QEditorWindow::saveAs()
{
    QString filename = QFileDialog::getSaveFileName(this, "Save As");
    QFile file(filename);
    if(!file.open(QIODevice::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this, "Warning", "Can not save the file" + file.errorString());
        return;
    }
    currentFile = filename;
    ui->fileNameLabel->setText(filename.section('/', -1));
    ui->fileNameLabel->setAlignment(Qt::AlignCenter);
    QTextStream out(&file);
    out << editor->toPlainText();
    ui->actionRename->setEnabled(true);
    ui->actionDelete->setEnabled(true);

    file.close();
}

void QEditorWindow::rename()
{
    renameWindow->show();
}

void QEditorWindow::deleteFile()
{
    QMessageBox::StandardButton confirm = QMessageBox::question(this, "Delete", "Are you sure?", QMessageBox::Yes | QMessageBox::No);
    if(confirm == QMessageBox::Yes)
    {
        QFile file(currentFile);
        file.remove();
        this->newDocument();
    }
}

void QEditorWindow::close()
{
    QMessageBox::StandardButton confirm = QMessageBox::question(this, "Exit", "Are you sure?", QMessageBox::Yes | QMessageBox::No);
    if(confirm == QMessageBox::Yes)
        QCoreApplication::quit();
}

void QEditorWindow::undo()
{
    editor->undo();
}

void QEditorWindow::redo()
{
    editor->redo();
}

void QEditorWindow::copy()
{
    editor->copy();
}

void QEditorWindow::cut()
{
    editor->cut();
}

void QEditorWindow::paste()
{
    editor->paste();
}

void QEditorWindow::print()
{
#if defined(QT_PRINTSUPPORT_LIB) && QT_CONFIG(printer)
    QPrinter printDevice;
#if QT_CONFIG(printdialog)
    QPrintDialog dialog(&printDevice, this);
    if(dialog.exec() == QDialog::Rejected)
        return;
#endif // QT_CONFIG(printdialog)
    editor->print(&printDevice);
#endif // QT_CONFIG(printer)
}

void QEditorWindow::font()
{
    bool isFontSelected{};
    QFont font = QFontDialog::getFont(&isFontSelected, this);
    if(isFontSelected)
        editor->setFont(font);
}

void QEditorWindow::boldFont(bool isBold)
{
    QFont font;
    font.setBold(isBold);
    editor->setFont(font);
}

void QEditorWindow::italicFont(bool isItalic)
{
    QFont font;
    font.setItalic(isItalic);
    editor->setFont(font);
}

void QEditorWindow::underlineFont(bool isUnderline)
{
    QFont font;
    font.setUnderline(isUnderline);
    editor->setFont(font);
}

void QEditorWindow::zoomIn()
{
    editor->zoomIn();
}

void QEditorWindow::zoomOut()
{
    editor->zoomOut();
}

void QEditorWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    ui->formLayoutWidget->resize(event->size());
}

void QEditorWindow::closeEvent(QCloseEvent* event)
{
    QMessageBox::StandardButton confirm = QMessageBox::question(this, "Exit", "Are you sure?", QMessageBox::Yes | QMessageBox::No | QMessageBox::Save);
    if(confirm == QMessageBox::Yes)
        event->accept();
    else if(confirm == QMessageBox::No)
        event->ignore();
    else
    {
        this->save();
        event->accept();
    }
}
