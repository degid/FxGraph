#ifndef GRAFDATATAB_H
#define GRAFDATATAB_H

#include <QWidget>
#include <QTableView>
#include <QLabel>
#include <QStandardItemModel>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QString>

#include "grapher.h"
#include "msg.h"

namespace Ui {
class GrafDataTab;
}

class GrafDataTab : public QWidget
{
    Q_OBJECT

public:
    explicit GrafDataTab(QWidget *parent = nullptr);
    ~GrafDataTab();
    void setTableData(DataGraph *dataGraph);
    QLabel& getLabelFxFormula();

private slots:
    void on_btSave_clicked();
    void on_btLoad_clicked();

signals:
    void uploadData();


private:
    Ui::GrafDataTab *ui;
    QStandardItemModel *model;
    DataGraph *dGraph;
    void MsgError(QString);
};

#endif // GRAFDATATAB_H
