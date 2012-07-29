#ifndef CUBE_H
#define CUBE_H

#include <GLES2/gl2.h>
#include "glm/glm.hpp"


class Cube 
{
    
public:
    Cube();
    virtual ~Cube() 
    {
        glDeleteBuffers(2, vboIds);
        delete[] vboIds;
    }
    
    void draw(GLuint shaderProgram);

private:
    void initGeometry();
    
    GLuint *vboIds;
    
};

#endif // CUBE_H
