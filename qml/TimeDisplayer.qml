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

Item {
    property int ts: 0          // tenth of a second
    property int sec: 0
    property int min: 0
    property int hr: 0
    property int spacing: 0
    property int fontSize: 50
    property bool busy: false
    property bool isZero: (ts == 0 && sec == 0 && min == 0 && hr == 0)
    property bool blinkingText: false

    Row {
        id: row
        spacing: parent.spacing
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter

        TimeTextParticle {
            id: hoursText
            number: hr
            addNum: addHour
            minusNum: minusHour
            condition: timer.running
            blink: blinkingText
            font.pointSize: fontSize
        }

        TextParticle {
            text: ":"
            font.pointSize: fontSize/1.5
            anchors.verticalCenter: hoursText.verticalCenter
            font.family: ""
            blink: blinkingText
        }

        TimeTextParticle {
            id: minutesText
            number: min
            addNum: addMin
            minusNum: minusMin
            condition: timer.running
            blink: blinkingText
            font.pointSize: fontSize
        }

        TimeTextParticle {
            id: secondesText
            number: sec
            addNum: addSec
            minusNum: minusSec
            condition: timer.running
            blink: blinkingText
            font.pointSize: fontSize / 1.5
            anchors.baseline: minutesText.baseline

            TimeTextParticle {
                id: tenthSecondText
                text: String(ts)
                anchors.left: secondesText.right
                anchors.baseline: secondesText.baseline
                number: ts
                addNum: addTenthSec
                minusNum: minusTenthSec
                condition: timer.running
                blink: blinkingText
                font.pointSize: fontSize / 5
            }
        }
    }

    function toZero() {
        ts = 0;
        sec = 0;
        min = 0;
        hr = 0;
        blinkingText = false
    }

    function addTenthSec() {
        if (ts == 9) {
            ts = 0;
            addSec();
        }
        else ts = ts + 1;
    }

    function addSec() {
        if (sec == 59) {
            sec = 0;
            addMin();
        }
        else
            sec = sec +1;
    }

    function addMin() {
        if (min == 59) {
            min = 0;
            addHour();
        }
        else
            min = min + 1;
    }

    function addHour() {
        hr = hr + 1;
    }


    function minusTenthSec() {
        if (ts > 0) {
            ts = ts - 1;
        }
        else if (ts == 0 && !isZero) {
            ts = 9;
            minusSec();
        }
    }

    function minusSec() {
        if (sec > 0) {
            sec = sec - 1;
        }
        else if (sec == 0 && !isZero) {
            sec = 59;
            minusMin();
        }
    }

    function minusMin() {
        if (min != 0) {
            min = min - 1;
        }
        else if (min == 0 && hr != 0) {
            min = 59;
            minusHour();
        }
    }

    function minusHour() {
        if (hr != 0) {
            hr = hr - 1;
        }
    }
}
