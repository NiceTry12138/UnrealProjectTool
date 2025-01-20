#ifndef GITPATHLISTWIDGETITEM_H
#define GITPATHLISTWIDGETITEM_H

#include <QWidget>

namespace Ui {
class GitPathListWidgetItem;
}

class GitPathListWidgetItem : public QWidget
{
    Q_OBJECT

public:
    explicit GitPathListWidgetItem(QWidget *parent = nullptr);
    ~GitPathListWidgetItem();

    void UpdateInfo(const QString& InInfo, int InID);

public:
    void OnDeleteButtonClicked();
    void OnDetailButtonClicked();

Q_SIGNALS:
    void OnDelete(const QString& Info, int OutID);
    void OnDetail(const QString& Info, int OutID);

private:
    Ui::GitPathListWidgetItem *ui;

private:
    QString CacheInfo;

    int ID = -1;
};

#endif // GITPATHLISTWIDGETITEM_H
