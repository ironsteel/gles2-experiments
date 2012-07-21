#include "GLESDemo.h"


void GLESDemo::initShaders()
{
    const char vertShaderSource[] = 
        "attribute vec4 vPosition;   \n"
        "void main() \n"
        "{ \n"
        "   gl_Position = vPosition;  \n"
        "} \n";

    const char fragShaderSource[] = 
        "precision mediump float; \n"
        "void main() \n"
        "{ \n"
        "   gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0); \n"
        "} \n";


    GLuint vertexShader;
    GLuint fragmentShader;

    GLuint programObject;
    GLint linked;


    vertexShader = loadShader(vertShaderSource, GL_VERTEX_SHADER);
    fragmentShader = loadShader(fragShaderSource, GL_FRAGMENT_SHADER);

    programObject = glCreateProgram();

    glAttachShader(programObject, vertexShader);
    glAttachShader(programObject, fragmentShader);

    glBindAttribLocation(programObject, 0, "vPosition");
    
    glLinkProgram(programObject);

    glGetProgramiv(programObject, GL_LINK_STATUS, &linked);

    if (!linked) {
        GLint infoLen = 0;
        glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &infoLen);
        char *infoLog = (char*) malloc(sizeof(char) * infoLen);
        glGetProgramInfoLog(programObject, infoLen, NULL, infoLog);
        LOGI("Error linking shader program\n%s\n", infoLog);
        glDeleteProgram(programObject);
        free(infoLog);
        return; 

    }

    shaderProgramObject = programObject;
    return ;
}

void GLESDemo::drawOneFrame()
{
    GLfloat vertices[] = { 0.0f, 0.5f, 0.0f,
                           -0.5f, -0.5f, 0.0f,
                           0.5f, -0.5f, 0.0f };
    glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgramObject);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vertices);
    glEnableVertexAttribArray(0);

    glDrawArrays(GL_TRIANGLES, 0, 3);
    
}

void GLESDemo::positInit()
{
    initShaders();
}
