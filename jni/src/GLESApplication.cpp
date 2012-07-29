#include "GLESApplication.h"

int GLESApplication::initWindow(android_app *app)
{
    // initialize OpenGL ES and EGL

    /*
     * Here specify the attributes of the desired configuration.
     * Below, we select an EGLConfig with at least 8 bits per color
     * component compatible with on-screen windows
     */
    const EGLint attribs[] = {
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
            EGL_BLUE_SIZE, 5,
            EGL_GREEN_SIZE, 6,
            EGL_RED_SIZE, 5,
            EGL_DEPTH_SIZE, 1, 
            EGL_NONE
    };
   
    EGLint w, h, format;
    EGLint numConfigs;
    EGLConfig config;
    EGLSurface surface;
    EGLContext context;

    EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

    eglInitialize(display, 0, 0);

    /* Here, the application chooses the configuration it desires. In this
     * sample, we have a very simplified selection process, where we pick
     * the first EGLConfig that matches our criteria */
    eglChooseConfig(display, attribs, &config, 1, &numConfigs);

    /* EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
     * guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
     * As soon as we picked a EGLConfig, we can safely reconfigure the
     * ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID. 
     */
    eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);

    ANativeWindow_setBuffersGeometry(app->window, 0, 0, format);


    EGLint context_attr[] = {
        EGL_CONTEXT_CLIENT_VERSION, 2,
        EGL_NONE
    };

    surface = eglCreateWindowSurface(display, config, app->window, NULL);
    context = eglCreateContext(display, config, NULL, context_attr);

    if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
        LOGW("Unable to eglMakeCurrent");
        return -1;
    }

    eglQuerySurface(display, surface, EGL_WIDTH, &w);
    eglQuerySurface(display, surface, EGL_HEIGHT, &h);

    LOGI("+++++++++++++++++++++++++++++++++++++++++++++++");
    LOGI((const char*)glGetString(GL_VERSION));
    LOGI("+++++++++++++++++++++++++++++++++++++++++++++++");
    this->display = display;
    this->context = context;
    this->surface = surface;
    this->width = w;
    this->height = h;
    
    
    positInit();
    
    return 0;
}


GLuint GLESApplication::compileShader(const char *path, GLenum shaderType)
{
    GLuint shader;
    GLint compiled;
    
    char *shaderSrc = loadShaderFromFile(path);

    shader = glCreateShader(shaderType);

    if(shader == 0)
        return 0;

    glShaderSource(shader, 1,  const_cast<const GLchar**>(&shaderSrc), NULL);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

    if (!compiled) {
        GLint infoLen = 0;

        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);

        if (infoLen > 1) {
            char *infoLog = (char*) malloc(sizeof(char) * infoLen);
            glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
            LOGI("Error compiling shader:\n%s\n", infoLog);
            free(infoLog);
        }

        glDeleteShader(shader);
        return 0;
    }
    
    free(shaderSrc);
    return shader;
}

char* GLESApplication::loadShaderFromFile(const char *path)
{
    AAsset *shaderAsset= AAssetManager_open(androidContext->activity->assetManager, path, AASSET_MODE_BUFFER);
    size_t length = AAsset_getLength(shaderAsset);
    
    LOGI("Shader source size: %d\n", length);
    
    char* buffer = (char*) malloc(length);
    
    AAsset_read(shaderAsset, buffer, length);  
    
    buffer[length - 1] = '\0';
    
    LOGI("Shader source : %s\n", buffer);
    
    AAsset_close(shaderAsset);
    
    return buffer;
}

void GLESApplication::handleCommand(android_app *app, int32_t cmd)
{
    switch (cmd) {
        case APP_CMD_SAVE_STATE:

            break;
        case APP_CMD_INIT_WINDOW:
            // The window is being shown, get it ready.
            if (app->window != NULL) {
                initWindow(app);
                drawOneFrame(0);
            }
            break;
        case APP_CMD_TERM_WINDOW:
            terminateWindow(app);
            tearDownEGLContext();
            break;

        case APP_CMD_DESTROY:

            break;

        case APP_CMD_GAINED_FOCUS:
            gainedFocus(app);
            break;
        case APP_CMD_LOST_FOCUS:
            lostFocus(app);
            LOGI("LOST FOCUS state");
            break;
        case APP_CMD_START:
            onStart(app);
            break;
    }
}

double GLESApplication::getCuttentTime()
{
    static struct timeval start;
    
    gettimeofday(&start, NULL);
    return ((start.tv_sec * 1000) + (start.tv_usec / 1000.0));
}


void GLESApplication::_drawOneFrame(double ellapsedTime)
{
    if (this->display == NULL) {
        return;
    }
    drawOneFrame(ellapsedTime);
    eglSwapBuffers(this->display, this->surface);
}


static int32_t handle_input(struct android_app* app, AInputEvent* event) {
    GLESApplication *glApp = (GLESApplication*) app->userData;
    return glApp->handleInput(app, event);
}


static void handle_cmd(struct android_app* app, int32_t cmd) {
    GLESApplication *glesApp = (GLESApplication *)app->userData;
    glesApp->handleCommand(app, cmd);
    
}


void GLESApplication::tearDownEGLContext()
{
    if (this->display != EGL_NO_DISPLAY) {
        eglMakeCurrent(this->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (this->context != EGL_NO_CONTEXT) {
            eglDestroyContext(this->display, this->context);
        }
        if (this->surface != EGL_NO_SURFACE) {
            eglDestroySurface(this->display, this->surface);
        }
        eglTerminate(this->display);
    }

    this->display = EGL_NO_DISPLAY;
    this->context = EGL_NO_CONTEXT;
    this->surface = EGL_NO_SURFACE;
}

void GLESApplication::run()
{
    
    double delta = 1000.0/ 60.0;
    androidContext->userData = this;
    androidContext->onAppCmd = handle_cmd;
    androidContext->onInputEvent = handle_input;

    double startTime = 0;
    double timeSinceLastFrame = 0;
    double start = 0;
    int frames = 0;
    
    while (true) {
       
        startTime = getCuttentTime();
        
        
        int ident;
        int events;
        struct android_poll_source* source;

        // If not animating, we will block forever waiting for events.
        // If animating, we loop until all events are read, then continue
        // to draw the next frame of animation.
        while ((ident=ALooper_pollAll(0, NULL, &events,
                (void**)&source)) >= 0) {

            // Process this event.
            if (source != NULL) {
                source->process(androidContext, source);
            }

            // Check if we are exiting.
            if (androidContext->destroyRequested != 0) {
                tearDownEGLContext();
                return;
            }
        }
        
        
        if(timeSinceLastFrame >= delta) {
            if(start >= 1000) {
                LOGI("FPS: %d", frames);
                frames = 0;
                start = 0;
            }
            _drawOneFrame(timeSinceLastFrame);
            timeSinceLastFrame = 0;
            frames++;
        }
        double diff = getCuttentTime() - startTime;
        timeSinceLastFrame += diff;
        start += diff;
    }
    
}

int32_t GLESApplication::handleInput(android_app *app, AInputEvent *event)
{
    return 0;
}
