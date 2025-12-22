#version 330 core

in vec2 TexCoords;
in vec3 Normal;
uniform sampler2D tex;
uniform vec3 cameraFront;
out vec4 FragColor;

void main()
{
    // Visually inspect the normal directions
    float normalAlignment = dot(normalize(Normal), normalize(cameraFront));
    vec4 normalColor = vec4(0.0f);
    if( normalAlignment > 0.0f )
    {
        normalColor = vec4(normalAlignment, 0.0f, 0.0f, 0.0f); // Red color if we are facing the face
    }
    else
    {
        normalColor = vec4(0.0f, normalAlignment, 0.0f, 0.0f); // Red color if we are facing the face
    }

    FragColor = texture(tex, TexCoords) + normalColor;
}

