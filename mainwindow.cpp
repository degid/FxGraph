#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , doubleValidator(-1000, 1000, 2, this)
{
    ui->setupUi(this);

    tabUi = new GrafDataTab();
    tabUi->setWindowModality(Qt::ApplicationModal);

    QObject::connect(tabUi, SIGNAL(uploadData()), this, SLOT(on_btStart_clicked()));

    setFormData();

    blackPen = new QPen(Qt::black);
    bluePen =  new QPen(Qt::blue);
    blackPen->setWidth(1);
    bluePen->setWidth(2);

    scene = new QGraphicsScene(this);
    scene->setSceneRect(scene->itemsBoundingRect());
    ui->graphicsView->setScene(scene);

    plot = new DataGraph{0, 0, 0, 0, 0, 0, 0, 0, false, {}};
    Fx = new FxFormula;

    flagPause = false;
    flagBreak = false;
}

MainWindow::~MainWindow()
{
    delete plot;
    delete Fx;
    delete ui;
}

void MainWindow::setFormData()
{
    // FIXME реализовать валидатор
    QLocale locale(QLocale::English);
    doubleValidator.setNotation(QDoubleValidator::StandardNotation);
    doubleValidator.setLocale(locale);

    ui->lineEditA->setValidator(&doubleValidator);
    ui->lineEditB->setValidator(&doubleValidator);
    ui->lineEditC->setValidator(&doubleValidator);

    ui->lineEditFrom->setValidator(&doubleValidator);
    ui->lineEditTo->setValidator(&doubleValidator);
    ui->lineEditStep->setValidator(&doubleValidator);

    ui->cbxFunction->addItems(
                {
                    "f(x) = A*(x*x) + B*x + C",             // 0
                    "f(x) = A * sin(x) + B * cos( C*x )",   // 1
                    "f(x) = A*log( B*x )",                  // 2
                    "f(x) = A / ( sin(x*x) * B )"           // 3
                });
}

void MainWindow::getDataFromForm()
{
    plot->fX = ui->cbxFunction->currentIndex();

    plot->A = ui->lineEditA->text().toDouble();
    plot->B = ui->lineEditB->text().toDouble();
    plot->C = ui->lineEditC->text().toDouble();

    plot->From = ui->lineEditFrom->text().toDouble();
    plot->To = ui->lineEditTo->text().toDouble();
    plot->Step = ui->lineEditStep->text().toDouble();

    plot->X = plot->From;
    plot->fromSave = false;

    plot->data.clear();
}

void MainWindow::getDataFromData()
{
    ui->cbxFunction->setCurrentIndex(plot->fX);

    ui->lineEditA->setText(QString::number(static_cast<double>(plot->A)));
    ui->lineEditB->setText(QString::number(static_cast<double>(plot->B)));
    ui->lineEditC->setText(QString::number(static_cast<double>(plot->C)));

    ui->lineEditFrom->setText(QString::number(static_cast<double>(plot->From)));
    ui->lineEditTo->setText(QString::number(static_cast<double>(plot->To)));
    ui->lineEditStep->setText(QString::number(static_cast<double>(plot->Step)));
}

void MainWindow::statusForm(bool status)
{
    ui->cbxFunction->setEnabled(status);

    ui->lineEditA->setEnabled(status);
    ui->lineEditB->setEnabled(status);
    ui->lineEditC->setEnabled(status);

    ui->lineEditFrom->setEnabled(status);
    ui->lineEditTo->setEnabled(status);
    ui->lineEditStep->setEnabled(status);
}

bool MainWindow::checkForm()
{
    if(ui->lineEditA->text().isEmpty()
            || ui->lineEditB->text().isEmpty()
            || ui->lineEditC->text().isEmpty()
            || ui->lineEditFrom->text().isEmpty()
            || ui->lineEditTo->text().isEmpty()
            || ui->lineEditStep->text().isEmpty())
    {
        Msg::Error("Some parameters are not set");
        return false;
    }
    return true;
}

// FIXME реализовать обработку кликов через QGraphicsView
// !!!
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    // FIXME открываь только после завершения вычислений или на паузе
    if(event->pos().x() >= ui->graphicsView->pos().x()
            && event->pos().y() >= ui->graphicsView->pos().y()
            && event->pos().x() <= ui->graphicsView->pos().x() + ui->graphicsView->size().width()
            && event->pos().y() <= ui->graphicsView->pos().y() + ui->graphicsView->size().height()
            ){

        QLabel *lbFxFormula = &tabUi->getLabelFxFormula();
        lbFxFormula->setText(ui->cbxFunction->currentText());

        tabUi->setTableData(plot);
        tabUi->show();
    }
}

void MainWindow::setProgress(int progress, double endStep)
{
    ui->btStart->setText("Progress ("+ QString::number(static_cast<int>((progress / endStep) * 100)) + "%)");
}

void MainWindow::on_btStart_clicked()
{
    scene->clear();

    std::function<double(double, double, double, double)> fcnPtr;

    double fx_X1 = 0, fx_X2 = 0;
    double fx_Y1 = 0, fx_Y2 = 0;


    // FIXME
    // *рисовать график и координаты в разных группах
    // Координаты
    //xAxis = scene->addLine(QLineF(fx_X1, 0, plot->To, 0), *blackPen);
    //yAxis = scene->addLine(QLineF(0, fx_X1, 0, plot->To), *blackPen);

    int progress = 0;
    if(plot->fromSave){
        getDataFromData();

        std::list <PairXY> :: iterator it = plot->data.begin();
        for (int i = 0; it != plot->data.end(); it++, i++) {
            fx_X1 = it->x; fx_Y1 = it->y;
            it++;
            if(it != plot->data.end()){
                fx_X2 = it->x; fx_Y2 =  it->y;
                graphSegment = scene->addLine(QLineF(fx_X1, fx_Y1, fx_X2, fx_Y2), *bluePen);
                it--;
            } else {
                break;
            }
            progress++;
        }
        on_btPause_clicked();

    } else {
        getDataFromForm();
        fx_X1 = plot->From;
    }

    if(flagBreak){
        statusForm(true);
        flagBreak = false;
        flagPause = false;

        ui->btStart->setText("Start");
        ui->btPause->setEnabled(true);

        plot->data.clear();

        return;

    } else {
        if(checkForm()) {
            statusForm(false);
        } else {
            return;
        }

    }

    double allSteps = (abs(plot->From) + abs(plot->To)) / plot->Step;
    setProgress(progress, allSteps);

    fx_X2 = fx_X1 + plot->Step;
    fcnPtr = Fx->getFuncrion(ui->cbxFunction->currentIndex());

    ui->btStart->setEnabled(false);
    do {
        QCoreApplication::processEvents();
        if(flagPause && !flagBreak){
            continue;
        }

        // FIXME прервать если деление на 0
        // FIXME "QPainterPath: Adding point with invalid coordinates, ignoring call"
        fx_Y1 = fcnPtr(fx_X1, plot->A, plot->B, plot->C);
        fx_Y2 = fcnPtr(fx_X2, plot->A, plot->B, plot->C);

        graphSegment = scene->addLine(QLineF(fx_X1, fx_Y1, fx_X2, fx_Y2), *bluePen);

        plot->data.push_back(PairXY{fx_X1, fx_Y1});

        fx_X1 = fx_X2;
        plot->X = fx_X1;

        fx_X2 += plot->Step;
        progress++;
        setProgress(progress, allSteps);

        scene->setSceneRect(scene->itemsBoundingRect());

    } while (!flagBreak && fx_X1 <= plot->To);
    // FIXME
    // не останавливается цикл при закрытии формы на паузе

    ui->btStart->setText("New");
    ui->btStart->setEnabled(true);
    ui->btPause->setEnabled(false);
    flagBreak = true;
}

void MainWindow::on_btPause_clicked()
{
    if(flagPause)
    {
        ui->btPause->setText("Pause");
        flagPause = false;
    }
    else
    {
        ui->btPause->setText("Continue");
        flagPause = true;
    }
}

void MainWindow::on_btBreak_clicked()
{
    flagBreak = true;
}
