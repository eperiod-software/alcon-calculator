// Application with UI adaptability support template
import bb.cascades 1.0

// This template assumes that application has Orientation property set to "Auto-orient"
// in the Application bar-descriptor editor.
OrientationHandler {
    // the orientation handler skeleton
    id: orientationHandler
    onDisplayDirectionAboutToChange: {
        // set UI to be shown during rotation effect
    }
    onOrientationAboutToChange: {
        // setup the scene for the new orientation
        if (orientation == UIOrientation.Portrait) {
            // make some ui changes related to portrait
        } else if (orientation == UIOrientation.Landscape) {
            // make some ui changes related to landscape
        }
        if (displayDirection == DisplayDirection.North) {
            // do something specific to this device display direction
        }
    }
    onOrientationChanged: {
        // any additional changes to be performed after orientation change has occured
    }
    onDisplayDirectionChanged: {
        // can perform actions based on the direction of the display
    }
    onRotationCompleted: {
        // can perform actions after either direction or orientation change has been completed
    }
}
