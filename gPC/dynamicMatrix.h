//
//  dynamicMatrix.h
//  gPC
//
//  Created by 马 征 on 14-2-7.
//  Copyright (c) 2014年 马 征. All rights reserved.
//

#ifndef gPC_dynamicMatrix_h
#define gPC_dynamicMatrix_h

#include "dynamicVector.h"

template<class T> class dynamicMatrix:public dynamicVector<T>{
    int N;
public:
    dynamicMatrix(int = 0, int = 0, const T& = 0);
    T& operator()(int i, int j){ return this->component[i*N+j]; }  //(i,j)th element
    const T& operator()(int i, int j, const char*) const{
        return this->component[i*N+j];
    }  //  (i,j)th element (read only)
    int height() const{ return this->dim()/N; }
    int width() const{ return N; }
    const dynamicMatrix& operator+=(const dynamicMatrix&);
    const dynamicMatrix& operator-=(const dynamicMatrix&);
    const dynamicMatrix& operator*=(const T&);
    const dynamicMatrix& operator/=(const T&);
};

template<class T>
dynamicMatrix<T>::dynamicMatrix(int m, int n, const T&t){
    this->dimension = n*m;
    N = n;
    this->component = this->dimension ? new T[this->dimension] : 0;
    for(int i=0; i<this->dimension; i++)
        this->component[i] = t;
}  //  constructor

template<class T>
const dynamicMatrix<T>& dynamicMatrix<T>::operator+=(const dynamicMatrix<T>&v){
    for(int i = 0; i < this->dimension; i++)
        this->component[i] += v[i];
    return *this;
}  //  adding a dynamicMatrix to the current dynamicMatrix

template<class T>
const dynamicMatrix<T>& dynamicMatrix<T>::operator-=(const dynamicMatrix<T>&v){
    for(int i = 0; i < this->dimension; i++)
        this->component[i] -= v[i];
    return *this;
}  //  subtracting a dynamicMatrix from the current dynamicMatrix

template<class T>
const dynamicMatrix<T>& dynamicMatrix<T>::operator*=(const T& a){
    for(int i = 0; i < this->dimension; i++)
        this->component[i] *= a;
    return *this;
}  //  multiplying the current dynamicMatrix by a scalar

template<class T>
const dynamicMatrix<T>& dynamicMatrix<T>::operator/=(const T& a){
    for(int i = 0; i < this->dimension; i++)
        this->component[i] /= a;
    return *this;
}  //  dividing the current dynamicMatrix by a scalar

template<class T>
const dynamicMatrix<T> operator+(const dynamicMatrix<T>&u, const dynamicMatrix<T>&v){
    return dynamicMatrix<T>(u) += v;
}  //  dynamicMatrix plus dynamicMatrix

template<class T>
const dynamicMatrix<T> operator-(const dynamicMatrix<T>&u, const dynamicMatrix<T>&v){
    return dynamicMatrix<T>(u) -= v;
}  //  dynamicMatrix minus dynamicMatrix

template<class T>
const dynamicMatrix<T> operator*(const dynamicMatrix<T>&u, const T& a){
    return dynamicMatrix<T>(u) *= a;
}  //  dynamicMatrix times scalar

template<class T>
const dynamicMatrix<T> operator*(const T& a, const dynamicMatrix<T>&u){
    return dynamicMatrix<T>(u) *= a;
}  //  T times dynamicMatrix

template<class T>
const dynamicMatrix<T> operator/(const dynamicMatrix<T>&u, const T& a){
    return dynamicMatrix<T>(u) /= a;
}  //  dynamicMatrix divided by scalar

template<class T>
const dynamicMatrix<T> operator-(const dynamicMatrix<T>&u){
    return dynamicMatrix<T>(u) *= -1.;
}  //  negative of a dynamicMatrix

template<class T>
const dynamicVector<T> operator*(const dynamicMatrix<T>&u, const dynamicVector<T>&v){
    dynamicVector<T> tmp(v.dim(),0.0);
    for (int i=0; i<tmp.dim(); i++) {
        T sum=0.0;
        for (int k=0; k<u.width(); k++) {
            sum+=u(i,k,"read")*v[k];
        }
        tmp(i)=sum;
    }
    return tmp;
}  //  dynamicMatrix times dynamicVector


template<class T>
const dynamicMatrix<T> operator*(const dynamicMatrix<T>&u, const dynamicMatrix<T>&v){
    dynamicMatrix<T> tmp(u.height(),v.width(),0.0);
    for (int i=0; i<tmp.height(); i++) {
        for (int j=0; j<tmp.width(); j++) {
            T sum=0.0;
            for (int k=0; k<u.width(); k++) {
                sum+=u(i,k,"read")*v(k,j,"read");
            }
            tmp(i,j)=sum;
        }
    }
    return tmp;
}  //  dynamicMatrix times dynamicMatrix


template<class T>
void print(const dynamicMatrix<T>&v){
    for(int i = 0;i < v.height(); i++){
        for(int j = 0;j < v.width(); j++)
            printf("v[%d,%d]=%f;  ",i,j,v(i,j,"read"));
        printf("\n");
    }
}  //  printing a dynamicMatrix

template<class T>
void switchColumns(dynamicMatrix<T>&A,int j, int k){
    if(j != k)
        for(int i=0; i<A.height(); i++){
            T keep = A(i,k,"read");
            A(i,k) = A(i,j,"read");
            A(i,j) = keep;
        }
}  //  switch columns j and k

template<class T>
int maxRowI(const dynamicMatrix<T>&A,int i){
    int maxI = i;
    for(int j=i+1; j<A.width(); j++)
        if(fabs(A(i,j,"read"))>fabs(A(i,maxI,"read")))
            maxI = j;
    return maxI;
}  //  column of maximal element in row i

template<class T>
const dynamicMatrix<T>
LUP(const dynamicMatrix<T>&A, dynamicVector<int>&P){
    dynamicMatrix<T> LU = A;
    for(int j=0; j<A.width(); j++){
        P(j) = maxRowI(LU,j);
        switchColumns(LU,j,P[j]);
        for(int i=j+1; i<A.height(); i++){
            LU(i,j) = LU(i,j,"read") / LU(j,j,"read");
            for(int k=j+1; k<A.width(); k++)
                LU(i,k) -= LU(i,j,"read") * LU(j,k,"read");
        }
    }
    return LU;
}  //  LUP factorization

template<class T>
const T
det(const dynamicMatrix<T>&LU, const dynamicVector<int>&P){
    T detLU = 1;
    for(int i=0; i<LU.height(); i++){
        detLU *= LU(i,i,"read");
        if(P[i] != i)
            detLU *= -1;
    }
    return detLU;
}  //  determinant using LU factorization

template<class T>
const dynamicVector<T>
invert(const dynamicMatrix<T>&LU, const dynamicVector<int>&P, const dynamicVector<T>&v){
    dynamicVector<T> x = v;
    for(int i=1; i<v.dim(); i++)
        for(int j=0; j<i; j++)
            x(i) -= LU(i,j,"read") * x[j];
    for(int i=v.dim()-1; i>=0; i--){
        for(int j=v.dim()-1; j>i; j--)
            x(i) -= LU(i,j,"read") * x[j];
        x(i) /= LU(i,i,"read");
        
    }
    for(int i=v.dim()-2; i>=0; i--)
        if(i!=P[i]){
            T keep = x[i];
            x(i) = x[P[i]];
            x(P[i]) = keep;
        }
    return x;
}  //  invert for some right-hand-side vector

template<class T>
const dynamicMatrix<T>
invert(const dynamicMatrix<T>&LU, const dynamicVector<int>&P){
    dynamicMatrix<T> inverse(LU.height(),LU.width());
    dynamicVector<T> zero(LU.height(),0.);
    for(int j=0; j<LU.width(); j++){
        dynamicVector<T> ej = zero;
        ej(j) = 1.;
        dynamicVector<T> columnJ = invert(LU,P,ej);
        for(int i=0; i<LU.height(); i++)
            inverse(i,j) = columnJ[i];
        
    }
    return inverse;
}  //  invert a matrix using its LU factorization

template<class T>
const T det(const dynamicMatrix<T>&A){
    if(A.height()==1)
        return A(0,0,"read");
    T detA = 0.;
    int sign = 1;
    for(int j=0; j<A.height(); j++){
        detA += sign * A(0,j,"read") * det(minor(0,j,A));
        sign *= -1;
    }
    return detA;
}  //  determinant using the original definition

template<class T>
const dynamicMatrix<T>  minor(int k, int l, const dynamicMatrix<T>&m){
    dynamicMatrix<T> minorkl(m.height()-1,m.width()-1);
    int ii=-1;
    for(int i=0; i<m.height(); i++)
        if(i!=k){
            ii++;
            int jj=-1;
            for(int j=0; j<m.width(); j++)
                if(j!=l)
                    minorkl(ii,++jj) = m(i,j,"read");
        }
    return minorkl;
}  //  (k,l)th minor of m

template<class T>
const dynamicMatrix<T>
inverse(const dynamicMatrix<T>&A){
    dynamicMatrix<T> Ainverse(A.height(),A.width());
    for(int i=0; i<A.height(); i++)
        for(int j=0; j<A.width(); j++)
            Ainverse(i,j) = power(-1,i+j) * det(minor(j,i,A));
    return Ainverse/det(A);
}  //  inverse using Cramer's rule



double pythag(const double a, const double b) {
    double absa=fabs(a), absb=fabs(b);
    return (absa > absb ? absa*sqrt(1.0+(absb/absa)*(absb/absa)) :
            (absb == 0.0 ? 0.0 : absb*sqrt(1.0+(absa/absb)*(absa/absb))));
}

double sign(double a) {
    if (a < 0) {
        return -1.0;
    }
    else {
        return 1.0;
    }
}
double SIGN(double a, double b) {
    return fabs(a)*sign(b);
}


void tqli(dynamicVector<double>& d, dynamicVector<double>& e, dynamicMatrix<double>& z) {
    int m,l,iter,i,k, n=d.dim();
    double s,r,p,g,f,dd,c,b;
    const double EPS=std::numeric_limits<double>::epsilon();
    for (i=1;i<n;i++) e(i-1)=e[i];
    e(n-1)=0.0;
    for (l=0;l<n;l++) {
        iter=0;
        do {
            for (m=l;m<n-1;m++) {
                dd=fabs(d[m])+fabs(d[m+1]);
                if (fabs(e[m]) <= EPS*dd) break;
            }
            if (m != l) {
                if (iter++ == 30) throw("Too many iterations in tqli");
                g=(d[l+1]-d[l])/(2.0*e[l]);
                r=pythag(g,1.0);
                g=d[m]-d[l]+e[l]/(g+SIGN(r,g));
                s=c=1.0;
                p=0.0;
                for (i=m-1;i>=l;i--) {
                    f=s*e[i];
                    b=c*e[i];
                    e(i+1)=(r=pythag(f,g));
                    if(r==0.0){
                        d(i+1) -= p;
                        e(m)=0.0;
                        break;
                    }
                    s=f/r;
                    c=g/r;
                    g=d[i+1]-p;
                    r=(d[i]-g)*s+2.0*c*b;
                    d(i+1)=g+(p=s*r);
                    g=c*r-b;
                    for (k=0;k<n;k++) {
                        f=z(k,i+1);
                        z(k,i+1)=s*z(k,i,"read")+c*f;
                        z(k,i)=c*z(k,i,"read")-s*f;
                    }
                }
                if (r == 0.0 && i >= l) continue;
                d(l) -= p;
                e(l)=g;
                e(m)=0.0;
            }
        } while (m != l);
    }
}

#endif
