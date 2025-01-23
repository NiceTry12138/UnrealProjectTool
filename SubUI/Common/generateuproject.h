#ifndef GENERATEUPROJECT_H
#define GENERATEUPROJECT_H

#include <QDialog>

#include "QTimer"

namespace Ui {
class GenerateUProject;
}

class GenerateUProjectDialogue : public QDialog
{
    Q_OBJECT

public:
    explicit GenerateUProjectDialogue(QWidget *parent = nullptr);
    ~GenerateUProjectDialogue();

protected:
    void Init();
    void Update();

private:
    bool bIsFirstRunning = true;

    QString TargetUProjectFileName;

    int Count = 0;
    QTimer Timer;

    Ui::GenerateUProject *ui;
};

#endif // GENERATEUPROJECT_H
