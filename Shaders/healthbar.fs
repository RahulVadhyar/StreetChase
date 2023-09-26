#version 330 core
out vec4 FragColor;
uniform float health_percent;

void main()
{   
    // Apply the color and size to the fragment
    if(health_percent > 0.5)
    FragColor = vec4(1.0*(1-health_percent), 1.0, 0.0, 1.0);
    else
    FragColor = vec4(1.0, 1.0*health_percent, 0.0, 1.0);

}
