#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H
#include <GLES2/gl2.h>
#include <map>
#include <string>

#include "Log.h"

#define MVP string("mvp")
#define POSITION string("position")
#define TEX_COORDS string("atexcoord")
#define TEXTURE string("mytexture")

using namespace std;

class ShaderProgram 
{

public:
    ShaderProgram(GLuint vertexShader, GLuint fragmentShader);
    ~ShaderProgram() {};
    
    virtual bool link();
    GLint getAttributeLocation(string attributeName);
    GLint getUniformLocation(string uniformName);
    virtual GLuint getProgramId();
    
protected:
    virtual bool initUniformLocations();
    virtual bool initAttributeLocations();
    
    

private:
    GLuint vertexShader;
    GLuint fragmentShader;    
    
    GLuint shaderProgramObject;
    map<string, GLuint> attributes;
    map<string, GLint> uniforms;
    
};

#endif // SHADERPROGRAM_H
