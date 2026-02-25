
// add necessary includes here
#include "gmressolver.h"

class TridiagSolver:public GMResSolver{
    std::vector<double> d1;
    std::vector<double> d2;
    std::vector<double> d3;
public:
    TridiagSolver(std::vector<double> d1,std::vector<double> d2,std::vector<double> d3) :
        d1(d1),d2(d2),d3(d3){}
    virtual void linmap(std::vector<double>& v,const std::vector<double>& w) override{
        //ASSERT_THAT(v.size(),Eq(w.size()));
        size_t n = v.size();
        v[0] = d2[0] * w[0] + d2[1] * w[1];
        v[n-1] = d2[n-1]*w[n-2] + d3[n-1]*w[n-1];
        for(size_t i = 1;i<n-1;i++){
            v[i] = d1[i-1]*w[i-1] + d2[i] * w[i] + d3[i-1]*w[i+1];
        }
    }
};



class GMResTest : public QObject
{
    Q_OBJECT

public:
    GMResTest();
    ~GMResTest();

private slots:
    void test_case1();
};

GMResTest::GMResTest() {}

GMResTest::~GMResTest() {}

void GMResTest::test_case1() {
    size_t n = 10;
    std::default_random_engine generator;
    generator.seed();
    auto d1 = std::vector<double>(n),
        d2 = std::vector<double>(n),
        d3 = std::vector<double>(n);

    std::uniform_real_distribution dist = std::uniform_real_distribution<double>();
    for(size_t i = 0;i<d2.size();i++){
        d1[i] = dist(generator);
        d2[i] = dist(generator);
        d3[i] = dist(generator);
    }
    auto solver = TridiagSolver(d1,d2,d3);
    auto xtrue = std::vector<double>(d2.size());
    auto b = std::vector<double>(d2.size());
    for(auto& xi : xtrue)
        xi = 1.0;
    auto x = std::vector<double>(d2.size());
    solver.linmap(b,x);
    solver.solve(x,b);
    double err = 0;
    for(size_t i = 0;i<n;i++){
        double erri = x[i] - xtrue[i];
        err += erri*erri;
    }
}

QTEST_APPLESS_MAIN(GMResTest)

#include "tst_gmrestest.moc"
