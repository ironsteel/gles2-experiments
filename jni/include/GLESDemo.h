#ifndef GLESDEMO_H
#define GLESDEMO_H
#include "GLESApplication.h"
#include "glm/glm.hpp"
#include "glm/ext.hpp"
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
    
    glm::mat4 projection;
    glm::mat4 view;
    glm::mat4 model;
    
};


#endif // GLESDEMO_H
