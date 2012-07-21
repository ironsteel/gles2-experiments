#include <jni.h>
#include <errno.h>
#include "GLESDemo.h"

void android_main(struct android_app* state) 
{
    GLESDemo glesApp(state);
    app_dummy();
    glesApp.run();
}
//END_INCLUDE(all)
