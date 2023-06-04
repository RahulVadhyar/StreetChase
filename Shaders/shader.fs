#version 330 core
in vec2 TexCoord;
uniform sampler2D texture1;
out vec4 FragColor;
void main()
{   
    vec4 tex;
    tex = texture(texture1, TexCoord);
    if(tex.a < 0.1)
        discard;
    FragColor = tex;
}
