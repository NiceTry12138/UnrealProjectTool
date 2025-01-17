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

    void UpdateInfo(const QString& InInfo);

public:
    void OnDeleteButtonClicked();
    void OnDetailButtonClicked();

Q_SIGNALS:
    void OnDelete(const QString& Info);
    void OnDetail(const QString& Info);

private:
    Ui::GitPathListWidgetItem *ui;

private:
    QString CacheInfo;
};

#endif // GITPATHLISTWIDGETITEM_H
