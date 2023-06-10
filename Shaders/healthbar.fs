#version 330 core
in vec2 TexCoord;
out vec4 FragColor;

uniform float health_percent;

void main()
{   
    // Calculate the color transition based on health_percent
    vec3 color = mix(vec3(1.0, 0.0, 0.0), vec3(0.0, 1.0, 0.0), health_percent);

    // Set the alpha value based on the health_percent
    float alpha = clamp(health_percent * 2.0, 0.0, 1.0);

    // Apply the color and size to the fragment
    FragColor = vec4(color, alpha);

}
