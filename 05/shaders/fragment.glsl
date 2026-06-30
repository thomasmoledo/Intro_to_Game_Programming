#version 330

const float RED_LUM_CONSTANT   = 0.2126;
const float GREEN_LUM_CONSTANT = 0.7152;
const float BLUE_LUM_CONSTANT  = 0.0722;

uniform sampler2D texture0;
uniform vec2  lightPosition;
uniform float playerHealthRatio;

in vec2 fragTexCoord;
in vec2 fragPosition;

out vec4 finalColor;

void main()
{
    vec4 colour = texture(texture0, fragTexCoord);

    if (playerHealthRatio < 0.15)
    {
        float luminance = dot(
            vec3(RED_LUM_CONSTANT, GREEN_LUM_CONSTANT, BLUE_LUM_CONSTANT),
            colour.rgb
        );
        finalColor = vec4(vec3(luminance), colour.a);
    }
    else
    {
        finalColor = colour;
    }
}