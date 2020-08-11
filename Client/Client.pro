QT       += core gui network

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
    DataProcessing\DP_ClientManager.cpp \
    main.cpp \
    mainwindow.cpp \
    Widgets/UI_ChatManager.cpp \
    Widgets/UI_EntetingHandler.cpp \
    Widgets/UI_RegistrationManager.cpp

HEADERS += \
    Const\const_codes.h \
    DataProcessing\DP_ClientManager.h \
    mainwindow.h \
    Const\const_html.h \
    Widgets/UI_ChatManager.h \
    Widgets/UI_EntetingHandler.h \
    Widgets/UI_RegistrationManager.h \
    Const/const_users.h

FORMS += \
    mainwindow.ui \
    Widgets/UI_ChatManager.ui \
    Widgets/UI_EntetingHandler.ui \
    Widgets/UI_RegistrationManager.ui

INCLUDEPATH += \
    Const \
    Widgets \
    DataProcessing

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Res/client_files.qrc


windows:{
  RC_ICONS = Display.ico
}
