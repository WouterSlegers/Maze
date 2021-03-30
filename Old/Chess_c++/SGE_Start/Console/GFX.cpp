//---------------------------------------------------------------------
//---------------------------------------------------------------------
//This file may be regarded as a black box where no change is needed
//---------------------------------------------------------------------
//---------------------------------------------------------------------
#include "GFX.h"
//---------------------------------------------------------------------
//---------------------------------------------------------------------
GFX::GFX(){
	mGameOver = false;
	mST_IsConcealed[BLACK] = true;
	mST_IsConcealed[WHITE] = false;
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//                            Print Board
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void GFX::PrintBoard(){
	bool includeIdx = true;
	printf("--------------------------\n");
	For (y,RANKS){
		printf(" ");
		if (includeIdx) printf("%2d",RANKS-y);
		For (x,FILES){
			char p = mGamePosGFX[x][RANKS_1-y][0];
			//--------------------------------------
			if (GAME == STRATEGO){
				char s = mGamePosGFX[x][RANKS_1-y][1];
				if (p >= 'a' && p <= 'z'){
					if (mST_IsConcealed[BLACK] && s != KNOWN)
						printf(" X");
					else printf(" %c", p);
				}
				else if (p >= 'A' && p <= 'Z'){
					if (mST_IsConcealed[WHITE] && s != KNOWN)
						printf(" X");
					else printf(" %c", p);
				}
				else printf(" %c", p);
			}
			//--------------------------------------
			else printf(" %c", p);
			//--------------------------------------
		}
		if (GAME == STRATEGO && y == 3)
			printf("  %c %c",
				   mLastCaptured[BLACK][BLACK],
				   mLastCaptured[BLACK][WHITE]);
		if (GAME == STRATEGO && y == 6)
			printf("  %c %c",
				   mLastCaptured[WHITE][BLACK],
				   mLastCaptured[WHITE][WHITE]);
		printf("\n");
	}
	if (includeIdx){
		printf("    ");
		For (x,FILES) printf("%c ",65+x);
	}
	if (includeIdx) printf("\n");
	printf("--------------------------\n");
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------