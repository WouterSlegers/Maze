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
    void PrintBoard();
	//-----------------------------------------------------------------
	char mSelectedMove[2][4];
	char mCheckSqPos[2][2];
	char mGamePosGFX[FILES][RANKS][2];
	bool mGameOver;
	char mLastCaptured[2][2];//[B or W's turn][B or W piece captured]
	bool mST_IsConcealed[2];
	//-----------------------------------------------------------------
};
//---------------------------------------------------------------------
