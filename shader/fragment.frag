#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

struct Material
{
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    sampler2D texture_diffuse2;
    sampler2D texture_specular2;
    vec3 objColor;
    float shininess;
};

struct DirLight
{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
struct PointLight
{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

layout (std140) uniform Lights
{
    uniform DirLight SunLight;
    uniform PointLight LuminousBody;
};

uniform vec3 CameraPos;
uniform Material OneMaterial;

vec3 diffuseColor;
vec3 specularColor;

vec3 calculateDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 calculatePointLight(PointLight light, vec3 normal, vec3 viewDir);

void main(void)
{
    vec3 outputColor = vec3(0.0);
    vec3 viewDir = normalize(CameraPos - FragPos);//顶点指向摄影机
    vec3 normal = normalize(Normal);//经由法线矩阵变换后，标准化法向量
    if(OneMaterial.objColor == vec3(0.0))
    {
        diffuseColor = vec3(texture2D(OneMaterial.texture_diffuse1, TexCoord));
        specularColor = vec3(texture2D(OneMaterial.texture_specular1, TexCoord));
    }
    else {
        diffuseColor = OneMaterial.objColor;
        specularColor = OneMaterial.objColor;
    }

    // 第一阶段：定向光照
    outputColor += calculateDirLight(SunLight, normal, viewDir);
    // 第二阶段：点光源
    outputColor += calculatePointLight(LuminousBody, normal, viewDir);

    FragColor = vec4(outputColor, 1.0);
}

vec3 calculateDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    //环境光
    vec3 ambient = light.ambient * diffuseColor;
    //漫反射
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * diffuseColor;
    //镜面光
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), OneMaterial.shininess);
    vec3 specular = light.specular * spec * specularColor;

    return ambient + diffuse + specular;
}

vec3 calculatePointLight(PointLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - FragPos);
    //环境光
    vec3 ambient = light.ambient * diffuseColor;
    //漫反射
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * diffuseColor;
    //镜面光
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), OneMaterial.shininess);
    vec3 specular = light.specular * spec * specularColor;
    //衰减
    float distance = length(light.position - FragPos);//片段与光源的距离
    float attenuation = 1.0 / (light.constant + light.linear*distance + light.quadratic*distance*distance);
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return ambient + diffuse + specular;
}
