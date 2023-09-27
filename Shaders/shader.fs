#version 330 core
in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;
uniform sampler2D texture4;
uniform sampler2D texture5;

uniform float mixValue2;
uniform float mixValue3;
uniform float mixValue4;
uniform float mixValue5;

uniform float TexScale2;
uniform float TexScale3;
uniform float TexScale4;
uniform float TexScale5;

void main()
{   
    vec4 tex1 = texture(texture1, TexCoord);
    vec4 tex2 = texture(texture2, TexScale2 * TexCoord);
    vec4 tex3 = texture(texture3, TexScale3 * TexCoord);  
    vec4 tex4 = texture(texture4, TexScale4 * TexCoord);
    vec4 tex5 = texture(texture5, TexScale5 * TexCoord);
    vec4 texColor = mix(tex1, tex2, mixValue2);
    texColor = mix(texColor, tex3, mixValue3);
    texColor = mix(texColor, tex4, mixValue4);
    texColor = mix(texColor, tex5, mixValue5);
    if(texColor.a < 0.1)
        discard;
    FragColor = texColor;
}
