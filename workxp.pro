#-------------------------------------------------
#
# Project created by QtCreator 2018-07-09T21:57:40
#
#-------------------------------------------------

QT       += core gui core-private gui-private svg-private

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = workxp
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    svgdocument.cpp \
    workspace.cpp \
    basespace.cpp \
    rulebar.cpp \
    baseelement.cpp \
    utilities.cpp \
    loader.cpp \
    zoomadjust.cpp \
    document.cpp \
    pixmapelement.cpp \
    elementpicker.cpp \
    fontfamilies.cpp \
    thememanager.cpp \
    dockpanel.cpp \
    connectionform.cpp \
    portselector.cpp \
    preferences.cpp \
    workxpproxystyle.cpp \
    languagemanager.cpp \
    preferencedialog.cpp \
    leftbar.cpp \
    directioncontroller.cpp \
    wsplitter.cpp \
    elementfactory.cpp \
    rectelement.cpp \
    basepicker.cpp \
    drawtool.cpp \
    lineelement.cpp \
    linepicker.cpp \
    bezierelement.cpp \
    bezierpicker.cpp \
    textelement.cpp \
    advtreeview.cpp \
    folderelement.cpp \
    domtreeview.cpp \
    groupelement.cpp

HEADERS += \
        mainwindow.h \
    svgdocument.h \
    workspace.h \
    rulebar.h \
    baseelement.h \
    elementpicker.h \
    utilities.h \
    basespace.h \
    graphicsitemtype.h \
    loader.h \
    zoomadjust.h \
    document.h \
    pixmapelement.h \
    fontfamilies.h \
    thememanager.h \
    dockpanel.h \
    connectionform.h \
    portselector.h \
    preferences.h \
    workxpproxystyle.h \
    languagemanager.h \
    preferencedialog.h \
    leftbar.h \
    directioncontroller.h \
    wsplitter.h \
    elementfactory.h \
    rectelement.h \
    basepicker.h \
    drawtool.h \
    commondef.h \
    lineelement.h \
    linepicker.h \
    bezierelement.h \
    bezierpicker.h \
    textelement.h \
    advtreeview.h \
    folderelement.h \
    domtreeview.h \
    groupelement.h

FORMS += \
        mainwindow.ui \
    zoomadjust.ui \
    connectionform.ui \
    portselector.ui \
    preferencedialog.ui \
    leftbar.ui

include(./QtAwesome/QtAwesome.pri)


include(./QtMaterialWidgets/QtMaterialWidgets.pri)



RESOURCES += \
    resources.qrc \
    style.qrc
TRANSLATIONS += translations/wxp_ar.ts \
                translations/wxp_cs.ts \
                translations/wxp_de.ts \
                translations/wxp_el.ts \
                translations/wxp_en.ts \
                translations/wxp_eo.ts \
                translations/wxp_fr.ts \
                translations/wxp_it.ts \
                translations/wxp_jp.ts \
                translations/wxp_ko.ts \
                translations/wxp_no.ts \
                translations/wxp_ru.ts \
                translations/wxp_sv.ts \
                translations/wxp_zh.ts

DISTFILES += \
    workxp.qmodel
