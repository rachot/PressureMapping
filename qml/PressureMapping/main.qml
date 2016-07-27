import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Window 2.0
import QtQml 2.2
import QtQuick.Extras 1.4
import QtQuick.Controls.Styles 1.1
import QtQml.Models 2.3
import Qt.labs.controls 1.0
import Qt.labs.templates 1.0
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0
import QtQuick.Controls 1.4

import QtQuick 2.2
import QtQuick.Window 2.1
import QtGraphicalEffects 1.0
import QtQml 2.2
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1
import QtQuick.Layouts 1.0


Rectangle{
    id : window1
    width: 1000
    height: 1000

    gradient: Gradient {
        GradientStop {
            position: 0
            color: "#494646"
        }

        GradientStop {
            position: 0.98
            color: "#000000"
        }
    }


         property real abc

    function getDat() {
        if(window1.abc != 0)
            return abc;
        else
            return 77;
    }

         FontLoader { id: textFont; source: "/home/pchotpc/Downloads/ReFormation Sans Regular.ttf" }
          FontLoader { id: numericFont; source: "/home/pchotpc/Downloads/ReFormation Sans Regular.ttf" }
          FontLoader { id: fontSystem; source: "/home/pchotpc/Downloads/ReFormation Sans Regular.ttf" }


               Rectangle {
                   x: 18
                   y: 66
                   width: 880
                   height: 880
                   Image {
                       id: image
                       x: 0
                       y: 0
                       z: 0
                       source: "/home/pchotpc/Desktop/PressureMappingPictures/0.jpg"
                   }

                   Timer {
                           property int cnt: 1
                           interval: 7000
                           running: true
                           repeat: true
                           onTriggered:
                           {
                               if (image.status === Image.Ready)
                               {
                                   image.source = "/home/pchotpc/Desktop/PressureMappingPictures/" + cnt + ".jpg"
                                   console.log(image.source.toString())
                                   cnt++;

                               }
                           }
                       }//timer
                   Timer {
                           interval: 100
                           running: true
                           repeat: true
                           onTriggered:
                           {
                                console.log(window1.abc)
                               //update()
                               //window1.update()
                           }
                       }//timer1

               }


      Text {
          id: text1
          x: 251
          y: 8
          width: 466
          height: 58
          text: qsTr("PRESSURE MAPPING ")
          font.family: "Verdana"
          font.pixelSize: 45
          color : "white"
      }

      Rectangle{
          id : btnMeasure
          x: 904
          y: 66
          width : 96
          height: 75

          color: "grey"
          radius: 10
          layer.effect: Glow
          {
              color: "#2dc2f0"
              samples: 10
              transparentBorder: true
          }
          layer.enabled: false
          border.width: 5
          border.color: "#00000000"

          Text {
              id: txtMeasure
              x: 11
              y: 29

              color: "#000000"
              text: qsTr("Measure")
              font.bold: true
              font.family: textFont.name
              font.pointSize: 13

            }
          MouseArea {
              id: mouseArea3
              x: 0
              y: 0
              width: 92
              height: 76
          }

      }

      Text {
          id: text2
          x: 18
          y: 959
          width: 112
          height: 26
          color: "#ffffff"
          text: qsTr("DEVICE :")
          font.pixelSize: 24
          font.family: "Verdana"
      }

      Text {
          id: text3
          x: 136
          y: 959
          width: 112
          height: 26
          color: "#5bf358"
          text: qsTr("READY")
          font.pixelSize: 24
          font.family: "Verdana"
      }

        // CO2
        Text
        {
            id:txtPicnumber

            x: 384
            y: 954

            width : 91
            height : 38

            text : getDat()

            color: "#259df4"
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: 30

            font.family: numericFont.name


        }



}
