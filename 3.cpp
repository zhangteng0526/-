#include <stdio.h>
#include <time.h>
#include <memory.h>
#include <string.h>
#include <thread>
#include <algorithm>

#define MAXN 10
#define MAXNN 100

#ifdef WIN32
#include <conio.h>
#endif

typedef struct squarelist {
    int     i[MAXNN];
    int     time;                                           // 结果时间
    int     resultCount, partResultCount, totalCount;       // 结果序号，，已搜索排列数量
    struct squarelist  *next;
} SQUARENODE;

typedef struct square
{
    int working;                    // 排列进程进行中标志；
    int scope_start, scope_end;     // 循环开始和结束变量，便于将N*N的搜索切分为多块
    int i[MAXNN];                   // 1~NN
    int remain[MAXNN];              // 1~N
    int totalCount, resultCount, partResultCount;
    int t_start, time;
    int stdsum;       //标准和 例如九宫格的时候为十五
    int rsum[MAXN], csum[MAXN];           // 0~N-1
    SQUARENODE  head, *tail;
    int threshold_max[MAXN], threshold_min[MAXN];
} SQUARE;

void    beginWorker(SQUARE *ps);
int exam(SQUARE *ps);
void msquare(SQUARE *ps, int no);

int     exam(SQUARE *ps);
void    msquare( SQUARE *ps, int no);
void    fileWrite( char *filename, SQUARE *ps, int count) ;
void    initSquare( SQUARE  *ps, int start, int end);

SQUARENODE *sl_append(SQUARE *ps); 
void    sl_free( SQUARENODE *head);
void    calcThreshold( SQUARE *ps );

int NN, N;

int main(int argc, char *argv[])
{
    int groupSize, totals, i;// groupSize是要分的多线程每一组的大小 totals是实际分的组数
    SQUARE s[MAXNN];
    std::thread t[MAXNN];
    int sumtotalCount, sumpartResultCount, sumresultCount;

    printf("input a number(3-5) and group size: ");
    scanf("%d,%d", &N, &groupSize);

    NN = N * N;
    if (N <= 0 || N > 9 || groupSize <= 0 || groupSize > NN)
    {
        printf("N: 3~9;  group size: 1~N*N");
        return -1;
    }//检查输入的分组大小是否合理

    totals = NN / groupSize;
    if (NN % groupSize)
        totals++;//如果总格子数除想分的组数大小不是整数，那么总分组数加一

    for (i = 0; i < totals; i++)
    {
        initSquare(&(s[i]), i * groupSize + 1, std::min(i * groupSize + groupSize, NN));
    }

    for (i = 0; i < totals; i++)
    {
        t[i] = std::thread(beginWorker, &(s[i]));
    }

    printf("数量，满足对角线的，满足所有条件的，时间\r\n");

    int finished = false;
    while (!finished)
    {
        sumresultCount = 0;
        sumpartResultCount = 0;
        sumtotalCount = 0;

        for (i = 0; i < totals; i++)
        {
            finished |= s[i].working;
            sumresultCount += s[i].resultCount;
            sumpartResultCount += s[i].partResultCount;
            sumtotalCount += s[i].totalCount;
        }
        finished = !finished;

        printf("%-16d %-12d  %-8d %.1f\r", sumtotalCount, sumpartResultCount, sumresultCount, (clock() - s[0].t_start) / 1000.0);

        _sleep(1000);

        if (kbhit())
        {
            break;
        }
    }
    printf("\r\n");

    char filename[256];
    sprintf(filename, "%dx%d result.txt", N, N);
    fileWrite(filename, s, totals );//将结果写入文件

    for(i=0;i<totals;i++)
        sl_free(&(s[i].head));

    getchar();
}

void beginWorker(SQUARE *ps)
{
    ps->working = true;
    ps->t_start = clock();

    int i;

    for (i = ps->scope_start; i <= ps->scope_end; i++)
    {
        ps->i[0] = i;
        ps->remain[i] = 0;//将已用过的数标记为零
        ps->rsum[0] = i;
        ps->csum[0] = i;
        msquare(ps, 1);
        ps->remain[i] = 1;//将未用过的数标记为1
    }

    ps->working = false;
}

void msquare(SQUARE *ps, int no)
{
    int i;
    int r, c, rsum, csum, tempcsum, temprsum;

    r = no / N;
    c = no % N;

    rsum = ps->rsum[r];
    csum = ps->csum[c];

    //calcThreshold(ps);

    for (i = 1; i <= NN; i++)
    {
        if (ps->remain[i] == 0)
            continue;

        tempcsum = csum + i;
        temprsum = rsum + i;

        if (temprsum > ps->stdsum || tempcsum > ps->stdsum)//当前和与标准和比较
            break;

        if (c == N - 1 && temprsum != ps->stdsum)//同下条注释
            continue;
        if (r == N - 1 && tempcsum != ps->stdsum)//如果是行和不等于标准和就跳出
            continue;

        if (c > 1)
        {
            if ((temprsum + ps->threshold_max[c] < ps->stdsum) || temprsum + ps->threshold_min[c] > ps->stdsum)
                continue;
        }//当前和加上剩余理论最小值最大值与标准值的比较
        if (r > 1)
        {
            if ((tempcsum + ps->threshold_max[r] < ps->stdsum) || tempcsum + ps->threshold_min[r] > ps->stdsum)
                continue;
        }//当前和加上剩余理论最小值最大值与标准值的比较
        ps->totalCount++;

        ps->i[no] = i;
        ps->remain[i] = 0;//将用过的数字标零
        ps->rsum[r] = temprsum;
        ps->csum[c] = tempcsum;

        if (no == NN - 1)
        {
            ps->partResultCount++;
            if (exam(ps))//检查对角线是否满足，满足为真
            {
                ps->resultCount++;
                ps->time = clock() - ps->t_start;
                sl_append(ps);
                //print(ps->tail, false);
            }
        }
        else
        {
            msquare(ps, no + 1);
        }
        ps->remain[i] = 1;
        ps->i[no] = 0;
        //calcThreshold(ps);
    }
    ps->rsum[r] = rsum;
    ps->csum[c] = csum;
}

void initSquare(SQUARE *ps, int start, int end)
{
    int i, temp;

    memset(ps->i, 0, sizeof(ps->i));//将已经填好的数组清零
    ps->head.next = NULL;
    ps->tail = &(ps->head);

    ps->t_start = clock();
    ps->resultCount = ps->totalCount = ps->partResultCount = 0;
    ps->scope_start = start;
    ps->scope_end = end;

    temp = 0;//还原数据时用来记当前是第几个格子
    for (i = 1; i <= NN; i++)
    {
        ps->i[i] = 0;
        ps->remain[i] = 1;
        temp += i;
    }
    ps->stdsum = temp / N;//用来求标准和此时temp等于（1+N*N）*N/2
    ps->time = 0;

    for (i = 0; i < N; i++)
    {
        ps->csum[i] = 0;
        ps->rsum[i] = 0;
    }

    calcThreshold(ps);
}//还原结构体的所有数据

SQUARENODE *sl_append(SQUARE *ps)//链表建立节点
{
    SQUARENODE *newnode;

    newnode = new SQUARENODE;

    memcpy(&(newnode->i), ps->i, sizeof(int) * NN + 1);
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

    while (head->next != NULL)//判断是否结束链表
    {
        node = head->next;
        head->next = node->next;
        delete node;//删除节点，结束链表
    }
}

void fileWrite(char *filename, SQUARE *ps, int count )
{
    FILE *fp;
    SQUARENODE *n;
    char line[4096], temp[1024];
    int i, l, serialnum = 1;//serialnum为输出文件里行的标号

    fopen_s(&fp, filename, "w");//w为要打开的文件名称，此处可以改为其他自己想存储的文件

    for (l = 0; l < count; l++)
    {
        n = ps[l].head.next;
        sprintf(line, "\r\nthread %d, scope:%d~%d, ms used:%.3f\r\n", l + 1,
                ps[l].scope_start, ps[l].scope_end, ps[l].time / 1000.0);//格式化；规定一行的数据形式
        fwrite(line, strlen(line), 1, fp);//向文件中按行的模式输入

        while (n)
        {
            sprintf(line, "%6d [ ", serialnum++);
            for (i = 0; i < NN; i++)
            {
                sprintf(temp, "%2d ", n->i[i]);
                strcat_s(line, sizeof(line), temp);//strcat_s用于连接两个字符
            }
            strcat_s(line, sizeof(line), "] ");
            sprintf(temp, "%.3f  %ld  %ld\n", n->time / 1000.0, n->totalCount, n->partResultCount);
            strcat_s(line, sizeof(line), temp);

            fwrite(line, strlen(line), 1, fp);

            n = n->next;
        }
    }
    fclose(fp);
}

void calcThreshold(SQUARE *ps)
{
    int temp, i, count;

    //  N=4 ?, max[3]=0, max[2]=16, max[1]=31, max[0]=45
    count = N - 1;
    ps->threshold_max[count] = 0;
    for (i = 1; i < NN; i++)
    {
        temp = NN - i + 1;
        if (ps->remain[temp]==0)
            continue;
        ps->threshold_max[count - 1] = temp + ps->threshold_max[count];
        if (--count == 0)
            break;
    }

    // N=4??? min[3]=0, min[2]=1, min[1]=3, min[0]=6
    count = N - 1;
    ps->threshold_min[count] = 0;
    for (i = 1; i < NN; i++)
    {
        temp = i;
        if (ps->remain[temp]==0)
            continue;
        ps->threshold_min[count - 1] = temp + ps->threshold_min[count];
        if ( --count == 0 )
            break;
    }
}


int exam(SQUARE *ps)
{
    int j;
    int sum1, sum2;

    sum1 = 0;
    sum2 = 0;

    for (j = 0; j < N; j++)
    {
        sum1 += ps->i[j * N + j];         //M[j][j];
        sum2 += ps->i[j * N + N - 1 - j]; //M[j][2 - j];
    }//主副对角线求和

    
    if (sum1 != ps->stdsum || ps->stdsum != sum2)
        return 0;
    else
        return 1;
}
