QT += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++14

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Controllers/TcpController.cpp \
    Models/ExponentialMachine.cpp \
    Models/Round.cpp \
    Network/GameBrowser.cpp \
    Network/GameServer.cpp \
    Utility/Utility.cpp \
    Widgets/BrowserWidget.cpp \
    Widgets/GameWidget.cpp \
    Widgets/MainWindow.cpp \
    Widgets/RoundWidget.cpp \
    main.cpp

HEADERS += \
    Controllers/AGameController.h \
    Controllers/TcpController.h \
    Models/ExponentialMachine.h \
    Models/IEncryptionMachine.h \
    Models/Round.h \
    Network/GameBrowser.h \
    Network/GameServer.h \
    Utility/Utility.h \
    Widgets/BrowserWidget.h \
    Widgets/GameWidget.h \
    Widgets/MainWindow.h \
    Widgets/RoundWidget.h

RESOURCES += resources.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
