#version 330 core
out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;
in vec3 ourColor;
in vec2 TexCoord;

uniform vec3 viewPos; 
uniform vec3 lightPos; 
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 dirSuelo;
uniform float angulo;
uniform int usaTextura, fruta;

//Texturas
uniform sampler2D suelo;

uniform float ambientI;

void main()
{
    // ambient
    vec3 ambient = ambientI * lightColor;
  	
    //foco
    vec3 dirLuz = normalize(dirSuelo-lightPos);
    vec3 dirFrag = normalize (vec3(FragPos-lightPos));
    if(acos(dot(dirLuz, dirFrag))<radians(angulo)) {
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

	//Specular
	float specularStrength = 1.0;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);
	vec3 specular = specularStrength * spec * lightColor;
            
   	vec3 result = (ambient + diffuse + specular) * ourColor;
        if(usaTextura == 0){
            FragColor = vec4(result, 1.0);
        }
        else{ 
            vec4 tex =  texture(suelo, TexCoord);
            FragColor = vec4(result,1.0)*tex;
        }
   
   } else{
        vec3 result = (ambient) * ourColor;
        if(usaTextura == 0){
            FragColor = vec4(result, 1.0);
        }
        else{ 
            vec4 tex =  texture(suelo, TexCoord);
            FragColor = vec4(result, 1.0)*tex;
        }
   }
} 
