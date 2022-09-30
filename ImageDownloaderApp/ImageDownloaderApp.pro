QT       += core gui network core5compat

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# disables all the APIs deprecated before Qt 6.0.0
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

SOURCES += \
    src/cfoldersetting.cpp \
    src/cpexelsapi.cpp \
    src/cpexelsmainwindow.cpp \
    src/cphoto.cpp \
    src/cphotodownloader.cpp \
    src/cphotopage.cpp \
    src/cresultpage.cpp \
    src/imageviewer.cpp \
    src/main.cpp \
    src/restclient.cpp

HEADERS += \
    src/IParser.h \
    src/cfoldersetting.h \
    src/cpexelsapi.h \
    src/cpexelsmainwindow.h \
    src/cphoto.h \
    src/cphotodownloader.h \
    src/cphotopage.h \
    src/cresultpage.h \
    src/imageviewer.h \
    src/restclient.h \

FORMS += \
    ui/pexelsmainwindow.ui

RESOURCES += \
    resources/resources.qrc

DISTFILES += \
    resources/icons/baseline_folder_white_24dp.ico \
    resources/icons/baseline_folder_white_24dp.png \
    resources/icons/baseline_file_download_white_24dp.ico \
    resources/icons/baseline_file_download_white_24dp.png \
    resources/icons/baseline_search_white_48dp.ico \
    resources/icons/baseline_search_white_48dp.png \
