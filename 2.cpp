#include <stdio.h>
#include <conio.h>
#include <time.h>
#define     MAXNN   100
#define     MAXN    10
int N;

typedef struct square
{
    int i[MAXNN];
    int remain[MAXNN];
    int totalCount, resultCount, partResultCount;
    int t_start;
} SQUARE;

int exam(SQUARE *ps);
void print(SQUARE s);
int Used( SQUARE *ps, int no, int i);
void    msquare( SQUARE *ps, int no);

int main(int argc, char *argv[])
{
    printf("请输入一个3~5的数");
    scanf("%d",&N);
    SQUARE s;

    s.t_start = clock();
    s.resultCount = s.totalCount = s.partResultCount = 0;

    int     i;

    for(i=1;i<=N*N;i++) {
        s.i[i] = 0;
        s.remain[i] = 1;
    }


    msquare(&s, 0);

    _getch();
}

void    msquare( SQUARE *ps, int no)
{
    int     i;


    for(i=1;i<=N*N;i++) {
        if ( ps->remain[i]==0 )
            continue;
        ps->i[no] = i;
        ps->remain[i] = 0;
        if (no==N*N-1) {
            ps->totalCount ++;
            if (exam(ps)) {
                ps->resultCount ++;
                print(*ps);
            }
        } else
        {
            msquare( ps, no + 1);
        }
        ps->remain[i] = 1;
        ps->i[no] = 0;
    }

}

void print(SQUARE s)
{
    int j;

    printf("%d\t[ ", s.resultCount);
    for (j = 0; j < N*N; j++)
        printf("%d ", s.i[j]);
    printf("]\t\t[%d] [%d] \ttime:[%ld]\r\n", s.totalCount, s.partResultCount, clock() - s.t_start);
}

int exam(SQUARE *ps)
{
    int j, k, cflag, n;
    int sum1, sum2, rsum[3], csum[3], stdsum;

    stdsum = (N+N*N*N)/2;

    for (j = 0; j < N; j++)
    {
        csum[j] = 0;
        rsum[j] = 0;
    }
    sum1 = 0;
    sum2 = 0;


    for (j = 0; j < N; j++)
    {
        for (k = 0; k < N; k++)
        {
            csum[k] += ps->i[j * N + k]; //M[j][k];
            rsum[j] += ps->i[j * N + k]; //M[j][k];
        }
        sum1 += ps->i[j * N + j];     //M[j][j];
        sum2 += ps->i[j * N + N - 1 - j]; //M[j][2 - j];
    }

    if (sum1 != stdsum || stdsum != sum2)
        return 0;

    (ps->partResultCount)++;
    cflag = 1;
    for (j = 0; j < N; j++)
    {
        if (csum[j] != stdsum || rsum[j] != stdsum)
        {
            cflag = 0;
            break;
        }
    }
    return cflag;
}
