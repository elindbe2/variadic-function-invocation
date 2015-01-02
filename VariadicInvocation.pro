TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11

SOURCES += \
    variadic_invocation_test.cpp

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    easy_bind.h \
    StringUtilities.hpp \
    VariadicInvoker.h

