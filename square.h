#include <stdio.h>
#include <time.h>
#include <memory.h>
#include <string.h>

#define MAXN 10
#define MAXNN 100

typedef struct squarelist {
    int     i[MAXNN];
    int     time;                                           // 结果时间
    int     resultCount, partResultCount, totalCount;       // 结果序号，已搜索排列数量
    struct squarelist  *next;
} SQUARENODE;

typedef struct square
{
    int i[MAXNN];                   // 1~NN
    int remain[MAXNN];              // 1~N
    int totalCount, resultCount, partResultCount;
    int t_start, time;
    int stdsum;
    int rsum[MAXN], csum[MAXN];           // 0~N-1
    SQUARENODE  head, *tail;
    int threshold_max[MAXN], threshold_min[MAXN];
} SQUARE;

int exam(SQUARE *ps);
void msquare(SQUARE *ps, int no);

int     exam(SQUARE *ps);
void    print(SQUARENODE *psn, int direct);
void    msquare( SQUARE *ps, int no);
void    fileWrite( char *filename, SQUARE *ps) ;
void    initSquare( SQUARE  *ps);

SQUARENODE *sl_append(SQUARE *ps); 
void    sl_free( SQUARENODE *head);
void    calcThreshold( SQUARE *ps );
