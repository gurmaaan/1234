#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QGraphicsScene>
#include <QVector>
#include <QImage>
#include <QRgb>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QModelIndex>
#include <QTextStream>
#include <QMessageBox>
#include <math.h>
#include <QDebug>
#include <QTextStream>

//пути изображений
QVector<QVector<QString> > vec(100,QVector<QString>(100));
QList<QRgb> colors;
double PI = 3.1415926526;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //инициализация сцены
    scene = new QGraphicsScene();
    ui->graphicsView->setScene(scene);
    //очистка переменных
    kntFileName = "";
    inputPath = "";
    selectedRow = 0;
    selectedColumn = 0;
    nSquares.clear();
    polygonList.clear();
    cntList.clear();

    for (int i=0; i<18; i++)
    {
        priznaks[i] = "";
    }
    for (int i=0; i<21; i++)
    {
        dPriznaks[i] = 0;
    }
    ui->btnLoadKonturFile->setDisabled(true);
    //инициализация цветов
    colors<<qRgb(255,0,0);
    colors<<qRgb(0,255,0);
    colors<<qRgb(0,0,255);
    colors<<qRgb(255,0,255);
    colors<<qRgb(255,255,0);
    colors<<qRgb(0,255,255);
    colors<<qRgb(127,255,255);
    colors<<qRgb(255,127,255);
    colors<<qRgb(255,255,127);
    colors<<qRgb(127,127,255);
    colors<<qRgb(255,127,127);
    colors<<qRgb(127,255,127);
    colors<<qRgb(64,255,255);
    colors<<qRgb(255,64,255);
    colors<<qRgb(255,255,64);
    colors<<qRgb(64,64,255);
    colors<<qRgb(255,64,64);
    colors<<qRgb(64,255,64);
    selSquare = 0;
    ui->btnCalculate->setVisible(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}
//загрузка исходного изображения(ий)
void MainWindow::on_btnLoadImage_clicked()
{
    QString vName = QFileDialog::getOpenFileName();
    int itcnt=1;//длина списка изображений
    QTableWidgetItem *ti;
    int index=0;
    //установки таблицы
    ui->tableWidget->setColumnCount(1);
    ui->tableWidget->setRowCount(1);
    int rc=ui->tableWidget->rowCount();
    int cc=ui->tableWidget->columnCount();
        for (int i=0;i<rc;++i)
            ui->tableWidget->setRowHeight(i,75);
        for (int i=0;i<rc;++i)
            ui->tableWidget->setColumnWidth(i,100);
        //заполнение таблицы
        for (int i=0;i<rc;++i)
            for (int j=0;j<cc; ++j)
            {
                vec[i][j] = vName;
                pm.load(vName);
                    ti = new QTableWidgetItem;
                    //изображение
                    ti->setData(Qt::DecorationRole,pm.scaled(200,50));
                    //путь
                    ti->setData(Qt::DisplayRole,vec[i][j]);
                    ui->tableWidget->setItem(i,j,ti);
            }
}

//загрузка файла контуров
void MainWindow::on_btnLoadKonturFile_clicked()
{
    kntFileName = QFileDialog::getOpenFileName();
    if(!kntFileName.isNull())
    {
        QFile* file = new QFile(kntFileName);
        if (!file->open(QIODevice::ReadOnly | QIODevice::Text))
        {
            qDebug()<<tr("Невозможно открыть файл");
            return;
        }
        QTextStream in(file);
        int counter = 0;
        while(!in.atEnd()) {
            int state = 1;
            QString line = in.readLine();
            QStringList fields = line.split(".");
            //nSquares<<fields.at(0);
            QPolygon poly;
            QPolygon cnt;
            bool end = true;
            QList<int> points;
            for(int i = 0; i<fields.length(); i++)
            {
                if(state==1 && end) {
                    nSquares<<fields.at(i);
                    end = false;
                    continue;
                }
                if(fields.at(i)=="C") {
                    polygonList<<poly;
                    ui->listWidget->addItem(QString::number(counter));
                    counter++;
                    poly.clear();
                    state = 2;
                    continue;
                }
                if(fields.at(i)==";") {
                    cntList<<cnt;
                    cnt.clear();
                    state = 1;
                    end = true;
                    continue;
                }
                if(fields.at(i).contains(";"))
                {
                    cntList<<cnt;
                    cnt.clear();
                    state = 1;
                    QString var= fields.at(i);
                    nSquares<<var.replace(";","").trimmed();
                    end = false;
                    continue;
                }
                points<<fields.at(i).toInt();
                if(points.size()==2)
                {
                    QPoint p(points.at(0), points.at(1));
                    if(state==1)poly.append(p);
                    if(state==2)cnt.append(p);
                    points.clear();
                }
            }
            //polygonList<<poly;
            //cntList<<cnt;
        }
        file->close();

    }
    QMessageBox msg;
    msg.setText(tr("Выполнено!"));
    msg.exec();
}
//очистка
void MainWindow::on_btnClear_clicked()
{
    ui->tableWidget->clear();
    ui->graphicsView->scene()->clear();
    ui->tableWidget->clearContents();
    ui->tableWidget->setColumnCount(0);
    ui->tableWidget->setRowCount(0);
    kntFileName = "";
    selectedRow = 0;
    selectedColumn = 0;
    nSquares.clear();
    polygonList.clear();
    cntList.clear();

    for (int i=0; i<18; i++)
    {
        priznaks[i] = "";
    }
    for (int i=0; i<21; i++)
    {
        dPriznaks[i] = 0;
    }
    ui->txtParamValue->setText("");
    selSquare = 0;
    ui->listWidget->clear();
}
//уведичить масштаб
void MainWindow::on_btnIncrease_clicked()
{
    ui->graphicsView->scale(1.1,1.1);
}
//уменьшить масштаб
void MainWindow::on_btnDecrease_clicked()
{
    ui->graphicsView->scale(1/1.1,1/1.1);
}
//сохранить текущие результаты
void MainWindow::on_btnSaveRezults_clicked()
{
    QString saveFileName = QFileDialog::getSaveFileName();
    //сохранение в текстовый файл
    saveTextFile(saveFileName);
}
//сохранение текстового файла
void MainWindow::saveTextFile(QString saveFileName)
{
    QFile file( saveFileName );
    if ( file.open(QIODevice::ReadWrite) )
    {
        QTextStream stream( &file );
        stream << tr("Средняя яркость R: ") <<priznaks[0]<< "\r\n";
        stream << tr("Средняя яркость G: ") <<priznaks[1]<< "\r\n";
        stream << tr("Средняя яркость B: ") <<priznaks[2]<< "\r\n";
        stream << tr("СКО R: ") <<priznaks[3]<< "\r\n";
        stream << tr("СКО G: ") <<priznaks[4]<< "\r\n";
        stream << tr("СКО B: ") <<priznaks[5]<< "\r\n";
        stream << tr("Диапазон R: ") <<priznaks[6]<< "\r\n";
        stream << tr("Диапазон G: ") <<priznaks[7]<< "\r\n";
        stream << tr("Диапазон B: ") <<priznaks[8]<< "\r\n";
        stream << tr("Текстур призн МПС R: ") <<priznaks[9]<< "\r\n";
        stream << tr("Текстур призн МПС G: ") <<priznaks[10]<< "\r\n";
        stream << tr("Текстур призн МПС B: ") <<priznaks[11]<< "\r\n";
        stream << tr("Тек пр длины серий R: ") <<priznaks[12]<< "\r\n";
        stream << tr("Тек пр длины серий G: ") <<priznaks[13]<< "\r\n";
        stream << tr("Тек пр длины серий B: ") <<priznaks[14]<< "\r\n";
        stream << tr("Форма: \r\n\r\n") <<priznaks[15]<< "\r\n";
        stream << tr("Площадь: \r\n\r\n") <<priznaks[16]<< "\r\n";
        stream << tr("Периметр: \r\n\r\n") <<priznaks[17]<< "\r\n";

    }
}
//выбор изображения в таблице
void MainWindow::on_tableWidget_itemClicked(QTableWidgetItem *item)
{
    //взять текущую позицию в таблице
    selectedRow = item->row();
    selectedColumn = item->column();
    //загрузить изображение
    pm.load (vec[item->row()][item->column()]);
    pPixmapItem = new QGraphicsPixmapItem(pm);
    //добавить на сцену
    scene->addItem (pPixmapItem);
    pPixmapItem ->setFlags(QGraphicsItem::ItemIsMovable);
    //взять текущее изображение
    inputImage = pPixmapItem->pixmap().toImage();
    //взять текущий путь
    inputPath = vec[item->row()][item->column()];
    //очистить изо контуров
    kntFileName = "";
    ui->btnLoadKonturFile->setDisabled(false);
}
//обработчики радио кнопок
//установка выбранного признака и обновление данных в окне признаков
void MainWindow::on_radioB1_clicked()
{
    updatePrizn();
}

void MainWindow::on_radioB2_clicked()
{
    updatePrizn();
}

void MainWindow::on_radioB3_clicked()
{
    updatePrizn();
}

void MainWindow::on_radioB4_clicked()
{
    updatePrizn();
}

void MainWindow::on_radioB5_clicked()
{
    updatePrizn();
}

void MainWindow::on_radioB6_clicked()
{
    updatePrizn();
}

void MainWindow::on_radioB7_clicked()
{
    updatePrizn();
}

void MainWindow::on_radioB8_clicked()
{
    updatePrizn();
}

void MainWindow::on_radioB9_clicked()
{
    updatePrizn();
}

void MainWindow::on_radioB10_clicked()
{
    updatePrizn();
}

void MainWindow::on_radioB11_clicked()
{
    updatePrizn();
}

void MainWindow::on_radioB12_clicked()
{
    updatePrizn();
}

void MainWindow::on_radioB13_clicked()
{
    updatePrizn();
}

void MainWindow::on_radioB14_clicked()
{
    updatePrizn();
}

void MainWindow::on_radioB15_clicked()
{
    updatePrizn();
}

void MainWindow::on_radioB16_clicked()
{
    updatePrizn();
}

void MainWindow::on_radioB17_clicked()
{
    updatePrizn();
}

void MainWindow::on_radioB18_clicked()
{
    updatePrizn();
}
//обновление окна признаков
void MainWindow::updatePrizn()
{
    QString txtValue = "";
    if(ui->radioB1->isChecked())
    {
        txtValue += tr("Средняя яркость R: ") + priznaks[0] + "\r\n";
    }
    if(ui->radioB2->isChecked())
    {
        txtValue += tr("Средняя яркость G: ") + priznaks[1] + "\r\n";
    }
    if(ui->radioB3->isChecked())
    {
        txtValue += tr("Средняя яркость B: ") + priznaks[2] + "\r\n";
    }
    if(ui->radioB4->isChecked())
    {
        txtValue += tr("СКО R: ") + priznaks[3] + "\r\n";
    }
    if(ui->radioB5->isChecked())
    {
        txtValue += tr("СКО G: ") + priznaks[4] + "\r\n";
    }
    if(ui->radioB6->isChecked())
    {
        txtValue += tr("СКО B: ") + priznaks[5] + "\r\n";
    }
    if(ui->radioB7->isChecked())
    {
        txtValue += tr("Диапазон R: ") + priznaks[6] + "\r\n";
    }
    if(ui->radioB8->isChecked())
    {
        txtValue += tr("Диапазон G: ") + priznaks[7] + "\r\n";
    }
    if(ui->radioB9->isChecked())
    {
        txtValue += tr("Диапазон B: ") + priznaks[8] + "\r\n";
    }
    if(ui->radioB10->isChecked())
    {
        txtValue += tr("Текстур призн МПС R: ") + priznaks[9] + "\r\n";
    }
    if(ui->radioB11->isChecked())
    {
        txtValue += tr("Текстур призн МПС G: ") + priznaks[10] + "\r\n";
    }
    if(ui->radioB12->isChecked())
    {
        txtValue += tr("Текстур призн МПС B: ") + priznaks[11] + "\r\n";
    }
    if(ui->radioB13->isChecked())
    {
        txtValue += tr("Тек пр длины серий R: ") + priznaks[12] + "\r\n";
    }
    if(ui->radioB14->isChecked())
    {
        txtValue += tr("Тек пр длины серий G: ") + priznaks[13] + "\r\n";
    }
    if(ui->radioB15->isChecked())
    {
        txtValue += tr("Тек пр длины серий B: ") + priznaks[14] + "\r\n";
    }
    if(ui->radioB16->isChecked())
    {
        txtValue += tr("Форма: ") + priznaks[15] + "\r\n";
    }
    if(ui->radioB17->isChecked())
    {
        txtValue += tr("Площадь: ") + priznaks[16] + "\r\n";
    }
    if(ui->radioB18->isChecked())
    {
        txtValue += tr("Периметр: ") + priznaks[17] + "\r\n";
    }
    ui->txtParamValue->setText(txtValue);
}
//вычисление параметров
void MainWindow::paramsRGB() {
    //компоненты цвета
    int valueR = 0;
    int valueG = 0;
    int valueB = 0;
    //диапазоны
    int minR = 255;
    int maxR = 0;
    int minG = 255;
    int maxG = 0;
    int minB = 255;
    int maxB = 0;
    //длины серий
    double dsR = 0;
    double dsG = 0;
    double dsB = 0;
    //текстурные признаки
    double texR = 0;
    double texG = 0;
    double texB = 0;

    QPolygon poly = polygonList.at(selSquare);
    for (int x=0; x<poly.length(); x++)
    {
        QPoint p = poly.at(x);
        if(p.x()<inputImage.width() && p.y()<inputImage.height())
        {

            //получение компонент
            QRgb color = inputImage.pixel(p.x(),p.y());
            valueR = qRed(color);
            valueG = qGreen(color);
            valueB = qBlue(color);
            dPriznaks[0] += valueR;
            dPriznaks[1] += valueG;
            dPriznaks[2] += valueB;
            //определение диапазонов
            if(valueR<minR)
            {
                minR = valueR;
            }
            if(valueR>maxR)
            {
                maxR = valueR;
            }
            if(valueG<minG)
            {
                minG = valueG;
            }
            if(valueG>maxG)
            {
                maxG = valueG;
            }
            if(valueB<minB)
            {
                minB = valueB;
            }
            if(valueB>maxB)
            {
                maxB = valueB;
            }
            //длины серий (энтропия)
            dsR += (double)valueR*log((double)valueR);
            dsG += (double)valueG*log((double)valueG);
            dsB += (double)valueB*log((double)valueB);
            //текстурный признак - энергия
            texR += pow((double)valueR,2.0f);
            texG += pow((double)valueG,2.0f);
            texB += pow((double)valueB,2.0f);
        }
    }
    //средняя яркость
    dPriznaks[0] = (double)dPriznaks[0] / ((double)poly.length());
    dPriznaks[1] = (double)dPriznaks[1] / ((double)poly.length());
    dPriznaks[2] = (double)dPriznaks[2] / ((double)poly.length());
    //диапазоны
    dPriznaks[6] = minR;
    dPriznaks[7] = maxR;
    dPriznaks[8] = minG;
    dPriznaks[9] = maxG;
    dPriznaks[10] = minB;
    dPriznaks[11] = maxB;
    //текстурные признаки
    dPriznaks[12] = texR / ((double)poly.length());
    dPriznaks[13] = texG / ((double)poly.length());
    dPriznaks[14] = texB / ((double)poly.length());
    //длины серий
    dPriznaks[15] = dsR / ((double)poly.length());
    dPriznaks[16] = dsG / ((double)poly.length());
    dPriznaks[17] = dsB / ((double)poly.length());
}
//вычисление СКО
void MainWindow::SKORGB() {
    int valueR = 0;
    int valueG = 0;
    int valueB = 0;
    double skoR = 0;
    double skoG = 0;
    double skoB = 0;
    QPolygon poly = polygonList.at(selSquare);
    for (int x=0; x<poly.length(); x++)
    {
        QPoint p = poly.at(x);
        if(p.x()<inputImage.width() && p.y()<inputImage.height())
        {
            QRgb color = inputImage.pixel(p.x(),p.y());
            valueR = qRed(color);
            valueG = qGreen(color);
            valueB = qBlue(color);
            skoR += pow(valueR-dPriznaks[0],2);
            skoG += pow(valueR-dPriznaks[1],2);
            skoB += pow(valueR-dPriznaks[2],2);
        }
    }
    //СКО
    dPriznaks[3] = sqrt(skoR/((double)poly.length()));
    dPriznaks[4] = sqrt(skoG/((double)poly.length()));
    dPriznaks[5] = sqrt(skoB/((double)poly.length()));
}

//кнопка выполнить
void MainWindow::on_btnCalculate_clicked()
{
    if(kntFileName.trimmed().length()==0)
    {
        QMessageBox msg2;
        msg2.setText(tr("Не выбран файл контуров!"));
        msg2.exec();
        return;
    }
    if(inputPath.trimmed().length()==0)
    {
        QMessageBox msg2;
        msg2.setText(tr("Не выбран файл изображения!"));
        msg2.exec();
        return;
    }
    //вычисление параметров
    paramsRGB();
    //вычисление СКО
    SKORGB();
    //сохранение признаков
    priznaks[0] = QString::number(dPriznaks[0]);
    priznaks[1] = QString::number(dPriznaks[1]);
    priznaks[2] = QString::number(dPriznaks[2]);
    priznaks[3] = QString::number(dPriznaks[3]);
    priznaks[4] = QString::number(dPriznaks[4]);
    priznaks[5] = QString::number(dPriznaks[5]);
    priznaks[6] = QString::number(dPriznaks[6]) + " - " + QString::number(dPriznaks[7]);
    priznaks[7] = QString::number(dPriznaks[8]) + " - " + QString::number(dPriznaks[9]);
    priznaks[8] = QString::number(dPriznaks[10]) + " - " + QString::number(dPriznaks[11]);
    priznaks[9] = QString::number(dPriznaks[12]);
    priznaks[10] = QString::number(dPriznaks[13]);
    priznaks[11] = QString::number(dPriznaks[14]);
    priznaks[12] = QString::number(dPriznaks[15]);
    priznaks[13] = QString::number(dPriznaks[16]);
    priznaks[14] = QString::number(dPriznaks[17]);
    //залитьт красным цветом расчитанное изображение в таблице
    ui->tableWidget->item(selectedRow, selectedColumn)->setBackground(Qt::red);
}

void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
    for (int i=0; i<21; i++)
    {
        dPriznaks[i] = 0;
    }
    selSquare = item->text().toInt();
    QImage image;
    image = inputImage.copy();
    priznaks[15] = tr("Множество овалов");
    dPriznaks[19] = 0;
    dPriznaks[20] = 0;
    QPolygon poly = polygonList.at(selSquare);
    QPolygon cnt = cntList.at(selSquare);
    //foreach (QPolygon poly, polygonList) {
    for (int x=0; x<poly.length(); x++)
    {
        QPoint p = poly.at(x);
        if(p.x()<image.width() && p.y()<image.height())
        {
            QRgb color = qRgb(255,0,0);
            color = colors.at(0);
            if(ui->showSquare->isChecked())
            {
                image.setPixel(p.x(),p.y(),color);
            }
            dPriznaks[19] ++;
        }
    }
    for (int x=0; x<cnt.length(); x++)
    {
        QPoint p = cnt.at(x);
        if(p.x()<image.width() && p.y()<image.height())
        {
            QRgb color = qRgb(255,0,0);
            color = colors.at(0);
            if(ui->showContour->isChecked())
            {
                image.setPixel(p.x(),p.y(),color);
            }
            dPriznaks[20]++;
        }
    }
    //}
    priznaks[16] = QString::number(dPriznaks[19]);
    priznaks[17] = QString::number(dPriznaks[20]);

    scene->clear();
    pPixmapItem = new QGraphicsPixmapItem(QPixmap::fromImage(image));
    //добавить на сцену
    scene->addItem (pPixmapItem);
    pPixmapItem ->setFlags(QGraphicsItem::ItemIsMovable);
    on_btnCalculate_clicked();
    updatePrizn();
}
