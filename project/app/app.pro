include(../defaults.pri)

TEMPLATE = app


SOURCES +=  main.cpp
SOURCES += mainwindow.cpp

HEADERS += mainwindow.h

FORMS    += mainwindow.ui

QT += serialport



CONFIG(debug, debug|release) {
   # message(Debug)
}

CONFIG( debug, debug|release ) {
    # debug
     #message(Debug)


     LIBS += -L../src/debug/
     LIBS += -L../src/
     LIBS +=  -lqTestProjectTemplateAppd
} else {
    # release
    #message(Release)
    LIBS += -L../src/release/
    LIBS += -L../src/
    LIBS +=  -lqTestProjectTemplateApp

}

