import bb.cascades 1.0

Container {
    layout: StackLayout {
        orientation: LayoutOrientation.TopToBottom
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

    property string text: inputTextField.text
    property string outputText: outputTextField.text
    
    property bool evaluated
    
    Container {
        layout: StackLayout {
            orientation: LayoutOrientation.LeftToRight
        }
        TextField {
            id: inputTextField
            backgroundVisible: false
            hintText: "y=sin(pi/2)"
            textStyle.color: Color.Black
        }
        Button {
            id: getPrev
            imageSource: "asset:///images/buttons/prev.png"
            preferredWidth: 90
            enabled: false
            onClicked: {
                if (mathEngine.canGetPrevious)
                	inputTextField.text = mathEngine.getPrevious()

                testNextPrev()
            }
        }
    }
    Container {
        layout: StackLayout {
            orientation: LayoutOrientation.LeftToRight
        }
        TextField {
            id: outputTextField
            text: "0"
            clearButtonVisible: false
            backgroundVisible: false
            hintText: " "
            textStyle.color: Color.Black
        }
        Button {
            id: getNext
            imageSource: "asset:///images/buttons/next.png"
            preferredWidth: 90
            enabled: false
            onClicked: {
                if (mathEngine.canGetNext)
                	inputTextField.text = mathEngine.getNext()

                testNextPrev()
            }
        }
    }
    
    function addText(newText) {
        if (evaluated == true) {
        	inputTextField.text = newText
        	evaluated = false
        }
        else
        	inputTextField.text += newText
        	
        testNextPrev()
    }
    
    function clearText() {
        inputTextField.text = ""
    }
    
    function deleteText() {
        // Delete the last piece of text
        var txt = inputTextField.text
        txt = txt.substr(0, txt.length - 1)
        inputTextField.text = txt
        evaluated = false
    }
    
    function evaluate() {
        outputTextField.text = mathEngine.evaluate(inputTextField.text)
        evaluated = true
        testNextPrev()
    }
    
    function testNextPrev() {
        getPrev.enabled = mathEngine.canGetPrevious
        getNext.enabled = mathEngine.canGetNext
    }
}
