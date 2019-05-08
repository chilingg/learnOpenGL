#version 330 core
out vec4 FragColor;
in vec3 normal;
in vec3 fragPos;

struct Material
{
    vec3 objectColor;
    vec3 specular;
    float shininess;
};
struct Light
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform vec3 lightPos;
uniform vec3 cameraPos;
uniform Material material;
uniform Light light;

void main(void)
{
    //环境光
    vec3 ambient = light.ambient * material.objectColor;

    //漫反射
    vec3 lightDir = normalize(lightPos - fragPos);//片段指向光源的标准向量
    //片段标准法向量与lightDir夹角的cos值，大于90度时为负取零，0~90度cos值为1-0
    float diff = max(dot(normalize(normal), lightDir), 0.0);
    //决定漫反射量
    vec3 diffuse = light.diffuse * material.objectColor * diff;

    //镜面光
    vec3 viewDir = normalize(cameraPos - fragPos);//顶点指向摄影机
    //反射向量，-lightDir为光源指向顶点
    vec3 reflectDir = reflect(-lightDir, normal);
    //指数运算控制反光度大小，因底数为1~0，所以指数越高反光度越小
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * material.specular * spec;

    FragColor = vec4(ambient + diffuse + specular, 1.0);
}
