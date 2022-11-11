 #-------------------------------------------------
#
# Project created by QtCreator 2019-10-11T22:27:23
#
#-------------------------------------------------

QT       += core gui charts
QT += printsupport

QT += widgets

TARGET = LTAReader
TEMPLATE = app
DEFINES -= UNICODE
DEFINES += LTAREADERLIB_EXPORTS
DEFINES += WIN32

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    cchildwindow.cpp \
    cctrlchart.cpp \
    chartview.cpp \
    compareResultForm.cpp \
        main.cpp \
        mainwindow.cpp \
    cmyltarchive.cpp \
    subform.cpp

HEADERS += \
    CMyLinesSeries.h \
    MyValueAxis.h \
    Utility.h \
    cchildwindow.h \
    cctrlchart.h \
    chartview.h \
    compareResultForm.h \
    ltareaderlib.h \
        mainwindow.h \
    cmyltarchive.h \
    subform.h

FORMS += \
        compareResultForm.ui \
        mainwindow.ui \
        subform.ui


target.path = $$[QT_INSTALL_EXAMPLES]/charts/linechart
INSTALLS += target

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.


#win32: LIBS += -L$$PWD/./ -lLTAReaderLib

#INCLUDEPATH += $$PWD/.
#DEPENDPATH += $$PWD/.

RESOURCES += \
    LtaReader.qrc
