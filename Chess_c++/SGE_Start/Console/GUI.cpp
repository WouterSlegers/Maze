//---------------------------------------------------------------------
//---------------------------------------------------------------------
//This file may be regarded as a black box where no change is needed
//---------------------------------------------------------------------
//---------------------------------------------------------------------
#include "GUI.h"
//---------------------------------------------------------------------
//---------------------------------------------------------------------
GUI::GUI(){
    //----------------------------
	mGFX   = new GFX();
	mLogic = new Logic();
    //----------------------------
	mDispInfo = false;
    //-------------------------------------------
	mGameRandSeedOffset = 0;
    //-------------------------------------------
	mIsMachine[BLACK] = true;
	mIsMachine[WHITE] = false;
    //-------------------------------------------
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
GUI::~GUI(){delete mGFX, delete mLogic;}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//                             Init
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void GUI::Init(){

    //---------------------------------------
	if (GAME == CHESS) mLogic->InitChess();
	else mLogic->ST_Init();//STRATEGO
	Game_Reset();
    //---------------------------------------
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//                             Game Loop
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void GUI::GameLoop(){
	char s[256];
	Display();
	printf("To input a command, start with '#', e.g. \"#4\""
		   " for the command '4'\n");
	if (GAME == STRATEGO){
		printf("In Stratego, please replace 10 with ':', "
			   "e.g. \"a10\" with \"a:\"\n");
	}
	
	while (true){
		int w = !bool(mPly % 2);
		if (mIsMachine[w] && !mGFX->mGameOver){Display(); continue;}
		if (w) printf("White's "); else printf("Black's ");
		
		printf("Move at Ply %d\nInput: ", mPly);
		scanf("%s", s);
		
		if (s[0] == '#'){
			//Excecute Command
			Keys(s[1]);
		}
		else {
			//Excecute Move
			char sq[2];
			sq[0] = s[0];
			sq[1] = s[1];
			MakeMove_Manual(sq);
		}
		Display();
	}
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------

















//---------------------------------------------------------------------
//---------------------------------------------------------------------
//                             Basics
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void GUI::Display(){
	
	bool isWhite = !bool(mPly % 2);
	if (mIsMachine[isWhite]){
		if (mEvalAtCurrentFrame){
			Game_Eval_Move();
			mEvalAtCurrentFrame = false;
		}
		else mEvalAtCurrentFrame = true;
	}
	
    mGFX->PrintBoard();
	
	if (mDispInfo) DispInfo();
		
	if (GAME == STRATEGO){
		FindChange(mPly-1);
		FindChange(mPly);
	}
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void GUI::SetWindowTitle(){
	if (mIsMachine[WHITE]){
		if (mIsMachine[BLACK])
			printf("\nComputer - Computer\n\n");
		else printf("\nComputer - Human\n\n");
	}
	else {
		if (mIsMachine[BLACK]) printf("\nHuman - Computer\n\n");
		else printf("\nHuman - Human\n\n");
	}
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void GUI::DispInfo(){//STRATEGO
	//-------------------------
	if (GAME == CHESS) return;
    //-------------------------
	printf("  SCORE: %3.1f\n                 R  B\n",
		   mLogic->GoalFunc(false, RAND_OFF, mPly) + .00001f);
    //-------------------------
	const char LABELS[][32] = {
		"A  FLAG      ",
		"B  SPY       ",
		"C  SCOUT     ", 
		"D  MINER     ",
		"E  SERGEANT  ",
		"F  LIEUTENANT",
		"G  CAPTAIN   ",
		"H  MAJOR     ",
		"I  COLONEL   ",
		"J  GENERAL   ",
		"K  MARSHAL   ",
		"L  TRAP      ",
	};
    //-------------------------CountPieces
	char pieceFreq[12][2];
	For (i,12) For (j,2) pieceFreq[i][j] = 0;
	For (x,FILES) For (y,RANKS){
		char p = mLogic->mGamePos[mPly].sq[x][y];
		if (p >= 'a' && p <= 'z') pieceFreq[p-'a'][BLACK]++;
		else if (p >= 'A' && p <= 'Z') pieceFreq[p-'A'][WHITE]++;
	}
    //-------------------------
	For (i,12){
		printf("  %s  ", LABELS[i]);
		printf("%d  ", pieceFreq[i][WHITE]);
		printf("%d\n", pieceFreq[i][BLACK]);
	}
	printf("\n");
    //-------------------------
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------

















//---------------------------------------------------------------------
//---------------------------------------------------------------------
//                             Keys
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void GUI::Keys(char key){
	
    switch (key) {
		case '-' : Game_StepBack(); break;
		case '&' :
			if (GAME == STRATEGO){
				if (mPly > 0) break;
				mLogic->ST_NewPreSet_GamePos_Start(true);
				Game_CopyBoardToGFX(mPly);
			}
			break;
		case '/' :
			if (GAME == STRATEGO){
				if (mPly > 0) break;
				mLogic->ST_NewPreSet_GamePos_Start(false);
				Game_CopyBoardToGFX(mPly);
			}
			break;
		case '(' :
			if (GAME == STRATEGO){
				if (mPly > 0) break;
				mLogic->ST_NewRand_GamePos_Start(true);
				Game_CopyBoardToGFX(mPly);
			}
			break;
		case ')' :
			if (GAME == STRATEGO){
				if (mPly > 0) break;
				mLogic->ST_NewRand_GamePos_Start(false);
				Game_CopyBoardToGFX(mPly);
			}
			break;
		case '0' : Game_Reset(); break;
        case '1' :
			mIsMachine[WHITE] = false; mIsMachine[BLACK] = true;
			SetWindowTitle();
			//break;
		case '!' ://STRATEGO
			mGFX->mST_IsConcealed[WHITE] = false;
			mGFX->mST_IsConcealed[BLACK] = true;
			break;
        case '2' :
			mIsMachine[WHITE] = true; mIsMachine[BLACK] = false;
			SetWindowTitle();
			//break;
		case '"' ://STRATEGO
			mGFX->mST_IsConcealed[WHITE] = true;
			mGFX->mST_IsConcealed[BLACK] = false;
			break;
        case '3' :
			mIsMachine[WHITE] = mIsMachine[BLACK] = false;
			SetWindowTitle();
			//break;
		case '#' ://STRATEGO
			mGFX->mST_IsConcealed[WHITE] = false;
			mGFX->mST_IsConcealed[BLACK] = false;
			break;
        case '4' :
			mIsMachine[WHITE] = mIsMachine[BLACK] = true;
			SetWindowTitle();
			mGFX->mST_IsConcealed[WHITE] = false;//STRATEGO
			mGFX->mST_IsConcealed[BLACK] = false;
			break;
		case '5' ://STRATEGO
			mGFX->mST_IsConcealed[WHITE] = true;
			mGFX->mST_IsConcealed[BLACK] = true;
			break;
		case 'i' : mDispInfo = !mDispInfo; break;//STRATEGO
		case 'r' : mLogic->Print_Level(mPly); break;
    }
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void GUI::MakeMove_Manual(char sq[2]){
	
	//Basics
	if (mGFX->mGameOver || mPly >= MAX_PLY-1) return;
	mLogic->mRandSeed = (unsigned int)(mPly + mGameRandSeedOffset);
	int w = mLogic->mIsWhite = !bool(mPly%2);
	
	//Get suggested piece position from mouse input
	char selSq[2];
	
	//Console
	selSq[0] = sq[0] - 'a';
	selSq[1] = sq[1] - '1';
	
	//Is square occupied by this player
	bool selStartSq = mLogic->SqBelongsToPlayer(selSq[0], selSq[1],
												mLogic->mIsWhite,
												mPly);
	
	//--------------------------------------------------------
	// If selStartSq =>
	// the selected square selSq[...] is a start square
	//--------------------------------------------------------
	
	if (selStartSq){
		
		//Logic: Manual swap piece positions at start in STRATEGO 
		if (mPly < 2 && GAME == STRATEGO &&
			mGFX->mSelectedMove[w][0] > -1){
			char s0 = mGFX->mSelectedMove[w][0];
			char s1 = mGFX->mSelectedMove[w][1];
			char s2 = selSq[0];
			char s3 = selSq[1];
			char piece  = mLogic->mGamePos[mPly].sq[s0][s1];
			mLogic->mGamePos[mPly].sq[s0][s1] =
			mLogic->mGamePos[mPly].sq[s2][s3];
			mLogic->mGamePos[mPly].sq[s2][s3] = piece;
			mGFX->mSelectedMove[w][0] = mGFX->mSelectedMove[w][1] = -1;
			Game_CopyBoardToGFX(mPly);
			return;
		}
		
		//GFX: Highlight start square (current move)
		For (i,2) mGFX->mSelectedMove[w][i] = selSq[i];
		
		//Console
		printf("Valid Start Square Found: %c%c\n",
			   selSq[0] + 'a', selSq[1] + '1');
		
		//GFX: Reset destination square (current move)
		mGFX->mSelectedMove[w][2] = -1;
	}
	
	//--------------------------------------------------------
	// Else => selected square is not a start square.
	// If we already have a start square, which in that case
	// is stored in mGFX->mSelectedMove[w][...] =>
	// selectes square selSq[...] is the destination square
	//--------------------------------------------------------
	
	else if (mGFX->mSelectedMove[w][0] > -1){//If we expect dest sq
		
		//Build complete move string
		char s[4];
		s[0] = mGFX->mSelectedMove[w][0];
		s[1] = mGFX->mSelectedMove[w][1];
		s[2] = selSq[0]; s[3] = selSq[1];
		
		//Is move OK? If TRUE: execute move
		if (mLogic->IsMoveOK_ManualPlay(s, mPly)){
			
			//Logic: Logical update of game position (= board)
			mLogic->UpdateGamePos(0, mPly+1);//Child Index 0
			mLogic->GoalFunc(false, RAND_OFF, mPly+1);
			
			//Logic: Is opponent´s king in check?
			mLogic->mIsWhite = 1-w;
			mLogic->mMove[mPly+1].isInCheck =
			mLogic->King_IsInCheck(mPly+1);
			
			//GFX: Mark square if opponent´s king is in Check
			if (mLogic->mMove[mPly+1].isInCheck){
				mGFX->mCheckSqPos[mLogic->mIsWhite][0] =
				mLogic->mKingPos[0];
				mGFX->mCheckSqPos[mLogic->mIsWhite][1] =
				mLogic->mKingPos[1];
			}
			else mGFX->mCheckSqPos[mLogic->mIsWhite][0] = -1;
			
			//GFX: Reset highlights for previous move
			mGFX->mSelectedMove[1-w][0] =
			mGFX->mSelectedMove[1-w][2] = -1;
			
			//GFX: Highlight destination square (current move)
			mGFX->mSelectedMove[w][2] = selSq[0];
			mGFX->mSelectedMove[w][3] = selSq[1];
			
			//Console
			printf("Performed Move: %c%c to %c%c\n",
				   mGFX->mSelectedMove[w][0] + 'a',
				   mGFX->mSelectedMove[w][1] + '1',
				   sq[0], sq[1]);
			
			//GFX: Graphics update of game position (= board)
			Game_CopyBoardToGFX(mPly+1);
			
			//-----------------------------------------Logic
			
			//Logic: If opponent cannot move in next PLY => Game Over
			if (GAME == CHESS){
				if (mLogic->FindChildren(mPly+1) == 0){
					if (mLogic->mMove[mPly+1].isInCheck)
						printf("\nCheck Mate!\n\n");
					else printf("\nStalemate!\n\n");
					mGFX->mGameOver = true;
					mPly++;
					return;
				}
			}
			else {
				if (mLogic->ST_IsFlagCaptured(bool(1-w), mPly+1)){
					printf("\nFlag Captured!\n\n");
					mGFX->mGameOver = true;
				}
				else if (mLogic->FindChildren(mPly+1) == 0){
					printf("\nStalemate!\n\n");
					mGFX->mGameOver = true;
				}
				if (mGFX->mGameOver == true){
					mGFX->mST_IsConcealed[WHITE] = false;
					mGFX->mST_IsConcealed[BLACK] = false;
					mPly++;
					return;
				}
			}
			
			mPly++;//Complete move
			
			//-----------------------------------------GFX
			
			//GFX: Unless check mate, unmark check for this king,
			//since king is not allowed to move into a check position
			mGFX->mCheckSqPos[w][0] = -1;
			
		}
		//--------------------------------
	}
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------

















//---------------------------------------------------------------------
//---------------------------------------------------------------------
//                               Game
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void GUI::Game_CopyBoardToGFX(int ply){
	For (x,FILES) For (y,RANKS){
		mGFX->mGamePosGFX[x][y][0] = mLogic->mGamePos[ply].sq[x][y];
		if (GAME == STRATEGO) mGFX->mGamePosGFX[x][y][1] =
			mLogic->mGamePosStatus[ply].sq[x][y];
	}
	return;
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void GUI::Game_Reset(){
	mGFX->mGameOver = false;
	Game_CopyBoardToGFX(0);
	mPly = 0;
	For (i,2) For (j,4) mGFX->mSelectedMove[i][j] = -1;
	mGFX->mCheckSqPos[0][0] =
	mGFX->mCheckSqPos[1][0] = -1;
	mEvalAtCurrentFrame = false;
	//-----------------------------------------
	SetWindowTitle();
	//-----------------------------------------
	mGameRandSeedOffset++;
	//-----------------------------------------STRATEGO
	if (GAME == CHESS) return;
	For (i,2) For (j,2) mGFX->mLastCaptured[i][j] = '*';
	mLogic->mST2_VirtualPlay  = false;
	mLogic->mMove[0].children = 0;
	//-----------------------------------------
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void GUI::Game_StepBack(){
	mGFX->mGameOver = false;
	if (mPly < 2) return;
	mPly -= 2;
	Game_CopyBoardToGFX(mPly);
	For (i,2){
		mGFX->mCheckSqPos[i][0]   =
		mGFX->mSelectedMove[i][0] = mGFX->mSelectedMove[i][2] = -1;
	}
	//-----------------------------------------
	SetWindowTitle();
	//-----------------------------------------
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void GUI::FindChange(int ply){
	//---------------------------------------------
	if (ply < 1) return;
	//---------------------------------------------
	//At this moment
	int w = !bool(ply%2);
	int selectedChild = mLogic->mMove[ply-1].selectedChild;
	char m[4];
	//Get move coords
	For (i,4) m[i] = mLogic->mMove[ply-1].child[selectedChild][i];
	//Pieces in above coords before move
	char p0 = mLogic->mGamePos[ply-1].sq[m[0]][m[1]];
	char p1 = mLogic->mGamePos[ply-1].sq[m[2]][m[3]];
	//--------------------------------------------------
	//Piece in above coords after move
	char pFinal = mLogic->mGamePos[ply].sq[m[2]][m[3]];
	//--------------------------------
	if (p1 == '*'){//No capture
		mGFX->mLastCaptured[1-w][0] = '*';
		mGFX->mLastCaptured[1-w][1] = '*';
	}
	else if (pFinal == '*'){//Both captured
		mGFX->mLastCaptured[1-w][0] = p0;
		mGFX->mLastCaptured[1-w][1] = p1;
	}
	else if (pFinal > 'Z'){//White captured
		mGFX->mLastCaptured[1-w][0] = '*';
		mGFX->mLastCaptured[1-w][1] = w ? p1 : p0;
	}
	else {//Black captured
		mGFX->mLastCaptured[1-w][0] = w ? p0 : p1;
		mGFX->mLastCaptured[1-w][1] = '*';
	}
	//---------------------------------------------
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void GUI::Game_Eval_Move(){
	
	//-----------------------------------------Logic

	//Logic: Basics
	if (mGFX->mGameOver || mPly >= MAX_PLY-1) return;
	mLogic->mRandSeed = (unsigned int)(mPly + mGameRandSeedOffset);
	int w = mLogic->mIsWhite = !bool(mPly%2);
	
	//Logic: Find best move
	mLogic->FindBestMove(w, mPly);
	
	//Logic: Update of game position (= board)
	mLogic->UpdateGamePos(mLogic->mBestChildIdx, mPly+1);
	mLogic->GoalFunc(false, RAND_OFF, mPly+1);
	
	//Logic: Is opponent´s king in check?
	mLogic->mIsWhite = 1-w;
	mLogic->mMove[mPly+1].isInCheck = mLogic->King_IsInCheck(mPly+1);
	
	//-----------------------------------------GFX
	
	//GFX: Mark square if opponent´s king is in Check
	if (mLogic->mMove[mPly+1].isInCheck)
		For (i,2) mGFX->mCheckSqPos[1-w][i] = mLogic->mKingPos[i];
	else mGFX->mCheckSqPos[1-w][0] = -1;
	
	//GFX: Reset highlights for previous move
	mGFX->mSelectedMove[1-w][0] = mGFX->mSelectedMove[1-w][2] = -1;
	
	//GFX: Highlight current move
	For (i,4) mGFX->mSelectedMove[w][i] =
		mLogic->mMove[mPly].child[mLogic->mBestChildIdx][i];
	
	//Console
	printf("Computer Move: %c%c%c%c\n",
		   mGFX->mSelectedMove[w][0] + 'a',
		   mGFX->mSelectedMove[w][1] + '1',
		   mGFX->mSelectedMove[w][2] + 'a',
		   mGFX->mSelectedMove[w][3] + '1');
	
	//GFX: Graphics update of game position (= board)
	Game_CopyBoardToGFX(mPly+1);
	
	//-----------------------------------------Logic
	
	//Logic: If opponent cannot move in next PLY => Game Over
	if (GAME == CHESS){
		if (mLogic->FindChildren(mPly+1) == 0){
			if (mLogic->mMove[mPly+1].isInCheck)
				printf("\nCheck Mate!\n\n");
			else printf("\nStalemate!\n\n");
			mGFX->mGameOver = true;
			mPly++;
			return;
		}
	}
	else {//STRATEGO
		if (mLogic->ST_IsFlagCaptured(bool(1-w), mPly+1)){
			printf("\nFlag Captured!\n\n");
			mGFX->mGameOver = true;
		}
		else if (mLogic->FindChildren(mPly+1) == 0){
			printf("\nStalemate!\n\n");
			mGFX->mGameOver = true;
		}
		if (mGFX->mGameOver == true){
			mGFX->mST_IsConcealed[WHITE] = false;
			mGFX->mST_IsConcealed[BLACK] = false;
			mPly++;
			return;
		}
	}
	
	mPly++;//Logic: Complete move
	
	//-----------------------------------------GFX
	
	//GFX: Unless check mate, unmark check for this king,
	//since king is not allowed to move into a check position
	mGFX->mCheckSqPos[w][0] = -1;

}
//---------------------------------------------------------------------
//---------------------------------------------------------------------







