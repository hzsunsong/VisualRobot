/**
 * @file Main.qml
 * @brief Direction buttons that update the displayed operation without sending device commands.
 */

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    /**
     * @brief Displays the last clicked direction locally; these controls do not invoke the device API.
     */
    ColumnLayout {
        anchors.centerIn: parent

        RowLayout {
            Item {
                Layout.fillWidth: true
            }

            Button {
                text: qsTr("Forward")

                onClicked: {
                    infoText.text = text
                }
            }

            Item {
                Layout.fillWidth: true
            }
        }

        RowLayout {
            Button {
                text: qsTr("Turn Left")

                onClicked: {
                    infoText.text = text
                }
            }

            Button {
                text: qsTr("Stop")

                onClicked: {
                    infoText.text = text
                }
            }

            Button {
                text: qsTr("Turn Right")

                onClicked: {
                    infoText.text = text
                }
            }
        }

        RowLayout {
            Item {
                Layout.fillWidth: true
            }

            Button {
                text: qsTr("Backward")

                onClicked: {
                    infoText.text = text
                }
            }

            Item {
                Layout.fillWidth: true
            }
        }

        RowLayout {
            Layout.alignment: Qt.AlignHCenter

            Text {
                text: qsTr("Current operation:")
                horizontalAlignment: Text.AlignRight
                Layout.alignment: Qt.AlignRight
            }

            Text {
                id: infoText
                text: qsTr("Null")
            }
        }
    }
}
