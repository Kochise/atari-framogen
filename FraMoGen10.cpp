// /// Mocha's FRActal MOuntain GENerator ///
// /// Original Version by Mega1 - 1992
// /// BigScreen Version by KOSS - 1999/2001
// /// Version 7 : Optimized & Maximized Version by KOSS

// sens     = (0/180)*PI
// hauteur  = (0/180)*PI

/*
  sTopoType     l_asTopoType[] =
  { { "AERO",  0, 7383, 20306, eTtCommunication } // Aéroport, héliport, terrain d'aviation                             -> Airport
  , { "AMER",  1, 9914, 20306, eTtHydrographie  } // Amer, bouée, balise, tourelle, phare                               -> Lighthouse
  };

  int l_nMin;
  int l_nMed;
  int l_nMax;
  int l_nMaxArray = sizeof(l_asTopoType) / sizeof(sTopoType);

  //

  l_nMin = 0;
  l_nMax = l_nMaxArray;

  while
  (
       ((l_nTempo = strcmp(l_oStrTempo, l_asTopoType[l_nMed = (l_nMin + l_nMax) >> 1].anDesign)) != 0)
    && (l_nMax > (l_nMin + 1))
  )
  {
    if(l_nTempo < 0)
    {
      l_nMax = l_nMed;
    }
    else
    {
      l_nMin = l_nMed;
    }
  }

  if(l_nTempo == 0) // Si on est tombé juste (comparaison 'strcmp' intégrale)

*/

int g_anPow2[] =
{          1 // 0
,          2 // 1
,          4 // 2
,          8 // 3
,         16 // 4
,         32 // 5
,         64 // 6
,        128 // 7
,        256 // 8
,        512 // 9
,       1024 // 10
,       2048 // 11
,       4096 // 12
,       8192 // 13
,      16384 // 14
,      32768 // 15
,      65536 // 16
,     131072 // 17
,     262144 // 18
,     524288 // 19
,    1048576 // 20
,    2097152 // 21
,    4194304 // 22
,    8388608 // 23
,   16777216 // 24
,   33554432 // 25
,   67108864 // 26
,  134217728 // 27
,  268435456 // 28
,  536870912 // 29
, 1073741824 // 30
, 2147483648 // 31
//, 4294967296 // 32
};

#define pow2(x) g_anPow2[x]

int g_nPrecArrayMax = 9;
int g_nPrecDispMax  = 0;

int g_nSectMax  = g_nPrecDispMax - 1;
int g_nPrecMax;

int g_nSectCur;
int g_nPrecCur;

int g_nAltDevMax;

int g_nCalcCur  = 0;
int g_nMaxDisp  = 0;

int g_nArraySize = pow2(g_nPrecArrayMax + 1);

int g_anMntDev[g_nArraySize + 1][g_nArraySize + 1];  // Pass 1
int g_anMntAlt[g_nArraySize + 1][g_nArraySize + 1];  // Pass 2

// Temporary Height Field (Pass 1)
int g_nMntDevLevel = 256;              // Central Value
int g_nMntDevChaos = 128;              // Range

// Final Height Field (Pass 2)
int g_nMntAltLevel = -36;              // Central Value
int g_nMntAltChaos = 384;              // Range

int g_an3dMntX[4];
int g_an3dMntY[4];
int g_an3dMntZ[4];
int g_an3dAffX[4];
int g_an3dAffY[4];

int g_nScreenW;
int g_nScreenH;

int g_nParamMntColor = 0;              // Color to Display
int g_nParamMntType  = 1;              // HeightField to Display
int g_nParamTxtColor = 1;              // Change ScreenColor
int g_nParamDisp3D   = 0;

int g_nTimeStart;

char stab[] = "\t"                     // PrintInfo Tab (Time & Date)

char g_anStrTitle[] = "/// Mocha's FRActal MOuntain GENerator - Version 7 - KOSS 1999-2001 ///\r\n";

// *** WorkROUTINES *************************************************************

int main(void)
{
  // OK : 06/28/12/01

  int l_nMenu;

  GetScreenRes();                      // GetScreenRes

  g_nAltDevMax = pow2(g_nPrecDispMax);      // MaxHeight to Divide @ each Iter

  while(TRUE)
  {
    cls();

    VSETCOLOR 0, &H111;                // VERY DARK GRAY

    if(g_nParamTxtColor == 0)
    {
      VSETCOLOR 1, &H162;              // GREEN
    }
    else
    {
      VSETCOLOR 1, &H168;              // AMBER
    }

    printf(g_anStrTitle);
    printf("\r\n");
    printf("g_nMntDevLevel = %d %s g_nMntAltLevel = %d\r\n", g_nMntDevLevel, stab, g_nMntAltLevel);
    printf("g_nMntDevChaos = %d %s g_nMntAltChaos = %d\r\n", g_nMntDevChaos, stab, g_nMntAltChaos);
    printf("\r\n");
    printf("g_nArraySize   = %d %s g_nAltDevMax = %d\r\n", g_nArraySize, stab, g_nAltDevMax);
    printf("g_nCalcCur  = %d %s g_nMaxDisp = %d\r\n", g_nCalcCur, stab, g_nMaxDisp);
    printf("\r\n");
    printf("g_nScreenW  = %d %s g_nScreenH = %d\r\n", g_nScreenW, stab, g_nScreenH);
    printf("\r\n");
    printf("g_nParamMntType  = %d : %s\r\n", g_nParamMntType,  (g_nParamMntType  == 0)?"Variation Field":"Height Field");
    printf("g_nParamMntColor = %d : %s\r\n", g_nParamMntColor, (g_nParamMntColor == 0)?"Height Color":"Variation Color");
    printf("\r\n");
    printf("g_nParamDisp3D   = %d : %s\r\n", g_nParamDisp3D,   (g_nParamDisp3D == 0)?"Ground View":"Space View");
    printf("\r\n");
    printf("Action : -0- QUIT\r\n")
    printf("         -1- Change parameters\r\n")
    printf("         -2- Generate Landscape\r\n")
    printf("         -3- Enhance  Lanscape\r\n")
    printf("         -4- Display MAP\r\n")
    printf("         -5- Display 3D\r\n")
    printf("         -6- Variation Color <-> Height Color\r\n")
    printf("         -7- Variation Field <-> Height Field\r\n")
    printf("         -8- Texte VERT      <-> AMBRE\r\n")
    printf("         -9- Ground View     <-> Space View\r\n")
    printf("\r\n");

    do
    {
      l_nMenu = scanf(" => ");
    }
    while(l_nMenu >= 0 && l_nMenu <= 9);

    switch(l_nMenu)
    {
      case 0 :
        return 0;                                                   // Sort du Programme
      case 1 :
        GetChaosLevel();                                            // Définition des Niveaux de Chaos
        break;
      case 2 :
        GetMntParameters();                                         // Calcul de la Montagne Fractale
        break;
      case 3 :
        EnhanceMnt();
        break;
      case 4 :
        SetColorPal();
        Display2D(g_nSectCur, g_nMaxDisp, g_nParamMntColor, g_nParamMntType); // Affichage en Carte Topologique
        break;
      case 5 :
        SetColorPal();
        Display3D(g_nSectCur, g_nMaxDisp, g_nParamMntColor, g_nParamMntType); // Affichage en 3D
        break;
      case 6 :
        g_nParamMntColor = InvertParam(g_nParamMntColor);                     // Color to Display
        break;
      case 7 :
        g_nParamMntType  = InvertParam(g_nParamMntType);                      // Change HeightField to Display
        break;
      case 8 :
        g_nParamTxtColor = InvertParam(g_nParamTxtColor);                     // Change Text Color
        break;
      case 9 :
        g_nParamDisp3D   = InvertParam(g_nParamDisp3D);                       // Change 3D View Point
        break;
    }
  }

  return 0;
}

void GetScreenRes(void)
{
  // OK : 06/23/06/00

  // Get ScreenRez

/*
  int asm_rout$;
  int asm_rout;

  asm_rout$ = MKL$(&HA0004E75);              // dc.w $A000 + RTS
  asm_rout  = VARPTR(asm_rout$);
  linea     = C:asm_rout();
  g_nScreenW  = DPEEK(linea - 12);
  g_nScreenH  = DPEEK(linea - 4);
*/  

  int l_nLoop;

  for
  ( l_nLoop  = g_nPrecArrayMax + 1
  ; l_nLoop >  0
  ; l_nLoop -= 1
  )
  {
    if(pow2(l_nLoop) > g_nScreenH)
    {
      g_nPrecDispMax = l_nLoop - 1;
    }
  }
}

void GetChaosLevel(void)
{
  // OK : 03/27/06/00

  // Temporary Height Field (Pass 1)
  g_nMntDevLevel = scanf("g_nMntDevLevel = ");
  g_nMntDevChaos = scanf("g_nMntDevChaos = ");

  // Final Height Field (Pass 2)
  g_nMntAltLevel = scanf("g_nMntAltLevel = ");
  g_nMntAltChaos = scanf("g_nMntAltChaos = ");

  // Deviation Level
  g_nAltDevMax = scanf("g_nAltDevMax = ");
}

void GetMntParameters(void)
{
  // OK : 01/13/02/00

  g_nSectMax = g_nPrecDispMax - 1;
  
  //
  // Select the number of Sectors to Compute
  // More Sectors mean less Zoom (Satellite's Photo)
  // Less Sectors mean more Zoom (Spot's Photo)
  //
  //   0 1 2
  //  +--+--+- and each square is +--+--+ at g_nTimeStart (g_nPrecCur = 1)
  // 0|  |  |                     |  |  |
  //  +--+--+-                    +--+--+
  // 1|  |  |                     |  |  |
  //  +--+--+-                    +--+--+
  // 2|  |  |
  //

  printf("\r\n");
  
  do
  {
    printf("Nombre de secteurs 2^(0-%d) : ", g_nSectMax)
    g_nSectCur = scanf("Sect = ");
  }
  while(g_nSectCur >= 0 && g_nSectCur <= g_nSectMax);

  g_nPrecMax = g_nPrecDispMax - g_nSectCur;
  
  //
  // Each sectors will be sliced more and more...
  // Each time, the sector will be more precize !
  //
  // g_nPrecCur = 1 is +--+--+, g_nPrecCur = 2 is +--+--+--+--+, and so on...
  //                   |  |  |                    |  |  |  |  |
  //                   +--+--+                    +--+--+--+--+
  //                   |  |  |                    |  |  |  |  |
  //                   +--+--+                    +--+--+--+--+
  //                                              |  |  |  |  |
  //                                              +--+--+--+--+
  //                                              |  |  |  |  |
  //                                              +--+--+--+--+
  //

  printf("\r\n");
  
  do
  {
    printf("Précision g_nCalcCur (1-%d) : ", g_nPrecMax)
    g_nCalcCur = scanf("g_nCalcCur = ");
  }
  while(g_nCalcCur >= 1 && g_nCalcCur <= g_nPrecMax);
  
  g_nMaxDisp = g_nCalcCur;
  
  RandomizeMnt(g_nSectCur);

  ComputeMnt(g_nSectCur, 1, g_nCalcCur);
}

void EnhanceMnt(void)
{
  // OK : 06/28/12/01

  if(g_nCalcCur == 0)
  {
    GetMntParameters();
  }

  printf("\r\n");

  do
  {
    printf("Précision ACTUELLE : %d", g_nCalcCur);
    printf("Précision g_nSectCur (1-%d) : ", g_nPrecMax);
    g_nPrecCur = scanf("Prec = ");
  }
  while(g_nPrecCur >= 1 && g_nPrecCur <= g_nPrecMax);

  if(g_nPrecCur > g_nCalcCur)
  { // 
    ComputeMnt(g_nSectCur, g_nCalcCur, g_nPrecCur);

    g_nCalcCur = g_nPrecCur;
  }

  g_nMaxDisp = g_nPrecCur;
}

void RandomizeMnt(int i_nSect)
{
  // OK : 05/22/06/00

  // Since Version 4, the Height Field Sector Size is FIXED !
  // So that, we have to compute a little there, because
  // there will be NO MORE Shift'n Copy Routine left...
  //
  // With g_nPrecArrayMax = 9, the Height Field Sector Size is (2^9)+1 = 513
  // If i_nSect = 0, the Height Field is :
  //
  // 0,0   +------------------------- 512 -------------------------+ 512,0
  //       |                                                       |
  //       |                                                       |
  //       |                                                       |
  //       |                                                       |
  //       |                                                       |
  //       |                                                       |
  //       |                                                       |
  //       |                                                       |
  //       |                                                       |
  //       |                                                       |
  //       |                                                       |
  //       |                                                       |
  //       |                                                       |
  //       |                                                       |
  //       ~                                                       ~
  //      512                                                     512
  //       ~                                                       ~
  //       |                                                       |
  //       |                                                       |
  //       |                                                       |
  //       |                                                       |
  //       |                                                       |
  //       |                                                       |
  //       |                                                       |
  //       |                                                       |
  //       |                                                       |
  //       |                                                       |
  //       |                                                       |
  //       |                                                       |
  //       |                                                       |
  //       |                                                       |
  // 0,512 +------------------------- 512 -------------------------+ 512,512
  //
  // So the step is : 0 TO g_nArraySize STEP 2^(g_nPrecArrayMax-i_nSect) = 512
  //
  // If i_nSect = 1, the Height Field is :
  //
  // 0,0   +----------- 256 -----------+----------- 256 -----------+ 512,0
  //       |                           |256,0                      |
  //       |                           |                           |
  //       |                           |                           |
  //       |                           |                           |
  //       |                           |                           |
  //       |                           |                           |
  //       ~                           ~                           ~
  //      256                         256                         256
  //       ~                           ~                           ~
  //       |                           |                           |
  //       |                           |                           |
  //       |                           |                           |
  //       |                           |                           |
  //       |                           |                           |
  //       |                           |                           |
  // 0,256 +---------------------------+---------------------------+ 512,256
  //       |                           |256,256                    |
  //       |                           |                           |
  //       |                           |                           |
  //       |                           |                           |
  //       |                           |                           |
  //       |                           |                           |
  //       ~                           ~                           ~
  //      256                         256                         256
  //       ~                           ~                           ~
  //       |                           |                           |
  //       |                           |                           |
  //       |                           |                           |
  //       |                           |                           |
  //       |                           |                           |
  //       |                           |                           |
  // 0,512 +----------- 256 -----------+----------- 256 -----------+ 512,512
  //                                    256,512
  //
  // So the step is : 0 TO g_nArraySize STEP 2^(g_nPrecArrayMax - i_nSect) = 256
  //
  // And so the Story goes...
  //
  //          +- \
  //          |  |
  //          |  |
  //          |  |
  // _level -+   > 2 * _chaos
  //          |  |
  //          |  |
  //          |  |
  //          +- /
  //
  // That's to say : alt = (2 * _chaos * RND) - _chaos + _level
  //
  
  int l_nTmpMntDevRnd = 2 * g_nMntDevChaos;
  int l_nTmpMntDevVar = 0 - g_nMntDevChaos + g_nMntDevLevel;
  int l_nTmpMntAltRnd = 2 * g_nMntAltChaos;
  int l_nTmpMntAltVar = 0 - g_nMntAltChaos + g_nMntAltLevel;

  int l_nLoopX;
  int l_nLoopY;
  int l_nStep = pow2(g_nPrecArrayMax - i_nSect);
  
  for
  ( l_nLoopY  = 0
  ; l_nLoopY <  g_nArraySize
  ; l_nLoopY += l_nStep
  )
  {
    for
    ( l_nLoopX  = 0
    ; l_nLoopX <  g_nArraySize
    ; l_nLoopX += l_nStep
    )
    {
      g_anMntDev[l_nLoopX][l_nLoopY] = (l_nTmpMntDevRnd * rand()) + l_nTmpMntDevVar;
      g_anMntAlt[l_nLoopX][l_nLoopY] = (l_nTmpMntAltRnd * rand()) + l_nTmpMntAltVar;
    }
  }
}

void ComputeMnt(int i_nSect, int i_nPrecStart, int i_nPrecEnd)
{
  // OK : 06/28/12/01

  int l_nAltPrevX;
  int l_nAltNextX;
  int l_nAltPrevY;
  int l_nAltNextY;

  int l_nAltAmplitude;
  int l_nLoopX;
  int l_nLoopY;
  int n;

  int l_nPrecLoop;

  int l_nWorkCur = 0;
  int l_nWorkEnd = ((pow2(i_nSect + i_nPrecEnd) + 1)^2) - ((pow2(i_nSect + (i_nPrecStart - 1)) + 1)^2);
  
  cls();

  printf("Computing of %d plot in progress...", l_nWorkEnd); // * 2);
  
  g_nTimeStart = GetTickCount();
  
  for
  ( l_nPrecLoop  = i_nPrecStart
  ; l_nPrecLoop <  i_nPrecEnd
  ; l_nPrecLoop += 1
  )
  {
    //
    // Here i_nPrecEnd = g_nPrecCur !
    //
    // With g_nPrecArrayMax = 9, the Height Field Sector Size is (pow29)+1 = 513
    // If i_nSect = 0, the Height Field is :
    //
    // 0,0   +------------------------- 512 -------------------------+ 512,0
    //       |                                                       |
    //       |                                                       |
    //       |                                                       |
    //       |                                                       |
    //       |                                                       |
    //       |                                                       |
    //       |                                                       |
    //       |                                                       |
    //       |                                                       |
    //       |                                                       |
    //       |                                                       |
    //       |                                                       |
    //       |                                                       |
    //       |                                                       |
    //       ~                                                       ~
    //      512                                                     512
    //       ~                                                       ~
    //       |                                                       |
    //       |                                                       |
    //       |                                                       |
    //       |                                                       |
    //       |                                                       |
    //       |                                                       |
    //       |                                                       |
    //       |                                                       |
    //       |                                                       |
    //       |                                                       |
    //       |                                                       |
    //       |                                                       |
    //       |                                                       |
    //       |                                                       |
    // 0,512 +------------------------- 512 -------------------------+ 512,512
    //
    // If g_nPrecCur = 1, the Height Field become :
    //
    // 0,0   +----------- 256 -----------+----------- 256 -----------+ 512,0
    //       |                           |256,0                      |
    //       |                           |                           |
    //       |                           |                           |
    //       |                           |                           |
    //       |                           |                           |
    //       |                           |                           |
    //       ~                           ~                           ~
    //      256                         256                         256
    //       ~                           ~                           ~
    //       |                           |                           |
    //       |                           |                           |
    //       |                           |                           |
    //       |                           |                           |
    //       |                           |                           |
    //       |                           |                           |
    // 0,256 +---------------------------+---------------------------+ 512,256
    //       |                           |256,256                    |
    //       |                           |                           |
    //       |                           |                           |
    //       |                           |                           |
    //       |                           |                           |
    //       |                           |                           |
    //       ~                           ~                           ~
    //      256                         256                         256
    //       ~                           ~                           ~
    //       |                           |                           |
    //       |                           |                           |
    //       |                           |                           |
    //       |                           |                           |
    //       |                           |                           |
    //       |                           |                           |
    // 0,512 +----------- 256 -----------+----------- 256 -----------+ 512,512
    //                                    256,512
    //
    // If g_nPrecCur = 2, the Height Field become :
    //
    // 0,0   +---- 128 ----+---- 128 ----+---- 128 ----+---- 128 ----+ 512,0
    //       |             |128,0        |256,0        |384,0        |
    //       |             |             |             |             |
    //       |             |             |             |             |
    //      128           128           128           128           128
    //       |             |             |             |             |
    //       |             |             |             |             |
    //       |             |             |             |             |
    // 0,128 +---- 128 ----+---- 128 ----+---- 128 ----+---- 128 ----+ 512,128
    //       |             |128,128      |256,128      |384,128      |
    //       |             |             |             |             |
    //       |             |             |             |             |
    //      128           128           128           128           128
    //       |             |             |             |             |
    //       |             |             |             |             |
    //       |             |             |             |             |
    // 0,256 +---- 128 ----+---- 128 ----+---- 128 ----+---- 128 ----+ 512,256
    //       |             |128,256      |256,256      |384,256      |
    //       |             |             |             |             |
    //       |             |             |             |             |
    //      128           128           128           128           128
    //       |             |             |             |             |
    //       |             |             |             |             |
    //       |             |             |             |             |
    // 0,384 +---- 128 ----+---- 128 ----+---- 128 ----+---- 128 ----+ 512,384
    //       |             |128,384      |256,384      |384,384      |
    //       |             |             |             |             |
    //       |             |             |             |             |
    //      128           128           128           128           128
    //       |             |             |             |             |
    //       |             |             |             |             |
    //       |             |             |             |             |
    // 0,512 +---- 128 ----+---- 128 ----+---- 128 ----+---- 128 ----+ 512,512
    //                      128,512       256,512       384,512
    //
    // ----> BUT !!! But, but, but, don't forget that - IF -... <----
    // If i_nSect = 1, the Height Field is :
    //
    // 0,0   +----------- 256 -----------+----------- 256 -----------+ 512,0
    //       |                           |256,0                      |
    //       |                           |                           |
    //       |                           |                           |
    //       |                           |                           |
    //       |                           |                           |
    //       |                           |                           |
    //       ~                           ~                           ~
    //      256                         256                         256
    //       ~                           ~                           ~
    //       |                           |                           |
    //       |                           |                           |
    //       |                           |                           |
    //       |                           |                           |
    //       |                           |                           |
    //       |                           |                           |
    // 0,256 +---------------------------+---------------------------+ 512,256
    //       |                           |256,256                    |
    //       |                           |                           |
    //       |                           |                           |
    //       |                           |                           |
    //       |                           |                           |
    //       |                           |                           |
    //       ~                           ~                           ~
    //      256                         256                         256
    //       ~                           ~                           ~
    //       |                           |                           |
    //       |                           |                           |
    //       |                           |                           |
    //       |                           |                           |
    //       |                           |                           |
    //       |                           |                           |
    // 0,512 +----------- 256 -----------+----------- 256 -----------+ 512,512
    //
    // And IF g_nPrecCur = 1, the Height Field become :
    //
    // 0,0   +---- 128 ----+---- 128 ----+---- 128 ----+---- 128 ----+ 512,0
    //       |             |128,0        |256,0        |384,0        |
    //       |             |             |             |             |
    //       |             |             |             |             |
    //      128           128           128           128           128
    //       |             |             |             |             |
    //       |             |             |             |             |
    //       |             |             |             |             |
    // 0,128 +---- 128 ----+---- 128 ----+---- 128 ----+---- 128 ----+ 512,128
    //       |             |128,128      |256,128      |384,128      |
    //       |             |             |             |             |
    //       |             |             |             |             |
    //      128           128           128           128           128
    //       |             |             |             |             |
    //       |             |             |             |             |
    //       |             |             |             |             |
    // 0,256 +---- 128 ----+---- 128 ----+---- 128 ----+---- 128 ----+ 512,256
    //       |             |128,256      |256,256      |384,256      |
    //       |             |             |             |             |
    //       |             |             |             |             |
    //      128           128           128           128           128
    //       |             |             |             |             |
    //       |             |             |             |             |
    //       |             |             |             |             |
    // 0,384 +---- 128 ----+---- 128 ----+---- 128 ----+---- 128 ----+ 512,384
    //       |             |128,384      |256,384      |384,384      |
    //       |             |             |             |             |
    //       |             |             |             |             |
    //      128           128           128           128           128
    //       |             |             |             |             |
    //       |             |             |             |             |
    //       |             |             |             |             |
    // 0,512 +---- 128 ----+---- 128 ----+---- 128 ----+---- 128 ----+ 512,512
    //                      128,512       256,512       384,512
    //
    // And so the Story goes...
    //
    // So that, l_nCellHalfSize = pow2(g_nPrecArrayMax - i_nSect - l_nPrecLoop)
    //
    
    int l_nCellHalfSize = pow2(g_nPrecArrayMax - i_nSect - l_nPrecLoop);
    int l_nCellSize     = l_nCellHalfSize * 2;
    int l_nAltDivider   = pow2(l_nPrecLoop - 1); // -> 100% au-lieu de 2^x
    int l_nAltAmplitude = g_nAltDevMax / l_nAltDivider;

    int l_nTempoMntDev;
    int l_nTempoMntAlt;
    
    //
    // Into the Height Field
    //
    
    for
    ( l_nLoopY  = l_nCellHalfSize
    ; l_nLoopY <  (g_nArraySize - l_nCellHalfSize)
    ; l_nLoopY += l_nCellSize
    )
    {
      for
      ( l_nLoopX  = l_nCellHalfSize
      ; l_nLoopX <  (g_nArraySize - l_nCellHalfSize)
      ; l_nLoopX += l_nCellSize
      )
      {
        //
        // Imagine a Matrix of, hum... 5*5 points, that's to say :
        //
        // +-------+-------+-------+-------+
        // |       |       |       |       |
        // |       |       |       |       |
        // |       |       |       |       |
        // +-------+-------+-------+-------+
        // |       |       |       |       |
        // |       |       |       |       |
        // |       |       |       |       |
        // +-------+-------+-------+-------+
        // |       |       |       |       |
        // |       |       |       |       |
        // |       |       |       |       |
        // +-------+-------+-------+-------+
        // |       |       |       |       |
        // |       |       |       |       |
        // |       |       |       |       |
        // +-------+-------+-------+-------+
        //
        // Now, we'll calculate all the interlaced points in EACH line :
        //
        // +---*---+---*---+-------+-------+
        // |   |   |   |   |       |       |
        // *---*---*---*-> |       |       |
        // |       |       |       |       |
        // +-------+-------+-------+-------+
        // |       |       |       |       |
        // |       |       |       |       |
        // |       |       |       |       |
        // +-------+-------+-------+-------+
        // |       |       |       |       |
        // |       |       |       |       |
        // |       |       |       |       |
        // +-------+-------+-------+-------+
        // |       |       |       |       |
        // |       |       |       |       |
        // |       |       |       |       |
        // +-------+-------+-------+-------+
        //
        // Then, until...
        //
        // +---*---+---*---+---*---+---*---+
        // |   |   |   |   |   |   |   |   |
        // *---*---*---*---*---*---*---*---*
        // |   |   |   |   |   |   |   |   |
        // +---*---+---*---+---*---+---*---+
        // |   |   |   |   |   |   |   |   |
        // *---*---*---*---*---*---*---*---*
        // |   |   |   |   |       |       |
        // +---*---+---*---+-------+-------+
        // |   |   |   |   |       |       |
        // *---*---*---*-> |       |       |
        // |       |       |       |       |
        // +-------+-------+-------+-------+
        // |       |       |       |       |
        // |       |       |       |       |
        // |       |       |       |       |
        // +-------+-------+-------+-------+
        //
        // ...this states :
        //
        // +---*---+---*---+---*---+---*---+
        // |   |   |   |   |   |   |   |   |
        // *---*---*---*---*---*---*---*---*
        // |   |   |   |   |   |   |   |   |
        // +---*---+---*---+---*---+---*---+
        // |   |   |   |   |   |   |   |   |
        // *---*---*---*---*---*---*---*---*
        // |   |   |   |   |   |   |   |   |
        // +---*---+---*---+---*---+---*---+
        // |   |   |   |   |   |   |   |   |
        // *---*---*---*---*---*---*---*---*
        // |   |   |   |   |   |   |   |   |
        // +---*---+---*---+---*---+---*---+
        // |   |   |   |   |   |   |   |   |
        // *---*---*---*---*---*---*---*---*
        // |       |       |       |       |
        // +-------+-------+-------+-------+
        //
        // Let's START :
        //
        // First of ALL, let's define the Matrix on which we'R about to work :
        //
        // l_nLoopX-l_nCellHalfSize, +----------------*----------------+-- l_nLoopX+l_nCellHalfSize,
        // l_nLoopY-l_nCellHalfSize  |                |l_nLoopX,l_nLoopY-l_nCellHalfSize  |   l_nLoopY-l_nCellHalfSize
        //  -> P-1-1    |                | -> P+0-1       |    -> P+1-1
        //              ~                ~                ~
        //
        //              ~                ~                ~
        //              |                |                |
        //              |                |                |
        // l_nLoopX-l_nCellHalfSize, *----------------*---------------(*)- l_nLoopX+l_nCellHalfSize,
        // l_nLoopY           |                |l_nLoopX,l_nLoopY           |   l_nLoopY
        //  -> P-1+0    |                |Central Point   |    -> P+1+0
        //              ~                ~ -> P+0+0       ~
        //
        //              ~                ~                ~
        //              |                |                |
        //              |                |                |
        // l_nLoopX-l_nCellHalfSize, +---------------(*)---------------+-- l_nLoopX+l_nCellHalfSize,
        // l_nLoopY+l_nCellHalfSize  |                |l_nLoopX,l_nLoopY+l_nCellHalfSize  |   l_nLoopY+l_nCellHalfSize
        //  -> P-1+1                       -> P+0+1            -> P+1+1
        //
        // There the following points are known :
        //  -> P-1-1 \
        //  -> P+1-1  \ They have been calculated in PROCEDURE RandomizeMnt
        //  -> P-1+1  /
        //  -> P+1+1 /
        //
        // We have NOW to calculate the following points :
        //  -> P+0-1
        //  -> P-1+0
        //  -> P+0+0
        //
        // The following points will be calculated at the NEXT inter
        //  -> P+1+0, then also known as -> P-1+0
        //  -> P+0+1, then also known as -> P+0-1
        //
        
        l_nAltPrevX = l_nLoopX - l_nCellHalfSize;
        l_nAltNextX = l_nLoopX + l_nCellHalfSize;
        l_nAltPrevY = l_nLoopY - l_nCellHalfSize;
        l_nAltNextY = l_nLoopY + l_nCellHalfSize;
        
        //
        // And now, it's time to calculate -> P-1+0
        //  -> P-1+0 = (P-1-1 + P-1+1)/2 + random_height
        //
        
        l_nTempoMntDev = (g_anMntDev(l_nAltPrevX, l_nAltPrevY) + g_anMntDev(l_nAltPrevX, l_nAltNextY)) / 2 + INT((rand() - 0.5) * l_nAltAmplitude);
        g_anMntDev(l_nAltPrevX, l_nLoopY) = l_nTempoMntDev;
        
        //
        // There, it's high time to calculate for the Final Height Field :
        //  -> P-1+0 = (P-1-1 + P-1+1)/2 + fractal_height
        //
        
        l_nTempoMntAlt = abs(l_nTempoMntDev) / l_nAltDivider;
        g_anMntAlt(l_nAltPrevX, l_nLoopY) = (g_anMntAlt(l_nAltPrevX, l_nAltPrevY) + g_anMntAlt(l_nAltPrevX, l_nAltNextY)) / 2 + INT((rand() - 0.5) * l_nTempoMntAlt);
        
        //
        //  -> P+0-1 = (P-1-1 + P+1-1)/2 + random_height
        //
        
        l_nTempoMntDev = (g_anMntDev(l_nAltPrevX, l_nAltPrevY) + g_anMntDev(l_nAltNextX, l_nAltPrevY)) / 2 + INT((rand() - 0.5) * l_nAltAmplitude);
        g_anMntDev(l_nLoopX, l_nAltPrevY) = l_nTempoMntDev;
        
        //
        //  -> P+0-1 = (P-1-1 + P+1-1)/2 + fractal_height
        //
        
        l_nTempoMntAlt = abs(l_nTempoMntDev) / l_nAltDivider;
        g_anMntAlt(l_nLoopX, l_nAltPrevY) = (g_anMntAlt(l_nAltPrevX, l_nAltPrevY) + g_anMntAlt(l_nAltNextX, l_nAltPrevY)) / 2 + INT((rand() - 0.5) * l_nTempoMntAlt);
        
        //
        //  -> P+0+0 = (P-1-1 + P-1+1 + P+1-1 + P+1+1)/4 + random_height
        //
        
        l_nTempoMntDev = (g_anMntDev(l_nAltPrevX, l_nAltPrevY) + g_anMntDev(l_nAltPrevX, l_nAltNextY) + g_anMntDev(l_nAltNextX, l_nAltPrevY) + g_anMntDev(l_nAltNextX, l_nAltNextY)) / 4 + INT((rand() - 0.5) * l_nAltAmplitude);
        g_anMntDev(l_nLoopX, l_nLoopY) = l_nTempoMntDev;
        
        //
        //  -> P+0+0 = (P-1-1 + P-1+1 + P+1-1 + P+1+1)/4 + fractal_height
        //
        
        l_nTempoMntAlt = abs(l_nTempoMntDev) / l_nAltDivider;
        g_anMntAlt(l_nLoopX, l_nLoopY) = (g_anMntAlt(l_nAltPrevX, l_nAltPrevY) + g_anMntAlt(l_nAltPrevX, l_nAltNextY) + g_anMntAlt(l_nAltNextX, l_nAltPrevY) + g_anMntAlt(l_nAltNextX, l_nAltNextY)) / 4 + INT((rand() - 0.5) * l_nTempoMntAlt);
        
        //
        
        l_nWorkCur = l_nWorkCur + 3;
        
        if(l_nLoopY == g_nArraySize - l_nCellHalfSize)
        {
          //
          // End of Rows
          //
          // We have now, i_nPrecStart the previous DOUBLE LOOP :
          //
          // +---*---+---*---+---*---+---*---+
          // |   |   |   |   |   |   |   |   |
          // *---*---*---*---*---*---*---*---*
          // |   |   |   |   |   |   |   |   |
          // +---*---+---*---+---*---+---*---+
          // |   |   |   |   |   |   |   |   |
          // *---*---*---*---*---*---*---*---*
          // |   |   |   |   |   |   |   |   |
          // +---*---+---*---+---*---+---*---+
          // |   |   |   |   |   |   |   |   |
          // *---*---*---*---*---*---*---*---*
          // |   |   |   |   |   |   |   |   |
          // +---*---+---*---+---*---+---*---+
          // |   |   |   |   |   |   |   |   |
          // *---*---*---*---*---*---*---*---*
          // |       |       |       |       |
          // +-------+-------+-------+-------+
          //
          // OK, now we'R there, @ the end of a Row :
          //
          // End of a Row :
          //
          //       |                      |                      |
          //       |                      |                      |
          //  -----+----------------------*----------------------+-----
          //  l_nLoopX-l_nCellHalfSize, g_nArraySize l_nLoopX, g_nArraySize    l_nLoopX+l_nCellHalfSize, g_nArraySize
          //         -> P-1                 -> P+0                 -> P+1
          //
          // There, the following points are known :
          //  -> P-1 \ They have been calculated in PROCEDURE RandomizeMnt
          //  -> P+1 /
          //
          // We have now to calculate the following point :
          //  -> P+0
          //
          // And now, it's time to calculate -> P+0
          //  -> P+0 = (P-1 + P+1)/2 + random_height
          //
          // What we have to do now, is to end the LAST ROW and the LAST LINE
          //
          // +---*---+---*---+---*---+---*---+
          // |   |   |   |   |   |   |   |   |
          // *---*---*---*---*---*---*---*---*
          // |   |   |   |   |   |   |   |   |
          // +---*---+---*---+---*---+---*---+
          // |   |   |   |   |   |   |   |   |
          // *---*---*---*---*---*---*---*---*
          // |   |   |   |   |   |   |   |   |
          // +---*---+---*---+---*---+---*---+
          // |   |   |   |   |   |   |   |   |
          // *---*---*---*---*---*---*---*---*
          // |   |   |   |   |   |   |   |   |
          // +---*---+---*---+---*---+---*---+
          // |   |   |   |   |   |   |   |   |
          // *---*---*---*---*---*---*---*---*
          // |   |   |   |   |       |       |
          // +---*---+---*---+-------+-------+
          //             ->
          //
          // Then, we'll finaly get :
          //
          // +---*---+---*---+---*---+---*---+
          // |   |   |   |   |   |   |   |   |
          // *---*---*---*---*---*---*---*---*
          // |   |   |   |   |   |   |   |   |
          // +---*---+---*---+---*---+---*---+
          // |   |   |   |   |   |   |   |   |
          // *---*---*---*---*---*---*---*---*
          // |   |   |   |   |   |   |   |   |
          // +---*---+---*---+---*---+---*---+
          // |   |   |   |   |   |   |   |   |
          // *---*---*---*---*---*---*---*---*
          // |   |   |   |   |   |   |   |   |
          // +---*---+---*---+---*---+---*---+
          // |   |   |   |   |   |   |   |   |
          // *---*---*---*---*---*---*---*---*
          // |   |   |   |   |   |   |   |   |
          // +---*---+---*---+---*---+---*---+
          //
          // @ the next inter, we'll get :
          //
          // +-*-+-*-+-*-+-*-+-*-+-*-+-*-+-*-+
          // *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
          // +-*-+-*-+-*-+-*-+-*-+-*-+-*-+-*-+
          // *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
          // +-*-+-*-+-*-+-*-+-*-+-*-+-*-+-*-+
          // *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
          // +-*-+-*-+-*-+-*-+-*-+-*-+-*-+-*-+
          // *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
          // +-*-+-*-+-*-+-*-+-*-+-*-+-*-+-*-+
          // *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
          // +-*-+-*-+-*-+-*-+-*-+-*-+-*-+-*-+
          // *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
          // +-*-+-*-+-*-+-*-+-*-+-*-+-*-+-*-+
          // *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
          // +-*-+-*-+-*-+-*-+-*-+-*-+-*-+-*-+
          // *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
          // +-*-+-*-+-*-+-*-+-*-+-*-+-*-+-*-+
          //
          
          l_nTempoMntDev = (g_anMntDev(l_nAltPrevX, g_nArraySize) + g_anMntDev(l_nAltNextX, g_nArraySize)) / 2 + INT((rand() - 0.5) * l_nAltAmplitude);
          g_anMntDev(l_nLoopX, g_nArraySize) = l_nTempoMntDev;
          
          l_nTempoMntAlt = abs(l_nTempoMntDev) / l_nAltDivider;
          g_anMntAlt(l_nLoopX, g_nArraySize) = (g_anMntAlt(l_nAltPrevX, g_nArraySize) + g_anMntAlt(l_nAltNextX, g_nArraySize)) / 2 + INT((rand() - 0.5) * l_nTempoMntAlt);
          
          l_nWorkCur += 1;
        }
      }
      
      //
      // OK, there, let's END the LINE
      //
      // We've something like this...
      //
      // +---*---+---*---+---*---+---*---+
      // |   |   |   |   |   |   |   |   |
      // *---*---*---*---*---*---*---*   |
      // |       |       |       |       |
      // +-------+-------+-------+-------+
      // |       |       |       |       |
      //
      // ...and we shouda get something like this :
      //
      // +---*---+---*---+---*---+---*---+
      // |   |   |   |   |   |   |   |   |
      // *---*---*---*---*---*---*---*---*
      // |       |       |       |       |
      // +-------+-------+-------+-------+
      // |       |       |       |       |
      //
      // End of a Line :
      //
      //       |
      //  -----+ g_nArraySize,l_nLoopX - l_nCellHalfSize
      //       |  -> P-1
      //       |
      //       |
      //       |
      //  -----* g_nArraySize,l_nLoopX
      //       |  -> P+0
      //       |
      //       |
      //       |
      //  -----+ g_nArraySize,l_nLoopX + l_nCellHalfSize
      //       |  -> P+1
      //
      // There, the following points are known :
      //  -> P-1 \ They have been calculated in PROCEDURE RandomizeMnt
      //  -> P+1 /
      //
      // We have now to calculate the following point :
      //  -> P+0
      //
      // And now, it's time to calculate -> P+0
      //  -> P+0 = (P-1 + P+1)/2 + random_height
      //
      
      l_nTempoMntDev = (g_anMntDev(g_nArraySize, l_nAltPrevY) + g_anMntDev(g_nArraySize, l_nAltNextY)) / 2 + INT((rand() - 0.5) * l_nAltAmplitude);
      g_anMntDev(g_nArraySize, l_nLoopY) = l_nTempoMntDev;
      
      l_nTempoMntAlt = abs(l_nTempoMntDev) / l_nAltDivider;
      g_anMntAlt(g_nArraySize, l_nLoopY) = (g_anMntAlt(g_nArraySize, l_nAltPrevY) + g_anMntAlt(g_nArraySize, l_nAltNextY)) / 2 + INT((rand() - 0.5) * l_nTempoMntAlt);
      
      l_nWorkCur += 1;
      
      DisplayProgress(l_nWorkCur, l_nWorkEnd);
    }
  }
  
  printf("\r\n");
  printf("That's FINI (%d points/seconde) !!!\r\n", l_nWorkEnd / (current / (2 * 200)));
  
  waitkey();
}

void DisplayProgress(int i_nWorkCur, int i_nWorkEnd)
{
  // OK : 01/13/02/00

  // This WorkTimer let you know some infos about the l_nTimeCurrent calculation
  
  printf(AT(2,3);"Start   : %d seconde ", (g_nTimeStart - g_nTimeStart) / 1000)
  
  int l_nTimeCurrent = GetTickCount() - g_nTimeStart;
  
  printf(AT(2,4);"Current : %d secondes ", l_nTimeCurrent / 1000)
  printf(AT(2,5);"Work    : %d ", (i_nWorkCur  * 100) / i_nWorkEnd)
  
  int l_nTimeLeft = ((l_nTimeCurrent * i_nWorkEnd) / i_nWorkCur) - l_nTimeCurrent;
  
  printf(AT(2,6);"Left    : %d secondes ", l_nTimeLeft / 1000)
  
  int l_nTimeEnd = l_nTimeCurrent + l_nTimeLeft;
  
  printf(AT(2,7);"End     : %d secondes ", l_nTimeEnd / 1000)
}

void Display2D(i_nCurSect, i_nMaxDisp, i_nMntColor, i_nMntType)
{
  // OK : 06/28/12/01

  // IF i_nCurSect = 1 && g_nPrecCur = 1, the Height Field is :
  //
  // 0,0   +---- 128 ----+---- 128 ----+---- 128 ----+---- 128 ----+ 512,0
  //       |             |128,0        |256,0        |384,0        |
  //       |             |             |             |             |
  //       |             |             |             |             |
  //      128           128           128           128           128
  //       |             |             |             |             |
  //       |             |             |             |             |
  //       |             |             |             |             |
  // 0,128 +---- 128 ----+---- 128 ----+---- 128 ----+---- 128 ----+ 512,128
  //       |             |128,128      |256,128      |384,128      |
  //       |             |             |             |             |
  //       |             |             |             |             |
  //      128           128           128           128           128
  //       |             |             |             |             |
  //       |             |             |             |             |
  //       |             |             |             |             |
  // 0,256 +---- 128 ----+---- 128 ----+---- 128 ----+---- 128 ----+ 512,256
  //       |             |128,256      |256,256      |384,256      |
  //       |             |             |             |             |
  //       |             |             |             |             |
  //      128           128           128           128           128
  //       |             |             |             |             |
  //       |             |             |             |             |
  //       |             |             |             |             |
  // 0,384 +---- 128 ----+---- 128 ----+---- 128 ----+---- 128 ----+ 512,384
  //       |             |128,384      |256,384      |384,384      |
  //       |             |             |             |             |
  //       |             |             |             |             |
  //      128           128           128           128           128
  //       |             |             |             |             |
  //       |             |             |             |             |
  //       |             |             |             |             |
  // 0,512 +---- 128 ----+---- 128 ----+---- 128 ----+---- 128 ----+ 512,512
  //                      128,512       256,512       384,512
  //
  // So we have to pick up the Height every :
  // l_nStep = pow2(g_nPrecArrayMax - i_nCurSect - i_nMaxDisp)
  //
  
  int l_nLoopX;
  int l_nLoopY;
  int n;
  int t;
  int l_nColor;
  
  //
  
  int affratio  = pow2(g_nPrecArrayMax - g_nPrecDispMax);
  int l_nStep   = pow2(g_nPrecArrayMax - i_nCurSect - i_nMaxDisp);
  int ratio     = g_nSectCur + 1;
  int l_nStartX = (g_nScreenW - (g_nArraySize / affratio)) / 2;
  int l_nStartY = (g_nScreenH - (g_nArraySize / affratio)) / 2;
  
  cls();

  TEXT (g_nScreenW - 208) / 2, 10, 220, "FRActal MOuntain GENerator"

  for
  ( l_nLoopY  = 0
  ; l_nLoopY <  (g_nArraySize - l_nStep)
  ; l_nLoopY += l_nStep
  )
  {
    for
    ( l_nLoopX  = 0
    ; l_nLoopY <  (g_nArraySize - l_nStep)
    ; l_nLoopY += l_nStep
    )
    {
      GetMnt2D(i_nMntType);

      if(i_nMntColor == 1)
      {
        l_nColor = GetColorLum(g_an3dMntY(0), g_an3dMntY(1), g_an3dMntY(2), g_an3dMntY(3), l_nStep);
      }
      else
      {
        if(i_nMntType == 0)
        {
          l_nColor = GetColorAlt(g_anMntDev(l_nLoopX, l_nLoopY));
        }
        else
        {
          l_nColor = GetColorAlt(g_anMntAlt(l_nLoopX, l_nLoopY));
        }
      }

      //

      g_an3dAffX(0) = l_nStartX +  l_nLoopX            / affratio;
      g_an3dAffY(0) = l_nStartY +  l_nLoopY            / affratio;
      g_an3dAffX(1) = l_nStartX + (l_nLoopX + l_nStep) / affratio - 1;
      g_an3dAffY(1) = l_nStartY +  l_nLoopY            / affratio;
      g_an3dAffX(2) = l_nStartX + (l_nLoopX + l_nStep) / affratio - 1;
      g_an3dAffY(2) = l_nStartY + (l_nLoopY + l_nStep) / affratio - 1;
      g_an3dAffX(3) = l_nStartX +  l_nLoopX            / affratio;
      g_an3dAffY(3) = l_nStartY + (l_nLoopY + l_nStep) / affratio - 1;

      //

      DEFFILL l_nColor
      POLYFILL 4, g_an3dAffX(), g_an3dAffY()
    }
  }

  //

  waitkey();
}

void Display3D(int i_nCurSect, int i_nMaxDisp, int i_nMntColor, int i_nMntType)
{
  // OK : 06/28/12/01

  // IF i_nCurSect = 1 && i_nMaxDisp = 1, the Height Field is :
  //
  // 0,0   +---- 128 ----+---- 128 ----+---- 128 ----+---- 128 ----+ 512,0
  //       |             |128,0        |256,0        |384,0        |
  //       |             |             |             |             |
  //       |             |             |             |             |
  //      128           128           128           128           128
  //       |             |             |             |             |
  //       |             |             |             |             |
  //       |             |             |             |             |
  // 0,128 +---- 128 ----+---- 128 ----+---- 128 ----+---- 128 ----+ 512,128
  //       |             |128,128      |256,128      |384,128      |
  //       |             |             |             |             |
  //       |             |             |             |             |
  //      128           128           128           128           128
  //       |             |             |             |             |
  //       |             |             |             |             |
  //       |             |             |             |             |
  // 0,256 +---- 128 ----+---- 128 ----+---- 128 ----+---- 128 ----+ 512,256
  //       |             |128,256      |256,256      |384,256      |
  //       |             |             |             |             |
  //       |             |             |             |             |
  //      128           128           128           128           128
  //       |             |             |             |             |
  //       |             |             |             |             |
  //       |             |             |             |             |
  // 0,384 +---- 128 ----+---- 128 ----+---- 128 ----+---- 128 ----+ 512,384
  //       |             |128,384      |256,384      |384,384      |
  //       |             |             |             |             |
  //       |             |             |             |             |
  //      128           128           128           128           128
  //       |             |             |             |             |
  //       |             |             |             |             |
  //       |             |             |             |             |
  // 0,512 +---- 128 ----+---- 128 ----+---- 128 ----+---- 128 ----+ 512,512
  //                      128,512       256,512       384,512

  // So we have to pick up the Height every :
  // l_nStep = pow2(g_nPrecArrayMax - i_nCurSect - i_nMaxDisp)

  int l_nLoopX;
  int l_nLoopY;
  int l_nStep  = pow2(g_nPrecArrayMax - i_nCurSect - i_nMaxDisp);

  int d;
  int t;
  int p;
  int l_nColor;
  int ok;
  int x;
  int y;
  
  //

  t       = pow2(i_nCurSect + i_nMaxDisp);
  d       = g_nScreenW;
  
  if(g_nParamDisp3D == 0)
  {
    ratio = 1;
    sad1  = pow2i_nCurSect;
    sad2  = pow2(i_nCurSect + 1);
  }
  else
  {
    ratio = (i_nCurSect + 1) * 2;
    sad1  = 1;
    sad2  = 2;
  }

  cls();

  if(g_nParamDisp3D == 0)
  {
    printf(AT(2 + 2 * stab, 3);"Ground View")
  }
  else
  {
    printf(AT(2 + 2 * stab, 3);"Space View")
  }

  DisplayTimeDate(0);

  for
  ( l_nLoopY  = 0
  ; l_nLoopY <  (g_nArraySize - l_nStep)
  ; l_nLoopY += l_nStep
  )
  {
    for
    ( l_nLoopX  = 0
    ; l_nLoopX <  (g_nArraySize - l_nStep)
    ; l_nLoopX += l_nStep
    )
    {
      GetMnt2D(i_nMntType);

      if(i_nMntColor == 1)
      {
        l_nColor = GetColorLum(g_an3dMntY(0), g_an3dMntY(1), g_an3dMntY(2), g_an3dMntY(3), l_nStep);
      }
      else
      {
        if(i_nMntType == 0)
        {
          l_nColor = GetColorAlt(g_anMntDev(l_nLoopX, l_nLoopY));
        }
        else
        {
          l_nColor = GetColorAlt(g_anMntAlt(l_nLoopX, l_nLoopY));
        }
      }

      x = l_nLoopX / l_nStep;
      y = l_nLoopY / l_nStep;

      g_an3dMntX(0) = (-sad1 * g_nScreenW) + (sad2 * g_nScreenW) *  x      / t;
      g_an3dMntX(1) = (-sad1 * g_nScreenW) + (sad2 * g_nScreenW) * (x + 1) / t;
      g_an3dMntX(2) = g_an3dMntX(1);
      g_an3dMntX(3) = g_an3dMntX(0);

      GetMnt3D(i_nMntType);

      g_an3dMntZ(0) = (sad2 * g_nScreenW) - (sad2 * g_nScreenW) *  y      / t;
      g_an3dMntZ(1) = g_an3dMntZ(0);
      g_an3dMntZ(2) = (sad2 * g_nScreenW) - (sad2 * g_nScreenW) * (y + 1) / t;
      g_an3dMntZ(3) = g_an3dMntZ(2);

      ok = 4;

      for
      ( p  = 0
      ; p <  3
      ; p += 1
      )
      {
        g_an3dAffX(p) = g_an3dMntX(p) * d / (d + g_an3dMntZ(p));
        g_an3dAffY(p) = g_an3dMntY(p) * d / (d + g_an3dMntZ(p));

        if(abs(g_an3dAffX(p)) > (g_nScreenW / 2) || abs(g_an3dAffY(p)) > (g_nScreenW / 2))
        {
          ok -= 1;
        }
      }

      if(ok > 0)
      {
        for
        ( p  = 0
        ; p <  3
        ; p += 1
        )
        {
          g_an3dAffX(p) += g_nScreenW / 2;
          g_an3dAffY(p) += g_nScreenH / 2;
        }

        DEFFILL  l_nColor;
        POLYFILL 4, g_an3dAffX(), g_an3dAffY();
      }

    } // for(l_nLoopX)

    printf(AT(2 +     stab, 4); "Time = %s", TIME$)
    printf(AT(2 + 2 * stab, 5); "Done = %d", (l_nLoopY / (g_nArraySize - l_nStep)) * 100)
  } // for(l_nLoopY)

  DisplayTimeDate(1);

  waitkey();
}

void DisplayTimeDate(int i_nMode)
{
  // OK : 06/10/11/2000

  // Print the Time & Date of Start & End of Display
  printf(AT(1,1); g_anStrTitle);

  if(i_nMode == 0)
  {
    printf(AT(2       , 3); "Start @");
  }
  else
  {
    printf(AT(2 + stab, 3); "End @");
  }

  printf(AT(2, 4); "%s Time = %s", (i_nMode != 0)?stab;"", TIME$);
  printf(AT(2, 5); "%s Date = %s", (i_nMode != 0)?stab;"", DATE$);
}

void GetMnt2D(int i_nX, int i_nY, int i_nStep, int i_nMntType)
{
  // OK : 06/23/06/00

  if(i_nMntType == 0)
  {
    g_an3dMntY(0) = g_anMntDev[i_nX          ][i_nY          ];
    g_an3dMntY(1) = g_anMntDev[i_nX + i_nStep][i_nY          ];
    g_an3dMntY(2) = g_anMntDev[i_nX + i_nStep][i_nY + i_nStep];
    g_an3dMntY(3) = g_anMntDev[i_nX          ][i_nY + i_nStep];
  }
  else
  {
    g_an3dMntY(0) = g_anMntAlt[i_nX          ][i_nY          ];
    g_an3dMntY(1) = g_anMntAlt[i_nX + i_nStep][i_nY          ];
    g_an3dMntY(2) = g_anMntAlt[i_nX + i_nStep][i_nY + i_nStep];
    g_an3dMntY(3) = g_anMntAlt[i_nX          ][i_nY + i_nStep];
  }
}

void GetMnt3D(int i_nX, int i_nY, int i_nStep, int i_nMntType)
{
  // OK : 06/23/06/00

  if(i_nMntType == 0)
  {
    g_an3dMntY(0) = (g_nScreenH / 2) + (g_anMntDev[i_nX          ][i_nY          ] / ratio) * (g_anMntDev[i_nX          ][i_nY          ] / ratio > 0) / 2;
    g_an3dMntY(1) = (g_nScreenH / 2) + (g_anMntDev[i_nX + i_nStep][i_nY          ] / ratio) * (g_anMntDev[i_nX + i_nStep][i_nY          ] / ratio > 0) / 2;
    g_an3dMntY(2) = (g_nScreenH / 2) + (g_anMntDev[i_nX + i_nStep][i_nY + i_nStep] / ratio) * (g_anMntDev[i_nX + i_nStep][i_nY + i_nStep] / ratio > 0) / 2;
    g_an3dMntY(3) = (g_nScreenH / 2) + (g_anMntDev[i_nX          ][i_nY + i_nStep] / ratio) * (g_anMntDev[i_nX          ][i_nY + i_nStep] / ratio > 0) / 2;
  }
  else
  {
    g_an3dMntY(0) = (g_nScreenH / 2) + (g_anMntAlt[i_nX          ][i_nY          ] / ratio) * (g_anMntAlt[i_nX          ][i_nY          ] / ratio > 0) / 2;
    g_an3dMntY(1) = (g_nScreenH / 2) + (g_anMntAlt[i_nX + i_nStep][i_nY          ] / ratio) * (g_anMntAlt[i_nX + i_nStep][i_nY          ] / ratio > 0) / 2;
    g_an3dMntY(2) = (g_nScreenH / 2) + (g_anMntAlt[i_nX + i_nStep][i_nY + i_nStep] / ratio) * (g_anMntAlt[i_nX + i_nStep][i_nY + i_nStep] / ratio > 0) / 2;
    g_an3dMntY(3) = (g_nScreenH / 2) + (g_anMntAlt[i_nX          ][i_nY + i_nStep] / ratio) * (g_anMntAlt[i_nX          ][i_nY + i_nStep] / ratio > 0) / 2;
  }
}

int GetColorAlt(int i_nAlt)
{
  // OK : 06/28/12/01

  int l_nColor;
  
  if(i_nAlt <= 0)
  {
    l_nColor = 1;
  }
  else
  {
    l_nColor = (i_nAlt / 28) + 2; // 28 = (4000 / 10) / (16 - 2)

    if(l_nColor > 15)
    {
      l_nColor = 15;
    }
  }

  return l_nColor
}

void InitColorAlt(void)
{
  // OK : 06/28/12/01

  VSETCOLOR 0,  0;
  VSETCOLOR 1,  &HFA0;
  VSETCOLOR 2,  &H0A2;
  VSETCOLOR 3,  &H0A4;
  VSETCOLOR 4,  &H0A6;
  VSETCOLOR 5,  &H0A8;
  VSETCOLOR 6,  &H088;
  VSETCOLOR 7,  &H068;
  VSETCOLOR 8,  &H048;
  VSETCOLOR 9,  &H028;
  VSETCOLOR 10, &H448;
  VSETCOLOR 11, &H668;
  VSETCOLOR 12, &H888;
  VSETCOLOR 13, &HAAA;
  VSETCOLOR 14, &HDDD;
  VSETCOLOR 15, &HFFF;
}

int GetColorLum(int i_nAltNW, int i_nAltNE, int i_nAltSE, int i_nAltSW, int n)
{
  // OK : 06/28/12/01
  
  int l_nAltMoy = (i_nAltNW + i_nAltNE + i_nAltSE + i_nAltSW) / 4;
  int l_nAltHyp = n * SQR(2);
  int l_nColor;

  if(i_nAltNW <= 0)
  {
    l_nColor = 1;
  }
  else
  {
    l_nColor = 8 + (((i_nAltNW - l_nAltMoy) + (i_nAltSW - l_nAltMoy) + (l_nAltMoy - i_nAltSE) + (l_nAltMoy - i_nAltNE)) / 4) / l_nAltHyp;

    if(l_nColor < 2)
    {
      l_nColor = 2;
    }
    
    if(l_nColor > 15)
    {
      l_nColor = 15;
    }
  }

  return l_nColor;
}

void InitColorLum(void)
{
  // OK : 06/28/12/01

  VSETCOLOR 0,  0;
  VSETCOLOR 1,  &H750;
  VSETCOLOR 2,  &H111;
  VSETCOLOR 3,  &H222;
  VSETCOLOR 4,  &H444;
  VSETCOLOR 5,  &H555;
  VSETCOLOR 6,  &H666;
  VSETCOLOR 7,  &H777;
  VSETCOLOR 8,  &H888;
  VSETCOLOR 9,  &H999;
  VSETCOLOR 10, &HAAA;
  VSETCOLOR 11, &HBBB;
  VSETCOLOR 12, &HCCC;
  VSETCOLOR 13, &HDDD;
  VSETCOLOR 14, &HEEE;
  VSETCOLOR 15, &HFFF;
}

void SetColorPal(void)
{
  // OK : 06/28/12/01

  // Color to Display
  if(g_nParamMntColor = 0)
  {
    InitColorAlt();
  }
  else
  {
    InitColorLum();
  }
}

int InvertParam(int i_nVar)
{
  // OK : 06/28/12/01

  if(i_nVar == 0)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

void OnError(void)
{
  // OK : 01/13/02/00

  // In case of an Error
  cls();
  
  printf("Erreur...\r\n")
  
  waitkey();
  
  exit();
}

void waitkey(void)
{
  getchar();
}
