#version 410

layout(location=0) in vec4 inColor;

out vec4 fragColor;

void main()
{
  fragColor = inColor;
}