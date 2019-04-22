
#include "square.h"

int NN, N;
int interval;

int main(int argc, char *argv[])
{
    SQUARE s;

    printf("input a number(3-5) and interval:");
    scanf("%d,%d", &N, &interval);
    NN = N * N;

    initSquare(&s);
    msquare(&s, 0);

    print(s.tail, true);

    char filename[256];
    sprintf(filename, "%dx%d result.txt", N, N);
    fileWrite(filename, &s);
    sl_free( &(s.head) );

    getchar();
}

void msquare(SQUARE *ps, int no)
{
    int i;
    int r, c, rsum, csum, tempcsum, temprsum;

    r = no / N;
    c = no % N;

    rsum = ps->rsum[r];
    csum = ps->csum[c];

    for (i = 1; i <= NN; i++)
    {
        if (ps->remain[i] == 0)
            continue;

        tempcsum = csum + i;
        temprsum = rsum + i;

        if (temprsum > ps->stdsum || tempcsum > ps->stdsum)
            break;

        if (c == N - 1 && temprsum != ps->stdsum)
            continue;
        if (r == N - 1 && tempcsum != ps->stdsum)
            continue;

        if ( c>1 ) {
            if ( (temprsum+ ps->threshold_max[c]<ps->stdsum) || temprsum+ps->threshold_min[c]>ps->stdsum )
                continue;
        }
        if ( r>1 ) {
            if ( (tempcsum+ ps->threshold_max[r]<ps->stdsum) || tempcsum+ps->threshold_min[r]>ps->stdsum )
                continue;
        }
        ps->totalCount++;

        ps->i[no] = i;
        ps->remain[i] = 0;
        ps->rsum[r] = temprsum;
        ps->csum[c] = tempcsum;

        if (no == NN - 1)
        {
            ps->partResultCount++;
            if (exam(ps))
            {
                ps->resultCount++;
                ps->time = clock()- ps->t_start;
                sl_append( ps );
                print(ps->tail, false);
            }
        }
        else
        {
            msquare(ps, no + 1);
        }
        ps->remain[i] = 1;
        ps->i[no] = 0;
    }
    ps->rsum[r] = rsum;
    ps->csum[c] = csum;
}

void initSquare(SQUARE *ps)
{
    int i, temp;

    memset( ps->i, 0, sizeof(ps->i));
    ps->head.next = NULL;
    ps->tail = &(ps->head);

    ps->t_start = clock();
    ps->resultCount = ps->totalCount = ps->partResultCount = 0;

    ps->t_start = clock();
    temp = 0;
    for (i = 1; i <= NN; i++)
    {
        ps->i[i] = 0;
        ps->remain[i] = 1;
        temp += i;
    }
    ps->stdsum = temp / N;
    ps->time = 0;

    for (i = 0; i < N; i++)
    {
        ps->csum[i] = 0;
        ps->rsum[i] = 0;
    }

    calcThreshold(ps);
}

SQUARENODE *sl_append(SQUARE *ps)
{
    SQUARENODE *newnode;

    newnode = new SQUARENODE;

    memcpy(&(newnode->i), ps->i, sizeof(int)*NN+1);
    newnode->partResultCount = ps->partResultCount;
    newnode->time = ps->time;
    newnode->resultCount = ps->resultCount;
    newnode->totalCount = ps->totalCount;

    newnode->next = ps->tail->next;
    ps->tail->next = newnode;
    ps->tail = newnode;

    return newnode;
}

void sl_free(SQUARENODE *head)
{
    SQUARENODE *node;

    while (head->next != NULL)
    {
        node = head->next;
        head->next = node->next;
        delete node;
    }
}


void print(SQUARENODE *psn, int direct)
{
    int j;

4    if (psn->resultCount%interval==0 || (direct) ) {
        printf("%-6d[ ", psn->resultCount);
        for (j = 0; j < NN; j++)
            printf("%2d ", psn->i[j]);
        printf("] %ld %ld %ld\r\n", psn->time, psn->partResultCount, psn->totalCount );
    }
}

void fileWrite(char *filename, SQUARE *ps)
{
    FILE *fp;
    SQUARENODE *n;
    char line[4096], temp[1024];
    int i;

    fopen_s(&fp, filename, "w");
    n = ps->head.next;

    while (n)
    {
        sprintf(line, "%6d [ ", n->resultCount);
        for (i = 0; i < NN; i++)
        {
            sprintf(temp, "%2d ", n->i[i]);
            strcat_s(line, sizeof(line), temp);
        }
        strcat_s(line, sizeof(line), "] ");
        sprintf(temp, "%10d  %ld  %ld\n", n->time, n->partResultCount, n->totalCount );
        strcat_s(line, sizeof(line), temp);
        fwrite(line, strlen(line), 1, fp);

        n = n->next;
    }

    fclose(fp);
}

void    calcThreshold( SQUARE *ps )
{
    int     temp, i;

    //  N=4 时, max[3]=0, max[2]=16, max[1]=31, max[0]=45
    ps->threshold_max[N-1] = 0;
    temp = NN;
    for(i=1;i<N;i++) {
        ps->threshold_max[N-1-i] = temp+ps->threshold_max[N-1-i+1];
        temp--;
    }

    // N=4时， min[3]=0, min[2]=1, min[1]=3, min[0]=6
    ps->threshold_min[N-1] = 0;
    temp = 1;
    for(i=1;i<N;i++) {
        ps->threshold_min[N-1-i] = temp+ps->threshold_min[N-1-i+1];
        temp++;
    }
}

// 检查数组i各元素是否满足九宫格要求，如果满足返回非0。
int exam(SQUARE *ps)
{
    int j;
    int sum1, sum2;

    // 初始化各求和变量，准备求和检查
    sum1 = 0;
    sum2 = 0;

    // 按行/列、对角线求和
    for (j = 0; j < N; j++)
    {
        sum1 += ps->i[j * N + j];         //M[j][j];
        sum2 += ps->i[j * N + N - 1 - j]; //M[j][2 - j];
    }

    // 检查对角线是否相等
    if (sum1 != ps->stdsum || ps->stdsum != sum2)
        return 0;
    else
        return 1;
}
