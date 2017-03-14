#include <iostream>
#include <stdio.h>
#include <math.h>
#include <string>
#include <iomanip>
#include <cmath>

using namespace std;

float d[20]={0};
float ans[20][20], z[20];
float mat[20][20] = {{2, -1, 5,0,5}, {2,3,12,1,13},{-18,-12,0,0,0}};
float b[20], temp[20][20], constants[20];
int R, C;
int rank1, rank2, i, j, k, in_var, var=2, eqn=2;
int countVal=0,countVal1=0, slackVar = 0;
bool unbounded = false;
bool infinite = false;

void input(){
    string inequality;
    printf("Enter no of variables\n");
    scanf("%d",&var);

    printf("Enter no. of equations\n");
    scanf("%d",&eqn);

    for(i = 0 ; i < eqn ; i++)
    {
        printf("Input equations #%d Format: ** coeff [<|>] rhs ** \n" , i + 1);
        for(j = 0 ; j < var ; j++)
        {
            scanf("%f",&mat[i][j]);
        }
        cin >> inequality;
        if (inequality.at(0) == '<')
        {
             slackVar++;
             scanf("%f", &mat[i][j]);
        }
        else if (inequality.at(0) == '>')
        {
         // Can be extended for Big-M Method
        }
    }
    printf("Enter objective function and d\n");
    for(int j = 0; j <= var ; j++){
          scanf("%f", &mat[eqn][j]);
          if(j != var)
               mat[eqn][j] = (-1)*mat[eqn][j];
    }
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
     for(int i = 0 ; i <= eqn ; i++){
          for(int j = 0 ; j <= var; j++){
               cout << mat[i][j] << " ";
           }
           cout << endl;
     }
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
int main(){
     input();
     compute();
        float frac_max = 0, int_part=-1;
        int frac_index = -1;
     for (int i=0;i<eqn;i++)
     if (frac_max < modf(mat[i][var], &int_part)){
        frac_max = modf(mat[i][var], &int_part);
        frac_index = i;
     };

    for (int i=0; i<=var; i++){
        mat[eqn+1][i] = modf(mat[frac_index][i], &int_part);
        if (mat[eqn+1][i]<0) mat[eqn+1][i] = -1*(1+mat[eqn+1][i]);
        else mat[eqn+1][i] = -1*mat[eqn+1][i];
        //cout<<mat[eqn+1][i]<<" ";
    }
    eqn++;
    int r1 = eqn;
    int r2 = eqn-1;
    for (i=0; i<=var; ++i){
      float temp;
      temp = mat[r2][i];
      mat[r2][i] = mat[r1][i];
      mat[r1][i] = temp;
    }

    cout<<"Dual Simple Tabaleu:"<<endl<<endl;;
    compute_dual();
    //cout<< frac_max<<frac_index;
     if(infinite)
          printf("Infinite Solutions\nANSWER %f \n", mat[eqn][var]);
     else if(unbounded)
          printf("Unbounded\n");
     else
          printf("ANSWER %f \n", mat[eqn][var]);
     return 0;
}

/*

Example:
Enter no of variables
2
Enter no. of equations
2
Input equations #1 Format: ** coeff [<|>] rhs **
2 -1 < 5
Input equations #2 Format: ** coeff [<|>] rhs **
2 3 < 13
Enter objective function and d
18 12 0

2 -1 5
2 3 12
-18 -12 0
Tableau no.1
  2.000  -1.000   5.000
  2.000   3.000  12.000
-18.000 -12.000   0.000

Tableau no.2
  0.500  -0.500   2.500
 -1.000   4.000   7.000
  9.000 -21.000  45.000

Tableau no.3
  0.375   0.125   3.375
 -0.250   0.250   1.750
  3.750   5.250  81.750

Dual Simple Tabaleu:

Tableau no.4
  0.375   0.125   3.375
 -0.250   0.250   1.750
 -0.750  -0.250  -0.750
  3.750   5.250  81.750

Tableau no.5
  0.500  -0.000   3.000
 -0.333   0.333   2.000
 -1.333   0.333   1.000
  5.000   4.000  78.000

ANSWER 78.000000


*/
