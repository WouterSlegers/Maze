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
	For (i,3) mEye[i] = 0.;
	mLookAt[0] = mLookAt[2] = 0.; mLookAt[1] = -1.;
    //----------------------------
    mW_Last = mW = 1280, mH_Last = mH = 800;
	mWinPosX = 890, mWinPosY = 390;
    mMouseX = mMouseY = 0;
    mMouseMotionX = mMouseMotionY = 0;
	mNearPlane = 1.;
    mFarPlane = 50.;
	mDispInfo = false;
    //-------------------------------------------
	mGameRandSeedOffset = 0;
    //-------------------------------------------
	mIsMachine[BLACK] = true;
	mIsMachine[WHITE] = false;
	mPause = false;
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
void GUI::Init(int argc, char **argv){

    //---------------------------------------
	if (GAME == CHESS) mLogic->InitChess();
	else mLogic->ST_Init();//STRATEGO
    Cam_Init();
	Game_Reset();
    //---------------------------------------
	
    //---------------------------------------
    glClearColor(.8f, .8f, 1., 1.);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glEnable(GL_LIGHTING);
    glColorMaterial (GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    
    glShadeModel(GL_SMOOTH);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    
	GLfloat posChess[]    = {1.0, 1.0, 1.0, .0};
    GLfloat posStratego[] = {5.0, 1.0, 5.0, .0};
    GLfloat ambient[]     = {0.3, 0.3, 0.3, 1.0};
    GLfloat diffuse[]     = {0.8, 0.8, 0.8, 1.0};
    GLfloat front_mat_shininess[] = {60.0};
    GLfloat front_mat_specular[]  = {0.2, 0.2, 0.2, 1.0};
    GLfloat front_mat_diffuse[]   = {0.5, 0.5, 0.28, 1.0};
    GLfloat back_mat_shininess[]  = {60.0};
    GLfloat back_mat_specular[]   = {0.5, 0.5, 0.2, 1.0};
    GLfloat back_mat_diffuse[]    = {1.0, 0.9, 0.2, 1.0};
    GLfloat lmodel_ambient[]      = {1.0, 1.0, 1.0, 1.0};
    
    if (GAME == CHESS) glLightfv(GL_LIGHT0, GL_POSITION, posChess);
    else glLightfv(GL_LIGHT0, GL_POSITION, posStratego);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, front_mat_shininess);
    glMaterialfv(GL_FRONT, GL_SPECULAR, front_mat_specular);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, front_mat_diffuse);
    glMaterialfv(GL_BACK, GL_SHININESS, back_mat_shininess);
    glMaterialfv(GL_BACK, GL_SPECULAR, back_mat_specular);
    glMaterialfv(GL_BACK, GL_DIFFUSE, back_mat_diffuse);
    
    const GLfloat lmodel_twoside[] = {GL_TRUE};
    glLightModelfv(GL_LIGHT_MODEL_TWO_SIDE, lmodel_twoside);
    
    //---------------------------------------

    glEnable(GL_LIGHT0);
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    //-----------------------------------------------------MultiSample
    glutInitDisplayString( "rgba double samples>=4 ");
	glEnable(GLUT_MULTISAMPLE);
    //-----------------------------------------------------
	//A hack fix for Mac for GLUT bug in use of
	//MouseAux_GetOpenGL3DPos() for start in non-Fullscreen mode
	if (!mFullScreen) Draw_Text2D(0,0,"*************");
    //-----------------------------------------------------
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------

















//---------------------------------------------------------------------
//---------------------------------------------------------------------
//                             Basics
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void GUI::Idle(){
    glutPostRedisplay();
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void GUI::Display(){
	
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
	Cam();
	
	bool isWhite = !bool(mPly % 2);
	if (!mPause && mIsMachine[isWhite]){
		if (mEvalAtCurrentFrame){
			Game_Eval_Move();
			mEvalAtCurrentFrame = false;
		}
		else mEvalAtCurrentFrame = true;
	}
	
    mGFX->DrawScene();
	
	if (mDispInfo) DispInfo();
		
	if (GAME == STRATEGO){
		FindChange(mPly-1);
		FindChange(mPly);
	}
	
	glutSwapBuffers();
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void GUI::ChangeSize(int w, int h){
	mW = w, mH = h;
    glViewport(0, 0, w, h);
    SetPerspective(mNearPlane, mFarPlane);
	Cam_SwitchTo3DMode();
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
inline void GUI::SetPerspective(double near, double far){
	double ratio = double(mW)/double(mH);
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	gluPerspective(45., ratio, near, far);
	glMatrixMode(GL_MODELVIEW);
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void GUI::SetWindowTitle(){
	if (mIsMachine[WHITE]){
		if (mIsMachine[BLACK])
			glutSetWindowTitle("Computer - Computer");
		else glutSetWindowTitle("Computer - Human");
	}
	else {
		if (mIsMachine[BLACK]) glutSetWindowTitle("Human - Computer");
		else glutSetWindowTitle("Human - Human");
	}
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void GUI::DispInfo(){//STRATEGO
	//-------------------------
	if (GAME == CHESS) return;
	//-------------------------
	glColor3ub(0,0,0);
	//-------------------------
	Cam_SwitchTo2DMode();
	glPushMatrix();
	glLoadIdentity();
	glBegin(GL_QUADS);
    glVertex2d( 15,mH-15 );
    glVertex2d( 15,mH-233);
    glVertex2d(169,mH-233);
    glVertex2d(169,mH-15 );
    glEnd();
	glPopMatrix();
	Cam_SwitchTo3DMode();
    //-------------------------
	const char LABELS[][20] = {
		"FLAG",
		"SPY",
		"SCOUT", 
		"MINER",
		"SERGEANT",
		"LIEUTENANT",
		"CAPTAIN",
		"MAJOR",
		"COLONEL",
		"GENERAL",
		"MARSHAL",
		"TRAP",
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
		glColor3ubv(mGFX->mColorPalette[i]);
		Draw_Text2D(27,mH-65-i*14, "%s", LABELS[i]);
		glColor3ub(240,0,0);
		Draw_Text2D(27+94,mH-65-i*14, "%d", pieceFreq[i][WHITE]);
		glColor3ub(50,100,255);
		Draw_Text2D(27+94+24,mH-65-i*14, "%d", pieceFreq[i][BLACK]);
	}
    //-------------------------
	glColor3ub(255,255,255);
	Draw_Text2D(27+12, mH-40, "SCORE: %5.1lf",
				mLogic->GoalFunc(false, RAND_OFF, mPly) + .00001f);
    //-------------------------
//	Draw_Text2D(mW-180, mH-6*14, "Mouse X:        %3d", mMouseX);
//	Draw_Text2D(mW-180, mH-5*14, "Mouse Y:        %3d", mMouseY);
//	Draw_Text2D(mW-180, mH-4*14, "Mouse Motion X: %3d", mMouseMotionX);
//	Draw_Text2D(mW-180, mH-3*14, "Mouse Motion Y: %3d", mMouseMotionY);
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------

















//---------------------------------------------------------------------
//---------------------------------------------------------------------
//                             Keys
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void GUI::Keyboard_NormalKeys(unsigned char key, int x, int y){
	
    switch (key) {
        case ESCAPE : exit(0);
		case '-' : Game_StepBack(); break;
		case ' ' : if (mPause) Game_Eval_Move(); break;
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
        case 'f' : mFullScreen = !mFullScreen;
            if (mFullScreen){
				glutFullScreen();
				mW_Last = mW;
				mH_Last = mH;
			}
            else {
				glutPositionWindow(300,300);
				glutReshapeWindow(mW_Last, mH_Last);}
            break;
		case 'h' :
			mGFX->mMarkSqs = !mGFX->mMarkSqs;
			break;
		case 'i' : mDispInfo = !mDispInfo; break;//STRATEGO
		case 'p' : mPause = !mPause; break;
		case 'r' : mLogic->Print_Level(mPly); break;
    }
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void GUI::Keyboard_NormalKeysUp(unsigned char key, int x, int y){
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void GUI::Keyboard_SpecialKeys(int key, int x, int y){
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------

















//---------------------------------------------------------------------
//---------------------------------------------------------------------
//                              Mouse
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void GUI::Mouse(int button, int state, int x, int y){
	
	
	if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON){
		mCamControlModeRevolve = false;
		MouseAux_MakeMove(x,y);
	}
	
    if (state == GLUT_UP && button == GLUT_RIGHT_BUTTON){
		mV_CamRotation += x - mMouseX;
		mTheta_CamRotation += y - mMouseY;
		mCamControlModeRevolve = false;
    }
	
	if (button == GLUT_RIGHT_BUTTON){
		mMouseX = x; mMouseY = y;
		mMouseMotionX = x; mMouseMotionY = y;
		mCamControlModeRevolve = true;
	}
	
	mLogic->mPromotionFlags = glutGetModifiers();
	//glutGetModifiers():
	//GLUT_ACTIVE_SHIFT = 1
	//GLUT_ACTIVE_CTRL  = 2
	//GLUT_ACTIVE_ALT   = 4
	//e.g. CTRL and SHIFT (binary 011) => glutGetModifiers() == 3 
};
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void GUI::Mouse_Motion(int x, int y){
	if (!mCamControlModeRevolve) return;
	mMouseMotionX = x; mMouseMotionY = y;
};
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void GUI::Mouse_PassiveMotion(int x, int y){
};
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//                             Mouse Aux
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void GUI::MouseAux_GetOpenGL3DPos(int x, int y, char s[2]){
	
	GLint       vport[4];
	GLdouble    px, py, pz, mview[16], proj[16];
	GLfloat     winX, winY, winZ;
    
	glGetDoublev(GL_MODELVIEW_MATRIX, mview);
	glGetDoublev(GL_PROJECTION_MATRIX, proj);
	glGetIntegerv(GL_VIEWPORT, vport);
    
	winX = (GLfloat)x; int iwinY = winY = (GLfloat)(vport[3] - y);
	glReadPixels(x, iwinY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
	gluUnProject(winX, winY, winZ, mview, proj, vport, &px, &py, &pz);
    
	if (winZ > .9999f){s[0] = s[1] = 0; return;}
	//----------------------------------------
	s[0] = char (floor(pz + .5 * double(FILES)));
	if (s[0] < 0) s[0] = 0; else if (s[0] > FILES_1) s[0] = FILES_1;
	s[1] = char (floor(px + .5 * double(RANKS)));
	if (s[1] < 0) s[1] = 0; else if (s[1] > RANKS_1) s[1] = RANKS_1;
	//----------------------------------------
};
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void GUI::MouseAux_MakeMove(int x, int y){
	
	//Basics
	if (mGFX->mGameOver || mPly >= MAX_PLY-1) return;
	mLogic->mRandSeed = (unsigned int)(mPly + mGameRandSeedOffset);
	int w = mLogic->mIsWhite = !bool(mPly%2);
	
	//Get suggested piece position from mouse input
	char selSq[2]; MouseAux_GetOpenGL3DPos(x,y,selSq);
	
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
			
			//GFX: Graphics update of game position (= board)
			Game_CopyBoardToGFX(mPly+1);
			
			//-----------------------------------------Logic
			
			//Logic: If opponent cannot move in next PLY => Game Over
			if (GAME == CHESS){
				if (mLogic->FindChildren(mPly+1) == 0){
					if (mLogic->mMove[mPly+1].isInCheck)
						glutSetWindowTitle("Check Mate!");
					else glutSetWindowTitle("Stalemate!");
					mGFX->mGameOver = true;
					mPly++;
					return;
				}
			}
			else {
				if (mLogic->ST_IsFlagCaptured(bool(1-w), mPly+1)){
					glutSetWindowTitle("Flag Captured!");
					mGFX->mGameOver = true;
				}
				else if (mLogic->FindChildren(mPly+1) == 0){
					glutSetWindowTitle("Stalemate!");
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
	
	//GFX: Graphics update of game position (= board)
	Game_CopyBoardToGFX(mPly+1);
	
	//-----------------------------------------Logic
	
	//Logic: If opponent cannot move in next PLY => Game Over
	if (GAME == CHESS){
		if (mLogic->FindChildren(mPly+1) == 0){
			if (mLogic->mMove[mPly+1].isInCheck)
				glutSetWindowTitle("Check Mate!");
			else glutSetWindowTitle("Stalemate!");
			mGFX->mGameOver = true;
			mPly++;
			return;
		}
	}
	else {//STRATEGO
		if (mLogic->ST_IsFlagCaptured(bool(1-w), mPly+1)){
			glutSetWindowTitle("Flag Captured!");
			mGFX->mGameOver = true;
		}
		else if (mLogic->FindChildren(mPly+1) == 0){
			glutSetWindowTitle("Stalemate!");
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

















//---------------------------------------------------------------------
//---------------------------------------------------------------------
//                              Cam
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void GUI::Cam_Init(){
    mCamControlModeRevolve = true;
    mTheta_CamRotation     = 280.;
    mV_CamRotation         = 2700.;
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void GUI::Cam(){
	const double PI = 3.14159265;
    const double C = -0.1*PI/180., D = - PI, E = -1./200.;
	const int MAXSIZE = FILES > RANKS ? FILES_1 : RANKS_1;
    const double R1 = 4. + 1.25 * double(MAXSIZE);
	double R[3], N[3], dx, dy, v, theta;
	
	dx = double(mMouseMotionX - mMouseX);
	dy = double(mMouseMotionY - mMouseY);
    v = C * (mV_CamRotation + dx);
    theta = D / (1.0 + exp(E * (mTheta_CamRotation + dy)));
	double s = sin(theta);
    N[0] =   s * sin(v);//X <=>  y
    N[1] = - cos(theta);//Y <=> -z
    N[2] =   s * cos(v);//Z <=>  x
    For (i,3) R[i] = R1 * N[i];
    
    For (i,3) mEye[i] = mLookAt[i] + R[i];
    
    glLoadIdentity();
    gluLookAt(mEye[0], mEye[1], mEye[2],
			  mLookAt[0], mLookAt[1], mLookAt[2], 0., 1., 0.);
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------

















//---------------------------------------------------------------------
//---------------------------------------------------------------------
//                              Aux Draw
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void GUI::Cam_SwitchTo2DMode(){//Switch to a 2D orthographic projection
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, (GLfloat)mW, 0, (GLfloat)mH);
	glMatrixMode(GL_MODELVIEW);
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void GUI::Cam_SwitchTo3DMode(){//Switch to a 3D perspectiv projection
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void GUI::Draw_Text2D(int x, int y, const char *fmt,...){
    //--------------------
	Cam_SwitchTo2DMode();
    //--------------------
	char text[256]; va_list	ap;
	if (fmt == NULL) return;
	//vsprintf is sometimes considered as unsafe since nothing limits
	//the length of the output string... If the stack is small it may
	//be overloaded. No worries though here...
	va_start(ap, fmt); vsprintf(text, fmt, ap); va_end(ap);
    glPushMatrix(); glLoadIdentity();
    For (i,2){
        glRasterPos2d(x+i,y);
        for (char *c=text; *c != '\0'; c++)
            glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);
    }
	glPopMatrix();
    //--------------------
	Cam_SwitchTo3DMode();
    //--------------------
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------







