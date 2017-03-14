#include<iostream>
#include<iomanip>
#include<cmath>

#define FORN(i,n) for(int i=0;i<n;i++)
#define MAX 10
#define m 2 // Player A
#define n 2 // Player B

float pay[MAX][MAX] = {{-5, 10},{20, -5}};
float mat[MAX][MAX];
float temp[MAX][MAX];
int eqn=0, var=0;
bool infinite, unbounded;

using namespace std;

void printGame(){
    cout<<"Game "<<endl;
     for(int i = 0 ; i <m ; i++){
          for(int j = 0 ; j < n; j++){
               cout << fixed<<setw(7)<<setprecision(3)<<std::setfill(' ') <<pay[i][j] << " ";
          }
          cout << endl;
     }
     cout<<endl;
}

void copyFromTemp(){
     for(int i = 0 ; i <=eqn ; i++){
          for(int j = 0 ; j <= var; j++){
               mat[i][j] = temp[i][j];
          }
     }
}

void printTableau(){
    static int no=0;
    cout<<"Tableau no."<<(++no)<<endl;
     for(int i = 0 ; i <=eqn ; i++){
          for(int j = 0 ; j <= var; j++){
               cout << fixed<<setw(7)<<setprecision(3)<<std::setfill(' ') <<mat[i][j] << " ";
          }
          cout << endl;
     }
     cout<<endl;
}

int getPivotRow(int pivotCol){
     int ind = -1;
     float currMin = 100000;
     for(int i = 0 ; i < eqn; i++){
          if(mat[i][pivotCol] <= 0)
               continue;
          if((mat[i][var]/mat[i][pivotCol]) < currMin){
               ind = i;
               currMin = (mat[i][var]/mat[i][pivotCol]);
          }
     }
     return ind;
}

int getMinFromLastRow(){
    int ind = -1;
    float currMin = 0;
    for(int i = 0 ; i < var ; i++){
          if(mat[eqn][i] > 0)
               continue;
          if(mat[eqn][i] == 0){
               infinite = true;
               return -1;
          }
          if(mat[eqn][i] < currMin){
               currMin = mat[eqn][i];
               ind = i;
         }
    }
    return ind;
}
int getPivotCol(int pivotRow){
     int ind = -1;
     float currMin = 100000;
     for(int i = 0 ; i < var; i++){
          if(mat[pivotRow][i] >= 0)
               continue;
          if(abs(mat[eqn][i]/mat[pivotRow][i]) < currMin){
               ind = i;
               currMin = abs(mat[eqn][i]/mat[pivotRow][i]);
          }
     }
     return ind;

}

int getMinFromLastCol(){
    int ind = -1;
    float currMin = 0;
    for(int i = 0 ; i < eqn ; i++){
          if(mat[i][var] > 0)
               continue;
          if(mat[i][var] == 0){
               infinite = true;
               return -1;
          }
          if(mat[i][var] < currMin){
               currMin = mat[i][var];
               ind = i;
         }
    }
    return ind;
}

void compute(){

     int pivotRow, pivotCol;
     while((pivotCol = getMinFromLastRow()) != -1){
          printTableau();
          if((pivotRow = getPivotRow(pivotCol)) == -1){
               unbounded = true;
               return;
          }
          for(int i = 0 ; i <= eqn ; i++){
               for(int j = 0 ; j <= var; j++){
                    if(i == pivotRow && j == pivotCol)
                         temp[i][j] = 1/mat[i][j];
                    else if(i == pivotRow)
                         temp[i][j] = (mat[i][j])/mat[pivotRow][pivotCol];
                    else if(j == pivotCol)
                         temp[i][j] = ((-1)*mat[i][j])/mat[pivotRow][pivotCol];
                    else{
                        temp[i][j] = ((mat[pivotRow][pivotCol]*mat[i][j]) - (mat[pivotRow][j]*mat[i][pivotCol]))/mat[pivotRow][pivotCol];
                    }
               }
          }
          copyFromTemp();
     }
     printTableau();

}
void compute_dual(){

     int pivotRow, pivotCol;
     while((pivotRow = getMinFromLastCol()) != -1){
          printTableau();
          if((pivotCol = getPivotCol(pivotRow)) == -1){
               unbounded = true;
               return;
          }
          for(int i = 0 ; i <= eqn ; i++){
               for(int j = 0 ; j <= var; j++){
                    if(i == pivotRow && j == pivotCol)
                         temp[i][j] = 1/mat[i][j];
                    else if(i == pivotRow)
                         temp[i][j] = (mat[i][j])/mat[pivotRow][pivotCol];
                    else if(j == pivotCol)
                         temp[i][j] = ((-1)*mat[i][j])/mat[pivotRow][pivotCol];
                    else{
                        temp[i][j] = ((mat[pivotRow][pivotCol]*mat[i][j]) - (mat[pivotRow][j]*mat[i][pivotCol]))/mat[pivotRow][pivotCol];
                    }
               }
          }
          copyFromTemp();
     }
     printTableau();

}

bool check_unstable(){
    float m1,m2;

    // Finding whether game is unstable or not
    float max = -10000000;
    FORN(i, m){
        float min=1000000;
        FORN(j, n)
            if (min>pay[i][j])
                min = pay[i][j];
        if (max<min) max = min;
    }

    m1 = max;

    float min = 10000000;
    FORN(j, n){
        float max=-1000000;
        FORN(i, m)
            if (max<pay[i][j])
                max = pay[i][j];
        if (max<min) min = max;
    }

    m2 = min;


    if (m2!=m1){
        cout<<"Unstable Game"<<endl;
        return true;
    }
    else
        return false;
}


int main(){

    printGame();
    if (!check_unstable())
        return 0;

// Solving for Player B using Primal Simplex Method
    var = n;
    eqn = m;

    FORN(i, m){
      FORN(j, n)
        mat[i][j] = pay[i][j];
      mat[i][n] = 1;
    }
    FORN(j, n)
      mat[m][j] = -1;

    mat[m][n] = 0;

    cout<<"Solving by Primal Simplex Method"<<endl;
    compute();
    cout<<"Game Value :"<<(1/mat[m][n])<<endl<<endl;

// Solving for Player A using Dual Simplex Method
    var = m;
    eqn = n;

    FORN(i, n){
      FORN(j, m)
        mat[i][j] = -pay[j][i];
      mat[i][m] = -1;
    }
    FORN(j, m)
      mat[n][j] = -1;

    mat[n][m] = 0;
    cout<<"Solving by Dual Simplex Method"<<endl;
    compute_dual();
    cout<<"Game Value :"<<(1/mat[n][m])<<endl<<endl;

    return 0;
}

/*

Output:

Game 
 -5.000  10.000 
 20.000  -5.000 

Unstable Game
Solving by Primal Simplex Method
Tableau no.1
 -5.000  10.000   1.000 
 20.000  -5.000   1.000 
 -1.000  -1.000   0.000 

Tableau no.2
  0.250   8.750   1.250 
  0.050  -0.250   0.050 
  0.050  -1.250   0.050 

Tableau no.3
  0.029   0.114   0.143 
  0.057   0.029   0.086 
  0.086   0.143   0.229 

Game Value :4.375

Solving by Dual Simplex Method
Tableau no.4
  5.000 -20.000  -1.000 
-10.000   5.000  -1.000 
 -1.000  -1.000   0.000 

Tableau no.5
 -0.250  -0.050   0.050 
 -8.750   0.250  -1.250 
 -1.250  -0.050   0.050 

Tableau no.6
 -0.029  -0.057   0.086 
 -0.114  -0.029   0.143 
 -0.143  -0.086   0.229 

Game Value :4.375


*/