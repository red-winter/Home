QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    DataProcessing/DP_DbHandler.cpp \
    DataProcessing/DP_ServerManger.cpp \
    Exchange/CLib/Byteoperation.cpp

HEADERS += \
    Const\const_codes.h \
    mainwindow.h \
    Const\const_html.h \
    Const\const_users.h \
    DataProcessing/DP_DbHandler.h \
    DataProcessing/DP_ServerManger.h \
    Const/const_sql.h \
    Exchange/CLib/crc32.h \
    Exchange/CLib/ByteOperation.h

FORMS += \
    mainwindow.ui



# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += \
   DataProcessing \
   Const \
   Exchange/CLib

windows:{
  RC_ICONS = Display.ico
}


