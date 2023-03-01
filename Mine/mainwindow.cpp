#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    // 长宽
    LENGTH = default_n * BLOCK_HEIGHT;
    HEIGHT = default_n * BLOCK_LENGTH + TITLE_HEIGHT;
    // 窗口大小
    app = this;
    ui->setupUi(this);
    this->setMinimumSize(LENGTH, HEIGHT);
    this->setMaximumSize(LENGTH, HEIGHT);

    // 初始化扫雷棋盘
    mine = new Mine();
    mine->init();
    state = mine->get_state();

    // 时间
    n_time = 0;
    runtime = new QTimer(this);
    connect(runtime,SIGNAL(timeout()),this,SLOT(onSecondAdd()));
    connect(runtime,SIGNAL(timeout()),this,SLOT(update()));

    // 位图
    mine_title = QPixmap(":/bmp/mine_title.bmp");
    block_close = QPixmap(":/bmp/block_close.bmp");
    block_open = QPixmap(":/bmp/block_open.bmp");
    red_flag = QPixmap(":/bmp/redflag.bmp");
    keil_boom = QPixmap(":/bmp/keilboom.bmp");
    click_boom = QPixmap(":/bmp/clickboom.bmp");
    boom = QPixmap(":/bmp/boom.bmp");
    qq_smile = QPixmap(":/bmp/smile.bmp");
    qq_lose = QPixmap(":/bmp/lose.bmp");
    qq_pround = QPixmap(":/bmp/proud.bmp");
    ele_0 = QPixmap(":/bmp/ele0.bmp");
    ele_1 = QPixmap(":/bmp/ele1.bmp");
    ele_2 = QPixmap(":/bmp/ele2.bmp");
    ele_3 = QPixmap(":/bmp/ele3.bmp");
    ele_4 = QPixmap(":/bmp/ele4.bmp");
    ele_5 = QPixmap(":/bmp/ele5.bmp");
    ele_6 = QPixmap(":/bmp/ele6.bmp");
    ele_7 = QPixmap(":/bmp/ele7.bmp");
    ele_8 = QPixmap(":/bmp/ele8.bmp");
    ele_9 = QPixmap(":/bmp/ele9.bmp");
    ele_10 = QPixmap(":/bmp/ele10.bmp");
    blk_1 = QPixmap(":/bmp/blk1.bmp");
    blk_2 = QPixmap(":/bmp/blk2.bmp");
    blk_3 = QPixmap(":/bmp/blk3.bmp");
    blk_4 = QPixmap(":/bmp/blk4.bmp");
    blk_5 = QPixmap(":/bmp/blk5.bmp");
    blk_6 = QPixmap(":/bmp/blk6.bmp");
    blk_7 = QPixmap(":/bmp/blk7.bmp");
//    blk_8 = QPixmap(":/bmp/blk8.bmp");
    // 调整大小
    mine_title = mine_title.scaled(QSize(LENGTH,40));
    block_close = block_close.scaled(QSize(BLOCK_LENGTH,BLOCK_HEIGHT));
    block_open = block_open.scaled(QSize(BLOCK_LENGTH,BLOCK_HEIGHT));
    red_flag = red_flag.scaled(QSize(BLOCK_LENGTH,BLOCK_HEIGHT));
    keil_boom = keil_boom.scaled(QSize(BLOCK_LENGTH,BLOCK_HEIGHT));
    click_boom = click_boom.scaled(QSize(BLOCK_LENGTH,BLOCK_HEIGHT));
    boom = boom.scaled(QSize(BLOCK_LENGTH,BLOCK_HEIGHT));
    blk_1 = blk_1.scaled(QSize(BLOCK_LENGTH,BLOCK_HEIGHT));
    blk_2 = blk_2.scaled(QSize(BLOCK_LENGTH,BLOCK_HEIGHT));
    blk_3 = blk_3.scaled(QSize(BLOCK_LENGTH,BLOCK_HEIGHT));
    blk_4 = blk_4.scaled(QSize(BLOCK_LENGTH,BLOCK_HEIGHT));
    blk_5 = blk_5.scaled(QSize(BLOCK_LENGTH,BLOCK_HEIGHT));
    blk_6 = blk_6.scaled(QSize(BLOCK_LENGTH,BLOCK_HEIGHT));
    blk_7 = blk_7.scaled(QSize(BLOCK_LENGTH,BLOCK_HEIGHT));
//    blk_8 = blk_8.scaled(QSize(BLOCK_LENGTH,BLOCK_HEIGHT));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    myPainter = &painter;

    paintTitle(myPainter);
    paintBoomBlock(myPainter);
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    // 获取用户鼠标点击的位置
    click_x = event->x();
    click_y = event->y();
    // 判断是否点了笑脸
    if(event->buttons()==(Qt::LeftButton)&&(click_x>(int)LENGTH*100/200-11)&&
                 (click_x<(int)LENGTH*100/200+11 +EXPRESSION_LENGTH)&&(click_y>7+TITLE_BEGIN)&&
                 (click_y<7+TITLE_BEGIN+EXPRESSION_HEIGHT))
    {
        // 重新初始化棋盘
        mine->rebuild(mine->get_rows(), mine->get_cols(), mine->get_prop());
        state = mine->get_state();
        // 停止时间
        n_time = 0;
//        runtime->stop();
    } else if (click_y > TITLE_HEIGHT) {
        // 获取坐标
        int real_y = click_x/BLOCK_LENGTH;
        int real_x = (click_y-TITLE_HEIGHT)/BLOCK_HEIGHT;

        // 左键点击
        if(event->buttons()==(Qt::LeftButton)) {
            mine->left_click(real_x, real_y);
        } else if (event->buttons()==(Qt::RightButton)) {
            // 右键点击
            mine->right_click(real_x, real_y);
        }
    }
    // 更新绘制
    update();

    // 测试，打印棋盘
//    mine->print_board();
}

void MainWindow::paintTitle(QPainter *painter)
{
    // 显示标题部分
    painter->drawPixmap(0,TITLE_BEGIN,mine_title,0,0,1000,1000);
    if(mine->get_game_state() == LOSE)
        painter->drawPixmap((int)LENGTH*100/200-11,7+TITLE_BEGIN,qq_lose,0,0,1000,1000);
    else if (mine->get_game_state() == WIN)
        painter->drawPixmap((int)LENGTH*100/200-11,7+TITLE_BEGIN,qq_pround,0,0,1000,1000);
    else
        painter->drawPixmap((int)LENGTH*100/200-11,7+TITLE_BEGIN,qq_smile,0,0,1000,1000);

    // 显示当前雷的数量
    int n_mines = mine->get_mines();
    if (n_mines < 0) {
        painter->drawPixmap((int)(LENGTH*40)/200-30,5+TITLE_BEGIN,ele_10,0,0,1000,1000);
    } else {
        painter->drawPixmap((int)(LENGTH*40)/200-30,5+TITLE_BEGIN,ele_0,0,0,1000,1000);
    }

    switch(n_mines / 10)
    {
    case 0:  painter->drawPixmap((int)(LENGTH*40)/200-30+ELENUM_LENGHT,5+TITLE_BEGIN,ele_0,0,0,1000,1000);break;
    case 1:  painter->drawPixmap((int)(LENGTH*40)/200-30+ELENUM_LENGHT,5+TITLE_BEGIN,ele_1,0,0,1000,1000);break;
    case 2:  painter->drawPixmap((int)(LENGTH*40)/200-30+ELENUM_LENGHT,5+TITLE_BEGIN,ele_2,0,0,1000,1000);break;
    case 3:  painter->drawPixmap((int)(LENGTH*40)/200-30+ELENUM_LENGHT,5+TITLE_BEGIN,ele_3,0,0,1000,1000);break;
    case 4:  painter->drawPixmap((int)(LENGTH*40)/200-30+ELENUM_LENGHT,5+TITLE_BEGIN,ele_4,0,0,1000,1000);break;
    case 5:  painter->drawPixmap((int)(LENGTH*40)/200-30+ELENUM_LENGHT,5+TITLE_BEGIN,ele_5,0,0,1000,1000);break;
    case 6:  painter->drawPixmap((int)(LENGTH*40)/200-30+ELENUM_LENGHT,5+TITLE_BEGIN,ele_6,0,0,1000,1000);break;
    case 7:  painter->drawPixmap((int)(LENGTH*40)/200-30+ELENUM_LENGHT,5+TITLE_BEGIN,ele_7,0,0,1000,1000);break;
    case 8:  painter->drawPixmap((int)(LENGTH*40)/200-30+ELENUM_LENGHT,5+TITLE_BEGIN,ele_8,0,0,1000,1000);break;
    case 9:  painter->drawPixmap((int)(LENGTH*40)/200-30+ELENUM_LENGHT,5+TITLE_BEGIN,ele_9,0,0,1000,1000);break;
    }
    switch (n_mines % 10)
    {
    case 0:  painter->drawPixmap((int)(LENGTH*40)/200-30+ELENUM_LENGHT+ELENUM_LENGHT,5+TITLE_BEGIN,ele_0,0,0,1000,1000);break;
    case 1:  painter->drawPixmap((int)(LENGTH*40)/200-30+ELENUM_LENGHT+ELENUM_LENGHT,5+TITLE_BEGIN,ele_1,0,0,1000,1000);break;
    case 2:  painter->drawPixmap((int)(LENGTH*40)/200-30+ELENUM_LENGHT+ELENUM_LENGHT,5+TITLE_BEGIN,ele_2,0,0,1000,1000);break;
    case 3:  painter->drawPixmap((int)(LENGTH*40)/200-30+ELENUM_LENGHT+ELENUM_LENGHT,5+TITLE_BEGIN,ele_3,0,0,1000,1000);break;
    case 4:  painter->drawPixmap((int)(LENGTH*40)/200-30+ELENUM_LENGHT+ELENUM_LENGHT,5+TITLE_BEGIN,ele_4,0,0,1000,1000);break;
    case 5:  painter->drawPixmap((int)(LENGTH*40)/200-30+ELENUM_LENGHT+ELENUM_LENGHT,5+TITLE_BEGIN,ele_5,0,0,1000,1000);break;
    case 6:  painter->drawPixmap((int)(LENGTH*40)/200-30+ELENUM_LENGHT+ELENUM_LENGHT,5+TITLE_BEGIN,ele_6,0,0,1000,1000);break;
    case 7:  painter->drawPixmap((int)(LENGTH*40)/200-30+ELENUM_LENGHT+ELENUM_LENGHT,5+TITLE_BEGIN,ele_7,0,0,1000,1000);break;
    case 8:  painter->drawPixmap((int)(LENGTH*40)/200-30+ELENUM_LENGHT+ELENUM_LENGHT,5+TITLE_BEGIN,ele_8,0,0,1000,1000);break;
    case 9:  painter->drawPixmap((int)(LENGTH*40)/200-30+ELENUM_LENGHT+ELENUM_LENGHT,5+TITLE_BEGIN,ele_9,0,0,1000,1000);break;
    }

    // 显示时间
    int t = n_time;
    switch  (t / 100)
        {
        case 0:  painter->drawPixmap((int)(LENGTH*163)/200-30,5+TITLE_BEGIN,ele_0,0,0,1000,1000);break;
        case 1:  painter->drawPixmap((int)(LENGTH*163)/200-30,5+TITLE_BEGIN,ele_1,0,0,1000,1000);break;
        case 2:  painter->drawPixmap((int)(LENGTH*163)/200-30,5+TITLE_BEGIN,ele_2,0,0,1000,1000);break;
        case 3:  painter->drawPixmap((int)(LENGTH*163)/200-30,5+TITLE_BEGIN,ele_3,0,0,1000,1000);break;
        case 4:  painter->drawPixmap((int)(LENGTH*163)/200-30,5+TITLE_BEGIN,ele_4,0,0,1000,1000);break;
        case 5:  painter->drawPixmap((int)(LENGTH*163)/200-30,5+TITLE_BEGIN,ele_5,0,0,1000,1000);break;
        case 6:  painter->drawPixmap((int)(LENGTH*163)/200-30,5+TITLE_BEGIN,ele_6,0,0,1000,1000);break;
        case 7:  painter->drawPixmap((int)(LENGTH*163)/200-30,5+TITLE_BEGIN,ele_7,0,0,1000,1000);break;
        case 8:  painter->drawPixmap((int)(LENGTH*163)/200-30,5+TITLE_BEGIN,ele_8,0,0,1000,1000);break;
        case 9:  painter->drawPixmap((int)(LENGTH*163)/200-30,5+TITLE_BEGIN,ele_9,0,0,1000,1000);break;
        }
        switch  (t / 10)
        {
        case 0:  painter->drawPixmap((int)(LENGTH*163)/200-30+ELENUM_LENGHT,5+TITLE_BEGIN,ele_0,0,0,1000,1000);break;
        case 1:  painter->drawPixmap((int)(LENGTH*163)/200-30+ELENUM_LENGHT,5+TITLE_BEGIN,ele_1,0,0,1000,1000);break;
        case 2:  painter->drawPixmap((int)(LENGTH*163)/200-30+ELENUM_LENGHT,5+TITLE_BEGIN,ele_2,0,0,1000,1000);break;
        case 3:  painter->drawPixmap((int)(LENGTH*163)/200-30+ELENUM_LENGHT,5+TITLE_BEGIN,ele_3,0,0,1000,1000);break;
        case 4:  painter->drawPixmap((int)(LENGTH*163)/200-30+ELENUM_LENGHT,5+TITLE_BEGIN,ele_4,0,0,1000,1000);break;
        case 5:  painter->drawPixmap((int)(LENGTH*163)/200-30+ELENUM_LENGHT,5+TITLE_BEGIN,ele_5,0,0,1000,1000);break;
        case 6:  painter->drawPixmap((int)(LENGTH*163)/200-30+ELENUM_LENGHT,5+TITLE_BEGIN,ele_6,0,0,1000,1000);break;
        case 7:  painter->drawPixmap((int)(LENGTH*163)/200-30+ELENUM_LENGHT,5+TITLE_BEGIN,ele_7,0,0,1000,1000);break;
        case 8:  painter->drawPixmap((int)(LENGTH*163)/200-30+ELENUM_LENGHT,5+TITLE_BEGIN,ele_8,0,0,1000,1000);break;
        case 9:  painter->drawPixmap((int)(LENGTH*163)/200-30+ELENUM_LENGHT,5+TITLE_BEGIN,ele_9,0,0,1000,1000);break;
        }
        switch  (t)
        {
        case 0:  painter->drawPixmap((int)(LENGTH*163)/200-30+ELENUM_LENGHT+ELENUM_LENGHT,5+TITLE_BEGIN,ele_0,0,0,1000,1000);break;
        case 1:  painter->drawPixmap((int)(LENGTH*163)/200-30+ELENUM_LENGHT+ELENUM_LENGHT,5+TITLE_BEGIN,ele_1,0,0,1000,1000);break;
        case 2:  painter->drawPixmap((int)(LENGTH*163)/200-30+ELENUM_LENGHT+ELENUM_LENGHT,5+TITLE_BEGIN,ele_2,0,0,1000,1000);break;
        case 3:  painter->drawPixmap((int)(LENGTH*163)/200-30+ELENUM_LENGHT+ELENUM_LENGHT,5+TITLE_BEGIN,ele_3,0,0,1000,1000);break;
        case 4:  painter->drawPixmap((int)(LENGTH*163)/200-30+ELENUM_LENGHT+ELENUM_LENGHT,5+TITLE_BEGIN,ele_4,0,0,1000,1000);break;
        case 5:  painter->drawPixmap((int)(LENGTH*163)/200-30+ELENUM_LENGHT+ELENUM_LENGHT,5+TITLE_BEGIN,ele_5,0,0,1000,1000);break;
        case 6:  painter->drawPixmap((int)(LENGTH*163)/200-30+ELENUM_LENGHT+ELENUM_LENGHT,5+TITLE_BEGIN,ele_6,0,0,1000,1000);break;
        case 7:  painter->drawPixmap((int)(LENGTH*163)/200-30+ELENUM_LENGHT+ELENUM_LENGHT,5+TITLE_BEGIN,ele_7,0,0,1000,1000);break;
        case 8:  painter->drawPixmap((int)(LENGTH*163)/200-30+ELENUM_LENGHT+ELENUM_LENGHT,5+TITLE_BEGIN,ele_8,0,0,1000,1000);break;
        case 9:  painter->drawPixmap((int)(LENGTH*163)/200-30+ELENUM_LENGHT+ELENUM_LENGHT,5+TITLE_BEGIN,ele_9,0,0,1000,1000);break;
        }

}

void MainWindow::paintBoomBlock(QPainter *painter)
{
    int n_rows = mine->get_rows();
    int n_cols = mine->get_cols();
    for (int i = 0; i < n_rows; ++i) {
        for (int j = 0; j < n_cols; ++j) {
            if ((*state)[i][j] == CLOSE) {
                painter->drawPixmap(j*BLOCK_LENGTH,i*BLOCK_HEIGHT+TITLE_HEIGHT,block_close,0,0,BLOCK_LENGTH,BLOCK_HEIGHT);
            } else if ((*state)[i][j] == BOOM) {
                painter->drawPixmap(j*BLOCK_LENGTH,i*BLOCK_HEIGHT+TITLE_HEIGHT,boom,0,0,BLOCK_LENGTH,BLOCK_HEIGHT);
            } else if ((*state)[i][j] == FLAG) {
                painter->drawPixmap(j*BLOCK_LENGTH,i*BLOCK_HEIGHT+TITLE_HEIGHT,red_flag,0,0,BLOCK_LENGTH,BLOCK_HEIGHT);
            } else if ((*state)[i][j] == BLANK) {
                painter->drawPixmap(j*BLOCK_LENGTH,i*BLOCK_HEIGHT+TITLE_HEIGHT,block_open,0,0,BLOCK_LENGTH,BLOCK_HEIGHT);
            } else if ((*state)[i][j] == ONE) {
                painter->drawPixmap(j*BLOCK_LENGTH,i*BLOCK_HEIGHT+TITLE_HEIGHT,blk_1,0,0,BLOCK_LENGTH,BLOCK_HEIGHT);
            } else if ((*state)[i][j] == TWO) {
                painter->drawPixmap(j*BLOCK_LENGTH,i*BLOCK_HEIGHT+TITLE_HEIGHT,blk_2,0,0,BLOCK_LENGTH,BLOCK_HEIGHT);
            } else if ((*state)[i][j] == THREE) {
                painter->drawPixmap(j*BLOCK_LENGTH,i*BLOCK_HEIGHT+TITLE_HEIGHT,blk_3,0,0,BLOCK_LENGTH,BLOCK_HEIGHT);
            } else if ((*state)[i][j] == FOUR) {
                painter->drawPixmap(j*BLOCK_LENGTH,i*BLOCK_HEIGHT+TITLE_HEIGHT,blk_4,0,0,BLOCK_LENGTH,BLOCK_HEIGHT);
            } else if ((*state)[i][j] == FIVE) {
                painter->drawPixmap(j*BLOCK_LENGTH,i*BLOCK_HEIGHT+TITLE_HEIGHT,blk_5,0,0,BLOCK_LENGTH,BLOCK_HEIGHT);
            } else if ((*state)[i][j] == SIX) {
                painter->drawPixmap(j*BLOCK_LENGTH,i*BLOCK_HEIGHT+TITLE_HEIGHT,blk_6,0,0,BLOCK_LENGTH,BLOCK_HEIGHT);
            } else if ((*state)[i][j] == SEVEN) {
                painter->drawPixmap(j*BLOCK_LENGTH,i*BLOCK_HEIGHT+TITLE_HEIGHT,blk_7,0,0,BLOCK_LENGTH,BLOCK_HEIGHT);
            } else if ((*state)[i][j] == EIGHT) {
                painter->drawPixmap(j*BLOCK_LENGTH,i*BLOCK_HEIGHT+TITLE_HEIGHT,blk_8,0,0,BLOCK_LENGTH,BLOCK_HEIGHT);
            }
        }
    }


}

void MainWindow::onSecondAdd()
{
    ++n_time;
}
