QT       += core gui widgets

CONFIG   += c++17

TEMPLATE  = app
TARGET    = ClinicaMedica

SOURCES  += \
    main.cpp        \
    mainwindow.cpp  \
    sistema.cpp

HEADERS  += \
    mainwindow.h    \
    sistema.h

FORMS    += \
    mainwindow.ui

DEFINES  += QT_DEPRECATED_WARNINGS
DEFINES  += QT_DISABLE_DEPRECATED_BEFORE=0x060000
RC_FILE = appicon.rc