QT += widgets gui sql charts

HEADERS       = mainwindow.h \
                mdichild.h \
    connectiondialog.h \
    projectpassportdialog.h \
    projectreadinesswidget.h \
    projectswidget.h \
    redmineinstance.h \
    redmineobjects.h \
    charts/mainslice.h \
    charts/donutbreakdownchart.h \
    charts/drilldownslice.h \
    charts/drilldownchart.h \
    metrics/internal/recoverabilitymetrics.h \
    metrics/internal/reliabilitycompliancemetrics.h \
    metrics/internal/resultinginternalmetrics.h \
    metrics/external/resultingexternalmetrics.h \
    projectcodelinesdialog.h \
    projectcodemetricsdialog.h \
    metrics/internal/internalmaturitymetrics.h \
    metrics/external/externalmaturitymetrics.h \
    metrics/internal/internalfaulttolerancemetrics.h \
    metrics/external/externalfaulttolerancemetrics.h
SOURCES       = main.cpp \
                mainwindow.cpp \
                mdichild.cpp \
    connectiondialog.cpp \
    projectpassportdialog.cpp \
    projectreadinesswidget.cpp \
    projectswidget.cpp \
    redmineinstance.cpp \
    redmineobjects.cpp \
    charts/mainslice.cpp \
    charts/donutbreakdownchart.cpp \
    charts/drilldownslice.cpp \
    charts/drilldownchart.cpp \
    metrics/internal/recoverabilitymetrics.cpp \
    metrics/internal/reliabilitycompliancemetrics.cpp \
    metrics/internal/resultinginternalmetrics.cpp \
    metrics/external/resultingexternalmetrics.cpp \
    projectcodelinesdialog.cpp \
    projectcodemetricsdialog.cpp \
    metrics/internal/internalmaturitymetrics.cpp \
    metrics/external/externalmaturitymetrics.cpp \
    metrics/internal/internalfaulttolerancemetrics.cpp \
    metrics/external/externalfaulttolerancemetrics.cpp
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
    connectiondialog.ui \
    projectpassportdialog.ui \
    projectcodelinesdialog.ui \
    projectreadinesswidget.ui
