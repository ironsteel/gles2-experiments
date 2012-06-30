#include <android_native_app_glue.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include "Log.h"

class GLESApplication
{
    public:
        GLESApplication()
        {
            this->display = EGL_NO_DISPLAY;
            this->surface = EGL_NO_SURFACE;
            this->context = EGL_NO_CONTEXT;
            this->width = 0;
            this->height = 0;
        }
        virtual ~GLESApplication() {};

        void saveState(android_app *app);
        int initWindow(android_app *app);
        void terminateWindow(android_app *app) {};
        void gainedFocus(android_app *app);
        void lostFocus(android_app *app);
        void drawOneFrame();
        void tearDownEGLContext();
    private:
        EGLDisplay display;
        EGLSurface surface;
        EGLContext context;
        int width;
        int height;
};
