#include<stdio.h>
#include<time.h>


int main()
{
    int a[3][3]; 
    int i,j;
    int cflag;
    int *m;
    int sum1,sum2,csum[3],rsum[3];
    int resultcount;
    clock_t start,finish;
    int N=3;
    m=&a[0][0];
    start = clock();
    for( m[0]= 1;m[0]<=9; m[0]++)
    {
       for(m[1] = 1;m[1]<=9;m[1]++)
       {
           if (m[1]==m[0]) 
               continue;
           for(m[2]=1;m[2]<=9;m[2]++)
           {

                if (m[2]==m[1]||m[2]==m[0])
                    continue;
                for(m[3]=1;m[3]<=9;m[3]++)
                {
                    if(m[3]==m[2]||m[3]==m[1]||m[3]==m[0])
                        continue;
                    for(m[4]=1;m[4]<=9;m[4]++)
                    {
                        if(m[4]==m[3]||m[4]==m[2]||m[4]==m[1]||m[4]==m[0])
                            continue;
                        for(m[5]=1;m[5]<=9;m[5]++)
                        {
                            if (m[5]==m[0]||m[5]==m[1]||m[5]==m[2]||m[5]==m[3]||m[5]==m[4])
                                    continue;
                            for(m[6]=1;m[6]<=9;m[6]++)
                            {
                                if(m[6]==m[0]||m[6]==m[1]||m[6]==m[2]||m[6]==m[3]||m[6]==m[4]||m[6]==m[5])
                                    continue;
                                for(m[7]=1;m[7]<=9;m[7]++)
                                {
                                    if(m[7]==m[0]||m[7]==m[1]||m[7]==m[2]||m[7]==m[3]||m[7]==m[4]||m[7]==m[5]||m[7]==m[6])
                                        continue;
                                    for(m[8]=1;m[8]<=9;m[8]++)
                                    {
                                        if(m[8]==m[7]||m[8]==m[6]||m[8]==m[5]||m[8]==m[4]||m[8]==m[3]||m[8]==m[2]||m[8]==m[1]||m[8]==m[0])
                                            continue;
                                          
                                        sum1=0,sum2=0,cflag=0;
                                        
                                        for(i=0;i<=2;i++)
                                        {
                                            csum[i]=0;
                                            rsum[i]=0;
                                        }

                                        for(j=0;j<=2;j++)
                                        {
											sum1 += a[j][j];
											sum2 += a[2-j][j];
										}
                                        
                                        for (j=0;j<=2;j++) 
                                        {
											for (i=0;i<=2;i++)
                                            {
												csum[i] += a[j][i];
												rsum[j] += a[j][i];
											}
										}
                                        	
										if (sum1!=((1+N*N)*N/2)||sum2!=((1+N*N)*N/2))
											continue;
										for (j=0;j<=2;j++) 
                                        {
											if (csum[j]!=((1+N*N)*N/2)||rsum[j]!=((1+N*N)*N/2)) 
                                            {
												cflag=1;
												break;
											}
										}
                                        
                                        
                                        if(cflag==0)
                                        {   
                                            resultcount++;
                                            printf(" [ ");
                                            for(i=0;i<=8;i++)
                                            {
                                                printf("%d ",m[i]);
                                            }
                                            printf("]\n");
										}
                                    }
                                }     
                            }
                        }
                    }
                }
            }        
        }
    }
    finish=clock();
    printf("所用时间time=%f ms\n",(double)finish-start/CLK_TCK);
    printf("符合条件的一共有%d个",resultcount);	
}

    