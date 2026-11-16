/**
 * @file Main.qml
 * @brief Robot list selection and forwarding of device-open requests.
 */

import QtQuick
import QtQuick.Controls

Item {
    id: root
    /** @brief Reuses translated display labels to group list entries and select their plugin. */
    readonly property string agvRobotName: qsTr("Agv Robot")
    readonly property string humanoidRobotName: qsTr("Humanoid Robot")
    /**
         * @brief Stores the translated plugin key passed to C++; its value must match the plugin name.
         * @note Translating this key can prevent plugin lookup.
         */
    readonly property string agvRobotPluginName: qsTr("AgvRobotPlugin")
    /**
         * @brief Stores the translated humanoid plugin key passed to C++.
         * @note Its translated value must still match the name registered by the plugin.
         */
    readonly property string humanoidRobotPluginName: qsTr("HumanoidRobotPlugin")

    /**
         * @brief Notifies listeners of a clicked entry after its device-open attempt.
         * @param[in] robotName The translated display label; multiple entries share this value.
         */
    signal robotClicked(string robotName)

    ListView {
        id: robotListView
        anchors.fill: parent
        anchors.margins: 12
        clip: true
        currentIndex: -1
        /** @brief Keep a zero-based ID in each entry so repeated robot names remain distinguishable. */
        model: [
            { "id": 0, "name": root.agvRobotName },
            { "id": 1, "name": root.agvRobotName },
            { "id": 2, "name": root.agvRobotName },
            { "id": 3, "name": root.humanoidRobotName },
            { "id": 4, "name": root.humanoidRobotName },
            { "id": 5, "name": root.humanoidRobotName }
        ]

        delegate: ItemDelegate {
            width: robotListView.width
            text: modelData.name
            highlighted: ListView.isCurrentItem

            /** @brief Attempts to open the selected device, then updates selection and emits robotClicked even on failure. */
            onClicked: {
                const robotId = modelData.id
                const robotName = modelData.name
                console.log("Robot clicked: id =", robotId, ", name =", robotName)

                if (!root.openDevice(root.devicePluginName(robotName), robotId)) {
                    console.error("open the device failed!deviceName:", robotName, ", deviceId:", robotId)
                } else {
                    console.log("open the device success.deviceName:", robotName, ", deviceId:", robotId)
                }

                robotListView.currentIndex = index
                root.robotClicked(robotName)
            }
        }
    }

    /**
         * @brief Maps a translated robot display label to its configured plugin key.
         * @param[in] robotName The display label of the selected entry.
         * @return The corresponding plugin key, or an empty string for an unknown label.
         */
    function devicePluginName(robotName : string) : string {
        if (root.agvRobotName === robotName) return root.agvRobotPluginName
        else if (root.humanoidRobotName === robotName) return root.humanoidRobotPluginName

        return ""
    }

    /**
         * @brief Forwards a device-open request to the application context object.
         * @param[in] devicePluginName The plugin registration key.
         * @param[in] deviceId The identifier stored in the list entry.
         * @return The result reported by $app.openDevice().
         */
    function openDevice(devicePluginName : string, deviceId : int) : bool {
        return $app.openDevice(devicePluginName, deviceId)
    }
}
