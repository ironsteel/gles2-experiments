#ifndef GLESDEMO_H
#define GLESDEMO_H
#include "GLESApplication.h"

using namespace std;

class GLESDemo : public GLESApplication
{
public:
    GLESDemo(android_app *app) : GLESApplication(app) {}
    ~GLESDemo() {}
    
    
protected:
    void initShaders();
    void drawOneFrame();
    void positInit();
    
};


#endif // GLESDEMO_H
