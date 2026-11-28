/**
 * @file Main.qml
 * @brief Tabbed host loading the Home and Remote Control UI plugin components.
 */

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QuickKit 1.0

ApplicationWindow {
    id: window
    width: 640
    height: 480
    minimumWidth: 200
    minimumHeight: 250
    visible: true
    title: qsTr("Visual Robot")

    ColumnLayout {
        anchors.fill: parent

        TabBar {
            Layout.fillWidth: true

            TabButton {
                text: qsTr("Home")
                icon.source: null !== $app ? $app.icon("HomePlugin") : ""
                icon.color: "transparent"

                onClicked: {
                    stackLayout.currentIndex = 0
                }
            }

            TabButton {
                text: qsTr("Remote Control")
                icon.source: null !== $app ? $app.icon("RemoteControlPlugin") : ""
                icon.color: "transparent"

                onClicked: {
                    stackLayout.currentIndex = 1
                }
            }
        }

        /**
         * @brief Keeps plugin pages available while tab clicks select the visible page.
         */
        StackLayout {
            id: stackLayout
            Layout.fillWidth: true
            Layout.fillHeight: true

            Loader {
                sourceComponent: null != $app ? $app.qmlComponent("HomePlugin") : null
            }

            Loader {
                sourceComponent: null !== $app ? $app.qmlComponent("RemoteControlPlugin") : null
            }
        }

        MyItem {
            Layout.fillWidth: true
            Layout.preferredHeight: 60
        }

        MyItemControls {
            Layout.fillWidth: true
            Layout.preferredHeight: 60
        }
    }
}
