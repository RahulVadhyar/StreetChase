#version 330 core
in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D atlas;
uniform int flip;
uniform float num_rows;
uniform float num_cols;
uniform int current_texture;
void main()
{   
    vec2 te = TexCoord;
    if(flip == 1){
        te = TexCoord * vec2(-1.0, 1.0);
    }
    int col = int(current_texture / num_cols);
    int row = int(current_texture / num_rows);

    te.x = (te.x+row)/num_rows;
    te.y = 1 - (col + 1 - te.y)/num_cols;

    vec4 tex = texture(atlas, te);
    if(tex.a < 0.1)
        discard;
    FragColor = tex;
}
