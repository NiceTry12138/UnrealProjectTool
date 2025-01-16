#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE


class UpdatePluginDialogue;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void Init();
    void BindCallabck();

protected:
    void OnUpdatePluginBtnClicked();
    void OnUpdatePluginDialogueClosed();

private:
    Ui::MainWindow *ui;

    std::shared_ptr<UpdatePluginDialogue> UpdatePluginDialoguePtr;
};
#endif // MAINWINDOW_H
