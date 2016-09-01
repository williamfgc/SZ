/**
 *  @file test_compress.c
 *  @author Sheng Di
 *  @date April, 2015
 *  @brief This is an example of using compression interface
 *  (C) 2015 by Mathematics and Computer Science (MCS), Argonne National Laboratory.
 *      See COPYRIGHT in top-level directory.
 */


#include <stdio.h>
#include <stdlib.h>
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
    int r5=0,r4=0,r3=0,r2=0,r1=0;
    char outDir[640], oriFilePath[640], outputFilePath[640];
    char *cfgFile;
    
    if(argc < 3)
    {
	printf("Test case: testdouble_compress [config_file] [srcFilePath] [dimension sizes...]\n");
	printf("Example: testdouble_compress sz.config testdouble_8_8_128.dat 8 8 128\n");
	exit(0);
    }
   
    cfgFile=argv[1];
    sprintf(oriFilePath, "%s", argv[2]);
    if(argc>=4)
    	r1 = atoi(argv[3]); //8
    if(argc>=5)
    	r2 = atoi(argv[4]); //8
    if(argc>=6)
	r3 = atoi(argv[5]); //128
    if(argc>=7)
    	r4 = atoi(argv[6]);
    if(argc>=8)
	r5 = atoi(argv[7]);
 
    printf("cfgFile=%s\n", cfgFile); 
    SZ_Init(cfgFile);
    
    sprintf(outputFilePath, "%s.sz", oriFilePath);
   
    ulong nbEle;
    double *data = readDoubleData(oriFilePath, &nbEle);
   
    int outSize;
    cost_start(); 
    char *bytes = SZ_compress(SZ_DOUBLE, data, &outSize, r5, r4, r3, r2, r1);
    cost_end();
    printf("timecost=%f\n",totalCost);
    //char *bytes = (char *)malloc(nbEle*sizeof(double)); //
    //SZ_compress_args2(SZ_DOUBLE, data, bytes, &outSize, ABS, 0.000001, 0.000001, r5, r4, r3, r2, r1);

    writeByteData(bytes, outSize, outputFilePath);
    free(data);
    free(bytes);
    printf("done\n");
    
    SZ_Finalize();
    
    return 0;
}
