#include "grafdatatab.h"
#include "ui_grafdatatab.h"

GrafDataTab::GrafDataTab(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GrafDataTab)
{
    ui->setupUi(this);
}

GrafDataTab::~GrafDataTab()
{
    delete ui;
}

void GrafDataTab::setTableData(DataGraph *dataGraph)
{
    dGraph = dataGraph;

    model = new QStandardItemModel(dGraph->data.size(),2,this);
    model->setHeaderData(0, Qt::Horizontal, "X");
    model->setHeaderData(1, Qt::Horizontal, "Y");
    ui->tableView->setModel(model);

    std::map <double, double> :: iterator it = dGraph->data.begin();
    for (int i = 0; it != dGraph->data.end(); it++, i++)
      {
          QModelIndex index = model->index(i, 0, QModelIndex());
          model->setData(index, (*it).first);

          index = model->index(i, 1, QModelIndex());
          model->setData(index, (*it).second);
      }
}

QLabel &GrafDataTab::getLabelFxFormula()
{
    return *ui->label;
}

void GrafDataTab::on_btSave_clicked()
{
    this->hide();
    QString sFile = QFileDialog::getSaveFileName(this, tr("Save data..."), "", tr("CSV Files (*.csv)"));
    if(!sFile.isEmpty())
    {
        QString textData;
        textData += QString::number(static_cast<double>(dGraph->fX)) + ", ";
        textData += QString::number(static_cast<double>(dGraph->A)) + ", ";
        textData += QString::number(static_cast<double>(dGraph->B)) + ", ";
        textData += QString::number(static_cast<double>(dGraph->C)) + ", ";

        textData += QString::number(static_cast<double>(dGraph->From)) + ", ";
        textData += QString::number(static_cast<double>(dGraph->To)) + ", ";
        textData += QString::number(static_cast<double>(dGraph->Step)) + ", ";
        textData += QString::number(static_cast<double>(dGraph->X)) + ", ";
        textData += QString::number(static_cast<int>(dGraph->progress)) + "\n";

        for(int i = 0; i < ui->tableView->model()->rowCount(); i++){
            textData += model->data(model->index(i,0)).toString();
            textData += ", ";
            textData += model->data(model->index(i,1)).toString();
            textData += "\n";
        }

        QFile csvFile(sFile);
        if(csvFile.open(QIODevice::WriteOnly)) {
            QTextStream out(&csvFile);
            out << textData;
            csvFile.close();
        }
    }
}

void GrafDataTab::on_btLoad_clicked()
{
    model->clear();
    this->hide();

    QString sFile = QFileDialog::getOpenFileName(this, tr("Open data"), "", tr("CSV Files (*.csv)"));
    if(!sFile.isEmpty()){
        QFile csvFile(sFile);
        if(!csvFile.exists() || !csvFile.open(QIODevice::ReadOnly)) {
            return;
        }

        do {
            QString textData;
            QStringList txtList;

            // Параметры формы
            dGraph->fromSave = true;
            textData = csvFile.readLine();
            txtList = textData.split(",");

            if(txtList.count() != 9){
                dGraph->fromSave = false;
                MsgError("Bad data file");
                break;
            }

            dGraph->fX = txtList[0].trimmed().toInt();
            dGraph->A = txtList[1].trimmed().toDouble();
            dGraph->B = txtList[2].trimmed().toDouble();
            dGraph->C = txtList[3].trimmed().toDouble();

            dGraph->From = txtList[4].trimmed().toDouble();
            dGraph->To = txtList[5].trimmed().toDouble();
            dGraph->Step = txtList[6].trimmed().toDouble();
            dGraph->X = txtList[7].trimmed().toDouble();
            dGraph->progress = txtList[8].trimmed().toDouble();

            // Значения таблицы
            dGraph->data.clear();
            while(!csvFile.atEnd()){
                textData = csvFile.readLine();
                txtList = textData.split(",");

                if(txtList.count() != 2){
                    dGraph->fromSave = false;
                    MsgError("Bad data file");
                    break;
                }
                dGraph->data.insert(std::pair<double,double>(txtList[0].trimmed().toDouble(), txtList[1].trimmed().toDouble()));
            }
        } while(0);

        csvFile.close();

        // Построение загрузенного графика
        emit uploadData();
    }
}

void GrafDataTab::MsgError(QString msg)
{
    QMessageBox MsgError;
    MsgError.setText(msg);
    MsgError.setIcon(QMessageBox::Critical);
    MsgError.exec();
}
