attribute vec4 vPosition;
attribute vec2 a_texCoord;

uniform mat4 mvp;

varying vec2 v_texCoord;



void main()
{
    v_texCoord = a_texCoord;    
    gl_Position = mvp * vPosition;
}
