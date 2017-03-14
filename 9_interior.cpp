#include<iostream>
#include<iomanip>

#define MAX 20
#define FORN(i,n) for(int i=0;i<n;i++)

using namespace std;


int copy(double a[][MAX], double b[][MAX]){
    FORN(i,MAX)
        FORN(j,MAX)
            a[i][j] = b[i][j];
}

void initialize_unity(double b[][MAX]){
    FORN(i,MAX)
        FORN(j,MAX)
        if (i==j) b[i][i] = 1.0;
        else b[i][j] = 0;
}
void initialize_null(double b[][MAX]){
    FORN(i,MAX)
        FORN(j, MAX)
            b[i][j] = 0;
}
void initialize_null(double b[]){
    FORN(i,MAX)
            b[i] = 0;
}

void print_matrix(double b[][MAX], int n, int m=0){
    if (m==0) m=n;

    FORN(i, n){
        FORN(j, m)
            cout<<setw(7)<<setprecision(3)<<std::setfill(' ')<<b[i][j];
        cout<<endl;
    }
}

void print_vector(double b[MAX], int n){
    cout<<"[ ";
    FORN(i, n){
        cout<<setw(7)<<setprecision(3)<<std::setfill(' ')<<b[i]<<" ";
    }
    cout<<"] "<<endl;
}


int calculate(double b1[][MAX], int n){
    double eita[MAX], e[MAX];
    
    double b[MAX][MAX]={0};
    double bi[MAX][MAX];

    initialize_unity(b);
    copy(bi,b);


    FORN(r,n){

        double E[MAX][MAX] = {0};
        double btemp[MAX][MAX];

        // Calculating e
        FORN(j,n){
            e[j] = 0;
            FORN(k,n){
                e[j] += bi[j][k]*b1[k][r];
            }
        }

        // Calculating eita
        FORN(j,n){
            if (j==r)
                eita[j] = 1/e[r];
            else
                eita[j] = -e[j]/e[r];
        }


        initialize_unity(E);

        FORN(i,n){
            b[i][r] = b1[i][r];
            E[i][r] = eita[i];
        }

        FORN(i,n)
            FORN(j,n){
                btemp[i][j] = 0;
                FORN(k,n){
                    btemp[i][j] += E[i][k] * bi[k][j];
                }
            }

        // cout<<"e:    ";
        // print_vector(e,n);
        // cout<<"eita: ";
        // print_vector(eita,n);
        // cout<<"E: "<<endl;
        // print_matrix(E,n);
        // cout<<"Binv: "<<endl;
        // print_matrix(btemp,n);

        copy(bi, btemp);

    }
    copy(b1,bi);
}



int main(){
    
    cout.setf(ios::fixed, ios::floatfield);
    cout.setf(ios::showpoint);

    double alpha = 0.5;

    int m,n,N;
    m = 3;
    n = 2;
    N = m + n;

    double c[MAX] = {1, 3, 0, 0, 0, 0}, ct[MAX], cp[MAX];
    double A[MAX][MAX] = {{1, 1, 1,0,0,},{1,2,0,1,0,0},{1,4,0,0,1,0}}, At[MAX][MAX];
    double d[MAX][MAX], di[MAX][MAX];
    double bt[MAX];
    double b[MAX]= {10,11,16};

    double p[MAX][MAX] = {0};
    initialize_unity(p);

    double temp1[MAX][MAX] = {0.0}, temp2[MAX][MAX] = {0.0};


    double x[MAX] = {0.0005,0.0005,9.999,10.9985,15.9975}, xt[MAX]; // Trial Solution 


    cout<<"Initial X  = ";
    print_vector(x, N);
    cout<<"\n";


    FORN(q,23){





    // Generate D
        FORN(i, N)
            FORN(j, N)
                if (i==j)
                    d[i][j] = x[i];
                else
                    d[i][j] = 0;

    // D inverse
        FORN(i, N)
            FORN(j, N)
                if (i==j)
                    di[i][j] = 1.0/x[i];
                else
                    di[i][j] = 0;

    // Finding xt
        initialize_null(xt);
        FORN(i, N)
            xt[i] = di[i][i] * x[i];

    // Calculating At
        initialize_null(At);
        FORN(i,m)
            FORN(j,N)
            At[i][j] = A[i][j] * d[j][j];

    // Calculating ct
        initialize_null(ct);
        FORN(i, N)
            ct[i] = d[i][i] * c[i];

    // Finding projection matrix
        // Finding At * At'
        FORN(i,m)
            FORN(j, m){
                temp1[i][j] = 0.0;
                FORN(k, N)
                    temp1[i][j] += At[i][k] * At[j][k];
            }

        // Finding Inverse
        calculate(temp1,m);
        // print_matrix(temp1,m);

        // Finding Product
        FORN(i,m)
            FORN(j,N){
                temp2[i][j] = 0;
                FORN(k,m)
                    temp2[i][j] += temp1[i][k] * At[k][j];
            }

        FORN(i,N)
            FORN(j,N){
                temp1[i][j] = 0;
                FORN(k,m)
                    temp1[i][j] += At[k][i] * temp2[k][j] ;
            }

        initialize_unity(p);

                
        FORN(i,N)
            FORN(j,N)
                p[i][j] -= temp1[i][j];

    // Finding Cp
        initialize_null(cp);
        FORN(i, N){
            FORN(j,N)
                cp[i] += p[i][j] * ct[j];
        }
        
        double v=0;
        FORN(i,N){
            if ((cp[i]<0) && (v>cp[i]))
                v = cp[i];
        }
        v = -v;

        FORN(i,N)
            xt[i] = xt[i] + (alpha/v)*cp[i];

        initialize_null(x);
        FORN(i,N)
            FORN(j, N)
                x[i] += d[i][j] * xt[j];

        // print_matrix(p,N,N);

        cout<<"Iteration #"<<q+1<<endl;
        // cout<<"Value of P"<<endl;
        // print_matrix(p,N,N);

        // cout<<"X~ = ";
        // print_vector(xt, N);

        cout<<"X  = ";
        print_vector(x, N);

        cout<<"\n";



    }


}