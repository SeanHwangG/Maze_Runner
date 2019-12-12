#version 330 core
// This is a sample fragment shader.

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float attenuationPara;
};

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
in float sampleExtraOutput;
in vec3 vertNormal;
in vec3 fragPos;
in vec2 texCoord;


uniform sampler2D barkTex;
uniform sampler2D leafTex;
uniform vec3 cameraPos;
uniform int type;
//uniform vec3 color;
//uniform int colorType;
//uniform vec3 viewPos;

//uniform Light light;
//uniform Material material;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 fragColor;

void main()
{
    vec3 objColor;
    vec4 texColor;
    if(type == 1)
    {
        objColor = texture(barkTex, texCoord).rgb;
        fragColor = vec4(objColor, sampleExtraOutput);
    }
    else if(type == 2)
    {
        texColor = texture(leafTex, texCoord);
        if(texColor.a < 0.5) discard;
        fragColor = texColor;
        //objColor = vec3(texCoord.x,texCoord.x,texCoord.x );
    }
    
    //vec3 I = normalize(fragPos - cameraPos);
    //vec3 R = reflect(I, normalize(vertNormal));
    //objColor = texture(skybox, R).rgb;
    //objColor = (vertNormal + vec3(1,1,1)) / 2.0f;
    
    //objColor = vec3(texCoord.x,texCoord.x,texCoord.x );
    
}