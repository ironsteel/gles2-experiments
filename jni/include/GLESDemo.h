#ifndef GLESDEMO_H
#define GLESDEMO_H

#include "GLESApplication.h"

class GLESDemo : public GLESApplication
{
public:
    GLESDemo(android_app *app) : GLESApplication(app) {}
    ~GLESDemo() {}
    
    
protected:
    virtual void initShaders();
    virtual void drawOneFrame();
    virtual void positInit();
};


#endif // GLESDEMO_H
