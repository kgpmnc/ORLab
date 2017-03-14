
#include <iostream>
#include <stdio.h>
#include <math.h>
#include <string>
#include <iomanip>

using namespace std;

float d[20]={0};
float ans[20][20], z[20];
float mat[20][20], b[20], temp[20][20], constants[20];
int R, C;
int rank1, rank2, i, j, k, in_var, var, eqn;
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

int getPivotCol(int pivotRow){
     int ind = -1;
     float currMin = 100000;
     for(int i = 0 ; i < var; i++){
          if(mat[pivotRow][i] <= 0)
               continue;
          if((mat[pivotRow][var]/mat[pivotRow][i]) < currMin){
               ind = i;
               currMin = (mat[pivotRow][var]/mat[pivotRow][i]);
          }
     }
     return ind;
}

int getMinFromLastCol(){
    int ind = -1;
    float currMin = 0;
    for(int i = 0 ; i < eqn ; i++){
          cout<<mat[i][var];
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
     while((pivotCol = getMinFromLastCol()) != -1){
          printTableau();
          cout<<pivotCol;
          if((pivotRow = getPivotCol(pivotCol)) == -1){
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
     if(infinite)
          printf("Infinite Solutions\nANSWER %f \n", mat[eqn][var]);
     else if(unbounded)
          printf("Unbounded\n");
     else
          printf("ANSWER %f \n", mat[eqn][var]);
     return 0;
}

