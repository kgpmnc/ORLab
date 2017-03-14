
//Program for Big-M method of calculation in C
#include <stdio.h>
int bk=1,g,l;
void max();
void min();
int main()
{
    int i;
    printf("nNOTE: SLACK VARIABLES HELP IN FORMING THE UNIT MAQTRIX. BUT
    OBJECTIVEn");
    printf("FUNCTIONS WITH <= WILL PROVIDE SLACK VARIABLES BUT THOSE
    WITH >= WILL NOT.n");
    printf("HENCE TO MAKE A UNIT MATRIX, WE MUST TAKE HELP OF ARTIFICIAL
    VARIABLES. THISn");
    printf("INCLUDE ANOTHER COEFFICIENT CALLED BIG-M. “);
    printf("n OBJECTIVE? Maximization(1) or Minimization(2) or any other to Exit...:n");
    scanf("%d",&i);
    if (i==1)
    max();
    else if (i==2)
    min();
    else
    return 0;
}
void min()
{
    int i,j,row,col,kc,kr,sub,var,flag=-1,k;
    double sum,max,ba,kn;
    double cj[20],basis[20],c[20],colmat[20],tab[20][20];
    bk=0;
    printf("nnTAKE "M" AS A BIT LARGER THAN ANY LARGEST NUMBER IN THE
    printf("nTOTAL NUMBER OF >= EQUALITIES IN SUBJECTIVE FUNCTIONS:n");
    scanf("%d", &g);
    printf("nTOTAL NUMBER OF <= EQUALITIES IN SUBJECTIVE FUNCTIONS:n");
    scanf("%d", &l);
    printf("nTOTAL NUMBER OF "ARTIFICIAL VARIABLES": %dn",g);
    printf("ENTER THE TOTAL NUMBER OF VARIABLES IN OBJECTIVE
    FUNCTION:n");
    scanf("%d", &var);
    printf("nSTART ENTERING THE COEFFICIENTS OF THE OBJECTIVE FUNCTION:n");
    for(i=0;i<var;i++)
    {
        scanf("%lf", &cj[i]);
    }
    printf("nENTER TOTAL NUMBER OF SUBJECTIVE FUNCTIONS:n");
    scanf("%d", &sub);
    //DECIDING TABLE DIMENSION
    col=var+4;
    row=sub+2;
    for(i=1;i<row-1;i++)
    {
        printf("nENTER ELEMENTS IN %d-th ROW:n",i);
        for(j=4;j<col;j++) {scanf("%lf",&tab[i][j]); }
    }
    printf("nENTER THE BASIS:n");
    for(i=0;i<sub;i++)
    scanf("%lf", &basis[i]);
    printf("nENTER THE VALUES OF "C" FROM OBJECTIVE FUNCTIONn");
    for(i=0;i<sub;i++)
    scanf("%lf", &c[i]);
    printf("nENTER THE COLUMN MATRIX:n");
    for(i=0;i<sub;i++)
    scanf("%lf", &colmat[i]);
    //INITIALIZING THE TABLE
    for(i=1;i<row-1;i++)
    tab[i][0]=i;
    for(i=1;i<row-1;i++)
    {
        tab[i][1]=basis[i-1];
        tab[i][2]=c[i-1];
        tab[i][3]=colmat[i-1];
    }
    printf("nPRINTING THE MATRIX YOU HAVE INSERTED:n");
    printf("---------------------------------------n");
    for(i=1;i<row-1;i++)
    {
        for(j=0;j<col;j++)
        {
            printf("%.3lft",tab[i][j]);
        }
        printf("n");
    }
    printf("---------------------------------------n");
    for(i=4;i<col;i++)
    tab[row-1][i]=0;
    tab[row-1][3]=0;
    //STARTING THE ITERATION
    for(k=0;k<10;k++)
    {
        //INITIALIAZING FLAG
        flag=-1.00;
        //Z0
        for(i=1;i<row-1;i++)
        tab[row-1][3]=tab[row-1][3]+tab[i][3]*tab[i][2];
        //Zj-Cj
        for(i=4;i<col;i++)
        {
            sum=0;
            for(j=1;j<row-1;j++)
            sum=tab[j][i]*tab[j][2]+sum;
            tab[row-1][i]=sum-cj[i-4];
        }
        //FINDING MAXIMUM IN Zj-Cj
        max=tab[row-1][4];
        kc=4;
        for(i=4;i<col;i++)
        {
            if(max<tab[row-1][i])
            {
                max=tab[row-1][i];
                kc=i;
            }
        }
        //FINDING b/a RATIO
        for(j=1;j<row-1;j++)
        {
            if(tab[j][kc]>0)
            {
                ba=(colmat[j-1]/tab[j][kc]);
                goto pop;
            }
        }
        pop:
        kr=j;
        for(;j<row-1;j++)
        {
            if((tab[j][kc]>0) && ((colmat[j-1]/tab[j][kc])<ba))
            kr=j;
        }
        //SWAPPING KEY COLUMN WITH BASIS
        tab[kr][1]=kc-3;
        kn=tab[kr][kc];
        tab[kr][2]=cj[kc-4];
        //DIVIDING OTHER ROWS BY THE FORMULA
        for(i=1;i<row-1;i++)
        {
            if(i==kr)
            continue;
            else
            {
                for(j=3;j<kc;j++)
                tab[i][j]=tab[i][j]-((tab[i][kc]*tab[kr][j])/kn);
            }
        }
        printf("nn");
        for(i=1;i<row-1;i++)
        {
            if(i==kr)
            continue;
            else
            {
                for(j=kc+1;j<col;j++)
                tab[i][j]=tab[i][j]-((tab[i][kc]*tab[kr][j])/kn);
            }
        }
        for(i=1;i<row-1;i++)
        {
            if(i==kr)
            continue;
            else
            tab[i][kc]=tab[i][kc]-((tab[i][kc]*tab[kr][kc])/kn);
        }
        //DIVIDING KEY ROW BY KEY NUMBER
        for(i=3;i<col;i++)
        tab[kr][i]=tab[kr][i]/kn;
        //CHECKING IF Zj-Cj ARE ALL NEGATIVE
        for(i=4;i<col;i++)
        {
            if(tab[row-1][i]>0)
            {
                flag=1;
            }
        }
        //BREAKING THE LOOP
        if(flag==-1)
        goto sos;
    }
    sos:
    printf("nTHE SOLUTION IS...n");
    for(i=1;i<row-1;i++)
    printf("X%d=%lfn",(int)tab[i][1],tab[i][3]);
}
void max()
{
    int i,j,row,col,kc,kr,sub,var,flag=-1,k;
    double sum,max,ba,kn;
    double cj[20],basis[20],c[20],colmat[20],tab[20][20];
    bk=0;
    printf("nnTAKE "M" AS A BIT SMALLER THAN ANY SMALLEST NUMBER IN
    THE EQUATIONS...nnn");
    printf("nTOTAL NUMBER OF >= EQUALITIES IN SUBJECTIVE FUNCTIONS:n");
    scanf("%d", &g);
    printf("nTOTAL NUMBER OF <= EQUALITIES IN SUBJECTIVE FUNCTIONS:n");
    scanf("%d", &l);
    printf("nTOTAL NUMBER OF "ARTIFICIAL VARIABLES": %dn",g);
    printf("ENTER THE TOTAL NUMBER OF VARIABLES IN OBJECTIVE
    FUNCTION:n");
    scanf("%d", &var);
    printf("nSTART ENTERING THE COEFFICIENTS OF THE OBJECTIVE FUNCTION:
    n");
    for(i=0;i<var;i++)
    {
        scanf("%lf", &cj[i]);
    }
    printf("nENTER TOTAL NUMBER OF SUBJECTIVE FUNCTIONS:n");
    scanf("%d", &sub);
    //DECIDING TABLE DIMENSION
    col=var+4;
    row=sub+2;
    for(i=1;i<row-1;i++)
    {
        printf("nENTER ELEMENTS IN %d-th ROW:n",i);
        for(j=4;j<col;j++)
        {
            scanf("%lf",&tab[i][j]);
        }
    }
    printf("nENTER THE BASIS:n");
    for(i=0;i<sub;i++)
    scanf("%lf", &basis[i]);
    printf("nENTER THE VALUES OF "C" FROM OBJECTIVE FUNCTIONn");
    for(i=0;i<sub;i++)
    scanf("%lf", &c[i]);
    printf("nENTER THE COLUMN MATRIX:n");
    for(i=0;i<sub;i++)
    scanf("%lf", &colmat[i]);
    //INITIALIZING THE TABLE
    for(i=1;i<row-1;i++)
    tab[i][0]=i;
    for(i=1;i<row-1;i++)
    {
        tab[i][1]=basis[i-1];
        tab[i][2]=c[i-1];
        tab[i][3]=colmat[i-1];
    }
    printf("nPRINTING THE MATRIX YOU HAVE INSERTED:n");
    printf("---------------------------------------n");
    for(i=1;i<row-1;i++)
    {
        for(j=0;j<col;j++)
        {
            printf("%.3lft",tab[i][j]);
        }
        printf("n");
    }
    printf("---------------------------------------n");
    for(i=4;i<col;i++)
    tab[row-1][i]=0;
    tab[row-1][3]=0;
    //STARTING THE ITERATION
    for(k=0;k<10;k++)
    {
        //INITIALIAZING FLAG
        flag=-1.00;
        //Z0
        for(i=1;i<row-1;i++)
        tab[row-1][3]=tab[row-1][3]+tab[i][3]*tab[i][2];
        //Zj-Cj
        for(i=4;i<col;i++)
        {
            sum=0;
            for(j=1;j<row-1;j++)
            sum=tab[j][i]*tab[j][2]+sum;
            tab[row-1][i]=sum-cj[i-4];
        }
        //FINDING MAXIMUM IN Zj-Cj
        max=tab[row-1][4];
        kc=4;
        for(i=4;i<col;i++)
        {
            if(max<tab[row-1][i])
            {
                max=tab[row-1][i];
                kc=i;
            }
        }
        //FINDING b/a RATIO
        for(j=1;j<row-1;j++)
        {
            if(tab[j][kc]>0)
            {
                ba=(colmat[j-1]/tab[j][kc]);
                goto pop;
            }
        }
        pop:
        kr=j;
        for(;j<row-1;j++)
        {
            if((tab[j][kc]>0) && ((colmat[j-1]/tab[j][kc])<ba))
            kr=j;
        }
        //SWAPPING KEY COLUMN WITH BASIS
        tab[kr][1]=kc-3;
        kn=tab[kr][kc];
        tab[kr][2]=cj[kc-4];
        //DIVIDING OTHER ROWS BY THE FORMULA
        for(i=1;i<row-1;i++)
        {
            if(i==kr)
            continue;
            else
            {
                for(j=3;j<kc;j++)
                tab[i][j]=tab[i][j]-((tab[i][kc]*tab[kr][j])/kn);
            }
        }
        printf("nn");
        for(i=1;i<row-1;i++)
        {
            if(i==kr)
            continue;
            else
            {
                for(j=kc+1;j<col;j++)
                tab[i][j]=tab[i][j]-((tab[i][kc]*tab[kr][j])/kn);
            }
        }
        for(i=1;i<row-1;i++)
        {
            if(i==kr)
            continue;
            else
            tab[i][kc]=tab[i][kc]-((tab[i][kc]*tab[kr][kc])/kn);
        }
        //DIVIDING KEY ROW BY KEY NUMBER
        for(i=3;i<col;i++)
        tab[kr][i]=tab[kr][i]/kn;
        //CHECKING IF Zj-Cj ARE ALL POSITIVE
        for(i=4;i<col;i++)
        {
            if(tab[row-1][i]<0)
            flag=1;
        }
        //BREAKING THE LOOP
        if(flag==-1)
        goto sos;
    }
    sos:
    printf("nTHE SOLUTION IS...n");
    for(i=1;i<row-1;i++)
    printf("X%d=%lfn",(int)tab[i][1],tab[i][3]);
}
