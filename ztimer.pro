DEFINES += ZTIMER_VERSION=\\\"0.2\\\"
DEFINES += ZTIMER_WEBSITE=\\\"https://github.com/SZinedine/ZTimer\\\"
QT += quick
CONFIG += c++11
SOURCES += \
        src/main.cpp

RESOURCES += qml.qrc \
    ztimer.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
