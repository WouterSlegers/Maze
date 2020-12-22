//---------------------------------------------------------------------
#pragma once
//---------------------------------------------------------------------
#include "Common.h"
//---------------------------------------------------------------------
class Logic : protected Common {
private:
    //-------------------------------------
    struct GAME_POS {char sq[FILES][RANKS];};
    //-------------------------------------
    static const int MAX_CHILDREN_PER_PLY = 128;
    struct MOVE {
        char child[MAX_CHILDREN_PER_PLY][4];
        int  selectedChild;
        int  children;
        bool isInCheck;//The king of this ply is in check
    };
    //-------------------------------------STRATEGO
    struct GAME_POS_ST {
        float pieceValue[128];//Sparse
        float deltaExpVal;//Change of Goal Function by Expected Value
    };
    //-------------------------------------
    //-------------------------------------
    //-------------------------------------
    //-------------------------------------
    //-------------------------------------
public:
    Logic();
    ~Logic();
    //-------------------------------------Init
    void  InitChess();
    //-------------------------------------Computer Play
    float GoalFunc(bool incrN, bool randOn, int ply);
    float FindBestMove(bool isWhite, int ply);
private:
    float Minimax(bool isWhite, int ply);
    float AlphaBeta(float alpha, float beta, bool isWhite, int ply);
    float Rand();
    //-------------------------------------Human play
public:
    bool  SqBelongsToPlayer(char x, char y, bool isWhite, int ply);
    bool  IsMoveOK_ManualPlay(char move[4], int ply);
    //-------------------------------------Aux
    void  UpdateGamePos(char childIdx, int ply);
    //-------------------------------------Find Children
    int   FindChildren(int ply);
private:
    void  OptChildOrder(int ply);
    int   MoveStatus(int x, int y, int ply);
    void  SuggestAddChild(int x0, int y0, int x1, int y1, int ply);
    void  Pawn(int x, int y, int ply);
    void  Rook(int x, int y, int ply);
    void  Knight(int x, int y, int ply);
    void  Bishop(int x, int y, int ply);
    //-------------------------------------Repetition Alert
    bool  RepetitionAlert(int ply);
    bool  IsMoveUnequal(int depth0, int depth1);
    //-------------------------------------King
    void  King(int x, int y, int ply);
public:
    bool  King_IsInCheck(int ply);
    //-------------------------------------Print
    void  Print_Board(bool includeIdx, int ply);
    void  Print_Child(int childIdx, int ply);
    void  Print_Level(int ply);
    void  Print_LevelVerbose(int ply);
    void  Print_Bar();
    //-------------------------------------
    //-------------------------------------STRATEGO
    void  ST_Init();//STRATEGO
private:
    void  ST_BuildDst2SrcMtx();
    void  ST_EvalPieceStatusValues(int ply);
    float ST_GoalFunc(bool incrN, bool randOn, int ply);
    int   ST_FindChildren(int ply);
    bool  ST_PositionIsWater(int x, int y);
    void  ST_BasicMoves(int x, int y, int ply);
    void  ST_UpdateGamePos(char childIdx, int ply);
public:
    bool  ST_IsFlagCaptured(bool targetIsWhiteFlag, int ply);
    void  ST_NewRand_GamePos_Start(bool isWhite);
    void  ST_NewPreSet_GamePos_Start(bool isWhite);
private:
    void  ST_ShufflePiecePos(int fromY, int toY);
    //-------------------------------------
    //-------------------------------------STRATEGO 2
    void  ST2_BuildExpValMtx(bool disp, int ply);
    void  ST2_BasicStats(bool disp, int ply);
    void  ST2_BuildExpValSubMtx(char s0, char s1,
                                int playerIsStriker, int ply);
    //-------------------------------------
    int   ST2_FindChildren(int ply);
    void  ST2_UpdateGamePos(char childIdx, int ply);
    //-------------------------------------
    //-------------------------------------
    //-------------------------------------Test CHESS
    void  Test_King_IsInCheck();
    bool  TestGamePosBatch(char gp[][8][16], int N,
                           char s[], bool isInCheck);
    bool  TestGamePos(char gp[][8][16], int index,
                      char s[], bool isInCheck);
    char  Test_B_2_Q(char p);
    char  Test_R_2_Q(char p);
    void  Test_OptChildren();
    //-------------------------------------Test STRATEGO 2
    void  Test_ST2_BasicStats();
    //-------------------------------------
    //-------------------------------------
    //-------------------------------------
    //-------------------------------------
    //-------------------------------------
    //SPARSE arrays are fast access arrays. The chars used to
    //denote pieces are of the set {'*', 'a'...'z','A'...'Z'}.
    //'*' has ASCII value 42, 'Z' 90 and 'z' 122 => SPARSE = 123:
private:
    static const int SPARSE = 'z' + 1;
    //-------------------------------------
    float mMaxValue, mOffensiveFactor;
    float mPieceValue[SPARSE];
    float mFileGrad[FILES], mRankGrad[RANKS];
    int   mStartPly, mMaxPly, mDeltaPly;
    unsigned int mN;
public:
    GAME_POS * mGamePos;
    MOVE     * mMove;
    int   mBestChildIdx;//Idx: short for "index" or "number"
    bool  mIsWhite;
    char  mKingPos[2];
    int   mPromotionFlags;
private:
    //-------------------------------------Repetition
    char  mInvalidRepetitiveMove[4];
    bool  mRepetitionAlertOn;
public:
    //-------------------------------------Variation
    unsigned int mRandSeed;
    float mRandAmplitude;
    GAME_POS * mGamePosStatus;//STRATEGO
    //-------------------------------------STRATEGO
    GAME_POS_ST * mGamePosST;
private:
    char  mST_CaptureMtx[SPARSE][SPARSE];
    float mST_PieceStatusValue[SPARSE][3];
    int   mST_PreSet_GamePosIdx;
    //-------------------------------------STRATEGO 2
    float mST2_StatusToProb4Piece[SPARSE][2];
    int   mST2_N_SealedDynamic;
    //-------------------------------------
    float mST2_CaptureProbMtx[SPARSE][2][2];
    //-------------------------------------
public:
    bool  mST2_VirtualPlay;
private:
    float mST2_ExpValMtx[SPARSE][3][2][2];
    float mST2_ExpValSuccessMtx[SPARSE][3][2][2];
    float mST2_ExpValFailureMtx[SPARSE][3][2][2];
    //-------------------------------------
};
//---------------------------------------------------------------------
