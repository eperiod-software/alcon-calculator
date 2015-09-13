import bb.cascades 1.0

Container {
    layout: StackLayout {
        orientation: LayoutOrientation.TopToBottom
    }
    
    property string func
    
    onFuncChanged: {
        fOfX.text = func + "(x)=";
        fOfY.text = func + "(y)=";
    }
    
    Container {
        layout: StackLayout {
            orientation: LayoutOrientation.LeftToRight
        }
        DropDown {
            layoutProperties: StackLayoutProperties {
                spaceQuota: -1
            }
            preferredWidth: 60.0
            verticalAlignment: VerticalAlignment.Center
            Option {
                id: fOfX
                text: "f(x)="
                selected: true
                onSelectedChanged: {
                    if (selected) {
                        inputTextField.hintText = "x*sin(x)";
                    }
                }
            }
            Option {
                id: fOfY
                text: "f(y)="
                onSelectedChanged: {
                    if (selected) {
                        inputTextField.hintText = "y*sin(y)";
                    }
                }
            }
        }
        TextField {
            id: inputTextField
            layoutProperties: StackLayoutProperties {
                spaceQuota: 1
            }
            hintText: "x*sin(x)"
            verticalAlignment: VerticalAlignment.Center
            input {
                submitKey: SubmitKey.Done
                flags: TextInputFlag.AutoCapitalizationOff | TextInputFlag.PredictionOff | TextInputFlag.WordSubstitutionOff | TextInputFlag.SpellCheckOff | TextInputFlag.AutoPeriodOff
                onSubmitted: {
                    // Todo check the syntax
                }
            }
        }
    }
    Container {
        Label {
            text: "Error"
            textStyle.color: Color.DarkRed
            visible: false
        }
    }
}

