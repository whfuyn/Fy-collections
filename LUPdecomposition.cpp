#include <cstdio>
#include "..\\project\\vector\\vector.hpp"
#include ".\Alice_Lib.hpp"
#include ".\Fraction.hpp"


typedef Fraction value_type;
typedef vector<vector<value_type> > Matrix;


vector<int> LUP_decomposition(Matrix& A){
    int n = (int)A.size();
    vector<int> P;
    for(int i = 0; i < n; i++) P.push_back(i);
    for(int i = 0; i < n; i++){
        value_type maxn = abs(A[i][i]);
        int select = i;
        for(int j = i + 1; j < n; j++){
            if(maxn < abs(A[j][i])){
                select = j;
                maxn = abs(A[j][i]);
            }
        }
        if(!maxn) throw("Singular Matrix.");
        swap(P[i], P[select]);
        swap(A[i], A[select]);
        for(int j = i + 1; j < n; j++){
            A[j][i] = A[j][i] / A[i][i];
            for(int k = i + 1; k < n; k++){
                A[j][k] -= A[j][i] * A[i][k];
            }
        }
    }
    return P;
}

vector<value_type> LUP_solve(const Matrix& LU, const vector<int>& P, const vector<value_type>& b){
    int n = (int)LU.size();
    vector<value_type> x(n), y(n);
    for(int i = 0; i < n; i++){
        y[i] = b[P[i]];
        for(int j = 0; j < i; j++){
            y[i] -= LU[i][j] * y[j];
        }
    }
    for(int i = n - 1; i >= 0; i--){
        x[i] = y[i];
        for(int j = i + 1; j < n; j++){
            x[i] -= LU[i][j] * x[j];
        }
        x[i] /= LU[i][i];
    }
    return x;
}

vector<value_type> solve(const Matrix& A, const vector<value_type>& b){
    Matrix LU = A;
    int n = (int)LU.size();
    vector<int> P;
    for(int i = 0; i < n; i++) P.push_back(i);
    for(int i = 0; i < n; i++){
        value_type maxn = abs(LU[i][i]);
        int select = i;
        for(int j = i + 1; j < n; j++){
            if(maxn < abs(LU[j][i])){
                select = j;
                maxn = abs(LU[j][i]);
            }
        }
        if(maxn == 0) throw("Singular Matrix.");
        swap(P[i], P[select]);
        swap(LU[i], LU[select]);
        for(int j = i + 1; j < n; j++){
            LU[j][i] = LU[j][i] / LU[i][i];
            for(int k = i + 1; k < n; k++){
                LU[j][k] -= LU[j][i] * LU[i][k];
            }
        }
    }
    vector<value_type> x(n), y(n);
    for(int i = 0; i < n; i++){
        y[i] = b[P[i]];
        for(int j = 0; j < i; j++){
            y[i] -= LU[i][j] * y[j];
        }
    }
    for(int i = n - 1; i >= 0; i--){
        x[i] = y[i];
        for(int j = i + 1; j < n; j++){
            x[i] -= LU[i][j] * x[j];
        }
        x[i] /= LU[i][i];
    }
    return x;
}

int main(void){
    Matrix A = {
        {1, Fraction(2, 1), Fraction(3, 1) },
        {Fraction(2, 1), Fraction(3, 1), Fraction(4, 1)},
        {Fraction(3, 1), Fraction(4, 1), Fraction(5, 1)}
    };
    puts("Matrix:");
    for(int i = 0; i < (int)A.size(); i++){
        for(int j = 0; j < (int)A.size(); j++){
            A[i][j].print('\t');
            //printf("%lf ", A[i][j]);
        }
        puts("");
    }
    printf("\nans:\t");
    vector<value_type> b = {Fraction(4, 1), Fraction(5, 1), Fraction(6,1)};
    vector<value_type> ans = solve(A, b);
    for(auto i : ans){
        //printf("%lf ", i);
        i.print(',');
        printf(" ");
    }
    puts("");
}




