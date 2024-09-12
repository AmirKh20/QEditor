#include "renamewindow.h"
#include "qeditorwindow.h"
#include "ui_qeditorwindow.h"
#include <QKeyEvent>
#include <QFile>
#include <QString>
#include <QMessageBox>

RenameWindow::RenameWindow(QEditorWindow* qEditorWindow, QWidget *parent) : QWidget{parent}, label{new QLabel(this)},
    lineEdit{new QLineEdit(this)} , qEditor{qEditorWindow},
    okPushButton{new QPushButton(this)}, cancelPushButton{new QPushButton(this)}
{
    this->setWindowTitle("Rename");
    this->setFixedSize(510, 100);
    label->setText("rename file: ");
    lineEdit->setGeometry(0, 20, 500, 30);
    lineEdit->setPlaceholderText("filename.txt");
    okPushButton->setGeometry(200, 60, 50, 30);
    okPushButton->setText("OK");
    cancelPushButton->setGeometry(260, 60, 100, 30);
    cancelPushButton->setText("Cancel");
    connect(okPushButton, &QAbstractButton::clicked, this, &RenameWindow::rename);
    connect(cancelPushButton, &QAbstractButton::clicked, this, &RenameWindow::cancel);
}

RenameWindow::~RenameWindow()
{
    delete label;
    delete lineEdit;
    delete okPushButton;
}

void RenameWindow::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
    {
        rename();
        return;
    }
    QWidget::keyReleaseEvent(event);
}

void RenameWindow::rename()
{
    QFile file(qEditor->currentFile);
    if(!file.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        QMessageBox::warning(this, "warning", file.errorString());
        return;
    }
    QString newFileName = lineEdit->text();
    lineEdit->clear();
    newFileName = file.fileName().section('/', 0, -2) + '/' + newFileName;
    if(!file.rename(newFileName))
    {
        QMessageBox::warning(this, "Warning", file.errorString());
        return;
    }
    qEditor->currentFile = newFileName;
    qEditor->ui->fileNameLabel->setText(newFileName.section('/', -1));
    qEditor->ui->fileNameLabel->setAlignment(Qt::AlignCenter);
    file.close();
    this->hide();
}

void RenameWindow::cancel()
{
    lineEdit->clear();
    this->hide();
}
