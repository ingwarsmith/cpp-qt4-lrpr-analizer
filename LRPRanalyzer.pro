#-------------------------------------------------
#
# Project created by QtCreator 2016-01-16T10:28:12
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LRPRanalyzer
TEMPLATE = app


SOURCES += main.cpp\
        mainwnd.cpp \
    trapdataforresoursesestimation.cpp \
    allocatetype.cpp \
    stratum.cpp \
    well.cpp \
    projectmanager.cpp \
    project.cpp \
    structure.cpp \
    specialwidget.cpp \
    trap.cpp \
    projecttreewidgetitem.cpp \
    calculationthread.cpp \
    realization.cpp \
    probability_pxx.cpp \
    extendedprobability_pxx.cpp \
    importopenedfiledlg.cpp \
    projecttreewidget.cpp \
    projecttreewidgetitemnew.cpp

HEADERS  += mainwnd.h \
    trapdataforresoursesestimation.h \
    allocatetype.h \
    stratum.h \
    well.h \
    projectmanager.h \
    project.h \
    structure.h \
    specialwidget.h \
    trap.h \
    projecttreewidgetitem.h \
    calculationthread.h \
    realization.h \
    probability_pxx.h \
    extendedprobability_pxx.h \
    importopenedfiledlg.h \
    projecttreewidget.h \
    projecttreewidgetitemnew.h

FORMS    += mainwnd.ui \
    importopenedfiledlg.ui

#DISTFILES += \
#    structure.h.old \
#    structure.cpp.old

QMAKE_LIBS += -lgomp -lpthread
QMAKE_CXXFLAGS += -msse3 -fopenmp
