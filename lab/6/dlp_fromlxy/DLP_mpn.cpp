#include <algorithm>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <time.h>
#include <gmp.h>
#include <iomanip>
#include <queue>
using namespace std;

//3421489917 n_sqr_root
mpz_t p, n, Alpha, Beta;
//alpha: g, beta: y, n: q
/*const char *str = "c26fb36d26672e8d0a8d2b09dc30f22aaedb23ec59a085c5aee946cb03ff5368";
const char *str_p = "31050371851708889440695779044384182719244728783";
const char *str_p_q = "1208925819614629174706189";
const char *str_cons = "2194";*/
//alpha: g, beta: y, n: q
mpz_t zero, one, two, three;
mpz_t reminder;
//int p = 809, n = 101, Alpha = 89, Beta = 618;
/*const char *str_p = "809";//测试数据
const char *str_n = "101";
const char *str_alpha = "89";
const char *str_beta = "618";*/
const char *str_p = "31050371851708889440695779044384182719244728783";
const char *str_n = "11706593258111142827";
const char *str_alpha = "30985928467877424648137737637834604687361856401";
const char *str_beta = "13821904325547285207847180361637528630753679004";

struct XAB{
        mpz_t x, a, b;
};

XAB f(mpz_t x, mpz_t a, mpz_t b){
    struct XAB temp;
    mpz_init(temp.x);
    mpz_init(temp.a);
    mpz_init(temp.b);

    //mpz_t reminder;
    //mpz_init(reminder);
    mpz_mod(reminder, x, three);
    //unsigned int rem = mpz_get_ui(reminder);

	if(mpz_cmp(reminder, zero) == 0){
		//temp.x = x * x % p;
        mpz_mul(temp.x, x, x);

		//temp.a = (2 * a) % n;
        mpz_mul(temp.a, two, a);
        mpz_mod(temp.a, temp.a, n);

		//temp.b = (2 * b) % n;
        mpz_mul(temp.b, two, b);
        mpz_mod(temp.b, temp.b, n);
	}
	else if(mpz_cmp(reminder, one) == 0){
		//temp.x = Beta * x % p;
        mpz_mul(temp.x, Beta, x);

		//temp.a = a;
        mpz_set(temp.a, a);

		//temp.b = (b + 1) % n;
        mpz_add(temp.b, b, one);
        mpz_mod(temp.b, temp.b, n);
	}
	else{
		//temp.x = Alpha * x % p;
        mpz_mul(temp.x, Alpha, x);

		//temp.a = (a + 1) % n;
        mpz_add(temp.a, a, one);
        mpz_mod(temp.a, temp.a, n);

		//temp.b = b;
        mpz_set(temp.b, b);
	}
    mpz_mod(temp.x, temp.x, p);
	return temp;
}

int main(){
    mpz_init_set_ui(zero, 0);
    mpz_init_set_ui(one, 1);
    mpz_init_set_ui(two, 2);
    mpz_init_set_ui(three, 3);
    mpz_init(reminder);

    mpz_init_set_str(p, str_p, 10);
    mpz_init_set_str(n, str_n, 10);
    mpz_init_set_str(Alpha, str_alpha, 10);
    mpz_init_set_str(Beta, str_beta, 10);

    struct XAB xab, xab_;

    struct XAB temp_xab;
    mpz_init(temp_xab.x);
    mpz_init(temp_xab.a);
    mpz_init(temp_xab.b);

    mpz_t output;
    mpz_init(output);

    clock_t start, finish;
    double duration, test = 0.01;
    start = clock();
    xab = f(one, zero, zero);
	xab_ = f(xab.x, xab.a, xab.b);
    //gmp_printf("xab: %Zd %Zd %Zd\n", xab.x, xab.a, xab.b);
    //gmp_printf("xab_: %Zd %Zd %Zd\n", xab_.x, xab_.a, xab_.b);
    gmp_printf("xab:\n%Zd\n%Zd\n%Zd\n\n\n", xab.x, xab.a, xab.b);
    gmp_printf("xab_:\n%Zd\n%Zd\n%Zd\n\n\n", xab_.x, xab_.a, xab_.b);


    int cnt = 1000000;
    //while(mpz_cmp(xab.x, xab_.x) != 0){
    while(cnt --){
		xab = f(xab.x, xab.a, xab.b);
		xab_ = f(xab_.x, xab_.a, xab_.b);
		xab_ = f(xab_.x, xab_.a, xab_.b);
        //gmp_printf("xab: %Zd %Zd %Zd\n", xab.x, xab.a, xab.b);
        //gmp_printf("xab_: %Zd %Zd %Zd\n", xab_.x, xab_.a, xab_.b);
        //gmp_printf("xab:\n%Zd\n%Zd\n%Zd\n\n\n", xab.x, xab.a, xab.b);
        //gmp_printf("xab_:\n%Zd\n%Zd\n%Zd\n\n\n", xab_.x, xab_.a, xab_.b);
	}
    finish = clock();
    duration = (double)(finish - start) / CLOCKS_PER_SEC;
    cout << setprecision(8) << duration << " seconds" << endl;
    printf("%.8f seconds\n", duration);

    mpz_t temp_a, temp_b, temp, gcd, inverse;
    mpz_init(temp_a);
    mpz_init(temp_b);
    mpz_init(temp);
    mpz_init(gcd);
    mpz_init(inverse);
    mpz_sub(temp_b, xab.b, xab_.b);
    mpz_gcd(gcd, temp_b, n);
	if (mpz_cmp(gcd, one) != 0)    cout << "failure" << endl;
	else{
        mpz_sub(temp_a, xab.a, xab_.a);
        mpz_sub(temp_b, xab_.b, xab.b);
        mpz_invert(inverse, temp_b, n);
        mpz_mul(temp, temp_a, inverse);
		mpz_mod(output, temp, n);
		gmp_printf("output: %Zd\n", output);
	}
}