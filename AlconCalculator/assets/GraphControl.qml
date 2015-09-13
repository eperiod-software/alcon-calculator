import bb.cascades 1.0

Container {
    layout: DockLayout {
    }
    
    property string text: inputTextField.text
    
    AppBackground {
        verticalAlignment: VerticalAlignment.Fill
        horizontalAlignment: HorizontalAlignment.Fill
    }
    Container {
        Container {
            layout: StackLayout { }
            
            horizontalAlignment: HorizontalAlignment.Fill
            layoutProperties: StackLayoutProperties {
                spaceQuota: -1
            }
            
            topPadding: 10.0
            leftPadding: 10.0
            bottomPadding: 10.0
            rightPadding: 10.0
            topMargin: 10.0
            leftMargin: 10.0
            bottomMargin: 10.0
            rightMargin: 10.0
            background: Color.create("#88dddddd")
            
            Container {
                layout: StackLayout {
                    orientation: LayoutOrientation.LeftToRight
                }
                TextField {
                    id: inputTextField
                    layoutProperties: StackLayoutProperties {
                        spaceQuota: 1
                    }
                    hintText: "x*sin(x)"
                    input {
                        submitKey: SubmitKey.Done
                        flags: TextInputFlag.AutoCapitalizationOff | TextInputFlag.PredictionOff | TextInputFlag.WordSubstitutionOff | TextInputFlag.SpellCheckOff | TextInputFlag.AutoPeriodOff
                        onSubmitted: {
                            //graphEngine.addPath(inputTextField.text);
                        }
                    }
                }
                Button {
                    id: clearGraph
                    layoutProperties: StackLayoutProperties {
                        spaceQuota: -1
                    }
                    imageSource: "asset:///images/buttons/clear-graph.png"
                    preferredWidth: 70.0
                    enabled: false
                    onClicked: {
                        //graphEngine.clearPaths();
                        evaluateAdd.enabled = true;
                        clearGraph.enabled = false;
                    }
                }
                Button {
                    id: evaluateAdd
                    layoutProperties: StackLayoutProperties {
                        spaceQuota: -1
                    }
                    imageSource: "asset:///images/buttons/evaluate-add.png"
                    preferredWidth: 70.0
                    onClicked: {
                        mathWorker.setEqn(0, inputTextField.text, false);
                        
                        /*if (graphEngine.addPath(inputTextField.text) == true)
                        	clearGraph.enabled = true;
                        
                        if (graphEngine.full() == true)
                        	evaluateAdd.enabled = false;*/
                    }
                }
            }
        }
        
        ForeignWindowControl {
            id: graphWindow
            windowId: "GraphWindowID"
            
            horizontalAlignment: HorizontalAlignment.Fill
            layoutProperties: StackLayoutProperties {
                spaceQuota: 1
            }
            
            preferredHeight: 600.0
            preferredWidth: 400.0
            
            updatedProperties: WindowProperty.Size | WindowProperty.Position
            
            property int startX
            property int startY
            property bool isPinching
            
            onTouch: {
	            /*if (isPinching)
	            	return;
	             
	            if (event.isDown()) {
	           		startX = event.localX;
	             	startY = event.localY;
                    graphEngine.beginMove();
	            }
	            else if (event.isMove()) {
                    graphEngine.updateMove(event.localX - startX, event.localY - startY);
	            }*/
            }
            
            gestureHandlers: [
                PinchHandler {
                    /*onPinchStarted: {
                        graphWindow.isPinching = true
                        graphEngine.beginZoom()
                    }
                    
                    onPinchUpdated: {
                        graphEngine.updateZoom(event.pinchRatio, event.pinchRatio)
                    }
                    
                    onPinchCancelled: {
                        graphWindow.isPinching = false
                    }
                    
                    onPinchEnded: {
                        graphWindow.isPinching = false
                    }*/
                }
            ]
            attachedObjects: [
                LayoutUpdateHandler {
                    id: handler
                    onLayoutFrameChanged: {
                        displayWorker.setSizeChanged();
                    }
                }
            ]
            verticalAlignment: VerticalAlignment.Bottom
        }
        onCreationCompleted: {
            var w = handler.layoutFrame.width;
            var h = handler.layoutFrame.height;
                
            if (w == 0) w = graphWindow.preferredWidth;
            if (h == 0) h = graphWindow.preferredHeight;
                
            if (displayWorker.createGraphWindow(graphWindow.windowGroup, graphWindow.windowId, w, h) == false) {
                inputTextField.text = "Unable to create screen."
                inputTextField.enabled = false;
                evaluate.enabled = false;
                evaluateAdd.enabled = false;
                graphWindow.visible = false;
            }
        }
    }
}
