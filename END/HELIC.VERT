/* Filename: TEST.VERT */

// версия языка шейдера
#version 330

// кто куда приходит
layout(location = 0) in vec3 InPosition;
layout(location = 1) in vec2 InTexCoord;
layout(location = 2) in vec3 InNormal;
layout(location = 3) in vec4 InColor;

// глобальные переменные
uniform float PartNo;
uniform mat4 MatrWorld;
uniform mat4 MatrView;
uniform mat4 MatrProj;
uniform mat4 MatrWVP;
uniform mat4 MatrWV;
uniform mat4 MatrWVInverse;
uniform mat4 MatrWInverse;
uniform mat4 MatrRot;
uniform mat4 MatrHelic;
uniform float Time;

// выходные параметры
out vec4 DrawColor;
out vec3 DrawNormal;
out vec3 DrawPosition;
out vec2 DrawTexCoord;

/* Transposing the matrix */
mat4 MatrTranspose( mat4 M )
{
  int i, j;
  
  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      if (j <= i)
        {
          float tmp = M[i][j];

          M[i][j] = M[j][i];
          M[j][i] = tmp;
        }

  return M;
} /* End of 'MatrTranspose' function */

void main( void )
{
  //if (PartNo != 11 && PartNo != 10)
  gl_Position = MatrWVP * vec4(InPosition, 1);
  /*else //if ((PartNo > 9 && PartNo < 12) && InPosition.x != 0 && InPosition.y != 0 && InPosition.z != 0)
  {
    mat4 Rot;

    Rot[0][0] = cos(900 * Time);
    Rot[0][1] = 0;
    Rot[0][2] = -sin(900 * Time);
    Rot[0][3] = 0;

    Rot[1][0] = 0;
    Rot[1][1] = 0;
    Rot[1][2] = 0;
    Rot[1][3] = 0;

    Rot[2][0] = sin(900 * Time);
    Rot[2][1] = 0;
    Rot[2][2] = cos(900 * Time);
    Rot[2][3] = 0;

    Rot[3][0] = 0;
    Rot[3][1] = 0;
    Rot[3][2] = 0;
    Rot[3][3] = 0;

    mat4 MatrTranslate1;

    MatrTranslate1[0][0] = 1;
    MatrTranslate1[0][1] = 0;
    MatrTranslate1[0][2] = 0;
    MatrTranslate1[0][3] = 0;

    MatrTranslate1[1][0] = 0;
    MatrTranslate1[1][1] = 1;
    MatrTranslate1[1][2] = 0;
    MatrTranslate1[1][3] = 0;

    MatrTranslate1[2][0] = 0;
    MatrTranslate1[2][1] = 0;
    MatrTranslate1[2][2] = 1;
    MatrTranslate1[2][3] = 0;

    MatrTranslate1[3][0] = -InPosition.x;
    MatrTranslate1[3][1] = -InPosition.y;
    MatrTranslate1[3][2] = -InPosition.z;
    MatrTranslate1[3][3] = 1;
    
    /*------------------------------*/
    
    //gl_Position = MatrTranspose(MatrTranslate1) * MatrWVP * vec4(InPosition / 2, 1);

    //gl_Position = vec4(ThPosition, 1);
    
    /*------------------------------*/

    /*mat4 MatrTranslate2;

    MatrTranslate2[0][0] = 1;
    MatrTranslate2[0][1] = 0;
    MatrTranslate2[0][2] = 0;
    MatrTranslate2[0][3] = 0;

    MatrTranslate2[1][0] = 0;
    MatrTranslate2[1][1] = 1;
    MatrTranslate2[1][2] = 0;
    MatrTranslate2[1][3] = 0;

    MatrTranslate2[2][0] = 0;
    MatrTranslate2[2][1] = 0;
    MatrTranslate2[2][2] = 1;
    MatrTranslate2[2][3] = 0;

    MatrTranslate2[3][0] = InPosition.x;
    MatrTranslate2[3][1] = InPosition.y;
    MatrTranslate2[3][2] = InPosition.z;
    MatrTranslate2[3][3] = 1;

    gl_Position = MatrWVP * transpose(MatrTranslate2) * transpose(MatrTranslate1) * vec4(InPosition, 1);
  }   */

  DrawNormal = mat3(MatrWInverse) * InNormal;
  DrawPosition = vec3(MatrWorld * vec4(InPosition, 1));
  DrawColor = InColor;
  DrawTexCoord = InTexCoord;
}
