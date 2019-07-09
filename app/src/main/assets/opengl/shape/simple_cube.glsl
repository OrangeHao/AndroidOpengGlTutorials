#version 300

attribute vec4 vertexPosition;
attribute vec3 vertexColour;
varying vec3 fragColour;
uniform mat4 projection;
uniform mat4 modeView;

void main() {
    gl_Position=projection*modeView*vertexPosition;
    fragColour=vertexColour;
}
