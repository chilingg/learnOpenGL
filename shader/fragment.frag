#version 330 core
out vec4 FragColor;
in vec3 normal;
in vec3 fragPos;
in vec2 texCoord;

struct Material
{
    sampler2D textureColor;
    sampler2D textureSpecular;
    vec3 objectColor;
    vec3 specular;
    float shininess;
};
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
};

uniform vec3 cameraPos;
uniform Material material;
uniform Light light;

void main(void)
{
    //平行光
    vec3 parallelDir = normalize(-light.parallelDir);
    float diff = max(dot(normalize(normal), parallelDir), 0.0);
    vec3 parallel = light.parallel * vec3(texture2D(material.textureColor, texCoord)) * diff;

    //环境光
    vec3 ambient = light.ambient * vec3(texture2D(material.textureColor, texCoord));

    //聚光灯
    vec3 lightDir = normalize(light.lightPos - fragPos);//片段指向光源的标准向量
    float theta = dot(lightDir, normalize(-light.direction));//片段向量与光源逆向量的夹角的余弦
    float epsilon = light.cutoff - light.outCutoff;
    //clamp约束返回值在指定范围内
    float intensity = clamp((theta - light.outCutoff) / epsilon, 0.0, 1.0);

    //漫反射
    //片段标准法向量与lightDir夹角的cos值，大于90度时为负取零，0~90度cos值为1-0
    diff = max(dot(normalize(normal), lightDir), 0.0);
    //决定漫反射量
    vec3 diffuse = light.diffuse * vec3(texture2D(material.textureColor, texCoord)) * diff * intensity;

    //镜面光
    vec3 viewDir = normalize(cameraPos - fragPos);//顶点指向摄影机
    //反射向量，-lightDir为光源指向顶点
    vec3 reflectDir = reflect(-lightDir, normal);
    //指数运算控制反光度大小，因底数为1~0，所以指数越高反光度越小
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.illuminant * vec3(texture2D(material.textureSpecular, texCoord)) * spec;

    //衰减
    float distance = length(light.lightPos - fragPos);//片段与光源的距离
    float attenuation = 1.0 / (light.constant + light.linear*distance + light.quadratic*distance*distance);
    diffuse *= attenuation;
    specular *= attenuation;

    FragColor = vec4(parallel + ambient + diffuse + specular, 1.0);
}
