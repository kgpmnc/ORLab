#include<iostream>
#include<vector>
#include<algorithm>
#include<iomanip>

#define FORV(i, vector) for(int i=0;i<(vector).size();i++)
#define FORN(i, n) for(int i=0;i<n;i++)
#define FORAB(i, a, b) for(int i=a;i<b;i++)


using namespace std;


typedef vector<float> Vec;
typedef vector< vector<float> > Mat;

Vec optimize;
bool isMaxProblem;
Mat values_index;

using namespace std;


Vec reverseVec(Vec &m){
    FORN(i, (m.size()/2)){
        int i2 = m.size()-i-1;
        float t = m[i];
        m[i] = m[i2];
        m[i2] = t;
    }

    return m;
}
void swapReversedRow(Mat &m, int i1, int i2){
    Vec r = reverseVec(m[i2]);
    m[i2] = reverseVec(m[i1]);
    m[i1] = r;
}

void swapRow(Mat &m, int i1, int i2){
    Vec r = m[i2];
    m[i2] = m[i1];
    m[i1] = r;
}

void addCol(Mat &m, int i, Vec col){
    FORV(p, m){
        m[p].insert(m[p].begin()+i, col[i]);
    }
}

bool isNegativeElement(Vec &v){

    FORV(i,v){
        if (v[i]<0)
            return true;
    }
    return false;
}

void deleteCol(Mat &m, int i){
    FORV(p, m){
        m[p].erase(m[p].begin()+i);
    }
}


void printMat(Mat mat){


    int numRows = mat.size();
    int numCols = mat[0].size();


	int i,j;

	for (i=0; i<numRows; i++){

		for (j=0; j<numCols; j++){

			cout<<setw(4)<<setprecision(2)<<mat[i][j]<<" ";
		}

		cout<<endl;
	}
}

bool isRowZero(Vec &row){

    bool zero = true;
    FORV(i, row){

        if (row[i]!=0) {
            zero = false;
            break;
        }
    }
    return zero;
}

int reduceMatrix(Mat &mat, Vec *v = NULL){

    int numRows = mat.size();
    int numCols = mat[0].size();

    if (v!=NULL){
        FORV(j, mat)
            mat[j].push_back((*v)[j]);
        numCols++;
    }

	int i,j, iter;

	if (mat[0][0] == 0) swapRow(mat, 0, 1);
	for (iter =0; iter<numRows-1; iter++){
		for (i=iter+1; i<numRows; i++){
			float fac = mat[i][iter] / mat[iter][iter];
			for (j = 0; j<numCols; j++){
				mat[i][j] = mat[i][j] - mat[iter][j] * fac;
			}
		}

	}

    int rank = 0;
    FORV(i, mat){
        if (!isRowZero(mat[i])) rank++;
    }


	if (v!=NULL){
        FORV(i, *v){
            (*v)[i] = mat[i][numCols-1];
        }

        deleteCol(mat, numCols-1);
    }


    return rank;

}



void reverseMat(Mat &m){

    int numRows = m.size();
    FORN(i, (numRows/2)){
        swapReversedRow(m,i,m.size()-i-1);
    }

    if (numRows%2 == 1) m[numRows/2] = reverseVec(m[numRows/2]);
}



void printVec(Vec &v){

    FORV(i, v){
        cout<<fixed<<v[i]<<" ";
    }
    cout<<endl<<endl;
}

Vec getResults(Vec &pattern, Mat &m, Vec &v){

    Vec results(pattern.size() + v.size(), 0);

    int lastRes=0;

    FORV(i, results){
        bool zero = false;
        FORV(j, pattern){
            if (i==pattern[j]) {
                zero = true;
                break;
            }
        }
        if (!zero) {
            results[i] = (v[lastRes]/m[lastRes][lastRes]);
            lastRes++;
        }
    }
    return results;
}

void computeCombination(Vec pattern, Mat &m, Vec &v){

    static int count = 0;

    Mat coeff(m);
    Vec rhs(v);

    FORV(i, pattern){
        deleteCol(coeff, pattern[i]-i);
    }

    reduceMatrix(coeff, &rhs);

    reverseMat(coeff);
    reverseVec(rhs);

    reduceMatrix(coeff, &rhs);

    reverseMat(coeff);
    reverseVec(rhs);

    Vec results = getResults(pattern, coeff, rhs);

    bool isNeg = isNegativeElement(results);
    float sum = 0;

    cout << "combination no " <<setw(2)<<setfill(' ')<< (++count)<<"  ";
    FORV(i, results){
        //cout<<"x"<<i+1<<" = "<<fixed<<setw(7)<<setprecision(3)<<std::setfill('0')<<results[i]<<"  ";
        cout<<" | "<<fixed<<setw(7)<<setprecision(3)<<std::setfill(' ')<<results[i]<<"  ";
        sum = sum + optimize[i] * results[i];
    }

    if ((!isNeg) && (sum==sum)){
        Vec save;
        save.push_back(count);
        save.push_back(sum);
        values_index.push_back(save);
    }

    cout<<" |   "<<sum;
    if (isNeg) cout<<" X ";
    cout<<endl;

}

void processCombination(int n, int r,Mat &m, Vec &v, int offset = 0){
    static Vec pattern;
    if (r==0){
        computeCombination(pattern, m, v);
        return;
    }

    for (int i=offset; i<=n-r; i++){

        pattern.push_back(i);
        processCombination(n,r-1, m,v,i+1);
        pattern.pop_back();
    }
}


void addSlackVariable(Mat &m, int index){

    FORV(i, m){
        if (i==index)
            m[i].push_back(1);
        else
            m[i].push_back(0);
    }

}

void addSurplusVariable(Mat &m, int index){

    FORV(i, m){
        if (i==index)
            m[i].push_back(-1);
        else
            m[i].push_back(0);
    }

}

void addArtificialVariable(Mat &m, int index){

    FORV(i, m){
        if (i==index)
            m[i].push_back(1);
        else
            m[i].push_back(0);
    }
}

bool compare_values(Vec a, Vec b){
    if (isMaxProblem)
        return (a[1]>b[1]);
    else
        return (a[1]<b[1]);
}

int main(){

	int numOfEqn, numOfVar;

    cout<<"Is maximization problem? ";
    cin>>isMaxProblem;

	cout<<"Enter number of equations :";
	cin>>numOfEqn;

	cout<<"Enter number of variables :";
	cin>>numOfVar;


    cout<<"Enter coeff of constraints\n";

    Mat m(numOfEqn, Vec(numOfVar+1,0));

    FORN(i, numOfEqn)
        FORN(j, numOfVar+1)
            cin>>m[i][j];

    optimize.resize(numOfVar, 0);

    cout<<"Enter coefficient of optimization function: \n";
    FORV(i, optimize){
        cin>>optimize[i];
    }

    Mat coeff(m);
    Vec rhs(numOfEqn, 0);


    FORV(i, m){
        rhs[i] = m[i][numOfVar];
    }


    deleteCol(coeff, numOfVar);



    int slack, surplus, arti;

    cout<<"Enter equation number to add slack variable (-1 to exit this)";

    cin>>slack;
    while (slack != -1){
        addSlackVariable(coeff, slack);
        optimize.push_back(0);
        numOfVar++;
        cin>>slack;
    }

    cout<<"Enter equation number to add surplus variable (-1 to exit this)";

    cin>>surplus;
    while (surplus != -1){
        addSurplusVariable(coeff, surplus);
        optimize.push_back(0);
        numOfVar++;
        cin>>surplus;
    }

    cout<<"Enter equation number to add artificial variable (-1 to exit this)";

    cin>>arti;
    while (arti != -1){
        addArtificialVariable(coeff, arti);

        // Adding penality for artificial variable
        if (isMaxProblem)
            optimize.push_back(float(-1000000));
        else
            optimize.push_back(float(1000000));

        numOfVar++;
        cin>>arti;
    }

    cout<<"Final matrix for compuations :-\n\n";
    printMat(coeff);
    cout<<endl;

    // Defining spaces for computations


    Mat coeff_copy(coeff);
    Vec rhs_copy(rhs);

    int rA = reduceMatrix(coeff_copy);

    coeff_copy = coeff;
    int rAb = reduceMatrix(coeff_copy, &rhs_copy);

    cout<< "Rank of A   "<<rA<<endl;
    cout<< "Rank of A|b "<<rAb<<endl;

    if (rA != rAb){
    // TODO Do this for infinite elements
        cout<<"No Solutions exists\n";
        return 0;
    }

    while (rA<numOfEqn){

        coeff.pop_back();
        rhs.pop_back();

        numOfEqn--;

        // Copy the elements
        coeff_copy = coeff;

        // Remember only give linearly dependent rows at the end.
        rA = reduceMatrix(coeff_copy);

    }

    ///////////////////////////////////////////////////////////////////////////
    cout<<"                   ";
    FORN(i, numOfVar)
        cout<<" | "<<"   x"<<i+1<<"    ";
    cout<<" |    Z"<<endl;

    processCombination(numOfVar,numOfVar - numOfEqn, coeff, rhs);


    std::sort(values_index.begin(), values_index.end(), compare_values);

    if (values_index.size()>1){

        if (values_index[0][1]==values_index[1][1]){
            cout<<"Infinite number of solutions\n";
            cout<<"Combination numbers "<<values_index[0][0]<<" "<<values_index[1][0]<<endl;
        }
        else {
            cout<<"Solution "<<values_index[0][0]<<"   "<<values_index[0][1]<<endl;
        }
    }

}
