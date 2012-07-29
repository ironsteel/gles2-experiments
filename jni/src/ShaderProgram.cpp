#include "ShaderProgram.h"

ShaderProgram::ShaderProgram(GLuint vertexShader, GLuint fragmentShader)
{
    this->vertexShader = vertexShader;
    this->fragmentShader = fragmentShader;
    this->shaderProgramObject = glCreateProgram();
}

bool ShaderProgram::link()
{
    GLint linked;
    
    glAttachShader(shaderProgramObject, vertexShader);
    glAttachShader(shaderProgramObject, fragmentShader);
    
    glBindAttribLocation(shaderProgramObject, 0, POSITION.c_str());
    glBindAttribLocation(shaderProgramObject, 1, TEX_COORDS.c_str());
    
    glLinkProgram(shaderProgramObject);

    glGetProgramiv(shaderProgramObject, GL_LINK_STATUS, &linked);

    if (!linked) {
        GLint infoLen = 0;
        glGetProgramiv(shaderProgramObject, GL_INFO_LOG_LENGTH, &infoLen);
        char *infoLog = (char*) malloc(sizeof(char) * infoLen);
        glGetProgramInfoLog(shaderProgramObject, infoLen, NULL, infoLog);
        LOGI("Error linking shader program\n%s\n", infoLog);
        glDeleteProgram(shaderProgramObject);
        free(infoLog);
        return false; 
    }
    
    
    
    if(!initUniformLocations()) {
        return false;
    }    
    
    if(!initAttributeLocations()) {
        return false;
    }
            
    
    return true;
}

GLint ShaderProgram::getAttributeLocation(string attributeName)
{
    return attributes[attributeName];
}

GLint ShaderProgram::getUniformLocation(string uniformName)
{
    return uniforms[uniformName];
}

bool ShaderProgram::initUniformLocations()
{
    GLint mvp = glGetUniformLocation(shaderProgramObject, MVP.c_str());
    if(mvp == -1) {
        LOGI("Cannot find %s", MVP.c_str());
        return false;
    }
    
    uniforms[MVP] = mvp;

    GLint sampler = glGetUniformLocation(shaderProgramObject, TEXTURE.c_str());
    if(sampler == -1) {
        LOGI("Cannot find %s", TEXTURE.c_str());
        return false;
    }
    uniforms[TEXTURE] = sampler;
    
    return true;
    
}

bool ShaderProgram::initAttributeLocations()
{
    attributes[POSITION] =  0;
    attributes[TEX_COORDS] = 1;
    return true;
}


GLuint ShaderProgram::getProgramId()
{
    return shaderProgramObject;
}
