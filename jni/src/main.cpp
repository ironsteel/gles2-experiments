#include <jni.h>
#include <errno.h>
#include "GLESApplication.h"

/**
 * Process the next input event.
 */
static int32_t engine_handle_input(struct android_app* app, AInputEvent* event) {
    if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) {
        return 1;
    }
    return 0;
}

/**
 * Process the next main command.
 */
static void engine_handle_cmd(struct android_app* app, int32_t cmd) {
    GLESApplication *glesApp = (GLESApplication *)app->userData;
    switch (cmd) {
        case APP_CMD_SAVE_STATE:

            break;
        case APP_CMD_INIT_WINDOW:
            // The window is being shown, get it ready.
            if (app->window != NULL) {
                glesApp->initWindow(app);
                glesApp->drawOneFrame();
            }
            break;
        case APP_CMD_TERM_WINDOW:
            // The window is being hidden or closed, clean it up.
            glesApp->terminateWindow(app);
            break;

        case APP_CMD_DESTROY:
            glesApp->tearDownEGLContext();
            break;

        case APP_CMD_GAINED_FOCUS:

            break;
        case APP_CMD_LOST_FOCUS:
            break;
    }
}

/**
 * This is the main entry point of a native application that is using
 * android_native_app_glue.  It runs in its own thread, with its own
 * event loop for receiving input events and doing other things.
 */
void android_main(struct android_app* state) {

    GLESApplication glesApp;

    // Make sure glue isn't stripped.
    app_dummy();

    state->userData = &glesApp;
    state->onAppCmd = engine_handle_cmd;
    state->onInputEvent = engine_handle_input;

    // loop waiting for stuff to do.

    while (1) {
        // Read all pending events.
        int ident;
        int events;
        struct android_poll_source* source;

        // If not animating, we will block forever waiting for events.
        // If animating, we loop until all events are read, then continue
        // to draw the next frame of animation.
        while ((ident=ALooper_pollAll(-1, NULL, &events,
                (void**)&source)) >= 0) {

            // Process this event.
            if (source != NULL) {
                source->process(state, source);
            }

            // Check if we are exiting.
            if (state->destroyRequested != 0) {
                glesApp.tearDownEGLContext();
                return;
            }
        }
        glesApp.drawOneFrame();
    }
}
//END_INCLUDE(all)
