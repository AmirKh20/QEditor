#ifndef RENAMEWINDOW_H
#define RENAMEWINDOW_H

#include "qeditorwindow.h"
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

class RenameWindow : public QWidget
{
    Q_OBJECT
public:
    explicit RenameWindow(QEditorWindow* qEditorWindow, QWidget *parent = nullptr);
    ~RenameWindow();

private:
    void keyReleaseEvent(QKeyEvent* event) override;
    void rename();
    void cancel();
    QLabel* label;
    QLineEdit* lineEdit;
    QEditorWindow* qEditor;
    QPushButton* okPushButton;
    QPushButton* cancelPushButton;
};

#endif // RENAMEWINDOW_H
