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

TextParticle {
    property int number: 0
    property bool condition: false
    property var addNum
    property var minusNum

    text: ((number >= 10) ? String(number) : "0" + String(number))

    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        onClicked: {
            if (condition) return;
            if (mouse.button == Qt.RightButton) {
                if (mouse.modifiers == Qt.NoModifier)           funcTimes(minusNum, 1);
                else if (mouse.modifiers == Qt.ControlModifier) funcTimes(minusNum, 3);
                else if (mouse.modifiers == Qt.ShiftModifier)   funcTimes(minusNum, 5);
                else                                            funcTimes(minusNum, 1);
            }
            else {
                if (mouse.modifiers == Qt.NoModifier)           funcTimes(addNum, 1);
                else if (mouse.modifiers == Qt.ControlModifier) funcTimes(addNum, 3);
                else if (mouse.modifiers == Qt.ShiftModifier)   funcTimes(addNum, 5);
                else                                            funcTimes(addNum, 1);
            }
        }
    }

    /* call a function a number of times */
    function funcTimes(func, times) {
        for (let i = 0 ; i < times ; i++)
            func();
    }
}
