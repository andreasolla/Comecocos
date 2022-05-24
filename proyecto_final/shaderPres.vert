#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 Normal;
out vec3 FragPos; 
out vec3 ourColor;
out vec2 TexCoord;

uniform mat4 transform;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 color;

void main()
{
   gl_Position = projection * view * transform * vec4(aPos, 1.0f);
   ourColor = color;
   Normal = mat3(transpose(inverse(transform))) * aNormal;  
   FragPos = vec3(transform * vec4(aPos, 1.0));
   TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}