QT       += core gui
QT += sql


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    bookholderwidget.cpp \
    bookrequestwidget.cpp \
    bookreturnwidget.cpp \
    dbsetup.cpp \
    inventorizecopieswidget.cpp \
    inventorydialog.cpp \
    librarianmode.cpp \
    main.cpp \
    readerinfowidget.cpp \
    readermode.cpp \
    readersignindialog.cpp \
    registrationdialog.cpp \
    requestwidget.cpp \
    widget.cpp

HEADERS += \
    bookholderwidget.h \
    bookrequestwidget.h \
    bookreturnwidget.h \
    dbsetup.h \
    inventorizecopieswidget.h \
    inventorydialog.h \
    librarianmode.h \
    readerinfowidget.h \
    readermode.h \
    readersignindialog.h \
    registrationdialog.h \
    requestwidget.h \
    widget.h

FORMS += \
    bookholderwidget.ui \
    bookrequestwidget.ui \
    bookreturnwidget.ui \
    dbsetup.ui \
    inventorizecopieswidget.ui \
    inventorydialog.ui \
    librarianmode.ui \
    readerinfowidget.ui \
    readermode.ui \
    readersignindialog.ui \
    registrationdialog.ui \
    requestwidget.ui \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
