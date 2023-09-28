import QtQuick 2.14
Window{
    visible: true
    width: 1280
    height: 640
    property int count: 0
    property var sourceImages: Array.from({length: 8}, (_,i) => "image://Landscape/{i}")
    Image{
        source: sourceImages[count]

    }
    Timer{
        interval: 500
        repeat: true
        running: true
        onTriggered:{
            count++
        }
    }
    Component.onCompleted:{
        console.warn("SOurce images is "+ sourceImages);
    }
}