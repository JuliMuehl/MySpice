#ifndef GMRESSOLVER_H
#define GMRESSOLVER_H

#include <vector>
#include <limits>

struct GivensRot{
    double c,s;
};

struct GMResResult{
    size_t numiters;
    double residual;
    bool ok;
};

class GMResSolver
{
    std::vector<std::vector<double>> arnoldiH;
    std::vector<std::vector<double>> arnoldiQ;
    std::vector<GivensRot> givensrots;
    std::vector<double> e1;
    std::vector<double> y;
    void arnoldiInit(const std::vector<double>& b,const std::vector<double>& xinit) noexcept;
    void arnoldiIter(size_t k) noexcept;
    void backsubstitute(std::vector<double>& x, size_t k) noexcept;
protected:
    virtual void linmap(std::vector<double>&, const std::vector<double>&) noexcept = 0;
public:
    GMResResult solve(std::vector<double>& x,
               const std::vector<double>& b,
               double eps = std::numeric_limits<double>::epsilon(),
               size_t maxiter=std::numeric_limits<size_t>::max()) noexcept;
};

#endif // GMRESSOLVER_H
