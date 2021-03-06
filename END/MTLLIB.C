/* FILENAME: MTLLIB.C
 * PROGRAMMER: PD6
 * PURPOSE: Material library support module.
 * LAST UPDATE: 15.06.2015
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "render.h"

/* ���������� ������� ���������� */
INT PD6_MtlLibSize = 1;
pd6MATERIAL PD6_MtlLib[PD6_MATERIAL_MAX] =
{
  {
    /* Material #0 - �������� �� ��������� */
    "Default",
    {0.1, 0.1, 0.1}, /* Ka */
    {0.9, 0.9, 0.9}, /* Kd */
    {0.5, 0.5, 0.5}, /* Ks */
    30.0,            /* Kp */
    1.0,             /* Kt */
    0                /* Texture id */
  }
};

/* ������� ���������� ��������� � ����������.
 * ���������:
 *   - ��������� � ����������� ���������:
 *       pd6MATERIAL *Mtl;
 * ������������ ��������:
 *   (INT) ���������� ����� ������������ ��������� (0 ��� ������).
 */
INT PD6_MtlAdd( pd6MATERIAL *Mtl )
{
  if (PD6_MtlLibSize < PD6_MATERIAL_MAX)
  {
    PD6_MtlLib[PD6_MtlLibSize] = *Mtl;
    return PD6_MtlLibSize++;
  }
  return 0;
} /* End of 'PD6_MtlAdd' function */

/* ������� ������ ��������� �� �����.
 * ���������:
 *   - ��� ���������:
 *       CHAR *Name;
 * ������������ ��������:
 *   (INT) ���������� ����� ���������� ��������� (0 ��� ������).
 */
INT PD6_MtlFind( CHAR *Name )
{
  INT i;

  for (i = 0; i < PD6_MtlLibSize; i++)
    if (strcmp(Name, PD6_MtlLib[i].Name) == 0)
      return i;
  return 0;
} /* End of 'PD6_MtlFind' function */

/* ������� �������� ��������� �� ����� (*.MTL).
 * ���������:
 *   - ��� ����� ���������:
 *       CHAR *FileName;
 * ������������ ��������:
 *   (INT) ���������� ����������� ����������.
 */
INT PD6_MtlLoad( CHAR *FileName )
{
  FILE *F;
  INT cnt = 0, i, j;
  pd6MATERIAL Mtl = PD6_MtlLib[0];
  static CHAR
    Buf[1000], Name[_MAX_PATH],
    path_buffer[_MAX_PATH],
    drive[_MAX_DRIVE],
    dir[_MAX_DIR],
    fname[_MAX_FNAME],
    ext[_MAX_EXT];

  _splitpath(FileName, drive, dir, fname, ext);

  if ((F = fopen(FileName, "r")) == NULL)
    return 0;

  while (fgets(Buf, sizeof(Buf), F) != NULL)
  {
    /* ���������� ���������� ������� */
    i = 0;
    while (Buf[i] != 0 && isspace(Buf[i]))
      i++;

    /* ������������ ������� ���������� */
    if (strncmp(Buf + i, "Ka", 2) == 0)
      sscanf(Buf + i + 3, "%f %f %f", &Mtl.Ka.X, &Mtl.Ka.Y, &Mtl.Ka.Z);
    else if (strncmp(Buf + i, "Kd", 2) == 0)
      sscanf(Buf + i + 3, "%f %f %f", &Mtl.Kd.X, &Mtl.Kd.Y, &Mtl.Kd.Z);
    else if (strncmp(Buf + i, "Ks", 2) == 0)
      sscanf(Buf + i + 3, "%f %f %f", &Mtl.Ks.X, &Mtl.Ks.Y, &Mtl.Ks.Z);
    else if (strncmp(Buf + i, "Ns", 2) == 0)
      sscanf(Buf + i + 3, "%f", &Mtl.Kp);
    else if (Buf[i] == 'D' || Buf[i] == 'd')
      sscanf(Buf + i + 2, "%f", &Mtl.Kt);
    else if (strncmp(Buf + i, "newmtl", 6) == 0)
    {
      /* �������� � ������ ������� ������� Buf */
      j = 0;
      i += 7;
      while (j < sizeof(Name) - 1 && Buf[i] != 0 && Buf[i] != '\n')
        Name[j++] = Buf[i++];
      Name[j] = 0;

      /* ��������� �������� */
      if (cnt++ != 0)
        PD6_MtlAdd(&Mtl);
      /* ������� �������� */
      Mtl = PD6_MtlLib[0];
      strcpy(Mtl.Name, Name);
    }
    else if (strncmp(Buf + i, "map_Kd", 6) == 0)
    {
      /* �������� � ������ ������� ������� Buf */
      j = 0;
      i += 7;
      while (j < sizeof(Name) - 1 && Buf[i] != 0 && Buf[i] != '\n')
        Name[j++] = Buf[i++];
      Name[j] = 0;

      /* ��������� �������� */
      _makepath(path_buffer, drive, dir, Name, "");
      Mtl.TexId = PD6_TextureLoad(path_buffer);
    }
  }
  /* ��������� �������� */
  if (cnt != 0)
    PD6_MtlAdd(&Mtl);
  fclose(F);
  return cnt;
} /* End of 'PD6_MtlLoad' function */

/* END OF 'MTLLIB.C' FILE */

