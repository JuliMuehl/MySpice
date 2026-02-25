#include "gmressolver.h"

#include <cassert>
#include <cmath>

#define DEBUG


static inline void applyGivensInplace(std::vector<double>& v,
                                      size_t i,size_t j,
                                      GivensRot rot){
    double vi = rot.c*v[i] - rot.s*v[j];
    double vj = rot.s*v[i] + rot.c*v[j];
    v[i] = vi;
    v[j] = vj;
}

static inline GivensRot make_givens(double a,double b){
    if(a == 0){
        return {0.0,1.0};
    }
    double r = hypot(a,b);
    return {a/r,-b/r};
}

static inline double dotProduct(const std::vector<double>& v, const std::vector<double>& w){
#ifdef DEBUG
    assert(v.size() == w.size());
#endif
    double res = 0.0;
    for(size_t i = 0;i<v.size();i++)
        res += v[i] * w[i];
    return res;
}

static inline double normalize(std::vector<double>& v){
    double norm = sqrt(dotProduct(v,v));
    if(norm == 0.0) return norm;
    for(size_t i = 0;i<v.size();i++)
        v[i] /= norm;
    return norm;
}

static inline std::vector<double>& getColOrPushZeros(std::vector<std::vector<double>>& mat,size_t j,size_t n){
    if(j < mat.size()){
        while(mat[j].size() < n) mat[j].push_back(0.0);
        for(double& x:mat[j]) x = 0;
        return mat[j];
    }
    auto zeros = std::vector<double>(n);
    mat.push_back(zeros);
    return mat.back();
}

void GMResSolver::arnoldiInit(const std::vector<double>& b,const std::vector<double>& xinit) noexcept {
#ifdef DEBUG
    assert(b.size() == xinit.size());
#endif
    size_t n = b.size();
    auto& q = getColOrPushZeros(arnoldiQ,0,n);
    linmap(q,xinit);
    for(size_t i = 0;i<n;i++)
        q[i] = b[i] - q[i];
    e1.clear();
    e1.push_back(normalize(q));
    givensrots.clear();
}

void GMResSolver::arnoldiIter(size_t k) noexcept {
    size_t n = arnoldiQ[0].size();
    auto& q = getColOrPushZeros(arnoldiQ,k+1,n);
    auto& h = getColOrPushZeros(arnoldiH,k,0);
    h.clear();
    linmap(q,this->arnoldiQ[k]);
    for(size_t i = 0;i<=k;i++){
        h.push_back(dotProduct(q,arnoldiQ[i]));
        for(size_t j = 0;j<n;j++)
            q[j] -= h[i] * arnoldiQ[i][j];
    }
    h.push_back(normalize(q));
}
void GMResSolver::backsubstitute(std::vector<double>& x, size_t k) noexcept {
    y.resize(e1.size());
    for(size_t i = 0;i < k;i++){
        size_t row = k - 1 - i;
        y[row] = e1[row];
        for(size_t j = 0;j<i;j++){
            size_t col = k - 1- j;
            y[row] -= arnoldiH[col][row] * y[col];
        }
        y[row] /= arnoldiH[row][row];
    }
    size_t n = x.size();
    for(size_t i = 0;i<n;i++){
        double sum = 0;
        for(size_t j = 0;j<k;j++){
            sum += arnoldiQ[j][i] * y[j];
        }
        x[i] = x[i] + sum;
    }
}

GMResResult GMResSolver::solve(std::vector<double>& x,
                               const std::vector<double>& b,
                               double eps,
                               size_t maxiter) noexcept {
    for(auto& h : arnoldiH) h.clear();
    for(auto& q : arnoldiQ) q.clear();
    size_t n = b.size();
    maxiter = std::min(maxiter,n);
    double nb = sqrt(dotProduct(b,b));
    if(nb == 0){
        for(size_t i = 0;i<n;i++)
            x[i] = 0.0;
        return {.numiters = 0,.residual = 0.0};
    }
    arnoldiInit(b,x);
    double residual = fabs(e1[0]/nb);
    if(residual < eps) {
        return {.numiters = 0,.residual = residual,.ok = true};
    }
    size_t k;
    for(k = 0;k<maxiter;k++){
        arnoldiIter(k);
        auto& h = arnoldiH[k];
        for(size_t l = 0;l<givensrots.size();l++){
            applyGivensInplace(h,l,l+1,givensrots[l]);
        }
        givensrots.push_back(make_givens(h[k],h[k+1]));
        applyGivensInplace(h,k,k+1,givensrots.back());
        e1.push_back(0);
        applyGivensInplace(e1,k,k+1,givensrots.back());
        residual = fabs(e1[k+1]/nb);
        if(residual != residual){
            return {.numiters=k,.residual=residual,.ok=false};
        }
        if(residual <= eps){
            k++;
            break;
        }
    }
    backsubstitute(x,k);
    return {.numiters=k,.residual=residual,.ok=true};
}
