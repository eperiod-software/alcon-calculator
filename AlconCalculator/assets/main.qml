// Application with UI adaptability support template
import bb.cascades 1.0

// create application UI page
TabbedPane {
    id: tabs
    showTabsOnActionBar: false
    onActiveTabChanged: {
        
        //graphTab.text = "Hello"
        //calcTab.text = "hello"
    }
    Tab {
        id: calcTab
        title: "Calculator"
        imageSource: "asset:///images/tab-calculator.png"
        
        property string text: calcControl.text
            
        Page {
            Container {
                id: calcControl
                
                layout: DockLayout { }
                
                property string text: display.text
                
                AppBackground {
                    verticalAlignment: VerticalAlignment.Fill
                    horizontalAlignment: HorizontalAlignment.Fill
                }
                Container {
                    horizontalAlignment: HorizontalAlignment.Fill
                    verticalAlignment: VerticalAlignment.Fill
                        
                        Container {
                            property int padding: 10
                            layout: StackLayout {
                                orientation: (orientationHandler.orientation == UIOrientation.Portrait) ? LayoutOrientation.TopToBottom : LayoutOrientation.RightToLeft
                            }

                            topPadding: padding
                            bottomPadding: padding
                            leftPadding: padding
                            rightPadding: padding
                            verticalAlignment: VerticalAlignment.Center
                            horizontalAlignment: HorizontalAlignment.Center

                            Container {
                                id: specialButtons
                                layout: StackLayout {
                                    orientation: LayoutOrientation.TopToBottom
                                }
                                topPadding: 10.0
                                leftPadding: 10.0
                                bottomPadding: 10.0
                                rightPadding: 10.0
                                DisplayControl {
                                    id: display
                                }
                                Container {
                                    layout: StackLayout {
                                        orientation: LayoutOrientation.LeftToRight
                                    }
                                    Button {
                                        imageSource: "asset:///images/buttons/log.png"
                                        onClicked: {
                                            display.addText("log(")
                                        }
                                    }
                                    Button {
                                        imageSource: "asset:///images/buttons/Ans.png"
                                        onClicked: {
                                            display.addText("Ans")
                                        }
                                    }
                                    Button {
                                        imageSource: "asset:///images/buttons/powten.png"
                                        onClicked: {
                                            display.addText("10^(")
                                        }
                                    }
                                    Button {
                                        imageSource: "asset:///images/buttons/pi.png"
                                        onClicked: {
                                            display.addText("pi")
                                        }
                                    }
                                    Button {
                                        imageSource: "asset:///images/buttons/delete.png"
                                        onClicked: {
                                            display.deleteText()
                                        }
                                    }
                                }
                                Container {
                                    layout: StackLayout {
                                        orientation: LayoutOrientation.LeftToRight
                                    }
                                    Button {
                                        imageSource: "asset:///images/buttons/ln.png"
                                        onClicked: {
                                            display.addText("ln(")
                                        }
                                    }
                                    Button {
                                        imageSource: "asset:///images/buttons/sin.png"
                                        onClicked: {
                                            display.addText("sin(")
                                        }
                                    }
                                    Button {
                                        imageSource: "asset:///images/buttons/cos.png"
                                        onClicked: {
                                            display.addText("cos(")
                                        }
                                    }
                                    Button {
                                        imageSource: "asset:///images/buttons/tan.png"
                                        onClicked: {
                                            display.addText("tan(")
                                        }
                                    }
                                    Button {
                                        imageSource: "asset:///images/buttons/C.png"
                                        onClicked: {
                                            display.clearText()
                                        }
                                    }
                                }
                                Container {
                                    layout: StackLayout {
                                        orientation: LayoutOrientation.LeftToRight
                                    }
                                    Button {
                                        imageSource: "asset:///images/buttons/x.png"
                                        onClicked: {
                                            display.addText("x")
                                        }
                                    }
                                    Button {
                                        imageSource: "asset:///images/buttons/y.png"
                                        onClicked: {
                                            display.addText("y")
                                        }
                                    }
                                    Button {
                                        imageSource: "asset:///images/buttons/z.png"
                                        onClicked: {
                                            display.addText("z")
                                        }
                                    }
                                    Button {
                                        imageSource: "asset:///images/buttons/xinverse.png"
                                        onClicked: {
                                            display.addText("^(-1)")
                                        }
                                    }
                                    Button {
                                        imageSource: "asset:///images/buttons/abs.png"
                                        onClicked: {
                                            display.addText("abs(")
                                        }
                                    }
                                }
                            }
                            Container {
                                id: mainButtons
                                layout: StackLayout {
                                    orientation: LayoutOrientation.TopToBottom
                                }
                                topPadding: 10.0
                                leftPadding: 10.0
                                bottomPadding: 10.0
                                rightPadding: 10.0
                                Container {
                                    layout: StackLayout {
                                        orientation: LayoutOrientation.LeftToRight
                                    }
                                    Button {
                                        imageSource: "asset:///images/buttons/e.png"
                                        onClicked: {
                                            display.addText("e")
                                        }
                                    }
                                    Button {
                                        imageSource: "asset:///images/buttons/powe.png"
                                        onClicked: {
                                            display.addText("exp(")
                                        }
                                    }
                                    Button {
                                        imageSource: "asset:///images/buttons/leftparen.png"
                                        onClicked: {
                                            display.addText("(")
                                        }
                                    }
                                    Button {
                                        imageSource: "asset:///images/buttons/rightparen.png"
                                        onClicked: {
                                            display.addText(")")
                                        }
                                    }
                                    Button {
                                        imageSource: "asset:///images/buttons/divide.png"
                                        onClicked: {
                                            display.addText("/")
                                        }
                                    }
                                }
                                Container {
                                    layout: StackLayout {
                                        orientation: LayoutOrientation.LeftToRight
                                    }
                                    Button {
                                        imageSource: "asset:///images/buttons/power.png"
                                        onClicked: {
                                            display.addText("^")
                                        }
                                    }
                                    Button {
                                        imageSource: "asset:///images/buttons/seven.png"
                                        onClicked: {
                                            display.addText("7")
                                        }
                                    }
                                    Button {
                                        imageSource: "asset:///images/buttons/eight.png"
                                        onClicked: {
                                            display.addText("8")
                                        }
                                    }
                                    Button {
                                        imageSource: "asset:///images/buttons/nine.png"
                                        onClicked: {
                                            display.addText("9")
                                        }
                                    }
                                    Button {
                                        imageSource: "asset:///images/buttons/multiply.png"
                                        onClicked: {
                                            display.addText("*")
                                        }
                                    }
                                }
                                Container {
                                    layout: StackLayout {
                                        orientation: LayoutOrientation.LeftToRight
                                    }
                                    Button {
                                        imageSource: "asset:///images/buttons/xsquared.png"
                                        onClicked: {
                                            display.addText("^2")
                                        }
                                    }
                                    Button {
                                        imageSource: "asset:///images/buttons/four.png"
                                        onClicked: {
                                            display.addText("4")
                                        }
                                    }
                                    Button {
                                        imageSource: "asset:///images/buttons/five.png"
                                        onClicked: {
                                            display.addText("5")
                                        }
                                    }
                                    Button {
                                        imageSource: "asset:///images/buttons/six.png"
                                        onClicked: {
                                            display.addText("6")
                                        }
                                    }
                                    Button {
                                        imageSource: "asset:///images/buttons/subtract.png"
                                        onClicked: {
                                            display.addText("-")
                                        }
                                    }
                                }
                                Container {
                                    layout: StackLayout {
                                        orientation: LayoutOrientation.LeftToRight
                                    }
                                    Button {
                                        imageSource: "asset:///images/buttons/sqrt.png"
                                        onClicked: {
                                            display.addText("sqrt(")
                                        }
                                    }
                                    Button {
                                        imageSource: "asset:///images/buttons/one.png"
                                        onClicked: {
                                            display.addText("1")
                                        }
                                    }
                                    Button {
                                        imageSource: "asset:///images/buttons/two.png"
                                        onClicked: {
                                            display.addText("2")
                                        }
                                    }
                                    Button {
                                        imageSource: "asset:///images/buttons/three.png"
                                        onClicked: {
                                            display.addText("3")
                                        }
                                    }
                                    Button {
                                        imageSource: "asset:///images/buttons/add.png"
                                        onClicked: {
                                            display.addText("+")
                                        }
                                    }
                                }
                                Container {
                                    layout: StackLayout {
                                        orientation: LayoutOrientation.LeftToRight
                                    }
                                    Button {
                                        imageSource: "asset:///images/buttons/equals.png"
                                        onClicked: {
                                            display.addText("=")
                                        }
                                    }
                                    Button {
                                        imageSource: "asset:///images/buttons/zero.png"
                                        onClicked: {
                                            display.addText("0")
                                        }
                                    }
                                    Button {
                                        imageSource: "asset:///images/buttons/decimal.png"
                                        onClicked: {
                                            display.addText(".")
                                        }
                                    }
                                    Button {
                                        imageSource: "asset:///images/buttons/negate.png"
                                        onClicked: {
                                            display.addText("-")
                                        }
                                    }
                                    Button {
                                        imageSource: "asset:///images/buttons/evaluate.png"
                                        onClicked: {
                                            display.evaluate()
                                        }
                                    }
                                }
                        }
                    }
                }
                attachedObjects: [
                    AppOrientationHandler {
                        id: orientationHandler
                    }
                ]
            }
        }
    }
    Tab {
        id: graphTab
        title: "Graph"
        imageSource: "asset:///images/tab-graph.png"
        
        property string text: graphControl.text
        
        Page {
            GraphControl {
                id: graphControl
            }
            actions: [
                ActionItem {
                    title: "Edit graphs"
                    ActionBar.placement: ActionBarPlacement.OnBar
                    imageSource: "asset:///images/action-edit-graphs.png"
                    onTriggered: editGraphSheet.open()
                }
            ]
            attachedObjects: [
            	// Sheet for showing the graph editor
            	Sheet {
                    id: editGraphSheet
                    content: Page {
                        titleBar: TitleBar {
                            title: "Edit graphs"
                            acceptAction: ActionItem {
                                title: "OK"
                                onTriggered: {
                                    // Todo save changes
                                	editGraphSheet.close()
                                }
                            }
                            dismissAction: ActionItem {
                                title: "Cancel"
                                onTriggered: editGraphSheet.close()
                            }
                        }
                        Container {
                            layout: DockLayout { }
                            Container {
                                verticalAlignment: VerticalAlignment.Center
                                horizontalAlignment: HorizontalAlignment.Center
                                topPadding: 20.0
                                leftPadding: 20.0
                                bottomPadding: 20.0
                                rightPadding: 20.0
                                GraphEditorControl {
                                    id: graphEditorControl1
                                    topPadding: 20.0
                                    bottomPadding: 20.0
                                    func: "f"
                                }
                                GraphEditorControl {
                                    id: graphEditorControl2
                                    topPadding: 20.0
                                    bottomPadding: 20.0
                                    func: "g"
                                }
                                GraphEditorControl {
                                    id: graphEditorControl3
                                    topPadding: 20.0
                                    bottomPadding: 20.0
                                    func: "h"
                                }
                            }
                        }
                    }
                }
            ]
        }
    }
    Menu.definition: MenuDefinition {
        helpAction: HelpActionItem {
        }

        settingsAction: SettingsActionItem {
        }
        
        actions: [
            ActionItem {
                title: "About"
                imageSource: "asset:///images/ic_info.png"
            }
        ]
    }
}

