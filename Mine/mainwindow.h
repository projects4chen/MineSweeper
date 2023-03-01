#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtDebug>
#include <QPainter>
#include <QPixmap>
#include <QTimer>
#include <QMouseEvent>
#include "MineSweeper.h"


//定义绘图常量
const int BLOCK_LENGTH = 41;
const int BLOCK_HEIGHT = 41;
const int ELENUM_LENGHT = 20;
const int EXPRESSION_LENGTH = 24;
const int EXPRESSION_HEIGHT = 24;
const int TITLE_BEGIN = 0;
const int TITLE_HEIGHT = TITLE_BEGIN + 40;
const int PRIMARY_LENGTH = 189;
const int MIDDLE_LENGTH = 336;
const int TOP_LENGTH = 630;


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    MainWindow *app;

    Mine *mine;  // 扫雷对象
    QTimer *runtime;    // 计时
    QPainter *myPainter;    // 绘制
    int click_x, click_y;
    int LENGTH, HEIGHT;
    int n_time; // 时间（秒数）
    int n_rows, n_cols;
    double proportion;
    std::vector<std::vector<macro_state>> *state;

    // 位图
    QPixmap mine_title;
    QPixmap block_close;
    QPixmap block_open;
    QPixmap red_flag;
    QPixmap keil_boom;
    QPixmap click_boom;
    QPixmap boom;
    QPixmap qq_smile;
    QPixmap qq_lose;
    QPixmap qq_pround;
    QPixmap ele_0;
    QPixmap ele_1;
    QPixmap ele_2;
    QPixmap ele_3;
    QPixmap ele_4;
    QPixmap ele_5;
    QPixmap ele_6;
    QPixmap ele_7;
    QPixmap ele_8;
    QPixmap ele_9;
    QPixmap ele_10;
    QPixmap blk_1;
    QPixmap blk_2;
    QPixmap blk_3;
    QPixmap blk_4;
    QPixmap blk_5;
    QPixmap blk_6;
    QPixmap blk_7;
    QPixmap blk_8;

    void paintTitle(QPainter  * painter);
    void paintBoomBlock(QPainter * painter);
    void onSecondAdd();

protected:
    virtual void paintEvent(QPaintEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);

};
#endif // MAINWINDOW_H
