#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
out vec2 TexCoord;
uniform float health_percent;
uniform mat4 transform;
void main()
{   
    gl_Position = transform * vec4(aPos, 1.0f);
    TexCoord = aTexCoords;

    // Calculate the new size of the health bar based on health_percent
    float barWidth = health_percent * aTexCoords.x;
    if(aPos.x*aPos.y > 0.0f)
        gl_Position.x -= barWidth;
}