//global variables

const static int MAX_LIGHTS = 1;
const static float k = 0.2f;

float4x4 World;
float4x4 View;
float4x4 Proj;
float3 cameraPos;
float3 lightPos[MAX_LIGHTS];
float4 lightColor[MAX_LIGHTS];

// Structures
struct InputVS
{
  float3 pos : POSITION0;
  float4 diffuse : COLOR0;
  float3 norm : NORMAL0;
  float4 tex : TEXCOORD0;
};

struct OutputVS
{
  float4 posH : POSITION0;
  float4 tex : TEXCOORD0;
  float4 posH_PS : TEXCOORD1;
  float3 norm : TEXCOORD2;
  float4 diffuse : COLOR0;
};

struct OutputPS
{
  float4 color : COLOR;
};

// Vertex shader
OutputVS TransformVS(const InputVS inVS)
{
  // Zero out our output.
  OutputVS outVS = (OutputVS)0;

  float4x4 WV = mul(World, View);
  float4x4 WVP = mul(WV, Proj);

  outVS.posH = mul(float4(inVS.pos,1.0f), WVP);
  outVS.posH_PS = mul(float4(inVS.pos, 1.0f), WV);
  outVS.norm = normalize(mul(inVS.norm, (float3x3)WV));
  outVS.diffuse = inVS.diffuse;
  outVS.tex = inVS.tex;

  return outVS;
}


//Pixel shader
OutputPS MinnaertPS(const OutputVS outVS)
{
  OutputPS outPS = (OutputPS)0;

  
  float4 cameraView = mul(float4(cameraPos,1.0f), View);
  float4 eye = normalize(cameraView - outVS.posH_PS);
  float  A[MAX_LIGHTS];
  for (int i = 0; i < MAX_LIGHTS; i++)
  {
    float4 lightView = mul(float4(lightPos[i],1.0f), View);
    float4 light = normalize(lightView - outVS.posH_PS);
    A[i] = saturate(dot((float3)light, outVS.norm));
  }

  float B = saturate(dot((float3)eye, outVS.norm));
  for (int j = 0; j < MAX_LIGHTS; j++)
  {
    outPS.color += lightColor[j] * pow(A[j], k) * pow(B, 1.0f - k);
  }
    
  return outPS;
}

OutputPS DiffusePS(const OutputVS outVS)
{
  OutputPS outPS = (OutputPS)0;
  float  A[MAX_LIGHTS];
  for (int i = 0; i < MAX_LIGHTS; i++)
  {
    float4 lightView = mul(float4(lightPos[i], 1.0f), View);
    float4 light = normalize(lightView - outVS.posH_PS);
    A[i] = saturate(dot((float3)light, outVS.norm));
  }

  for (int j = 0; j < MAX_LIGHTS; j++)
  {
    outPS.color += lightColor[j] * A[j];
  }
  return outPS;
}

technique Minnaert
{
  pass P0
  {
    vertexShader = compile vs_2_0 TransformVS();
    pixelShader  = compile ps_2_0 MinnaertPS();
  }
}

technique Diffuse
{
  pass P0
  {
    vertexShader = compile vs_2_0 TransformVS();
    pixelShader  = compile ps_2_0 DiffusePS();
  }
};

