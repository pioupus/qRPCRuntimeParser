include(../defaults.pri)

TEMPLATE = app
CONFIG += console
QT +=  testlib
QT +=  xml

DEFINES += EXPORT_APPLICATION

HEADERS += autotest.h
HEADERS += testrpcruntimeinterpreter.h
HEADERS += testqstring.h

SOURCES += main.cpp
SOURCES += testrpcruntimeinterpreter.cpp
SOURCES += testqstring.cpp

QPROTOCOL_INTERPRETER_PATH = ../..
include($$QPROTOCOL_INTERPRETER_PATH/qProtocollInterpreter_static.pri)

#message($$PWD/../)
INCLUDEPATH += $$PWD/../libs/googletest/googletest/include
INCLUDEPATH += $$PWD/../libs/googletest/googlemock/include



LIBS += -L$$PWD/../libs/build/googletest/googlemock/
LIBS += -L$$PWD/../libs/build/googletest/googlemock/gtest


LIBS += -lgmock
LIBS += -lgtest



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

COPY_DIR = "$$(UNIXTOOLS)\\cp -r"
#COPY_DIR = "C:\\Program Files\\Git\\usr\\bin\\cp -r"

#message($$COPY_DIR)
#message($$OUT_PWD/)
#message($$PWD/scripts)

#copies scripts into builds

#runtests.commands = $$RUNTEST
#runtests.depends = copydata

copydata.commands = $$COPY_DIR $$PWD/scripts $$OUT_PWD/
first.depends = $(first) copydata

export(first.depends)
export(copydata.commands)

QMAKE_EXTRA_TARGETS +=   first copydata




#
