######################################################################
# Automatically generated by qmake (3.0) Wed Aug 20 11:49:54 2014
######################################################################

TEMPLATE = app
TARGET = mrgui
INCLUDEPATH += .

QT += widgets

# Input
HEADERS += window.h nodes.h hexspinbox.h intelhexmem.h avrinfo.h node-generic.h node-iiab.h node-gim2.h
SOURCES += main.cpp window.cpp hexspinbox.cpp intelhexmem.cpp node-generic.cpp node-iiab.cpp node-gim2.cpp

DEFINES += MRGUI_VERSION=\\\"1.0.3\\\"

RESOURCES = mrgui.qrc

macx {
	QMAKE_MAC_SDK = macosx10.9
	APP_QML_FILES.files = avrdude/i386-apple-darwin11/avrdude-6.3.0
	APP_QML_FILES.path = Contents/MacOS/
	QMAKE_BUNDLE_DATA += APP_QML_FILES
	QMAKE_POST_LINK = macdeployqt mrgui.app -dmg
}

win32 {
	QMAKE_POST_LINK = ./win32post.sh
}
