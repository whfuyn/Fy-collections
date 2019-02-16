#include <cstdio>
#include ".\\Alice_Lib.hpp"
#include ".\\Fraction.hpp"
#include "..\\project\\vector\\vector.hpp"

typedef size_t size_type;
typedef Fraction value_type;
typedef vector<vector<value_type> > Matrix;

void LUP_decomposition(Matrix& A, vector<size_type>& P){
    size_type n = A.size();
    P.clear();
    for(size_type i = 0; i < n; i++) P.push_back(i);
    for(size_type k = 0; k < n; k++){
        size_type select = k;
        value_type maxn = abs(A[k][k]);
        for(size_type i = k + 1; i < n; i++){
            if(abs(A[i][k]) > maxn){
                select = i;
                maxn = abs(A[i][k]);
            }
        }
        if(!maxn) throw("Singular Matrix.");
        if(select != k){
            swap(P[select], P[k]);
            swap(A[select], A[k]);
        }
        for(size_type i = k + 1; i < n; i++){
            A[i][k] = A[i][k] / A[k][k];
            for(size_type j = k + 1; j < n; j++){
                A[i][j] -= A[i][k] * A[k][j];
            }
        }
    }
}

vector<value_type> LUP_solve(
        const Matrix& LU,
        const vector<size_type>& P,
        const vector<value_type>& b
        )
{
    size_type n = LU.size();
    vector<value_type> x(n), y(n);
    for(size_type i = 0; i < n; i++){
        y[i] = b[P[i]];
        for(size_type j = 0; j < i; j++){
            y[i] -= LU[i][j] * y[j];
        }
    }
    size_type i = n;
    while(i--){
        x[i] = y[i];
        for(size_type j = i + 1; j < n; j++){
            x[i] -= LU[i][j] * x[j];
        }
        x[i] /= LU[i][i];
    }
    return x;
}

vector<value_type> solve(const Matrix& A, const vector<value_type>& b){
    Matrix LU = A;
    vector<size_type> P;
    LUP_decomposition(LU, P);
    return LUP_solve(LU, P, b);
}

Matrix inverseMatrix(const Matrix& A){
    Matrix LU = A;
    size_type n = LU.size();
    vector<size_type> P;
    LUP_decomposition(LU, P);
    Matrix res(n), tmp;
    vector<value_type> b(n, 0);
    for(size_type i = 0; i < n; i++){
        b[i] = 1;
        tmp.push_back(LUP_solve(LU, P, b));
        b[i] = 0;
    }
    for(size_type i = 0; i < n; i++){
        for(size_type j = 0; j < n; j++){
            res[i].push_back(tmp[j][i]);
        }
    }
    return res;

}

int main(void){
    size_type n = 1;
    printf("input n: ");
    scanf("%llu", &n);
    //Matrix A(n, vector<value_type>(n, 0));
    //for(size_type i = 0; i < n; i++){
        //for(size_type j = 0; j < n; j++){
            //int tmp = 0;
            //scanf("%d", &tmp);
            //A[i][j] = tmp;
        //}
    //}
    Matrix A(n, vector<value_type>(n, 0));
    for(size_type i = 1; i <= n; i++){
        for(size_type j = 1; j <= n; j++){
            A[i - 1][j - 1] = Fraction(i + j - 1, 1);
        }
    }
    puts("A:");
    for(size_type i = 0; i < n; i++){
        for(size_type j = 0; j < n; j++){
            A[i][j].print('\t');
        }
        puts("");
    }
    printf("A^-1...");
    Matrix inversion = inverseMatrix(A);
    puts("done");
    for(size_type i = 0; i < n; i++){
        for(size_type j = 0; j < n; j++){
            inversion[i][j].print('\t');
        }
        puts("");
    }
    Matrix res(n);
    for(size_type i = 0; i < n; i++){
        res[i].resize(n);
        for(size_type j = 0; j < n; j++){
            for(size_type k = 0; k < n; k++){
                res[i][j] += A[i][k] * inversion[j][k];
            }
        }
    }
    puts("A * A^-1");
    for(size_type i = 0; i < n; i++){
        for(size_type j = 0; j < n; j++){
            res[i][j].print('\t');
        }
        puts("");
    }
}

/*
 *int main(void){
 *    size_type n = 1;
 *    printf("input n: ");
 *    scanf("%llu", &n);
 *    Matrix A(n, vector<value_type>(n, 0));
 *    for(size_type i = 0; i < n; i++){
 *        for(size_type j = 0; j < n; j++){
 *            int tmp = 0;
 *            scanf("%d", &tmp);
 *            A[i][j] = tmp;
 *        }
 *    }
 *    vector<value_type> b = { 1, 4, 5 };
 *    vector<value_type> ans = solve(A, b);
 *    for(size_type i = 0; i < n; i++){
 *        printf("%lf ", ans[i]);
 *        puts("");
 *    }
 *}
 */


//int main(void){
    //Matrix A = {
        //{2, -1, 3},
        //{4, 2, 5},
        //{2, 1, 2}
    //};
    //puts("Matrix:");
    //for(int i = 0; i < A.size(); i++){
        //for(int j = 0; j < A.size(); j++){
            //A[i][j].print('\t');
            ////printf("%lf ", A[i][j]);
        //}
        //puts("");
    //}
    //printf("\nans:\t");
    //vector<value_type> b = {1, 4, 5};
    //vector<value_type> ans = solve(A, b);
    //for(auto i : ans){
        ////printf("%lf ", i);
        //i.print(',');
        //printf(" ");
    //}
    //puts("");
//}

