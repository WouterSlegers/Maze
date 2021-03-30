//---------------------------------------------------------------------
//---------------------------------------------------------------------
/*
 *  This file may be regarded as a black box where no change is needed.
 *
 *  Copyright (c) Mikael Fridenfalk. All rights reserved.
 * 
 *  Redistribution and use in source and binary forms, with or 
 *  without modification, is hereby granted without fee provided 
 *  that the following conditions are met:
 * 
 *    1.  Redistributions of source code must retain the above 
 *        copyright notice, this list of conditions and the 
 *        following disclaimer.
 *    2.  Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the 
 *        following disclaimer in the documentation and/or other 
 *        materials provided with the distribution.
 *    3.  Neither the name of Mikael Fridenfalk may be used to
 *        endorse or promote products derived from this software
 *        without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY MIKAEL FRIDENFALK 'AS IS' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTIBILITY AND FITNESS 
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL MIKAEL
 * FRIDENFALK BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 */
//---------------------------------------------------------------------
//---------------------------------------------------------------------
#include "GFX.h"
//---------------------------------------------------------------------
//---------------------------------------------------------------------
GFX::GFX(){
	mMarkSqs  = true;
	mGameOver = false;
    const unsigned char COLOR_PALETTE[][3] = {
         65, 105, 225,  //A     FLAG          0   ROYAL BLUE
        176, 196, 222,  //B     SPY           1   LIGHT STEEL BLUE
        255, 255, 255,  //C     SCOUT         2   WHITE
        135, 206, 250,  //D     MINER         3   LIGHT SKY BLUE
          0, 128, 128,  //E     SERGEANT      4   TEAL        
          0, 255, 127,  //F     LIEUTENANT    5   SPRING GREEN
        173, 255,  47,  //G     CAPTAIN       6   GREEN YELLOW
        255, 255,   0,  //H     MAJOR         7   YELLOW
        255, 165,   0,  //I     COLONEL       8   ORANGE
        255,  69,   0,  //J     GENERAL       9   ORANGE RED
        139,   0,   0,  //K     MARSHAL       10  DARK RED    
         25,  25, 122,  //L     TRAP          11  MIDNIGHT BLUE
    };
	//Enable real-time change of palette:
	For (i,12) For (j,3) mColorPalette[i][j] = COLOR_PALETTE[i][j];

	mST_IsConcealed[BLACK] = true;
	mST_IsConcealed[WHITE] = false;
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//                             DrawScene
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void GFX::DrawScene(){
    DrawBoard();
	const int HALF = RANKS/2;
	if (GAME == CHESS) For (x,FILES) For (y,RANKS)
		DrawChessPiece(x, y, mGamePosGFX[x][y][0]);
	//-----------------------------------//STRATEGO
	else {
		const int HALF = RANKS/2;
		For (x,FILES) For (y,RANKS)
			DrawStrategoPiece(x, y, mGamePosGFX[x][y][0]);
		DrawStrategoPiece(     -2, HALF-1,
						  mLastCaptured[BLACK][BLACK]);
		DrawStrategoPiece(     -2, HALF,
						  mLastCaptured[BLACK][WHITE]);
		DrawStrategoPiece(FILES+1, HALF-1,
						  mLastCaptured[WHITE][BLACK]);
		DrawStrategoPiece(FILES+1, HALF,
						  mLastCaptured[WHITE][WHITE]);
	}
	//-----------------------------------
	
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------










//---------------------------------------------------------------------
//---------------------------------------------------------------------
void GFX::DrawKing(char x, char y){
    ChessCoordsTo3DCoords(x,y);
    const double a = 1.1, b = 1.1;
    DrawRing( .02*a,  .10*a, .30*b, .30*b);
    DrawRing( .10*a,  .40*a, .30*b, .20*b);
    DrawRing( .40*a,  .80*a, .20*b, .14*b);
    DrawRing( .80*a,  .80*a, .14*b, .20*b);
    DrawRing( .80*a,  .86*a, .20*b, .14*b);
    DrawRing( .86*a, 1.00*a, .14*b, .14*b);
    DrawRing(1.00*a, 1.14*a, .14*b, .21*b);
    DrawRing(1.14*a, 1.20*a, .21*b, .21*b);
    DrawRing(1.20*a, 1.20*a, .21*b, .10*b);
    DrawRing(1.20*a, 1.25*a, .10*b, .10*b);
    DrawRing(1.25*a, 1.25*a, .10*b,    .0);
    DrawCross(mPosition[0], 1.45*a, mPosition[2]);
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void GFX::DrawQueen(char x, char y){
    ChessCoordsTo3DCoords(x,y);
    const double a = 1.1, b = 1.1;
    DrawRing( .02*a,  .10*a, .30*b, .30*b);
    DrawRing( .10*a,  .40*a, .30*b, .20*b);
    DrawRing( .40*a,  .80*a, .20*b, .14*b);
    DrawRing( .80*a,  .80*a, .14*b, .20*b);
    DrawRing( .80*a,  .86*a, .20*b, .14*b);
    DrawRing( .86*a, 1.00*a, .14*b, .14*b);
    DrawRing(1.00*a, 1.14*a, .14*b, .21*b);
    DrawRing(1.14*a, 1.20*a, .21*b, .21*b);
    DrawRing(1.20*a, 1.20*a, .21*b, .17*b);
    DrawRing(1.20*a, 1.15*a, .17*b, .17*b);
    DrawRing(1.15*a, 1.25*a, .17*b, .05*b);
    DrawSphere(mPosition[0], 1.28*a, mPosition[2], .06*a, 24, 24);
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void GFX::DrawRook(char x, char y){
    ChessCoordsTo3DCoords(x,y);
    DrawRing( .02,  .10, .30, .30);
    DrawRing( .10,  .40, .30, .20);
    DrawRing( .40,  .80, .20, .20);
    DrawRing( .80,  .90, .20, .25);
    DrawRing( .90, 1.00, .25, .25);
    DrawRing(1.00, 1.00, .25, .15);
    DrawRing(1.00,  .95, .15, .15);
    DrawRing( .95,  .95,  .0, .15);
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void GFX::DrawBishop(char x, char y){
    ChessCoordsTo3DCoords(x,y);
    DrawRing(.02,  .10, .30, .30);
    DrawRing(.10,  .40, .30, .15);
    DrawRing(.40,  .70, .15, .10);
    DrawRing(.70,  .70, .10, .15);
    DrawRing(.70,  .75, .15, .10);
    DrawRing(.75,  .80, .10, .15);
    DrawRing(.80,  .90, .15, .15);
    DrawRing(.90, 1.15, .15, .03);
	//    DrawSphere(mPosition[0], .95, mPosition[2], .14, 128, 24);
	//    DrawRing( 1.002, 1.3, .13, .0);
    DrawSphere(mPosition[0], 1.15, mPosition[2], .06, 24, 24);
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void GFX::DrawKnight(char x0, char y0, bool isWhite){
    ChessCoordsTo3DCoords(x0,y0);
    DrawRing(.02, .10, .30, .30);
    DrawRing(.10, .40, .30, .15);
    DrawRing(.40, .50, .15, .10);
    DrawRing(.50, .50, .10, .15);
    DrawRing(.50, .55, .15, .10);
    DrawRing(.55, .55, .10,  .0);
    //----------------------------
    double x = mPosition[0], z;
    //----------------------------
    const int POINTS = 7;
    double XY[POINTS][2] = {
        .1,  .55,   -0.1, .55,   -0.17, .9,   .1, .65,
        .0, 1.05,     .4, .75,     .35, .6
    };
    if (!isWhite) For (i, POINTS) XY[i][0] = -XY[i][0];
    //----------------------------Head Coord Indices
    //                    4
    //
    //
    //      5                   2
    //
    //                  3
    //        6
    //                  0      1
    //----------------------------Left
    double const c = .07;
    z = mPosition[2] - c;
    double X1[][3] = {
        x+XY[0][0], XY[0][1], z,
        x+XY[1][0], XY[1][1], z,
        x+XY[2][0], XY[2][1], z,
        x+XY[3][0], XY[3][1], z
    };
    double X2[][3] = {
        x+XY[2][0], XY[2][1], z,
        x+XY[4][0], XY[4][1], z,
        x+XY[3][0], XY[3][1], z
    };
    double X3[][3] = {
        x+XY[3][0], XY[3][1], z,
        x+XY[4][0], XY[4][1], z,
        x+XY[5][0], XY[5][1], z,
        x+XY[6][0], XY[6][1], z
    };
    DrawPoly(X1,4);
    DrawPoly(X2,3);
    DrawPoly(X3,4);
    //----------------------------Right
    z = mPosition[2] + c;
    For (j,4){//Mirror
        X1[j][2] = z;
        X3[j][2] = z;
        if (j == 3) break;
        X2[j][2] = z;
    }
    DrawPoly(X1,4);
    DrawPoly(X2,3);
    DrawPoly(X3,4);
    //----------------------------
    z = mPosition[2];
    //----------------------------
    const int A[POINTS+1] = {0, 1, 2, 4, 5, 6, 3, 0};
    double X4[4][3];
    For (i, POINTS){
        int a = A[i], b = A[i+1];
        X4[0][0] = x+XY[a][0], X4[0][1] = XY[a][1], X4[0][2] = z-c,
        X4[1][0] = x+XY[a][0], X4[1][1] = XY[a][1], X4[1][2] = z+c,
        X4[2][0] = x+XY[b][0], X4[2][1] = XY[b][1], X4[2][2] = z+c,
        X4[3][0] = x+XY[b][0], X4[3][1] = XY[b][1], X4[3][2] = z-c;
        DrawPoly(X4,4);
    }
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void GFX::DrawPawn(char x, char y){
    ChessCoordsTo3DCoords(x,y);
    double a = .52, b = .13;
    DrawRing(  .02,   .10, .25, .25);
    DrawRing(  .10,   .25, .25, .14);
    DrawRing(  .25,     a, .14, .10);
    DrawRing(    a,     a, .10,   b);
    DrawRing(    a, a+.03,   b,   b);
    DrawRing(a+.03, a+.03,   b,  .0);
    DrawSphere(mPosition[0], a+.16, mPosition[2], .15, 24, 24);
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void GFX::DrawChessPiece(char x, char y, char piece){
	if (piece <= 'Z') glColor3ub(200,200,200);
	else glColor3ub(80,80,80);
	//-----------------------------------------
	switch (piece){
			//-----------------------------------------
		case 'P': case 'p': DrawPawn(x,y);   break;
		case 'R': case 'r': DrawRook(x,y);   break;
		case 'N': DrawKnight(x,y,true);      break;	
		case 'n': DrawKnight(x,y,false);     break;
		case 'B': case 'b': DrawBishop(x,y); break;
		case 'K': case 'k': DrawKing(x,y);   break;
		case 'Q': case 'q': DrawQueen(x,y);  break;
			//-----------------------------------------
	}
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void GFX::DrawStrategoPiece(char x, char y, char piece){
	
	if (piece == '*') return;
	bool isWhite = piece < 'a';
	if (isWhite) piece -= 'A';
	else piece -= 'a';
	
	//-----------
    ChessCoordsTo3DCoords(x,y);
	
	bool offSide = x < 0 || x > FILES;
	//---------------------------------------
	//if (isWhite) glColor3ub(230,0,0); else glColor3ub(65, 105, 225);
	if (isWhite) glColor3ub(230,0,0); else glColor3ub(65, 120, 255);
	const double a = .22, b = .9, d = .27;
	if (offSide) DrawRing(.02, .02, .0, d);
	DrawRing(.02, a,  d, d);
	DrawRing(  a, a, .0, d);
	//---------------------------------------
	
	bool reveal = offSide;
	
	if (isWhite){
		if (!mST_IsConcealed[WHITE] || mGamePosGFX[x][y][1] == KNOWN)
			reveal = true;
	}
	else {//is black
		if (!mST_IsConcealed[BLACK] || mGamePosGFX[x][y][1] == KNOWN)
			reveal = true;
	}
	
	if (reveal) glColor3ubv(mColorPalette[piece]);

	double T[4][4];
	double c = mPosition[1] + a + b;
    T[0][0] = 0., T[0][1] =  0., T[0][2] = -1., T[0][3] = mPosition[0];
    T[1][0] = 1., T[1][1] =  0., T[1][2] =  0., T[1][3] = c;
    T[2][0] = 0., T[2][1] = -1., T[2][2] =  0., T[2][3] = mPosition[2];
    T[3][0] = 0., T[3][1] =  0., T[3][2] =  0., T[3][3] = 1.;	
    DrawBlock(T,b,.22,.015);
	//---------------------------------------
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------










//---------------------------------------------------------------------
//---------------------------------------------------------------------
void GFX::DrawSquare(char x, char y){
	const GLubyte W[] = {220,220,220};
	const GLubyte B[] = {30,15,0};
    bool isWhite = bool((x+y)%2);
    double a = .01, s = .5;
	ChessCoordsTo3DCoords(x,y);
	//----------------------------------
	if (mMarkSqs){
		if ((x == mSelectedMove[1][0]  &&
			 y == mSelectedMove[1][1]) ||
			(x == mSelectedMove[1][2]  &&
			 y == mSelectedMove[1][3]))
			glColor3ub(160,255,160);
		else if ((x == mSelectedMove[0][0]  &&
				  y == mSelectedMove[0][1]) ||
				 (x == mSelectedMove[0][2]  &&
				  y == mSelectedMove[0][3]))
			glColor3ub(160,160,255);
		else if ((x == mCheckSqPos[0][0]  &&
				  y == mCheckSqPos[0][1]) ||
				 (x == mCheckSqPos[1][0]  &&
				  y == mCheckSqPos[1][1]))
			glColor3ub(240,240,0);
		else if (isWhite){
			if (GAME == CHESS) glColor3ubv(W);
			else glColor3ub(240,240,240);
		}
		else {
			if (GAME == CHESS) glColor3ubv(B);
			else glColor3ub(200,200,200);
		}
	}
	else {
		if (isWhite){
			if (GAME == CHESS) glColor3ubv(W);
			else glColor3ub(240,240,240);
		}
		else {
			if (GAME == CHESS) glColor3ubv(B);
			else glColor3ub(200,200,200);
		}
	}
	//-----------------------------------STRATEGO
	if (GAME == STRATEGO){//Water
		if ((y == 4 || y == 5) &&
			(x == 2 || x == 3 || x == 6 || x == 7))
			glColor3ub(64,64,255);
	}
	//-----------------------------------
	glBegin(GL_QUADS);
    glNormal3d(0., 1., 0.);
    glVertex3d(mPosition[0] - s, a, mPosition[2] - s);
    glVertex3d(mPosition[0] - s, a, mPosition[2] + s);
    glVertex3d(mPosition[0] + s, a, mPosition[2] + s);
    glVertex3d(mPosition[0] + s, a, mPosition[2] - s);
    glEnd();
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void GFX::DrawBoard(){
    double a = 0.005,
	sx = .5 * double(FILES) + .7,
	sy = .5 * double(RANKS) + .7;
	if (mGameOver) glColor3ub(200,0,0); else glColor3ub(30,30,30);
    glBegin(GL_QUADS);
    glNormal3d(0., 1., 0.);
    glVertex3d(-sy, a, -sx);
    glVertex3d(-sy, a,  sx);
    glVertex3d( sy, a,  sx);
    glVertex3d( sy, a, -sx);
    glEnd();
	For(x,FILES) For(y,RANKS) DrawSquare(x,y);
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//	y (1-8) in chess and (1-10) in Stratego
//	x ^
//	|
//	|
//	*------> x (a-h) in chess and (a-j) in Stratego
//	y      z
//	 
//	y points out from the plane of the screen
//	x, y, and z are coordinates in the 3D graphical environment
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void GFX::ChessCoordsTo3DCoords(char x, char y){
    mPosition[0] = double (y) - .5 * double(RANKS_1);
    mPosition[1] = 0.;//Not used
    mPosition[2] = double (x) - .5 * double(FILES_1);
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//                              Math
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void GFX::Vec_CrossProd(double out[3],
						const double a[3], const double b[3]){
    out[0] = a[1] * b[2] - b[1] * a[2];
    out[1] = b[0] * a[2] - a[0] * b[2];
    out[2] = a[0] * b[1] - b[0] * a[1];
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void GFX::Vec_Normalize(double a[3]){
    double invNorm = 1./sqrt(a[0]*a[0]+a[1]*a[1]+a[2]*a[2]);
    if (fabs(invNorm) > 1.0e-20)
        For (i,3) a[i] *= invNorm;
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//                             Aux Draw
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void GFX::DrawRing(double y0, double y1, double r0, double r1){
    double x, z, x0, z0, x1, z1;
    double u[3], v0[3], v1[3], v2[3], normal[3];
    int M = 128+1;
	const double PI = 3.14159265;
    double omega = 2.*PI/double(M-1);
    glBegin(GL_QUAD_STRIP);
    For (i,M){
        x  = cos(omega*double(i));
        z  = sin(omega*double(i));
        x0 = r0*x; z0 = r0*z; x1 = r1*x; z1 = r1*z;
        u[0] = x1 - x0; u[1] = y1 - y0; u[2] = z1 - z0;
        Vec_Normalize(u);
        v0[0] = 0.; v0[1] = 1.; v0[2] = 0.;
        v1[0] = x;   v1[1] = 0.; v1[2] = z;
        Vec_CrossProd(v2,v0,v1);
        Vec_CrossProd(normal,v2,u);
        glNormal3dv(normal);
        glVertex3d(mPosition[0]+x0, y0, mPosition[2]+z0);
        glVertex3d(mPosition[0]+x1, y1, mPosition[2]+z1);
    }
    glEnd();    
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void GFX::DrawSphere(double x, double y, double z,
						  double R, int V, int W){
    if (V < 3 || W < 2) return;
    double interp, s, v, w, N[3];
	const double PI = 3.14159265;
    double deltaV = 2. * PI / double(V), deltaW = PI / double(W);
    glBegin(GL_QUAD_STRIP);
    For (j,W) For (i,V){
        v = double(i) * deltaV;
        For (k,2){
            w = double(j+1-k) * deltaW; s = sin(w);
            N[0] = s * sin(v);//X <=>  y
            N[1] =   - cos(w);//Y <=> -z
            N[2] = s * cos(v);//Z <=>  x
            interp = .5 * (1. + N[1]);
            glNormal3dv(N);
            glVertex3d(x+R*N[0], y+R*N[1], z+R*N[2]);
        }
    }
    glEnd();
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void GFX::DrawCross(double x, double y, double z){
    double T[4][4];
    T[0][0] = 0., T[0][1] =  0., T[0][2] = -1., T[0][3] =  x;
    T[1][0] = 1., T[1][1] =  0., T[1][2] =  0., T[1][3] =  y;
    T[2][0] = 0., T[2][1] = -1., T[2][2] =  0., T[2][3] =  z;
    T[3][0] = 0., T[3][1] =  0., T[3][2] =  0., T[3][3] = 1.;
    DrawBlock(T,.2,.03,.03);
    T[0][0] = 0., T[0][1] =  1., T[0][2] =  0., T[0][3] =  x;
    T[1][0] = 0., T[1][1] =  0., T[1][2] =  1., T[1][3] =  y-.1;
    T[2][0] = 1., T[2][1] =  0., T[2][2] =  0., T[2][3] =  z+.1;
    T[3][0] = 0., T[3][1] =  0., T[3][2] =  0., T[3][3] = 1.;
    DrawBlock(T,.2,.03,.03);
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void GFX::DrawBlock(double T[4][4], double L, double sx, double sy){
	
    double p[8][3];
    For (i,3){
        p[0][i] = T[i][3] - L*T[i][0] - sx*T[i][1] - sy*T[i][2];
        p[1][i] = T[i][3] - L*T[i][0] - sx*T[i][1] + sy*T[i][2];
        p[2][i] = T[i][3] - L*T[i][0] + sx*T[i][1] + sy*T[i][2];
        p[3][i] = T[i][3] - L*T[i][0] + sx*T[i][1] - sy*T[i][2];
        p[4][i] = T[i][3]             - sx*T[i][1] - sy*T[i][2];
        p[5][i] = T[i][3]             - sx*T[i][1] + sy*T[i][2];
        p[6][i] = T[i][3]             + sx*T[i][1] + sy*T[i][2];
        p[7][i] = T[i][3]             + sx*T[i][1] - sy*T[i][2];
    }
    double Normals[6][3];
    For (i,3){
        Normals[0][i] =  T[i][0]; Normals[1][i] = -T[i][0];
        Normals[2][i] =  T[i][1]; Normals[3][i] = -T[i][1];
        Normals[4][i] =  T[i][2]; Normals[5][i] = -T[i][2];
    }
    
    glBegin(GL_QUADS);
    //  2        1             Up
    //     6  5                ^
    //     7  4                |
    //  3        0    Left <----
    int M[6][4] =
	//Front     Back       Left       Right      Up         Down
	{7,6,5,4,   0,1,2,3,   3,2,6,7,   4,5,1,0,   5,6,2,1,   7,4,0,3};
    
	For (i,6){
        glNormal3d(Normals[i][0], Normals[i][1], Normals[i][2]);
        For (j,4)
		glVertex3d(p[M[i][j]][0], p[M[i][j]][1], p[M[i][j]][2]);
    }
    glEnd();
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void GFX::DrawPoly(double x[][3], int N){
    if (N < 3) return;
    double a[3], b[3], normal[3];
    For (i,3){
        a[i] = x[2][i] - x[1][i];
        b[i] = x[0][i] - x[1][i];
    }
    Vec_CrossProd(normal, a, b);
    Vec_Normalize(normal);
    glNormal3dv(normal);
    glBegin(GL_POLYGON);
    For (i,N) glVertex3dv(x[i]);
    glEnd();
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------

