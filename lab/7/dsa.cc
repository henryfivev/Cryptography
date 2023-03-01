#include "gmp.h"
#include "gmpxx.h"
#include <vector>
#include <iostream>
using namespace std;

// parameter
mpz_class p("27456323421338749408149619353015133342340988417709732076734695406602472419442199885798599181572455969593961040140553416648391985350146662541043328616814052133525767351887067459826626162987664715766617187836264836086793909253277408380682165380732665398492148950221602274225178623486458355569654479167387798744261263704295769593467485002788014620001619818997270943237227602600905781684996049034408675327036237457682865398553882394128335025373756288093748649314761294002810729600199585823705119137610050542254352048645993804835781374787715746318850975878881961989319171864369176825047238633167838823869267569442832892909");
mpz_class q("104157279931481431886985012477114223269161211000653816000021509176830313880343");
mpz_class alpha("17074740003955525400395323583439413391693112243303604720053587149934685382763402013620278882208381505864232305097014224910837751143230513869044830864216182993222818468583319621195414395014060748482539791383489210214724919934454511641924663112938520925566237757963912347069748908666767155988851019403583372010642865701769169762197669291688726484196282029442136032608104594190554584041047902665478042544948073979712777272994165146032978989165072840451963957649607587925016279541333250128025812725501424939314594017089532384038486554629889307570760267255232058164556263283904459176736079705868862303082930208510912459615");
mpz_class a("20337251");
mpz_class beta;
mpz_class k("2022");
// input
mpz_class hashMes("95f0fcab4a4823de89deac3045f783e6efebcbedede44d5441e76de3b1531dd9", 16);
// output
mpz_class gama;
mpz_class delta;

// function definition
void sig();
void ver();

int main()
{
    // beta = alpha^a mod p
    mpz_powm(beta.get_mpz_t(), alpha.get_mpz_t(), a.get_mpz_t(), p.get_mpz_t());
    cout << "beta is " << beta << '\n';
    // signature
    sig();
    // verification
    ver();
}

// function implement
void sig()
{
    // gama = (alpha^k mod p) mod q
    mpz_powm(gama.get_mpz_t(), alpha.get_mpz_t(), k.get_mpz_t(), p.get_mpz_t());
    mpz_mod(gama.get_mpz_t(), gama.get_mpz_t(), q.get_mpz_t());
    cout << "gamma is " << gama << '\n';

    // delta = (hashMes + a*gama)(k ^ -1) mod q
    mpz_class k_invert;
    mpz_invert(k_invert.get_mpz_t(), k.get_mpz_t(), q.get_mpz_t());
    mpz_mod(delta.get_mpz_t(), mpz_class((hashMes + a * gama) * k_invert).get_mpz_t(), q.get_mpz_t());
    cout << "delta is " << delta << '\n';
}

void ver()
{
    mpz_class e1, e2, delta_invert, ret, alpha_e1, beta_e2;
    mpz_invert(delta_invert.get_mpz_t(), delta.get_mpz_t(), q.get_mpz_t());
    cout << "delta_invert is " << delta_invert << '\n';

    // e1 = (hashMes * (delta ^ -1)) mod q
    mpz_mod(e1.get_mpz_t(), mpz_class(hashMes * delta_invert).get_mpz_t(), q.get_mpz_t());
    cout << "e1 is " << e1 << '\n';

    // e2 = (gama * (delta ^ -1)) mod q
    mpz_mod(e2.get_mpz_t(), mpz_class(gama * delta_invert).get_mpz_t(), q.get_mpz_t());
    cout << "e2 is " << e2 << '\n';

    // alpha_e1 = (alpha ^ e1) mod p
    mpz_powm(alpha_e1.get_mpz_t(), alpha.get_mpz_t(), e1.get_mpz_t(), p.get_mpz_t());
    // beta_e2 = (beta ^ e2) mod p
    mpz_powm(beta_e2.get_mpz_t(), beta.get_mpz_t(), e2.get_mpz_t(), p.get_mpz_t());
    // ret = ((alpha ^ e1) * (beta ^ e2) mod p) mod q
    mpz_mod(ret.get_mpz_t(), mpz_class(alpha_e1 * beta_e2).get_mpz_t(), p.get_mpz_t());
    mpz_mod(ret.get_mpz_t(), ret.get_mpz_t(), q.get_mpz_t());
    cout << "ret is " << ret << '\n';

    if (ret == gama)
    {
        cout << "'it is authentic signature!\n";
    }
    else
    {
        cout << "we are wrong qwq\n";
    }
}