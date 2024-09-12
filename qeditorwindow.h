#ifndef QEDITORWINDOW_H
#define QEDITORWINDOW_H

#include <QMainWindow>
#include "codeeditor.h"

QT_BEGIN_NAMESPACE
namespace Ui { class QEditorWindow; }
QT_END_NAMESPACE

class RenameWindow;

class QEditorWindow : public QMainWindow
{
    Q_OBJECT
    friend class RenameWindow;

public:
    QEditorWindow(QWidget *parent = nullptr);
    ~QEditorWindow();

private slots:
    void open();
    void newDocument();
    void save();
    void saveAs();
    void rename();
    void deleteFile();
    void close();
    void undo();
    void redo();
    void copy();
    void cut();
    void paste();
    void print();
    void font();
    void boldFont(bool isBold);
    void italicFont(bool isItalic);
    void underlineFont(bool isUnderline);
    void zoomIn();
    void zoomOut();

private:
    void resizeEvent(QResizeEvent* event) override;
    void closeEvent(QCloseEvent* event) override;
    Ui::QEditorWindow *ui;
    QString currentFile;
    CodeEditor* editor;
    RenameWindow* renameWindow;
};

#endif // QEDITORWINDOW_H
