QT += widgets gui sql

HEADERS       = mainwindow.h \
                mdichild.h \
    connectiondialog.h \
    projectswidget.h \
    redmineinstance.h \
    redmineobjects.h
SOURCES       = main.cpp \
                mainwindow.cpp \
                mdichild.cpp \
    connectiondialog.cpp \
    projectswidget.cpp \
    redmineinstance.cpp \
    redmineobjects.cpp
RESOURCES     = yuliya.qrc

# install
#target.path = $$[QT_INSTALL_EXAMPLES]/mainwindows/mdi
#sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS yuliya.pro images
#sources.path = $$[QT_INSTALL_EXAMPLES]/mainwindows/mdi
#INSTALLS += target sources

#symbian: include($$QT_SOURCE_TREE/examples/symbianpkgrules.pri)
#maemo5: include($$QT_SOURCE_TREE/examples/maemo5pkgrules.pri)

#symbian: warning(This example might not fully work on Symbian platform)
#simulator: warning(This example might not fully work on Simulator platform)

FORMS += \
    connectiondialog.ui
