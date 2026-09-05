#version 330

precision mediump float;

// Input vertex attributes (from vertex shader)
varying vec2 fragTexCoord;
varying vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;

// output
out vec4 finalColor;

// NOTE: Add your custom variables here

void main()
{
    float gradient = fragTexCoord.x;

    // Calculate final fragment color
    finalColor = vec4(vec3(fragTexCoord.x), fragColor.a);
}