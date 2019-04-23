/*
 * This example comunicates with the arduino sketch of the same name
 * you can find this corresponding example sketch on the arduino website
 * visit: https://www.arduino.cc/en/Tutorial/BuiltInExamples
 * These examples are included in the Arduino IDE
 * as well as in the Web Editor under Examples/BUILT IN
 * for any issue on the ossia-score side, please report to:
 * https://github.com/OSSIA/score-user-library/issues
 */

import QtQuick 2.0
import Ossia 1.0 as Ossia

Ossia.Serial
{
    function openListening(address) {}
    function closeListening(address) {}

    function onMessage(message) { // evaluated each time a message is received

	//console.log(message);

    return [{ address: "/FSR", value: parseInt(message) }];
    }

    function createTree() {
        return [
        {
            name: "FSR",
            type:  Ossia.Type.Int,
            min: 0,
            max: 1023,
            access: Ossia.Access.Get,
            bounding: Ossia.Bounding.Clip,
            repetition_filter: Ossia.Repetitions.Filtered
        },
        {
            name: "Servo",
            type:  Ossia.Type.Int,
            min: 0,
            max: 150,
            access: Ossia.Access.Set,
            bounding: Ossia.Bounding.Clip,
            request: "s$val",
            repetition_filter: Ossia.Repetitions.Filtered
        },
        {
            name: "Ventilo",
            type:  Ossia.Type.Int,
            min: 0,
            max: 255,
            access: Ossia.Access.Set,
            bounding: Ossia.Bounding.Clip,
            request: "v$val",
            repetition_filter: Ossia.Repetitions.Filtered
        }
        ];
    }
}
