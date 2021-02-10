/*************************************************************************
 * Copyright (C) 2021  Zineddine Saibi
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *************************************************************************/

import QtQuick 2.6
import QtQuick.Window 2.2
import QtMultimedia 5.0
import Qt.labs.settings 1.0

Window {
    width: 290; height: 210
    minimumWidth: 280 ; minimumHeight: 210
    visible: true
    title: "ZTimer"
    id: root

    Item {
        anchors.fill: parent
        id: rec

        TimeDisplayer {
            id: timeDisplayer
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: 10
            busy: timer.running
            width: parent.width
            height: parent.height/2
            fontSize: setFontSize()

            onIsZeroChanged: {
                if (timeDisplayer.isZero && timer.running && operation.checked) {
                    alarm.play();
                    timeDisplayer.blinkingText = true;
                }
            }

            function setFontSize() {
                let res = (timeDisplayer.height + timeDisplayer.width) / 10
                let max = 160;
                let min = 55;
                if (res < min) return min
                else if (res > max) return max
                return res;
            }
        }

        Row {
            id: controlButtons
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: timeDisplayer.bottom
            width: rec.width
            height: setButtonsHeight()
            property int fontSize: setButtonsFontSize()

            ControlButton {
                id: startButton
                text: qsTr("Start")
                width: rec.width/3
                height: parent.height
                font.pointSize: parent.fontSize
                onClicked: {
                    if (timeDisplayer.isZero && !timer.running && operation.checked)
                        return;
                    start();
                }
            }

            ControlButton {
                id: pauseButton
                text: qsTr("Pause")
                width: rec.width/3
                height: parent.height
                font.pointSize: parent.fontSize
                onClicked: stop()
            }

            ControlButton {
                id: resetButton
                text: qsTr("Reset")
                width: rec.width/3
                height: parent.height
                font.pointSize: parent.fontSize
                onClicked: reset()
            }


            function setButtonsHeight() {
                let min = 40
                let max = 100
                let res = root.height / 8
                if (res < min) return min
                else if (res > max) return max
                return res;
            }

            function setButtonsFontSize() {
                let min = 10;
                let max = 50;
                let res = controlButtons.height / 4
                return res;
            }
        }

        OperationSwitch {
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: controlButtons.bottom
            anchors.topMargin: controlButtons.height / 3
            id: operation
        }
    }


    Timer {
        id: timer
        interval: 99
        running: false
        repeat: true

        onTriggered: {
            if (operation.timer) timeDisplayer.minusTenthSec();
            else timeDisplayer.addTenthSec();
        }
    }

    Audio {
        source: "qrc:/audio/alarm.mp3"
        id: alarm
    }

    Settings {
        id: settings
        property string operationType: "stopWatch"
        property int wSize: root.width
        property int hSize: root.height
    }

    Component.onDestruction: {
        timer.stop();
        alarm.stop();
        settings.operationType = (operation.checked) ? "timer" : "stopWatch"
        settings.wSize = root.width
        settings.hSize = root.height
    }

    Component.onCompleted: {
        if (settings.operationType == "timer") operation.checked = true;
        else operation.checked = false;
        root.width = settings.wSize
        root.height = settings.hSize
    }

    function start() {
        stop();
        timer.start();
    }

    function stop() {
        timer.stop();
        alarm.stop();
        timeDisplayer.blinkingText = false;
    }

    function reset() {
        stop();
        timeDisplayer.toZero();
    }
}
