#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QDoubleValidator>
#include <QGraphicsLineItem>
#include <QTimer>
#include <QDebug>
#include <QMouseEvent>

#include "grapher.h"
#include "grafdatatab.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btPause_clicked();
    void on_btBreak_clicked();
    void on_btStart_clicked();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;

    QPen *blackPen;
    QPen *bluePen;

    QGraphicsLineItem *xAxis;
    QGraphicsLineItem *yAxis;
    QGraphicsLineItem *graphSegment;

    QDoubleValidator doubleValidator;

    DataGraph *plot;
    FxFormula * Fx;

    void setFormData();
    void getDataFromForm();
    void getDataFromData();
    void statusForm(bool);
    void mousePressEvent(QMouseEvent*);
    void setProgress(int, double);

    bool flagPause;
    bool flagBreak;

    GrafDataTab *tabUi;

};
#endif // MAINWINDOW_H
