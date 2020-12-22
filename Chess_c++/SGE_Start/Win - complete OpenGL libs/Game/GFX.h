//---------------------------------------------------------------------
//This file may be regarded as a black box where no change is needed
//---------------------------------------------------------------------
#pragma once
//---------------------------------------------------------------------
#include "Common.h"
//---------------------------------------------------------------------
class GFX : protected Common {
public:
    //-----------------------------------------------------------------
    GFX();
    void DrawScene();
	//-----------------------------------------Chess
private:
    void DrawKing(char x, char y);
    void DrawQueen(char x, char y);
    void DrawRook(char x, char y);
    void DrawBishop(char x, char y);
    void DrawKnight(char x0, char y0, bool isWhite);
    void DrawPawn(char x, char y);
	void DrawChessPiece(char x, char y, char piece);
	//-----------------------------------------STRATEGO
	void DrawStrategoPiece(char x, char y, char piece);
	//-----------------------------------------
    void DrawSquare(char x, char y);
    void DrawBoard();
	//-----------------------------------------Math
    void ChessCoordsTo3DCoords(char x, char y);
    void Vec_CrossProd(double out[3],
					   const double a[3], const double b[3]);
    void Vec_Normalize(double a[3]);
	//-----------------------------------------Aux Draw
    void DrawRing(double y0, double y1, double r0, double r1);
    void DrawSphere(double x, double y, double z,
                    double R, int V, int W);
    void DrawCross(double x, double y, double z);
    void DrawBlock(double T[4][4], double L, double sx, double sy);
    void DrawPoly(double x[][3], int N);
    //-----------------------------------------------------------------
    double mPosition[3];
public:
	char   mSelectedMove[2][4];
	char   mCheckSqPos[2][2];
	char   mGamePosGFX[FILES][RANKS][2];
	bool   mMarkSqs;
	bool   mGameOver;
	char   mLastCaptured[2][2];//[B or W's turn][B or W piece captured]
	bool   mST_IsConcealed[2];
	unsigned char mColorPalette[12][3];
	//-----------------------------------------------------------------
};
//---------------------------------------------------------------------

