#include<iostream>
#include<iomanip>

#define MAX 5
#define FORN(i,n) for(int i=0;i<n;i++)

using namespace std;


int copy(float a[][MAX], float b[][MAX]){
    FORN(i,MAX)
        FORN(j,MAX)
            a[i][j] = b[i][j];
}

void initialize_unity(float b[][MAX]){
    FORN(i,MAX)
        b[i][i] = 1.0;
}

void print_matrix(float b[][MAX], int n){
    FORN(i, n){
        FORN(j, n)
            cout<<setw(5)<<setprecision(3)<<std::setfill(' ')<<b[i][j];
        cout<<endl;
    }
}

void print_vector(float b[MAX], int n){
    cout<<"[ ";
    FORN(i, n){
        cout<<setw(7)<<setprecision(3)<<std::setfill(' ')<<b[i]<<" ";
    }
    cout<<"] "<<endl;
}



int calculate(float b[][MAX], float bi[][MAX], float b1[][MAX], int n){
    float eita[MAX], e[MAX];


    FORN(r,n){

        float E[MAX][MAX] = {0};
        float btemp[MAX][MAX];

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

        cout<<"e:    ";
        print_vector(e,n);
        cout<<"eita: ";
        print_vector(eita,n);
        cout<<"E: "<<endl;
        print_matrix(E,n);
        cout<<"Binv: "<<endl;
        print_matrix(btemp,n);

        cout<<"\n\n\n";


        copy(bi, btemp);

    }


}

int main(){

    float b[MAX][MAX]={0};
    float bi[MAX][MAX];
    float b1[MAX][MAX] = {{1, 1, 0},{1,2,0},{1,4,1}};

    int n = 3;

/*
    cin>>n;

    FORN(i,n)
        FORN(j,n)
            cin>>b1[i][j];
*/
    initialize_unity(b);
    copy(bi,b);

    calculate(b,bi,b1,n);

    return 0;
}
