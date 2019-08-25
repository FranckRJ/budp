TEMPLATE = app
CONFIG += c++14 strict_c++
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -Wall -Wextra -pedantic
QMAKE_CXXFLAGS_DEBUG += -g
QMAKE_CXXFLAGS_RELEASE += -O2 -s

INCLUDEPATH += "C:\Program Files (x86)\SFML\include" "C:\Program Files (x86)\nlohmann-json-2.1.1"
LIBS += -L"C:\Program Files (x86)\SFML\lib"

CONFIG(debug, debug|release) {
    LIBS += -lsfml-network-d -lsfml-graphics-d -lsfml-window-d -lsfml-system-d
    CONFIG += console
}

CONFIG(release, debug|release) {
    LIBS += -lsfml-network -lsfml-graphics -lsfml-window -lsfml-system
}

SOURCES += \
    entity.tpp \
    game.cpp \
    gamePlayed.cpp \
    gameStateStack.cpp \
    global.cpp \
    internalServer.cpp \
    main.cpp \
    playState.cpp \
    textureManager.cpp \
    entityBuilder.cpp \
    component.cpp \
    externalServer.cpp \
    serverState.cpp \
    networkTool.cpp

HEADERS += \
    entity.hpp \
    game.hpp \
    gamePlayed.hpp \
    gameState.hpp \
    gameStateStack.hpp \
    global.hpp \
    internalServer.hpp \
    playState.hpp \
    server.hpp \
    textureManager.hpp \
    entityBuilder.hpp \
    component.hpp \
    externalServer.hpp \
    serverState.hpp \
    networkTool.hpp
