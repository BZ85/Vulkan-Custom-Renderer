#version 450 		// Use GLSL 4.5

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 col;
layout(location = 2) in vec2 tex;
layout(location = 3) in vec3 normal;

layout(set = 0, binding = 0) uniform UboViewProjection {
	mat4 projection;
	mat4 view;
} uboViewProjection;

// NOT IN USE, LEFT FOR REFERENCE
layout(set = 0, binding = 1) uniform UboModel {
	mat4 model;
} uboModel;

layout(push_constant) uniform PushModel {
	mat4 model;
    mat4 normalModel;
} pushModel;

layout(location = 0) out vec3 fragCol;
layout(location = 1) out vec2 fragTex;
layout(location = 2) out vec3 viewPosition;
layout(location = 3) out vec3 viewNormal;
layout(location = 4) out mat4 viewMatrix;
void main() {
	gl_Position = uboViewProjection.projection * uboViewProjection.view * pushModel.model * vec4(pos, 1.0);
	
    viewPosition =  (uboViewProjection.view * pushModel.model * vec4(pos, 1.0)).xyz;

    viewNormal = (uboViewProjection.view * pushModel.normalModel * vec4(normal, 0.0)).xyz;

    viewMatrix = uboViewProjection.view;

  //  fragCol = mat3(transpose(inverse( uboViewProjection.view * pushModel.model))) * normal;

  //   fragCol = ((pushModel.normalModel) * vec4(normal, 0.0)).xyz;
	//fragCol = normal;
    fragTex = tex;
}