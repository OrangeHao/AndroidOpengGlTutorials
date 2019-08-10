#version 300

attribute vec4 vertexPosition;
attribute vec3 vertexColour;

//法线
attribute vec3 vertextNormals;

varying vec3 fragColour;
uniform mat4 projection;
uniform mat4 modeView;

void main() {

    //sence
    vec3 transformedVertextNormal=normalize((modeView*vec4(vertextNormals,0.0)).xyz);
    vec3 inverseLightDirection=normalize(vec3(0.0,1.0,1.0));
    fragColour=vec3(0.0);

    //diffuse 散射
    vec3 difuseLightIntensity=vec3(1.0,1.0,1.0);
    vec3 vertexDiffuseReflectionConstant=vertexColour;
    float normalDotLight=max(0.0,dot(transformedVertextNormal,inverseLightDirection));
    fragColour+=normalDotLight*vertexDiffuseReflectionConstant*difuseLightIntensity;

    //ambient 环境
    vec3 ambientLightIntensity=vec3(0.1,0.1,0.1);
    vec3 vertextAmbientReflectionConstant=vertexColour;
    fragColour+=vertextAmbientReflectionConstant*ambientLightIntensity;

    //specular 镜面
    vec3 inverseEyeDirection=normalize(vec3(0.0,0.0,1.0));
    vec3 specularLightIntensity=vec3(1.0,1.0,1.0);
    vec3 vertextSpecularReflectionConstant=vec3(1.0,1.0,1.0);
    float shininess=2.0;

    vec3 lightReflectionDirection=reflect(vec3(0)-inverseLightDirection,transformedVertextNormal);
    float normalDoteReflection=max(0.0,dot(inverseEyeDirection,lightReflectionDirection));

    fragColour+=pow(normalDoteReflection,shininess)*vertextSpecularReflectionConstant*specularLightIntensity;

    clamp(fragColour,0.0,1.0);

    gl_Position=projection*modeView*vertexPosition;
}
