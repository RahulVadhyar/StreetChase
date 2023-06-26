#version 330 core
in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D textures[300];
uniform int activeTexture;
uniform int flip;

void main()
{   
    vec2 te;
    if(flip == 1){
        te = TexCoord * vec2(-1.0, 1.0);
    }
    vec4 tex = texture(textures[activeTexture], te);
    if(tex.a < 0.1)
        discard;
    FragColor = tex;
}
