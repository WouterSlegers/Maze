//---------------------------------------------------------------------
#pragma once
//---------------------------------------------------------------------
#ifdef _WIN32
#include <Math.h>
#include <stdio.h>
#include <stdarg.h>
#include "glut.h"
#else
#include <GLUT/glut.h>
#endif
//---------------------------------------------------------------------
#define For(i,N) for (int (i) = 0; (i) < (N); (i)++)
//---------------------------------------------------------------------
#define abs(x) x < 0 ? -x : x
//---------------------------------------------------------------------
class Common {
protected:
    //------------------------------
    enum {BLACK, WHITE};
    enum {RAND_OFF, RAND_ON};
    enum {DISP_OFF, DISP_ON};
    enum {CHESS, STRATEGO};
    enum {MINIMAX, ALPHABETA, OPTIM};
    enum {SEALED, MOBILE, KNOWN};//ST
    enum {PERF_INFO, IMPERF_INFO};//ST
    enum {ST2_PRINT_NONE, ST2_PRINT_CONCISE, ST2_PRINT_VERBOSE};//ST2
    //------------------------------Core
    static const int
    MAX_PLY     = 4096,
    CORE_ENGINE = OPTIM,
    GAME        = CHESS,
    ST_SYS      = IMPERF_INFO,//ST
    //------------------------------Tests
    TEST1 = false,
    TEST2 = false,
    TEST3 = false,//ST2
    //------------------------------ST2 Print
    ST2_PRINT = ST2_PRINT_NONE,
    ST2_SPECIAL_CASE = false,
    //------------------------------Board Size
    FILES = GAME == CHESS ? 8 : 10,
    RANKS = GAME == CHESS ? 8 : 10,
    //------------------------------
    FILES_1 = FILES - 1,
    RANKS_1 = RANKS - 1,
    DIAGS_1 = FILES < RANKS ? FILES_1 : RANKS_1;
    //------------------------------
};
//---------------------------------------------------------------------