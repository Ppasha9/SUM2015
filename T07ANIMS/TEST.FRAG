#version 330

uniform float Time;
uniform float PartNo;
uniform float TotalParts;

uniform mat4 MatrView;

out vec4 OutColor;

in vec4 DrawColor;
in vec3 DrawPosition;
in vec3 DrawNormal;
in vec2 DrawTexCoord;

uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform float Kp;
uniform float Kt;

uniform float IsTextureUse;
uniform sampler2D DrawTexture;


vec4 Shade( vec3 P, vec3 N )
{
  float a = 1;
  vec3 color = vec3(0, 0, 0);
  vec3 L = normalize(vec3(1, 1, 1));
  vec3 V = -vec3(-MatrView[3][0], -MatrView[3][1], -MatrView[3][2]);

  //N = faceforward(N, V, N);
  N = normalize(N);

  /*
  vec3
    Ka = vec3(0, 0, 0),
    Kd = vec3(0.6, 0.6, 0.6),
    Ks = vec3(0.5, 0.5, 0.5);
  float Kp = 1;
  */

  color = Ka;

  float nl = dot(N, L); 
  if (nl > 0)
  {
    if (IsTextureUse > 0)
    {
      vec4 tc = texture2D(DrawTexture, DrawTexCoord);
      a = tc.a;
      color += Kd * tc.rgb * nl;
    }
    else
      color += Kd * nl;
  }

  vec3 R = reflect(V, N);
  float rl = dot(R, L); 
  if (rl > 0)
    ;//color += Ks * pow(rl, Kp);
  return vec4(0.6 * color, (Kt * a));
}

void main( void )
{
  if (Kt != 1)
    ;//discard;
  OutColor = DrawColor * Shade(DrawPosition.xyz, DrawNormal);
}