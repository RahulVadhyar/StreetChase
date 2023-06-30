#version 330 core
in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2DArray textures;
uniform int activeTexture;
uniform int flip;

void main()
{   
    vec2 te = TexCoord;
    if(flip == 1){
        te = TexCoord * vec2(-1.0, 1.0);
    }
    vec4 tex = texture(textures, vec3(te, activeTexture));
    if(tex.a < 0.1)
        discard;
    FragColor = tex;
}
