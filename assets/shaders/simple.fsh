precision mediump float;

varying vec2 v_texCoord;
uniform sampler2D mytexture;

void main()
{
        gl_FragColor = texture2D(mytexture, v_texCoord.st);
}
