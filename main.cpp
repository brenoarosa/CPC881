/*
  CEC14 Test Function Suite for Single Objective Optimization
  Jane Jing Liang (email: liangjing@zzu.edu.cn; liangjing@pmail.ntu.edu.cn)
  Dec. 12th 2013
*/

#include <stdio.h>
#include <math.h>
#include <malloc.h>
#include <vector>


int main() {
    int i,j,k,n,m,func_num;
    double *f;
    FILE *fpt;
    char FileName[30];
    m=2;
    n=10;
    char x_str[40];

    std::vector<double> x(m*n, 0);

    f=(double *)malloc(sizeof(double)  *  m);
    for (i = 0; i < 30; i++)
    {
        func_num=i+1;
        CEC2014 objective_instance = CEC2014(func_num, n);

        sprintf(FileName, "input_data/shift_data_%d.txt", func_num);
        fpt = fopen(FileName,"r");
        if (fpt==NULL)
        {
            printf("\n Error: Cannot open input file for reading \n");
        }

        for(k=0;k<n;k++)
        {
                fscanf(fpt, "%s", x_str);
                x[k] = atof(x_str);
                //printf("%f\n",x[k]);
        }

        fclose(fpt);

            for (j = 0; j < n; j++)
            {
                x[1*n+j]=0.0;
                //printf("%f\n",x[1*n+j]);
            }


        for (k = 0; k < 1; k++)
        {
            objective_instance.fitness_all_population(x.data(), f, m);
            for (j = 0; j < 2; j++)
            {
                printf("f%d(x[%d]) = %f,",func_num,j+1,f[j]);
            }
            printf("\n");
        }

    }

    return 0;
}
