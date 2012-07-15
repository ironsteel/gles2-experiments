#include <android_native_app_glue.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include "Log.h"

class GLESApplication
{
    public:
        GLESApplication(android_app *context)
        {
            this->display = EGL_NO_DISPLAY;
            this->surface = EGL_NO_SURFACE;
            this->context = EGL_NO_CONTEXT;
            this->width = 0;
            this->height = 0;
            this->androidContext = context;
        }
        virtual ~GLESApplication() {};

        virtual void saveState(android_app *app) {};
        virtual void gainedFocus(android_app *app) {};
        virtual void lostFocus(android_app *app) {};
        virtual void drawOneFrame();
        virtual void onStart(android_app* app) {};
        int initWindow(android_app *app);
        void terminateWindow(android_app *app) {};
        void tearDownEGLContext();
    protected:
        EGLDisplay display;
        EGLSurface surface;
        EGLContext context;
        android_app *androidContext;
        int width;
        int height;
};
