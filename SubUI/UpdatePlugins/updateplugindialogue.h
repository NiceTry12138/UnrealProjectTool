#ifndef UPDATEPLUGINDIALOGUE_H
#define UPDATEPLUGINDIALOGUE_H

#include <QDialog>
#include "QStringListModel"
#include "QStyledItemDelegate"
#include "../../CommonData.h"

class GitPathStyleDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    GitPathStyleDelegate(QObject* parent = nullptr) : QStyledItemDelegate(parent) {}

    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;

protected:
    QPoint GetButtonAhchorPoint(const QRect& Rect) const;

signals:
    void ButtonClicked(const QModelIndex &index);
};

namespace Ui {
class UpdatePluginDialogue;
}

class UpdatePluginDialogue : public QDialog
{
    Q_OBJECT

public:
    explicit UpdatePluginDialogue(QWidget *parent = nullptr);
    ~UpdatePluginDialogue();

// Q_SIGNALS:
//     void OnClosed();

protected:
    void Init();
    void BindCallback();
    void InitData();
    void InitWidgets();

    EErrorType CheckGitPathValid(const QString& GitPath);

protected:
    void OnCloseBtnClicked();
    void OnAddBtnClicked();
    void OnUpdateBtnClicked();
    void OnDeleteListItemClicked(const QModelIndex &index);

private:
    Ui::UpdatePluginDialogue *ui;

private:
    // QObjects
    QStringListModel GitPathListModel;
    std::shared_ptr<GitPathStyleDelegate> GitPathDelegatePtr;

private:
    // Datas
    QStringList GitPathResources;


};

#endif // UPDATEPLUGINDIALOGUE_H
