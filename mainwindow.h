#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QTableWidget>
#include <QListWidgetItem>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void updatePrizn();
    void paramsRGB();
    void SKORGB();
    void saveTextFile(QString saveFileName);

private slots:
    void on_btnLoadImage_clicked();

    void on_btnLoadKonturFile_clicked();

    void on_btnClear_clicked();

    void on_btnIncrease_clicked();

    void on_btnDecrease_clicked();

    void on_btnCalculate_clicked();

    void on_btnSaveRezults_clicked();

    void on_tableWidget_itemClicked(QTableWidgetItem *item);

    void on_radioB1_clicked();

    void on_radioB2_clicked();

    void on_radioB3_clicked();

    void on_radioB4_clicked();

    void on_radioB5_clicked();

    void on_radioB6_clicked();

    void on_radioB7_clicked();

    void on_radioB8_clicked();

    void on_radioB9_clicked();

    void on_radioB10_clicked();

    void on_radioB11_clicked();

    void on_radioB12_clicked();

    void on_radioB13_clicked();

    void on_radioB14_clicked();

    void on_radioB15_clicked();

    void on_radioB16_clicked();

    void on_radioB17_clicked();

    void on_radioB18_clicked();

    void on_listWidget_itemClicked(QListWidgetItem *item);

private:
    Ui::MainWindow *ui;
    QPixmap pm;

    QImage inputImage;
    QString inputPath;
    QImage bn;
    int selectedRow, selectedColumn;

    QGraphicsPixmapItem *pPixmapItem;
    QGraphicsScene *scene;
    QString kntFileName;
    QString priznaks[18];
    double dPriznaks[21];
    QStringList nSquares;
    QList<QPolygon> polygonList;
    QList<QPolygon> cntList;
    int selSquare;
};

#endif // MAINWINDOW_H
