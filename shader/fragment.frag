#version 330 core
out vec4 FragColor;
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

struct Material
{
    sampler2D textureColor;
    sampler2D textureSpecular;
    vec3 objectColor;
    vec3 specular;
    float shininess;
};
/*
struct Light
{
    vec3 lightPos;
    vec3 direction;
    float cutoff;//光锥半径余弦
    float outCutoff;//衰减光锥半径余弦
    vec3 parallelDir;

    vec3 parallel;
    vec3 ambient;
    vec3 diffuse;
    vec3 illuminant;

    //衰减函数系数
    float constant;
    float linear;
    float quadratic;
};*/
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

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform vec3 CameraPos;
uniform Material OneMaterial;
uniform DirLight SunLight;
uniform PointLight LuminousBody;

vec3 calculateDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 calculatePointLight(PointLight light, vec3 normal, vec3 viewDir);

void main(void)
{
    vec3 outputColor;
    vec3 viewDir = normalize(CameraPos - FragPos);//顶点指向摄影机
    vec3 normal = normalize(Normal);//经由法线矩阵变换后，标准化法向量

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
    vec3 ambient = light.ambient * vec3(texture2D(OneMaterial.textureColor, TexCoord));
    //漫反射
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture2D(OneMaterial.textureColor, TexCoord));
    //镜面光
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), OneMaterial.shininess);
    vec3 specular = light.specular * spec * vec3(texture2D(OneMaterial.textureSpecular, TexCoord));

    return ambient + diffuse + specular;
}

vec3 calculatePointLight(PointLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - FragPos);
    //环境光
    vec3 ambient = light.ambient * vec3(texture2D(OneMaterial.textureColor, TexCoord));
    //漫反射
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture2D(OneMaterial.textureColor, TexCoord));
    //镜面光
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), OneMaterial.shininess);
    vec3 specular = light.specular * spec * vec3(texture2D(OneMaterial.textureSpecular, TexCoord));
    //衰减
    float distance = length(light.position - FragPos);//片段与光源的距离
    float attenuation = 1.0 / (light.constant + light.linear*distance + light.quadratic*distance*distance);
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return ambient + diffuse + specular;
}
