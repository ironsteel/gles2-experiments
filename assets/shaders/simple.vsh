attribute vec4 position;
attribute vec2 atexcoord;

uniform mat4 mvp;

varying vec2 v_texCoord;



void main()
{
    v_texCoord = atexcoord;    
    gl_Position = mvp * position;
}
