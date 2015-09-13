// Application with UI adaptability support template
import bb.cascades 1.0

// Disable orientation support for N-series device even if the application
// has Orientation property set to "Auto-orient" in Application tab of the manifest editor.
OrientationHandler {
    onCreationCompleted: {
        OrientationSupport.supportedDisplayOrientation = SupportedDisplayOrientation.CurrentLocked;
    }
}
