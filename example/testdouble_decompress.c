/**
 *  @file test_decompress.c
 *  @author Sheng Di
 *  @date April, 2015
 *  @brief This is an example of using Decompression interface.
 *  (C) 2015 by Mathematics and Computer Science (MCS), Argonne National Laboratory.
 *      See COPYRIGHT in top-level directory.
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "sz.h"
#include "rw.h"

struct timeval startTime;
struct timeval endTime;  /* Start and end times */
struct timeval costStart; /*only used for recording the cost*/
double totalCost = 0;


void cost_start()
{
        gettimeofday(&costStart, NULL);
}

void cost_end()
{
        double elapsed;
        struct timeval costEnd;
        gettimeofday(&costEnd, NULL);
        elapsed = ((costEnd.tv_sec*1000000+costEnd.tv_usec)-(costStart.tv_sec*1000000+costStart.tv_usec))/1000000.0;
        totalCost += elapsed;
}

int main(int argc, char * argv[])
{
    int r5=0,r4=0,r3,r2,r1;
    ulong nbEle;
    char zipFilePath[640], outputFilePath[640];
    char *cfgFile;
    if(argc < 6)
    {
		printf("Test case: testdouble_decompress [configFile] [srcFilePath] [dimension sizes...]\n");
		printf("Example: testdouble_decompress sz.config testdouble_8_8_128.dat.sz 8 8 128\n");
		exit(0);
	}	
   
    cfgFile = argv[1];
    sprintf(zipFilePath, "%s", argv[2]);
    r1 = atoi(argv[3]); //8
    r2 = atoi(argv[4]); //8
    r3 = atoi(argv[5]); //128
    if(argc>=7)
	r4 = atoi(argv[6]);
    if(argc>=8)
        r5 = atoi(argv[7]);
    
    SZ_Init(cfgFile);
    
    nbEle = r1*r2*r3;
    if(argc>=7)
	nbEle*=r4;
    if(argc>=8)
	nbEle*=r5;
    
    sprintf(outputFilePath, "%s.out", zipFilePath);
    
    long byteLength;
    char *bytes = readByteData(zipFilePath, &byteLength);

    cost_start();    
    double *data = SZ_decompress(SZ_DOUBLE, bytes, byteLength, r5, r4, r3, r2, r1);
    cost_end();    
    printf("timecost=%f\n",totalCost);
    
    //int i=0;
    //for(;i<8192;i++)
    //	printf("i=%d, data=%f\n",i,data[i]);
    writeDoubleData(data, nbEle, outputFilePath);
    
    printf("done\n");
    
    SZ_Finalize();
    

    char oriFilePath[640];
    strncpy(oriFilePath, zipFilePath, (unsigned)strlen(zipFilePath)-3);
    oriFilePath[strlen(zipFilePath)-3] = '\0';
    double *ori_data = readDoubleData(oriFilePath, &nbEle);
    int i;
    double Max, Min, diffMax;
    Max = ori_data[0];
    Min = ori_data[0];
    diffMax = fabs(data[0] - ori_data[0]);

    for (i = 0; i < nbEle; i++)
    {
    	if (Max < ori_data[i]) Max = ori_data[i];
    	if (Min > ori_data[i]) Min = ori_data[i];
    	if (diffMax < fabs(data[i] - ori_data[i]))
    		diffMax = fabs(data[i] - ori_data[i]);
    }

    printf ("Max absolute error = %f\n", diffMax);
    printf ("Max relative error = %f\n", diffMax/(Max-Min));

	return 0;
}
