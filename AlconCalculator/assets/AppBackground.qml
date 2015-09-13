// Application with UI adaptability support template
import bb.cascades 1.0

Container {
    // use tiled backround as default
    background: backgroundPattern.imagePaint
    attachedObjects: [
        ImagePaintDefinition {
            id: backgroundPattern
            repeatPattern: RepeatPattern.XY
            imageSource: "asset:///images/background.amd"
        }
    ]
}
