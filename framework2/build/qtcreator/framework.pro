QT       -= core
QT       -= gui

CONFIG   += console
CONFIG   += opengl
CONFIG   -= app_bundle

TEMPLATE = app

#DESTDIR = ../../../bin

QMAKE_CXXFLAGS += -std=c++11

SOURCES += ../../source/PlayState.cpp \
    ../../source/main.cpp \
    ../../source/InputManager.cpp \
    ../../source/CSprite.cpp \
    ../../source/CImage.cpp \
    ../../source/CGame.cpp \
    ../../source/tmxloader/QuadTreeNode.cpp \
    ../../source/tmxloader/MapLoaderPublic.cpp \
    ../../source/tmxloader/MapLoaderPrivate.cpp \
    ../../source/pugixml/pugixml.cpp \
    ../../source/MenuState.cpp \
    ../../source/TextureManager.cpp \
    ../../source/tinyxml/tinyxmlparser.cpp \
    ../../source/tinyxml/tinyxmlerror.cpp \
    ../../source/tinyxml/tinyxml.cpp \
    ../../source/tinyxml/tinystr.cpp

HEADERS += \
    ../../include/TextureManager.h \
    ../../include/PlayState.h \
    ../../include/InputManager.h \
    ../../include/Graphics.h \
    ../../include/FrameClock.h \
    ../../include/CSprite.h \
    ../../include/ClockHUD.h \
    ../../include/CImage.h \
    ../../include/CGameState.h \
    ../../include/CGame.h \
    ../../include/tmxloader/zlib.h \
    ../../include/tmxloader/zconf.h \
    ../../include/tmxloader/QuadTreeNode.h \
    ../../include/tmxloader/MapObject.h \
    ../../include/tmxloader/MapLoader.h \
    ../../include/tmxloader/Helpers.h \
    ../../include/pugixml/pugixml.hpp \
    ../../include/pugixml/pugiconfig.hpp \
    ../../include/tinyxml/tinyxml.h \
    ../../include/tinyxml/tinystr.h \
    ../../include/MenuState.h \
    ../../include/CAnim.h

###############################################
# CONFIGURATION
###############################################

#CONFIG += copy_dir_files

macx {
    CONFIG_PLATFORM_PATH  = mac
    CONFIG_ICON_EXTENSION = icns

    ICON = $${PWD}/resources/$${CONFIG_PLATFORM_PATH}/app.$${CONFIG_ICON_EXTENSION}
}

win32 {
    CONFIG_PLATFORM_PATH  = win32
    CONFIG_ICON_EXTENSION = png

    RC_FILE = $${PWD}/resources/$${CONFIG_PLATFORM_PATH}/app.rc
}

unix:!macx {
    CONFIG_PLATFORM_PATH  = unix
    CONFIG_ICON_EXTENSION = png

    ICON = $${PWD}/resources/$${CONFIG_PLATFORM_PATH}/app.$${CONFIG_ICON_EXTENSION}
}

###############################################
# SFML LIBRARY
###############################################

INCLUDEPATH += ../../include ../../include/tinyxml ../../include/tmxloader

#INCLUDEPATH += "$${PWD}/library/sfml/include"
LIBS += \#-L"$${PWD}/library/sfml/$${CONFIG_PLATFORM_PATH}/lib" \
    -lsfml-system \
    -lsfml-window \
    -lsfml-graphics \
    -lsfml-audio \
    -lsfml-network \
    -lz

#macx {
#    QMAKE_POST_LINK += $(MKDIR) Frameworks &&
#    QMAKE_POST_LINK += $${QMAKE_COPY} -R $${PWD}/library/sfml/$${CONFIG_PLATFORM_PATH}/lib/* Frameworks
#}

#win32 {
#    QMAKE_POST_LINK += $${QMAKE_COPY} $${PWD}/library/sfml/$${CONFIG_PLATFORM_PATH}/bin/* $(OBJECTS_DIR)
#}

unix:!macx {
    INCLUDEPATH += /usr/local/include/SFML
    LIBS += -lGLEW
}

OTHER_FILES += \
    resources/win32/app.rc
