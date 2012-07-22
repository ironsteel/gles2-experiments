#include "GLESDemo.h"


void GLESDemo::initShaders()
{
    GLuint vertexShader;
    GLuint fragmentShader;

    GLuint programObject;
    GLint linked;


    vertexShader = compileShader("shaders/simple.vsh", GL_VERTEX_SHADER);
    fragmentShader = compileShader("shaders/simple.fsh", GL_FRAGMENT_SHADER);
    
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
    
    return;
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

    glm::mat4 mvp = projection * view * model;
    
    GLint mvpId = glGetUniformLocation(shaderProgramObject, "mvp");
    glUniformMatrix4fv(mvpId, 1, GL_FALSE, &mvp[0][0]);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vertices);
    glEnableVertexAttribArray(0);

    glDrawArrays(GL_TRIANGLES, 0, 3);
    
}

void GLESDemo::positInit()
{
    initShaders();
    projection = glm::perspective(45.0, (double) width/height, 0.1, 100.0);
    view = glm::lookAt(glm::vec3(1, 1, 1), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    model = glm::mat4(1.0f);
}
