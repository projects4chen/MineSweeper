QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    MineSweeper.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    MineSweeper.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    Mine_zh_CN.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    bmp/blk1.bmp \
    bmp/blk2.bmp \
    bmp/blk3.bmp \
    bmp/blk4.bmp \
    bmp/blk5.bmp \
    bmp/blk6.bmp \
    bmp/blk7.bmp \
    bmp/block_close.bmp \
    bmp/block_open.bmp \
    bmp/boom.bmp \
    bmp/clickboom.bmp \
    bmp/ele0.bmp \
    bmp/ele1.bmp \
    bmp/ele10.bmp \
    bmp/ele2.bmp \
    bmp/ele3.bmp \
    bmp/ele4.bmp \
    bmp/ele5.bmp \
    bmp/ele6.bmp \
    bmp/ele7.bmp \
    bmp/ele8.bmp \
    bmp/ele9.bmp \
    bmp/keilboom.bmp \
    bmp/look.bmp \
    bmp/lose.bmp \
    bmp/mine_title.bmp \
    bmp/proud.bmp \
    bmp/redflag.bmp \
    bmp/smile.bmp
