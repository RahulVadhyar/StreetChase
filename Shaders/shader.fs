#version 330 core
in vec2 TexCoord;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float mixValue;
out vec4 FragColor;
void main()
{   
    vec4 texColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), mixValue);
    if(texColor.a < 0.1)
        discard;
    FragColor = texColor;
}
