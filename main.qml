import QtQuick 2.2
//import QtQuick.Dialogs 1.2
import QtQuick.Window 2.12
import Qt.labs.platform 1.1
import QtQuick.Layouts 1.12

Window {
    minimumWidth: 600
    minimumHeight: 480
    id: root
    visible: true
    title: qsTr("Crypt App")
    ColumnLayout {
        id: rootLayout
        anchors.fill: parent
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.left: parent.left
        spacing: 0

        Image {
            id: higherImage
            Layout.preferredHeight: parent.height/2
            Layout.preferredWidth: parent.width
            source: "./1622454358seamless-pattern-svg-background.svg"
            Layout.minimumHeight: 0
            Layout.minimumWidth: 0
        }

        RowLayout {
            id: encryptionLayout
            Layout.preferredHeight: parent.height/2
            spacing:0
            ColumnLayout {
                spacing:0
                id: passwordLayout
                Layout.preferredWidth: 3*parent.width/4

                RowLayout {
                    id: keyInputLayout
                    InputBlock {
                        id: keyInput
                        key.text: "key"
                        key.width: ivInput.key.width
                        maxLength: 32
                        value.width: maxLength*value.font.pointSize
                    }
                }
                RowLayout {
                    id: ivInputLayout
                    InputBlock {
                        id: ivInput
                        key.text: "iv"
                        maxLength: 16
                        value.width: maxLength*value.font.pointSize
                    }
                }
            }
            ColumnLayout{
                Layout.preferredWidth: parent.width/8
                id: encryptionButtonsLayout
                spacing:0
                RowLayout{
                    Button{
                        Layout.fillHeight: parent.height
                        Layout.fillWidth: parent.width
                        id: encryptButton
                        text: "Encrypt"
                        onClicked: {
                            keyInput.value.enabled=false;
                            ivInput.value.enabled=false;
                            if(!isCryptClickedErrornous()){
                                cryptor.setKey(keyInput.value.text);
                                cryptor.setIV(ivInput.value.text);
                                if(!cryptor.encrypt()){
                                    errorDialog.text = cryptor.getError();
                                    errorDialog.visible=true;
                                }
                            }
                            keyInput.value.enabled=true;
                            ivInput.value.enabled=true;

                        }
                    }
                }
                RowLayout{
                    Button{
                        Layout.fillHeight: parent.height
                        Layout.fillWidth: parent.width
                        id: decryptButton
                        text: "Decrypt"
                        onClicked: {
                            keyInput.value.enabled=false;
                            ivInput.value.enabled=false;
                            if(!isCryptClickedErrornous()){
                                cryptor.setKey(keyInput.value.text);
                                cryptor.setIV(ivInput.value.text);
                                if(!cryptor.decrypt()){
                                    errorDialog.text = cryptor.getError();
                                    errorDialog.visible=true;
                                }
                            }
                            keyInput.value.enabled=true;
                            ivInput.value.enabled=true;

                        }
                    }
                }
            }

            ColumnLayout{
                Layout.preferredWidth: parent.width/8
                id: fileButtonsLayout
                spacing:0
                RowLayout{
                    Button{
                        Layout.fillHeight: parent.height
                        Layout.fillWidth: parent.width
                        id: chooseButton
                        text: "Choose file"
                        onClicked: {
                            openDialog.visible=true
                        }
                    }
                }
                RowLayout{
                    Button{
                        Layout.fillHeight: parent.height
                        Layout.fillWidth: parent.width
                        id: saveButton
                        text: "Save to file"
                        onClicked: {
                            saveDialog.visible=true
                        }

                    }
                }
            }
        }
    }

    FileDialog {
        id: saveDialog
        title: "Save Dialog"
        folder: StandardPaths.writableLocation(StandardPaths.DocumentsLocation)
        fileMode: FileDialog.SaveFile
        onAccepted: {
            cryptor.setCryptionDest(file.toString())
        }
    }
    FileDialog {
        id: openDialog
        folder: StandardPaths.writableLocation(StandardPaths.DocumentsLocation)
        onAccepted: {
            cryptor.setCryptionSrc(file.toString())
        }
    }
    MessageDialog {
        id: messageDialog
        title: "wrong input(s)"
    }
    MessageDialog {
        id: errorDialog
        title: "Error(s)"
    }

    function isCryptClickedErrornous(){
        var x=(ivInput.value.text.length != 16)? "Th initialization vector characters must be 16.\n":"";
        if (keyInput.value.text.length != 32) x +="The key characters must be 32.\n";
        if (cryptor.isSrcEmpty()) x +="You must choose a file to be encrypted/decrypted.\n";
        if (cryptor.isDestEmpty()) x +="You must choose a directory to save the encrypted/decrypted file.\n";
        messageDialog.text=x;
        if (ivInput.value.text.length != 16 || keyInput.value.text.length != 32 || cryptor.isSrcEmpty() || cryptor.isDestEmpty())
        {
            messageDialog.visible=true;
            return true;
        }
    }

}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
