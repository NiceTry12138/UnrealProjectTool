#ifndef UPDATEPLUGINPROCESSDIALOGUE_H
#define UPDATEPLUGINPROCESSDIALOGUE_H

#include <QDialog>
#include "QStringListModel"
#include "QTimer"

namespace Ui {
class UpdatePluginProcessDialogue;
}

class UpdatePluginProcessDialogue : public QDialog
{
    Q_OBJECT

public:
    explicit UpdatePluginProcessDialogue(QWidget *parent = nullptr);
    ~UpdatePluginProcessDialogue();

    void StartWithGitPathResources(const QStringList& InGitPathResources);

protected:
    void CloneRepo(const QString &URL);
    void CheckProcess();

    void closeEvent(QCloseEvent *Event) override;

private:
    Ui::UpdatePluginProcessDialogue *ui;

    QStringList GitPathResources;

    QStringListModel Model;
    QStringList ModelContent;

    QTimer Timer;
};

#endif // UPDATEPLUGINPROCESSDIALOGUE_H
