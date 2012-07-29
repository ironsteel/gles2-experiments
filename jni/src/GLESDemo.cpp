#include "GLESDemo.h"
#include "res_texture.c"
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
    glBindAttribLocation(programObject, 1, "a_texCoord");
    
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
    glEnable(GL_DEPTH_TEST);
    return;
}

void GLESDemo::drawOneFrame(double ellapsedTime)
{
    float angle = ellapsedTime / 1000.0 * 75;
    model = glm::rotate(model, angle, glm::vec3(0, 1, 0));
    
    
    glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgramObject);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texId);
    
    
    
    glm::mat4 mvp = projection * view * model;
    
    GLint mvpId = glGetUniformLocation(shaderProgramObject, "mvp");
    
    GLint textureLocation = glGetUniformLocation(shaderProgramObject, "mytexture");
    
    glUniform1i(textureLocation, 0);
    
    glUniformMatrix4fv(mvpId, 1, GL_FALSE, &mvp[0][0]);
    
    cube->draw(shaderProgramObject);
    
}

void GLESDemo::positInit()
{
    createTexture();
    initShaders();
    cube = new Cube();
    
    projection = glm::perspective(65.0, (double) width / height, 0.1, 100.0);
    view = glm::lookAt(glm::vec3(0, 0, -10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    model = glm::mat4(1.0f);
}

void GLESDemo::terminateWindow(android_app *app)
{
    delete cube;
}

void GLESDemo::createTexture()
{
    glGenTextures(1, &texId);
    glBindTexture(GL_TEXTURE_2D, texId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glTexImage2D(GL_TEXTURE_2D, // target
                 0,  // level, 0 = base, no minimap,
                 GL_RGB, // internalformat
                 res_texture.width,  // width
                 res_texture.height,  // height
                 0,  // border, always 0 in OpenGL ES
                 GL_RGB,  // format
                 GL_UNSIGNED_BYTE, // type
                 res_texture.pixel_data);
}
