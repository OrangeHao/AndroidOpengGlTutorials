#version 300


attribute vec4 vertexPosition;
attribute vec2 vertexTextureCord;
varying vec2 textureCord;
uniform mat4 projection;
uniform mat4 modeView;

void main() {
    gl_Position=projection*modeView*vertexPosition;
    textureCord=vertexTextureCord;
}


