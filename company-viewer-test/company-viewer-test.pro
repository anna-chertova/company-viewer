QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  \
    ../companydatamodel.cpp \
    tst_companydatamodeltest.cpp

HEADERS += \
    ../companydatamodel.h \
    ../department.h
