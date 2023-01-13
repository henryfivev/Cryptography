#include "gmp.h"
#include "gmpxx.h"
#include <vector>
#include <iostream>
using namespace std;

// p -> G
// g -> alpha
// y -> beta
// q -> n

mpz_class p("31050371851708889440695779044384182719244728783");
mpz_class q("11706593258111142827");
mpz_class y("13821904325547285207847180361637528630753679004");
mpz_class g;
mpz_class p1;
mpz_class hash20337251("bb11f6ddc16eef19cf20d1ae91c06cf4c043341d983c0cc67e7d921514fe3f57", 16);
// mpz_class hash20337033("f74e3e0150f2fe7a5edf1cce9ba489503ff6be7d24e261f2cda25aa25d6d1786", 16);
// mpz_class hash20337211("c26fb36d26672e8d0a8d2b09dc30f22aaedb23ec59a085c5aee946cb03ff5368")
vector<mpz_class> f(vector<mpz_class>);
mpz_class pollard_rho();

int main()
{
    mpz_class num1("1208925819614629174706189");
    p1 = 2 * 1097 * num1 * q;
    mpz_class hashAfterMod;
    mpz_mod(hashAfterMod.get_mpz_t(), hash20337251.get_mpz_t(), p.get_mpz_t());
    mpz_powm(g.get_mpz_t(), hashAfterMod.get_mpz_t(), mpz_class(2 * 1097 * num1).get_mpz_t(), p.get_mpz_t());

    cout << "hashAfterMod is " << hashAfterMod << '\n';
    cout << "alpha is        " << g << '\n';
    cout << "beta is         " << y << '\n';
    cout << "G is            " << p << '\n';
    cout << "n is            " << q << '\n';

    mpz_class log_g_y = pollard_rho();
    cout << "log_g_y is " << log_g_y << '\n';
    cout << "it is ooko.........." << '\n';
}

vector<mpz_class> f(vector<mpz_class> input)
{
    // alpha = g, beta = y
    // input[0] = x, input[1] = a, input[2] = b
    mpz_class inputMod, retX, retA, retB;
    mpz_mod(inputMod.get_mpz_t(), input[0].get_mpz_t(), mpz_class(3).get_mpz_t());
    switch (inputMod.get_ui())
    {
    case 1:
        // S1 -> (beta*x, a, b+1)
        mpz_mod(retX.get_mpz_t(), mpz_class(y * input[0]).get_mpz_t(), p.get_mpz_t());
        mpz_mod(retB.get_mpz_t(), mpz_class(input[2] + 1).get_mpz_t(), q.get_mpz_t());
        return {retX, input[1], retB};

    case 0:
        // S2 -> (x*x, 2a, 2b)
        mpz_mod(retX.get_mpz_t(), mpz_class(input[0] * input[0]).get_mpz_t(), p.get_mpz_t());
        mpz_mod(retA.get_mpz_t(), mpz_class(2 * input[1]).get_mpz_t(), q.get_mpz_t());
        mpz_mod(retB.get_mpz_t(), mpz_class(2 * input[2]).get_mpz_t(), q.get_mpz_t());
        return {retX, retA, retB};
    default:
        // S3 -> (alpha*x, a+1, b)
        mpz_mod(retX.get_mpz_t(), mpz_class(g * input[0]).get_mpz_t(), p.get_mpz_t());
        mpz_mod(retA.get_mpz_t(), mpz_class(input[1] + 1).get_mpz_t(), q.get_mpz_t());
        return {retX, retA, input[2]};
    }
}

mpz_class pollard_rho()
{
    mpz_class counter_mc(0);
    mpz_class xVec[2] = {mpz_class(1), mpz_class(1)};
    mpz_class aVec[2] = {mpz_class(0), mpz_class(0)};
    mpz_class bVec[2] = {mpz_class(0), mpz_class(0)};
    cout << xVec[0] << "..." << aVec[0] << "..." << bVec[0] << '\n';
    cout << xVec[1] << "..." << aVec[1] << "..." << bVec[1] << '\n';

    while (xVec[0] != xVec[1] || xVec[0] == 1)
    {
        counter_mc++;
        vector<mpz_class> fret1 = f({xVec[0], aVec[0], bVec[0]});
        xVec[0] = fret1[0];
        aVec[0] = fret1[1];
        bVec[0] = fret1[2];
        vector<mpz_class> fret2 = f(f({xVec[1], aVec[1], bVec[1]}));
        xVec[1] = fret2[0];
        aVec[1] = fret2[1];
        bVec[1] = fret2[2];
    }

    cout << "counter is " << counter_mc << '\n';
    cout << xVec[0] << "..." << aVec[0] << "..." << bVec[0] << '\n';
    cout << xVec[1] << "..." << aVec[1] << "..." << bVec[1] << '\n';

    if (gcd(bVec[1] - bVec[0], p) != 1)
    {
        cout << "we failed........." << '\n';
    }
    mpz_class a_a;
    mpz_mod(a_a.get_mpz_t(), mpz_class(aVec[0] - aVec[1]).get_mpz_t(), q.get_mpz_t());
    mpz_class b_b_in;
    mpz_invert(b_b_in.get_mpz_t(), mpz_class(bVec[1] - bVec[0]).get_mpz_t(), q.get_mpz_t());
    mpz_class ret;
    mpz_mod(ret.get_mpz_t(), mpz_class(a_a * b_b_in).get_mpz_t(), q.get_mpz_t());
    return ret;
}