/*
  CEC14 Test Function Suite for Single Objective Optimization
  Jane Jing Liang (email: liangjing@zzu.edu.cn; liangjing@pmail.ntu.edu.cn)
  Dec. 20th 2013
*/


#include <stdio.h>
#include <math.h>
#include <malloc.h>
#include <vector>

#define INF 1.0e99
#define EPS 1.0e-14
#define E  2.7182818284590452353602874713526625
#define PI 3.1415926535897932384626433832795029

typedef std::vector<double> vector_double;

double *OShift,*M,*z,*x_bound;
int ini_flag=0,n_flag,func_flag,*SS;
char aux_reader[40];
vector_double y;


class CEC2014 {

public:

    // Constructor
    CEC2014() {}

    void cec14_test_func(double *x, double *f, int nx, int mx, int func_num) {
        int cf_num=10,i,j;
        if (ini_flag==1)
        {
            if ((n_flag!=nx)||(func_flag!=func_num))
            {
                ini_flag=0;
            }
        }

        if (ini_flag==0)
        {
            FILE *fpt;
            char FileName[256];
            free(M);
            free(OShift);
            free(z);
            free(x_bound);
            y.reserve(nx);
            z=(double *)malloc(sizeof(double)  *  nx);
            x_bound=(double *)malloc(sizeof(double)  *  nx);
            for (i=0; i<nx; i++)
                x_bound[i]=100.0;

            if (!(nx==2||nx==10||nx==20||nx==30||nx==50||nx==100))
            {
                printf("\nError: Test functions are only defined for D=2,10,20,30,50,100.\n");
            }
            if (nx==2&&((func_num>=17&&func_num<=22)||(func_num>=29&&func_num<=30)))
            {
                printf("\nError: hf01,hf02,hf03,hf04,hf05,hf06,cf07&cf08 are NOT defined for D=2.\n");
            }

            /* Load Matrix M*/
            sprintf(FileName, "input_data/M_%d_D%d.txt", func_num,nx);
            fpt = fopen(FileName,"r");
            if (fpt==NULL)
            {
                printf("\n Error: Cannot open input file for reading \n");
            }
            if (func_num<23)
            {
                M=(double*)malloc(nx*nx*sizeof(double));
                if (M==NULL)
                    printf("\nError: there is insufficient memory available!\n");
                for (i=0; i<nx*nx; i++)
                {
                    fscanf(fpt, "%s", aux_reader);
                    M[i] = atof(aux_reader);
                }
            }
            else
            {
                M=(double*)malloc(cf_num*nx*nx*sizeof(double));
                if (M==NULL)
                    printf("\nError: there is insufficient memory available!\n");
                for (i=0; i<cf_num*nx*nx; i++)
                {
                    fscanf(fpt, "%s", aux_reader);
                    M[i] = atof(aux_reader);
                }
            }
            fclose(fpt);

                /* Load shift_data */
                sprintf(FileName, "input_data/shift_data_%d.txt", func_num);
                fpt = fopen(FileName,"r");
                if (fpt==NULL)
                {
                    printf("\n Error: Cannot open input file for reading \n");
                }

                if (func_num<23)
                {
                    OShift=(double *)malloc(nx*sizeof(double));
                    if (OShift==NULL)
                    printf("\nError: there is insufficient memory available!\n");
                    for(i=0;i<nx;i++)
                    {
                        fscanf(fpt, "%s", aux_reader);
                        OShift[i] = atof(aux_reader);
                    }
                }
                else
                {
                    OShift=(double *)malloc(nx*cf_num*sizeof(double));
                    if (OShift==NULL)
                    printf("\nError: there is insufficient memory available!\n");
                    for(i=0;i<cf_num-1;i++)
                    {
                        for (j=0;j<nx;j++)
                        {
                            fscanf(fpt, "%s", aux_reader);
                            OShift[i*nx+j] = atof(aux_reader);
                        }
                        fscanf(fpt,"%*[^\n]%*c");
                    }
                    for (j=0;j<nx;j++)
                    {
                        fscanf(fpt, "%s", aux_reader);
                        OShift[(cf_num-1)*nx+j] = atof(aux_reader);
                    }

                }
                fclose(fpt);


                /* Load Shuffle_data */

                if (func_num>=17&&func_num<=22)
                {
                    sprintf(FileName, "input_data/shuffle_data_%d_D%d.txt", func_num, nx);
                    fpt = fopen(FileName,"r");
                    if (fpt==NULL)
                    {
                        printf("\n Error: Cannot open input file for reading \n");
                    }
                    SS=(int *)malloc(nx*sizeof(int));
                    if (SS==NULL)
                        printf("\nError: there is insufficient memory available!\n");
                    for(i=0;i<nx;i++)
                    {
                        fscanf(fpt,"%d",&SS[i]);
                    }
                    fclose(fpt);
                }
                else if (func_num==29||func_num==30)
                {
                    sprintf(FileName, "input_data/shuffle_data_%d_D%d.txt", func_num, nx);
                    fpt = fopen(FileName,"r");
                    if (fpt==NULL)
                    {
                        printf("\n Error: Cannot open input file for reading \n");
                    }
                    SS=(int *)malloc(nx*cf_num*sizeof(int));
                    if (SS==NULL)
                        printf("\nError: there is insufficient memory available!\n");
                    for(i=0;i<nx*cf_num;i++)
                    {
                        fscanf(fpt,"%d",&SS[i]);
                    }
                    fclose(fpt);
                }


                n_flag=nx;
                func_flag=func_num;
                ini_flag=1;
                //printf("Function has been initialized!\n");
            }


            for (i = 0; i < mx; i++)
            {
                switch(func_num)
                {
                case 1:
                    ellips_func(&x[i*nx],&f[i],nx,OShift,M,1,1);
                    f[i]+=100.0;
                    break;
                case 2:
                    bent_cigar_func(&x[i*nx],&f[i],nx,OShift,M,1,1);
                    f[i]+=200.0;
                    break;
                case 3:
                    discus_func(&x[i*nx],&f[i],nx,OShift,M,1,1);
                    f[i]+=300.0;
                    break;
                case 4:
                    rosenbrock_func(&x[i*nx],&f[i],nx,OShift,M,1,1);
                    f[i]+=400.0;
                    break;
                case 5:
                    ackley_func(&x[i*nx],&f[i],nx,OShift,M,1,1);
                    f[i]+=500.0;
                    break;
                case 6:
                    weierstrass_func(&x[i*nx],&f[i],nx,OShift,M,1,1);
                    f[i]+=600.0;
                    break;
                case 7:
                    griewank_func(&x[i*nx],&f[i],nx,OShift,M,1,1);
                    f[i]+=700.0;
                    break;
                case 8:
                    rastrigin_func(&x[i*nx],&f[i],nx,OShift,M,1,0);
                    f[i]+=800.0;
                    break;
                case 9:
                    rastrigin_func(&x[i*nx],&f[i],nx,OShift,M,1,1);
                    f[i]+=900.0;
                    break;
                case 10:
                    schwefel_func(&x[i*nx],&f[i],nx,OShift,M,1,0);
                    f[i]+=1000.0;
                    break;
                case 11:
                    schwefel_func(&x[i*nx],&f[i],nx,OShift,M,1,1);
                    f[i]+=1100.0;
                    break;
                case 12:
                    katsuura_func(&x[i*nx],&f[i],nx,OShift,M,1,1);
                    f[i]+=1200.0;
                    break;
                case 13:
                    happycat_func(&x[i*nx],&f[i],nx,OShift,M,1,1);
                    f[i]+=1300.0;
                    break;
                case 14:
                    hgbat_func(&x[i*nx],&f[i],nx,OShift,M,1,1);
                    f[i]+=1400.0;
                    break;
                case 15:
                    grie_rosen_func(&x[i*nx],&f[i],nx,OShift,M,1,1);
                    f[i]+=1500.0;
                    break;
                case 16:
                    escaffer6_func(&x[i*nx],&f[i],nx,OShift,M,1,1);
                    f[i]+=1600.0;
                    break;
                case 17:
                    hf01(&x[i*nx],&f[i],nx,OShift,M,SS,1,1);
                    f[i]+=1700.0;
                    break;
                case 18:
                    hf02(&x[i*nx],&f[i],nx,OShift,M,SS,1,1);
                    f[i]+=1800.0;
                    break;
                case 19:
                    hf03(&x[i*nx],&f[i],nx,OShift,M,SS,1,1);
                    f[i]+=1900.0;
                    break;
                case 20:
                    hf04(&x[i*nx],&f[i],nx,OShift,M,SS,1,1);
                    f[i]+=2000.0;
                    break;
                case 21:
                    hf05(&x[i*nx],&f[i],nx,OShift,M,SS,1,1);
                    f[i]+=2100.0;
                    break;
                case 22:
                    hf06(&x[i*nx],&f[i],nx,OShift,M,SS,1,1);
                    f[i]+=2200.0;
                    break;
                case 23:
                    cf01(&x[i*nx],&f[i],nx,OShift,M,1);
                    f[i]+=2300.0;
                    break;
                case 24:
                    cf02(&x[i*nx],&f[i],nx,OShift,M,1);
                    f[i]+=2400.0;
                    break;
                case 25:
                    cf03(&x[i*nx],&f[i],nx,OShift,M,1);
                    f[i]+=2500.0;
                    break;
                case 26:
                    cf04(&x[i*nx],&f[i],nx,OShift,M,1);
                    f[i]+=2600.0;
                    break;
                case 27:
                    cf05(&x[i*nx],&f[i],nx,OShift,M,1);
                    f[i]+=2700.0;
                    break;
                case 28:
                    cf06(&x[i*nx],&f[i],nx,OShift,M,1);
                    f[i]+=2800.0;
                    break;
                case 29:
                    cf07(&x[i*nx],&f[i],nx,OShift,M,SS,1);
                    f[i]+=2900.0;
                    break;
                case 30:
                    cf08(&x[i*nx],&f[i],nx,OShift,M,SS,1);
                    f[i]+=3000.0;
                    break;
                default:
                    printf("\nError: There are only 30 test functions in this test suite!\n");
                    f[i] = 0.0;
                    break;
                }

            }

    }


private:

    /* Sphere */
    void sphere_func (double *x, double *f, int nx, double *Os, double *Mr, int s_flag, int r_flag) {

        int i;
        f[0] = 0.0;
        sr_func(x, z, nx, Os, Mr, 1.0, s_flag, r_flag); /* shift and rotate */
        for (i=0; i<nx; i++)
        {
            f[0] += z[i]*z[i];
        }

    }

    /* Ellipsoidal */
    void ellips_func (double *x, double *f, int nx, double *Os,double *Mr, int s_flag, int r_flag) {

        int i;
        f[0] = 0.0;
        sr_func(x, z, nx, Os, Mr,1.0, s_flag, r_flag); /* shift and rotate */
        for (i=0; i<nx; i++)
        {
           f[0] += pow(10.0,6.0*i/(nx-1))*z[i]*z[i];
        }
    }

    /* Bent_Cigar */
    void bent_cigar_func (double *x, double *f, int nx, double *Os,double *Mr, int s_flag, int r_flag) {

        int i;
        sr_func(x, z, nx, Os, Mr,1.0, s_flag, r_flag); /* shift and rotate */

        f[0] = z[0]*z[0];
        for (i=1; i<nx; i++)
        {
            f[0] += pow(10.0,6.0)*z[i]*z[i];
        }


    }

    /* Discus */
    void discus_func (double *x, double *f, int nx, double *Os,double *Mr, int s_flag, int r_flag) {

        int i;
        sr_func(x, z, nx, Os, Mr,1.0, s_flag, r_flag); /* shift and rotate */
        f[0] = pow(10.0,6.0)*z[0]*z[0];
        for (i=1; i<nx; i++)
        {
            f[0] += z[i]*z[i];
        }
    }

    /* Different Powers */
    void dif_powers_func (double *x, double *f, int nx, double *Os,double *Mr,int s_flag, int r_flag) {

        int i;
        f[0] = 0.0;
        sr_func(x, z, nx, Os, Mr, 1.0, s_flag, r_flag); /* shift and rotate */

        for (i=0; i<nx; i++)
        {
            f[0] += pow(fabs(z[i]),2+4*i/(nx-1));
        }
        f[0]=pow(f[0],0.5);
    }


    /* Rosenbrock's */
    void rosenbrock_func (double *x, double *f, int nx, double *Os,double *Mr,int s_flag, int r_flag) {

        int i;
        double tmp1,tmp2;
        f[0] = 0.0;
        sr_func(x, z, nx, Os, Mr, 2.048/100.0, s_flag, r_flag); /* shift and rotate */
        z[0] += 1.0;//shift to orgin
        for (i=0; i<nx-1; i++)
        {
            z[i+1] += 1.0;//shift to orgin
            tmp1=z[i]*z[i]-z[i+1];
            tmp2=z[i]-1.0;
            f[0] += 100.0*tmp1*tmp1 +tmp2*tmp2;
        }
    }

    /* Schwefel's 1.2  */
    void schaffer_F7_func (double *x, double *f, int nx, double *Os,double *Mr,int s_flag, int r_flag) {

        int i;
        double tmp;
        f[0] = 0.0;
        sr_func(x, z, nx, Os, Mr, 1.0, s_flag, r_flag); /* shift and rotate */
        for (i=0; i<nx-1; i++)
        {
            z[i]=pow(y[i]*y[i]+y[i+1]*y[i+1],0.5);
            tmp=sin(50.0*pow(z[i],0.2));
            f[0] += pow(z[i],0.5)+pow(z[i],0.5)*tmp*tmp ;
        }
        f[0] = f[0]*f[0]/(nx-1)/(nx-1);
    }

    /* Ackley's  */
    void ackley_func (double *x, double *f, int nx, double *Os,double *Mr,int s_flag, int r_flag) {

        int i;
        double sum1, sum2;
        sum1 = 0.0;
        sum2 = 0.0;

        sr_func(x, z, nx, Os, Mr, 1.0, s_flag, r_flag); /* shift and rotate */

        for (i=0; i<nx; i++)
        {
            sum1 += z[i]*z[i];
            sum2 += cos(2.0*PI*z[i]);
        }
        sum1 = -0.2*sqrt(sum1/nx);
        sum2 /= nx;
            f[0] =  E - 20.0*exp(sum1) - exp(sum2) +20.0;
    }


    /* Weierstrass's  */
    void weierstrass_func (double *x, double *f, int nx, double *Os,double *Mr,int s_flag, int r_flag) {

        int i,j,k_max;
        double sum,sum2, a, b;
        a = 0.5;
        b = 3.0;
        k_max = 20;
        f[0] = 0.0;

        sr_func(x, z, nx, Os, Mr, 0.5/100.0, s_flag, r_flag); /* shift and rotate */

        for (i=0; i<nx; i++)
        {
            sum = 0.0;
            sum2 = 0.0;
            for (j=0; j<=k_max; j++)
            {
                sum += pow(a,j)*cos(2.0*PI*pow(b,j)*(z[i]+0.5));
                sum2 += pow(a,j)*cos(2.0*PI*pow(b,j)*0.5);
            }
            f[0] += sum;
        }
        f[0] -= nx*sum2;
    }


    /* Griewank's  */
    void griewank_func (double *x, double *f, int nx, double *Os,double *Mr,int s_flag, int r_flag) {

        int i;
        double s, p;
        s = 0.0;
        p = 1.0;

        sr_func(x, z, nx, Os, Mr, 600.0/100.0, s_flag, r_flag); /* shift and rotate */

        for (i=0; i<nx; i++)
        {
            s += z[i]*z[i];
            p *= cos(z[i]/sqrt(1.0+i));
        }
        f[0] = 1.0 + s/4000.0 - p;
    }

    /* Rastrigin's  */
    void rastrigin_func (double *x, double *f, int nx, double *Os,double *Mr,int s_flag, int r_flag) {

        int i;
        f[0] = 0.0;

        sr_func(x, z, nx, Os, Mr, 5.12/100.0, s_flag, r_flag); /* shift and rotate */

        for (i=0; i<nx; i++)
        {
            f[0] += (z[i]*z[i] - 10.0*cos(2.0*PI*z[i]) + 10.0);
        }
    }

    /* Noncontinuous Rastrigin's  */
    void step_rastrigin_func (double *x, double *f, int nx, double *Os,double *Mr,int s_flag, int r_flag) {

        int i;
        f[0]=0.0;
        for (i=0; i<nx; i++)
        {
            if (fabs(y[i]-Os[i])>0.5)
            y[i]=Os[i]+floor(2*(y[i]-Os[i])+0.5)/2;
        }

        sr_func(x, z, nx, Os, Mr, 5.12/100.0, s_flag, r_flag); /* shift and rotate */

        for (i=0; i<nx; i++)
        {
            f[0] += (z[i]*z[i] - 10.0*cos(2.0*PI*z[i]) + 10.0);
        }
    }

    /* Schwefel's  */
    void schwefel_func (double *x, double *f, int nx, double *Os,double *Mr,int s_flag, int r_flag) {

        int i;
        double tmp;
        f[0]=0.0;

        sr_func(x, z, nx, Os, Mr, 1000.0/100.0, s_flag, r_flag); /* shift and rotate */

        for (i=0; i<nx; i++)
        {
            z[i] += 4.209687462275036e+002;
            if (z[i]>500)
            {
                f[0]-=(500.0-fmod(z[i],500))*sin(pow(500.0-fmod(z[i],500),0.5));
                tmp=(z[i]-500.0)/100;
                f[0]+= tmp*tmp/nx;
            }
            else if (z[i]<-500)
            {
                f[0]-=(-500.0+fmod(fabs(z[i]),500))*sin(pow(500.0-fmod(fabs(z[i]),500),0.5));
                tmp=(z[i]+500.0)/100;
                f[0]+= tmp*tmp/nx;
            }
            else
                f[0]-=z[i]*sin(pow(fabs(z[i]),0.5));
            }
            f[0] +=4.189828872724338e+002*nx;

    }

    /* Katsuura  */
    void katsuura_func (double *x, double *f, int nx, double *Os,double *Mr,int s_flag, int r_flag) {

        int i,j;
        double temp,tmp1,tmp2,tmp3;
        f[0]=1.0;
        tmp3=pow(1.0*nx,1.2);

        sr_func(x, z, nx, Os, Mr, 5.0/100.0, s_flag, r_flag); /* shift and rotate */

        for (i=0; i<nx; i++)
        {
            temp=0.0;
            for (j=1; j<=32; j++)
            {
                tmp1=pow(2.0,j);
                tmp2=tmp1*z[i];
                temp += fabs(tmp2-floor(tmp2+0.5))/tmp1;
            }
            f[0] *= pow(1.0+(i+1)*temp,10.0/tmp3);
        }
        tmp1=10.0/nx/nx;
        f[0]=f[0]*tmp1-tmp1;

    }

    /* Lunacek Bi_rastrigin Function */
    void bi_rastrigin_func (double *x, double *f, int nx, double *Os,double *Mr,int s_flag, int r_flag) {

        int i;
        double mu0=2.5,d=1.0,s,mu1,tmp,tmp1,tmp2;
        double *tmpx;
        tmpx=(double *)malloc(sizeof(double)  *  nx);
        s=1.0-1.0/(2.0*pow(nx+20.0,0.5)-8.2);
        mu1=-pow((mu0*mu0-d)/s,0.5);

        if (s_flag==1) {
            shiftfunc(x, y.data(), nx, Os);
        }
        else {
            //shrink to the orginal search range
            for (i=0; i<nx; i++) {
                y[i] = x[i];
            }
        }
        //shrink to the orginal search range
        for (i=0; i<nx; i++) {
            y[i] *= 10.0/100.0;
        }

        for (i = 0; i < nx; i++) {
            tmpx[i]=2*y[i];
            if (Os[i] < 0.0) {
                tmpx[i] *= -1.;
            }
        }
        for (i=0; i<nx; i++) {
            z[i]=tmpx[i];
            tmpx[i] += mu0;
        }
        tmp1=0.0;tmp2=0.0;
        for (i=0; i<nx; i++) {
            tmp = tmpx[i]-mu0;
            tmp1 += tmp*tmp;
            tmp = tmpx[i]-mu1;
            tmp2 += tmp*tmp;
        }

        tmp2 *= s;
        tmp2 += d*nx;
        tmp=0.0;

        if (r_flag==1) {
            rotatefunc(z, y.data(), nx, Mr);
            for (i=0; i<nx; i++) {
                tmp+=cos(2.0*PI*y[i]);
            }
            if(tmp1<tmp2) {
                f[0] = tmp1;
            }
            else {
                f[0] = tmp2;
            }
            f[0] += 10.0*(nx-tmp);
        }
        else {
            for (i=0; i<nx; i++) {
                tmp+=cos(2.0*PI*z[i]);
            }
            if(tmp1<tmp2) {
                f[0] = tmp1;
            }
            else {
                f[0] = tmp2;
            }
            f[0] += 10.0*(nx-tmp);
        }

        free(tmpx);
    }

    /* Griewank-Rosenbrock  */
    void grie_rosen_func (double *x, double *f, int nx, double *Os,double *Mr,int s_flag, int r_flag) {

        int i;
        double temp,tmp1,tmp2;
        f[0]=0.0;

        sr_func(x, z, nx, Os, Mr, 5.0/100.0, s_flag, r_flag); /* shift and rotate */

        z[0] += 1.0;//shift to orgin
        for (i=0; i<nx-1; i++)
        {
            z[i+1] += 1.0;//shift to orgin
            tmp1 = z[i]*z[i]-z[i+1];
            tmp2 = z[i]-1.0;
            temp = 100.0*tmp1*tmp1 + tmp2*tmp2;
             f[0] += (temp*temp)/4000.0 - cos(temp) + 1.0;
        }
        tmp1 = z[nx-1]*z[nx-1]-z[0];
        tmp2 = z[nx-1]-1.0;
        temp = 100.0*tmp1*tmp1 + tmp2*tmp2;
        f[0] += (temp*temp)/4000.0 - cos(temp) + 1.0 ;
    }


    /* Expanded Scaffer??s F6  */
    void escaffer6_func (double *x, double *f, int nx, double *Os,double *Mr,int s_flag, int r_flag) {

        int i;
        double temp1, temp2;

        sr_func(x, z, nx, Os, Mr, 1.0, s_flag, r_flag); /* shift and rotate */

        f[0] = 0.0;
        for (i=0; i<nx-1; i++)
        {
            temp1 = sin(sqrt(z[i]*z[i]+z[i+1]*z[i+1]));
            temp1 =temp1*temp1;
            temp2 = 1.0 + 0.001*(z[i]*z[i]+z[i+1]*z[i+1]);
            f[0] += 0.5 + (temp1-0.5)/(temp2*temp2);
        }
        temp1 = sin(sqrt(z[nx-1]*z[nx-1]+z[0]*z[0]));
        temp1 =temp1*temp1;
        temp2 = 1.0 + 0.001*(z[nx-1]*z[nx-1]+z[0]*z[0]);
        f[0] += 0.5 + (temp1-0.5)/(temp2*temp2);
    }

    /* HappyCat, provdided by Hans-Georg Beyer (HGB) */
    /* original global optimum: [-1,-1,...,-1] */
    void happycat_func (double *x, double *f, int nx, double *Os,double *Mr,int s_flag, int r_flag) {

        int i;
        double alpha,r2,sum_z;
        alpha=1.0/8.0;

        sr_func(x, z, nx, Os, Mr, 5.0/100.0, s_flag, r_flag); /* shift and rotate */

        r2 = 0.0;
        sum_z=0.0;
        for (i=0; i<nx; i++) {
            z[i]=z[i]-1.0;//shift to orgin
            r2 += z[i]*z[i];
            sum_z += z[i];
        }

        f[0]=pow(fabs(r2-nx),2*alpha) + (0.5*r2 + sum_z)/nx + 0.5;
    }

    /* HGBat, provdided by Hans-Georg Beyer (HGB)*/
    /* original global optimum: [-1,-1,...,-1] */
    void hgbat_func (double *x, double *f, int nx, double *Os,double *Mr,int s_flag, int r_flag) {

        int i;
        double alpha,r2,sum_z;
        alpha=1.0/4.0;

        sr_func(x, z, nx, Os, Mr, 5.0/100.0, s_flag, r_flag); /* shift and rotate */

        r2 = 0.0;
        sum_z=0.0;
        for (i=0; i<nx; i++) {
            z[i]=z[i]-1.0;//shift to orgin
            r2 += z[i]*z[i];
            sum_z += z[i];
        }

        f[0]=pow(fabs(pow(r2,2.0)-pow(sum_z,2.0)),2*alpha) + (0.5*r2 + sum_z)/nx + 0.5;
    }

    /* Hybrid Function 1 */
    void hf01 (double *x, double *f, int nx, double *Os,double *Mr,int *S,int s_flag,int r_flag) {

        int i,tmp,cf_num=3;
        double fit[3];
        int G[3],G_nx[3];
        double Gp[3]={0.3,0.3,0.4};

        tmp=0;
        for (i=0; i<cf_num-1; i++)
        {
            G_nx[i] = ceil(Gp[i]*nx);
            tmp += G_nx[i];
        }
        G_nx[cf_num-1]=nx-tmp;
        G[0]=0;
        for (i=1; i<cf_num; i++)
        {
            G[i] = G[i-1]+G_nx[i-1];
        }

        sr_func(x, z, nx, Os, Mr, 1.0, s_flag, r_flag); /* shift and rotate */

        for (i=0; i<nx; i++)
        {
            y[i]=z[S[i]-1];
        }
        i=0;
        schwefel_func(&y[G[i]],&fit[i],G_nx[i],Os,Mr,0,0);
        i=1;
        rastrigin_func(&y[G[i]],&fit[i],G_nx[i],Os,Mr,0,0);
        i=2;
        ellips_func(&y[G[i]],&fit[i],G_nx[i],Os,Mr,0,0);
        f[0]=0.0;
        for(i=0;i<cf_num;i++)
        {
            f[0] += fit[i];
        }
    }

    /* Hybrid Function 2 */
    void hf02 (double *x, double *f, int nx, double *Os,double *Mr,int *S,int s_flag,int r_flag) {

        int i,tmp,cf_num=3;
        double fit[3];
        int G[3],G_nx[3];
        double Gp[3]={0.3,0.3,0.4};

        tmp=0;
        for (i=0; i<cf_num-1; i++)
        {
            G_nx[i] = ceil(Gp[i]*nx);
            tmp += G_nx[i];
        }
        G_nx[cf_num-1]=nx-tmp;

        G[0]=0;
        for (i=1; i<cf_num; i++)
        {
            G[i] = G[i-1]+G_nx[i-1];
        }

        sr_func(x, z, nx, Os, Mr, 1.0, s_flag, r_flag); /* shift and rotate */

        for (i=0; i<nx; i++)
        {
            y[i]=z[S[i]-1];
        }
        i=0;
        bent_cigar_func(&y[G[i]],&fit[i],G_nx[i],Os,Mr,0,0);
        i=1;
        hgbat_func(&y[G[i]],&fit[i],G_nx[i],Os,Mr,0,0);
        i=2;
        rastrigin_func(&y[G[i]],&fit[i],G_nx[i],Os,Mr,0,0);

        f[0]=0.0;
        for(i=0;i<cf_num;i++)
        {
            f[0] += fit[i];
        }
    }

    /* Hybrid Function 3 */
    void hf03 (double *x, double *f, int nx, double *Os,double *Mr,int *S,int s_flag,int r_flag) {

        int i,tmp,cf_num=4;
        double fit[4];
        int G[4],G_nx[4];
        double Gp[4]={0.2,0.2,0.3,0.3};

        tmp=0;
        for (i=0; i<cf_num-1; i++)
        {
            G_nx[i] = ceil(Gp[i]*nx);
            tmp += G_nx[i];
        }
        G_nx[cf_num-1]=nx-tmp;

        G[0]=0;
        for (i=1; i<cf_num; i++)
        {
            G[i] = G[i-1]+G_nx[i-1];
        }

        sr_func(x, z, nx, Os, Mr, 1.0, s_flag, r_flag); /* shift and rotate */

        for (i=0; i<nx; i++)
        {
            y[i]=z[S[i]-1];
        }
        i=0;
        griewank_func(&y[G[i]],&fit[i],G_nx[i],Os,Mr,0,0);
        i=1;
        weierstrass_func(&y[G[i]],&fit[i],G_nx[i],Os,Mr,0,0);
        i=2;
        rosenbrock_func(&y[G[i]],&fit[i],G_nx[i],Os,Mr,0,0);
        i=3;
        escaffer6_func(&y[G[i]],&fit[i],G_nx[i],Os,Mr,0,0);

        f[0]=0.0;
        for(i=0;i<cf_num;i++)
        {
            f[0] += fit[i];
        }
    }

    /* Hybrid Function 4 */
    void hf04 (double *x, double *f, int nx, double *Os,double *Mr,int *S,int s_flag,int r_flag) {

        int i,tmp,cf_num=4;
        double fit[4];
        int G[4],G_nx[4];
        double Gp[4]={0.2,0.2,0.3,0.3};

        tmp=0;
        for (i=0; i<cf_num-1; i++) {
            G_nx[i] = ceil(Gp[i]*nx);
            tmp += G_nx[i];
        }
        G_nx[cf_num-1]=nx-tmp;

        G[0]=0;
        for (i=1; i<cf_num; i++) {
            G[i] = G[i-1]+G_nx[i-1];
        }

        sr_func(x, z, nx, Os, Mr, 1.0, s_flag, r_flag); /* shift and rotate */

        for (i=0; i<nx; i++) {
            y[i]=z[S[i]-1];
        }
        i=0;
        hgbat_func(&y[G[i]],&fit[i],G_nx[i],Os,Mr,0,0);
        i=1;
        discus_func(&y[G[i]],&fit[i],G_nx[i],Os,Mr,0,0);
        i=2;
        grie_rosen_func(&y[G[i]],&fit[i],G_nx[i],Os,Mr,0,0);
        i=3;
        rastrigin_func(&y[G[i]],&fit[i],G_nx[i],Os,Mr,0,0);

        f[0]=0.0;
        for(i=0;i<cf_num;i++) {
            f[0] += fit[i];
        }
    }

    /* Hybrid Function 5 */
    void hf05 (double *x, double *f, int nx, double *Os,double *Mr,int *S,int s_flag,int r_flag) {

        int i,tmp,cf_num=5;
        double fit[5];
        int G[5],G_nx[5];
        double Gp[5]={0.1,0.2,0.2,0.2,0.3};

        tmp=0;
        for (i=0; i<cf_num-1; i++) {
            G_nx[i] = ceil(Gp[i]*nx);
            tmp += G_nx[i];
        }

        G_nx[cf_num-1]=nx-tmp;

        G[0]=0;
        for (i=1; i<cf_num; i++) {
            G[i] = G[i-1]+G_nx[i-1];
        }

        sr_func(x, z, nx, Os, Mr, 1.0, s_flag, r_flag); /* shift and rotate */

        for (i=0; i<nx; i++) {
            y[i]=z[S[i]-1];
        }

        i=0;
        escaffer6_func(&y[G[i]],&fit[i],G_nx[i],Os,Mr,0,0);
        i=1;
        hgbat_func(&y[G[i]],&fit[i],G_nx[i],Os,Mr,0,0);
        i=2;
        rosenbrock_func(&y[G[i]],&fit[i],G_nx[i],Os,Mr,0,0);
        i=3;
        schwefel_func(&y[G[i]],&fit[i],G_nx[i],Os,Mr,0,0);
        i=4;
        ellips_func(&y[G[i]],&fit[i],G_nx[i],Os,Mr,0,0);

        f[0]=0.0;
        for(i=0;i<cf_num;i++) {
            f[0] += fit[i];
        }
    }

    /* Hybrid Function 6 */
    void hf06 (double *x, double *f, int nx, double *Os,double *Mr,int *S,int s_flag,int r_flag) {

        int i,tmp,cf_num=5;
        double fit[5];
        int G[5],G_nx[5];
        double Gp[5]={0.1,0.2,0.2,0.2,0.3};

        tmp=0;
        for (i=0; i<cf_num-1; i++) {
            G_nx[i] = ceil(Gp[i]*nx);
            tmp += G_nx[i];
        }

        G_nx[cf_num-1]=nx-tmp;

        G[0]=0;
        for (i=1; i<cf_num; i++) {
            G[i] = G[i-1]+G_nx[i-1];
        }

        sr_func(x, z, nx, Os, Mr, 1.0, s_flag, r_flag); /* shift and rotate */

        for (i=0; i<nx; i++) {
            y[i]=z[S[i]-1];
        }

        i=0;
        katsuura_func(&y[G[i]],&fit[i],G_nx[i],Os,Mr,0,0);
        i=1;
        happycat_func(&y[G[i]],&fit[i],G_nx[i],Os,Mr,0,0);
        i=2;
        grie_rosen_func(&y[G[i]],&fit[i],G_nx[i],Os,Mr,0,0);
        i=3;
        schwefel_func(&y[G[i]],&fit[i],G_nx[i],Os,Mr,0,0);
        i=4;
        ackley_func(&y[G[i]],&fit[i],G_nx[i],Os,Mr,0,0);
        f[0]=0.0;
        for(i=0;i<cf_num;i++) {
            f[0] += fit[i];
        }
    }

    /* Composition Function 1 */
    void cf01 (double *x, double *f, int nx, double *Os,double *Mr,int r_flag) {

        int i,cf_num=5;
        double fit[5];
        double delta[5] = {10, 20, 30, 40, 50};
        double bias[5] = {0, 100, 200, 300, 400};

        i=0;
        rosenbrock_func(x,&fit[i],nx,&Os[i*nx],&Mr[i*nx*nx],1,r_flag);
        fit[i]=10000*fit[i]/1e+4;
        i=1;
        ellips_func(x,&fit[i],nx,&Os[i*nx],&Mr[i*nx*nx],1,r_flag);
        fit[i]=10000*fit[i]/1e+10;
        i=2;
        bent_cigar_func(x,&fit[i],nx,&Os[i*nx],&Mr[i*nx*nx],1,r_flag);
        fit[i]=10000*fit[i]/1e+30;
        i=3;
        discus_func(x,&fit[i],nx,&Os[i*nx],&Mr[i*nx*nx],1,r_flag);
        fit[i]=10000*fit[i]/1e+10;
        i=4;
        ellips_func(x,&fit[i],nx,&Os[i*nx],&Mr[i*nx*nx],1,0);
        fit[i]=10000*fit[i]/1e+10;
        cf_cal(x, f, nx, Os, delta,bias,fit,cf_num);
    }

    /* Composition Function 2 */
    void cf02 (double *x, double *f, int nx, double *Os,double *Mr,int r_flag) {

        int i,cf_num=3;
        double fit[3];
        double delta[3] = {20,20,20};
        double bias[3] = {0, 100, 200};

        i=0;
        schwefel_func(x,&fit[i],nx,&Os[i*nx],&Mr[i*nx*nx],1,0);
        i=1;
        rastrigin_func(x,&fit[i],nx,&Os[i*nx],&Mr[i*nx*nx],1,r_flag);
        i=2;
        hgbat_func(x,&fit[i],nx,&Os[i*nx],&Mr[i*nx*nx],1,r_flag);
        cf_cal(x, f, nx, Os, delta,bias,fit,cf_num);
    }

    /* Composition Function 3 */
    void cf03 (double *x, double *f, int nx, double *Os,double *Mr,int r_flag) {

        int i,cf_num=3;
        double fit[3];
        double delta[3] = {10,30,50};
        double bias[3] = {0, 100, 200};
        i=0;
        schwefel_func(x,&fit[i],nx,&Os[i*nx],&Mr[i*nx*nx],1,r_flag);
        fit[i]=1000*fit[i]/4e+3;
        i=1;
        rastrigin_func(x,&fit[i],nx,&Os[i*nx],&Mr[i*nx*nx],1,r_flag);
        fit[i]=1000*fit[i]/1e+3;
        i=2;
        ellips_func(x,&fit[i],nx,&Os[i*nx],&Mr[i*nx*nx],1,r_flag);
        fit[i]=1000*fit[i]/1e+10;
        cf_cal(x, f, nx, Os, delta,bias,fit,cf_num);
    }

    /* Composition Function 4 */
    void cf04 (double *x, double *f, int nx, double *Os,double *Mr,int r_flag) {

        int i,cf_num=5;
        double fit[5];
        double delta[5] = {10,10,10,10,10};
        double bias[5] = {0, 100, 200, 300, 400};
        i=0;
        schwefel_func(x,&fit[i],nx,&Os[i*nx],&Mr[i*nx*nx],1,r_flag);
        fit[i]=1000*fit[i]/4e+3;
        i=1;
        happycat_func(x,&fit[i],nx,&Os[i*nx],&Mr[i*nx*nx],1,r_flag);
        fit[i]=1000*fit[i]/1e+3;
        i=2;
        ellips_func(x,&fit[i],nx,&Os[i*nx],&Mr[i*nx*nx],1,r_flag);
        fit[i]=1000*fit[i]/1e+10;
        i=3;
        weierstrass_func(x,&fit[i],nx,&Os[i*nx],&Mr[i*nx*nx],1,r_flag);
        fit[i]=1000*fit[i]/400;
        i=4;
        griewank_func(x,&fit[i],nx,&Os[i*nx],&Mr[i*nx*nx],1,r_flag);
        fit[i]=1000*fit[i]/100;
        cf_cal(x, f, nx, Os, delta,bias,fit,cf_num);
    }

    /* Composition Function 4 */
    void cf05 (double *x, double *f, int nx, double *Os,double *Mr,int r_flag) {

        int i,cf_num=5;
        double fit[5];
        double delta[5] = {10,10,10,20,20};
        double bias[5] = {0, 100, 200, 300, 400};
        i=0;
        hgbat_func(x,&fit[i],nx,&Os[i*nx],&Mr[i*nx*nx],1,r_flag);
        fit[i]=10000*fit[i]/1000;
        i=1;
        rastrigin_func(x,&fit[i],nx,&Os[i*nx],&Mr[i*nx*nx],1,r_flag);
        fit[i]=10000*fit[i]/1e+3;
        i=2;
        schwefel_func(x,&fit[i],nx,&Os[i*nx],&Mr[i*nx*nx],1,r_flag);
        fit[i]=10000*fit[i]/4e+3;
        i=3;
        weierstrass_func(x,&fit[i],nx,&Os[i*nx],&Mr[i*nx*nx],1,r_flag);
        fit[i]=10000*fit[i]/400;
        i=4;
        ellips_func(x,&fit[i],nx,&Os[i*nx],&Mr[i*nx*nx],1,r_flag);
        fit[i]=10000*fit[i]/1e+10;
        cf_cal(x, f, nx, Os, delta,bias,fit,cf_num);
    }

    /* Composition Function 6 */
    void cf06 (double *x, double *f, int nx, double *Os,double *Mr,int r_flag) {
        int i,cf_num=5;
        double fit[5];
        double delta[5] = {10,20,30,40,50};
        double bias[5] = {0, 100, 200, 300, 400};
        i=0;
        grie_rosen_func(x,&fit[i],nx,&Os[i*nx],&Mr[i*nx*nx],1,r_flag);
        fit[i]=10000*fit[i]/4e+3;
        i=1;
        happycat_func(x,&fit[i],nx,&Os[i*nx],&Mr[i*nx*nx],1,r_flag);
        fit[i]=10000*fit[i]/1e+3;
        i=2;
        schwefel_func(x,&fit[i],nx,&Os[i*nx],&Mr[i*nx*nx],1,r_flag);
        fit[i]=10000*fit[i]/4e+3;
        i=3;
        escaffer6_func(x,&fit[i],nx,&Os[i*nx],&Mr[i*nx*nx],1,r_flag);
        fit[i]=10000*fit[i]/2e+7;
        i=4;
        ellips_func(x,&fit[i],nx,&Os[i*nx],&Mr[i*nx*nx],1,r_flag);
        fit[i]=10000*fit[i]/1e+10;
        cf_cal(x, f, nx, Os, delta,bias,fit,cf_num);
    }

    /* Composition Function 7 */
    void cf07 (double *x, double *f, int nx, double *Os,double *Mr,int *SS,int r_flag) {

        int i,cf_num=3;
        double fit[3];
        double delta[3] = {10,30,50};
        double bias[3] = {0, 100, 200};
        i=0;
        hf01(x,&fit[i],nx,&Os[i*nx],&Mr[i*nx*nx],&SS[i*nx],1,r_flag);
        i=1;
        hf02(x,&fit[i],nx,&Os[i*nx],&Mr[i*nx*nx],&SS[i*nx],1,r_flag);
        i=2;
        hf03(x,&fit[i],nx,&Os[i*nx],&Mr[i*nx*nx],&SS[i*nx],1,r_flag);
        cf_cal(x, f, nx, Os, delta,bias,fit,cf_num);
    }

    /* Composition Function 8 */
    void cf08 (double *x, double *f, int nx, double *Os,double *Mr,int *SS,int r_flag) {

        int i,cf_num=3;
        double fit[3];
        double delta[3] = {10,30,50};
        double bias[3] = {0, 100, 200};
        i=0;
        hf04(x,&fit[i],nx,&Os[i*nx],&Mr[i*nx*nx],&SS[i*nx],1,r_flag);
        i=1;
        hf05(x,&fit[i],nx,&Os[i*nx],&Mr[i*nx*nx],&SS[i*nx],1,r_flag);
        i=2;
        hf06(x,&fit[i],nx,&Os[i*nx],&Mr[i*nx*nx],&SS[i*nx],1,r_flag);
        cf_cal(x, f, nx, Os, delta,bias,fit,cf_num);
    }


    void shiftfunc (double *x, double *xshift, int nx,double *Os) {

        int i;
        for (i=0; i<nx; i++) {
            xshift[i]=x[i]-Os[i];
        }
    }

    void rotatefunc (double *x, double *xrot, int nx,double *Mr) {

        int i,j;
        for (i=0; i<nx; i++) {
            xrot[i]=0;

            for (j=0; j<nx; j++) {
                xrot[i]=xrot[i]+x[j]*Mr[i*nx+j];
            }
        }
    }

    /* shift and rotate */
    void sr_func(double *x, double *sr_x, int nx, double *Os,double *Mr, double sh_rate, int s_flag,int r_flag) {

        int i;
        if (s_flag==1) {
            if (r_flag==1) {
                shiftfunc(x, y.data(), nx, Os);

                //shrink to the original search range
                for (i=0; i<nx; i++) {
                    y[i]=y[i]*sh_rate;
                }
                rotatefunc(y.data(), sr_x, nx, Mr);
            }
            else {
                shiftfunc(x, sr_x, nx, Os);

                //shrink to the original search range
                for (i=0; i<nx; i++) {
                    sr_x[i]=sr_x[i]*sh_rate;
                }
            }
        }
        else {
            if (r_flag==1) {
                //shrink to the original search range
                for (i=0; i<nx; i++) {
                    y[i]=x[i]*sh_rate;
                }
                rotatefunc(y.data(), sr_x, nx, Mr);
            }
            else {
                //shrink to the original search range
                for (i=0; i<nx; i++) {
                    sr_x[i]=x[i]*sh_rate;
                }
            }
        }
    }

    void asyfunc (double *x, double *xasy, int nx, double beta) {

        int i;
        for (i=0; i<nx; i++) {
            if (x[i]>0) {
                xasy[i]=pow(x[i],1.0+beta*i/(nx-1)*pow(x[i],0.5));
            }
        }
    }

    void oszfunc (double *x, double *xosz, int nx) {

        int i,sx;
        double c1,c2,xx;
        for (i=0; i<nx; i++) {
            if (i==0||i==nx-1) {
                if (x[i]!=0) {
                    xx=log(fabs(x[i]));
                }
                if (x[i]>0) {
                    c1=10;
                    c2=7.9;
                }
                else {
                    c1=5.5;
                    c2=3.1;
                }
                if (x[i]>0) {
                    sx=1;
                }
                else if (x[i]==0) {
                    sx=0;
                }
                else {
                    sx=-1;
                }
                xosz[i]=sx*exp(xx+0.049*(sin(c1*xx)+sin(c2*xx)));
            }
            else {
                xosz[i]=x[i];
            }
        }
    }


    void cf_cal(double *x, double *f, int nx, double *Os,double * delta,double * bias,double * fit, int cf_num) {

        int i,j;
        double *w;
        double w_max=0,w_sum=0;
        w=(double *)malloc(cf_num * sizeof(double));
        for (i=0; i<cf_num; i++)
        {
            fit[i]+=bias[i];
            w[i]=0;
            for (j=0; j<nx; j++)
            {
                w[i]+=pow(x[j]-Os[i*nx+j],2.0);
            }
            if (w[i]!=0)
                w[i]=pow(1.0/w[i],0.5)*exp(-w[i]/2.0/nx/pow(delta[i],2.0));
            else
                w[i]=INF;
            if (w[i]>w_max)
                w_max=w[i];
        }

        for (i=0; i<cf_num; i++)
        {
            w_sum=w_sum+w[i];
        }
        if(w_max==0)
        {
            for (i=0; i<cf_num; i++)
                w[i]=1;
            w_sum=cf_num;
        }
        f[0] = 0.0;
        for (i=0; i<cf_num; i++)
        {
            f[0]=f[0]+w[i]/w_sum*fit[i];
        }
        free(w);
    }

};
