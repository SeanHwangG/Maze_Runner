#version 330 core
// NOTE: Do NOT use any version older than 330! Bad things will happen!

// This is an example vertex shader. GLSL is very similar to C.
// You can define extra functions if needed, and the main() function is
// called when the vertex shader gets run.
// The vertex shader gets called once per vertex.

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 tex;
layout (location = 3) in vec3 center;

// Uniform variables can be updated by fetching their location and passing values to that location
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 rotate;
//uniform int type;
// Outputs of the vertex shader are the inputs of the same name of the fragment shader.
// The default output, gl_Position, should be assigned something. You can define as many
// extra outputs as you need.
out float sampleExtraOutput;
out vec3 vertNormal;
out vec3 fragPos;
out vec2 texCoord;


void main()
{
    vec4 worldPosition = rotate * model * vec4(position ,0.0) 
                        + model * vec4(center,1.0);
    //vec4 worldPosition = model * vec4(position + center,1.0);
    gl_Position = projection * view *  worldPosition;

    fragPos = vec3(model * vec4(position, 1.0f));
    vertNormal = mat3(transpose(inverse(model)))*normal;
    vertNormal = normalize(vertNormal);
   
    sampleExtraOutput = 1.0f;
    texCoord = tex;
}