#-------------------------------------------------
#
# Project created by QtCreator 2011-06-03T14:28:09
#
#-------------------------------------------------

QT       -= core
QT       -= gui

TARGET = Framework
CONFIG   += console
CONFIG   += opengl
CONFIG   -= app_bundle

TEMPLATE = app

DESTDIR = ../../../bin

QMAKE_CXXFLAGS += -m32

LIBS += -framework SDL -framework SDL_image -framework SDL_image \
    -framework Cocoa \
    ../../../lib/Box2D-2.1.0/lib-OSX/libBox2D.a \
    ../../../lib/irrKlang-1.3.0/macosx-gcc/libirrklang.dylib \
    -lglew32
#    SDLMain.o


INCLUDEPATH += ../../../include \
    ../../../include/tinyxml \
    ../../../include/maploader \
    /Library/Frameworks/SDL.framework/Headers \
    /Library/Frameworks/SDL_image.framework/Headers \
    /Library/Frameworks/SDL_mixer.framework/Headers \
    ../../../lib/irrKlang-1.3.0/include \
    ../../../lib/Box2D-2.1.0/include

SOURCES += \
    ../../../source/TextureManager.cpp \
    ../../../source/ShaderProgram.cpp \
    ../../../source/ShaderManager.cpp \
    ../../../source/Shader.cpp \
    ../../../source/PlayState.cpp \
    ../../../source/PlayPhysics.cpp \
    ../../../source/PlayMapTop.cpp \
    ../../../source/PlayMapPhysics.cpp \
    ../../../source/PlayMapAI.cpp \
    ../../../source/PlayMap.cpp \
    ../../../source/PauseState.cpp \
    ../../../source/MenuState.cpp \
    ../../../source/main.cpp \
    ../../../source/CSprite.cpp \
    ../../../source/CPhysics.cpp \
    ../../../source/CMultiImage.cpp \
    ../../../source/CImage.cpp \
    ../../../source/CGame.cpp \
    ../../../source/CFont.cpp \
    ../../../source/maploader/TMXLoader.cpp \
    ../../../source/maploader/base64.cpp \
    ../../../source/tinyxml/tinyxmlparser.cpp \
    ../../../source/tinyxml/tinyxmlerror.cpp \
    ../../../source/tinyxml/tinyxml.cpp \
    ../../../source/tinyxml/tinystr.cpp

OBJECTIVE_SOURCES += \
    ../../../source/SDLMain.m

HEADERS += \
    ../../../include/TextureManager.h \
    ../../../include/TexRect.h \
    ../../../include/ShaderProgram.h \
    ../../../include/ShaderManager.h \
    ../../../include/Shader.h \
    ../../../include/SDLMain.h \
    ../../../include/PlayState.h \
    ../../../include/PlayPhysics.h \
    ../../../include/PlayMapTop.h \
    ../../../include/PlayMapPhysics.h \
    ../../../include/PlayMapAI.h \
    ../../../include/PlayMap.h \
    ../../../include/PauseState.h \
    ../../../include/MenuState.h \
    ../../../include/CSprite.h \
    ../../../include/CPhysics.h \
    ../../../include/CMultiImage.h \
    ../../../include/CImage.h \
    ../../../include/CGameState.h \
    ../../../include/CGame.h \
    ../../../include/CFont.h \
    ../../../include/maploader/TMXLoader.h \
    ../../../include/maploader/base64.h \
    ../../../include/tinyxml/tinyxml.h \
    ../../../include/tinyxml/tinystr.h \
    ../../../include/Graphics.h

OTHER_FILES += \
    ../../../bin/data/shaders/nightvision.vert
