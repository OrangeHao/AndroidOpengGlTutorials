#version 300

precision mediump float;
uniform sampler2D texture;
varying vec2 textureCord;

void main() {
    gl_FragColor=texture2D(texture,textureCord);
}
