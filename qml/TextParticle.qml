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

Text {
    property string initialColor: "black"
    property string blinkColor: "red"
    property bool blink: false

    color: initialColor
    font.family: segment7.name
    FontLoader { id: segment7; source: "qrc:/fonts/Segment7Standard.otf"; }

    SequentialAnimation on color {
        loops: Animation.Infinite
        running: blink

        ColorAnimation {
            from: initialColor
            to: blinkColor
            duration: 100
        }

        ColorAnimation {
            from: blinkColor
            to: initialColor
            duration: 300
        }
    }

    onBlinkChanged: {
        if (!blink)
            color = initialColor
    }
}
