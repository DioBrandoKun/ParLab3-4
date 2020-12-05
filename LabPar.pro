QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++11

QMAKE_EXTRA_TARGETS += before_build makefilehook

makefilehook.target = $(MAKEFILE)
makefilehook.depends = .beforebuild

PRE_TARGETDEPS += .beforebuild

before_build.target = .beforebuild
before_build.depends = FORCE
before_build.commands = chcp 1251
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    matrix.cpp \
    plane.cpp \
    qcustomplot.cpp \
    rls.cpp \
    taircraft.cpp \
    target.cpp

HEADERS += \
    mainwindow.h \
    matrix.h \
    plane.h \
    qcustomplot.h \
    rls.h \
    taircraft.h \
    target.h

FORMS += \
    mainwindow.ui \
    plane.ui \
    rls.ui

LIBS += -L"B:\Qt\armadillo-10.1.1\examples\lib_win64" -llibopenblas
INCLUDEPATH +="B:\Qt\armadillo-10.1.1\include"
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

