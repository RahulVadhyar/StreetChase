#version 330 core
layout (location = 0) in vec3 aPos;
uniform float health_percent;
uniform mat4 transform;
void main()
{   
    gl_Position = transform * vec4(aPos, 1.0f);

    // Calculate the new size of the health bar based on health_percent
    float barWidth = (1 - health_percent) * aPos.x;
    if(aPos.x > 0.0f)
        gl_Position.x -= 2*barWidth;
}