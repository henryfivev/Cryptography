#include <cstring>
#include <iostream>
#include <bitset>
#include <time.h>
#include <algorithm>
#include <stdio.h>
#include <gmp.h>
#include <iomanip>
#include <queue>
#include "SHA256.h"
using namespace std;

const char *str_p_q = "1208925819614629174706189";
const char *str_q = "11706593258111142827";
const char *str_cons = "2194";

const char *str_p = "31050371851708889440695779044384182719244728783";
const char *str_n = "11706593258111142827";
const char *str_alpha = "30985928467877424648137737637834604687361856401";
const char *str_beta = "13821904325547285207847180361637528630753679004";
/*const char *str_p = "809";//测试数据
const char *str_n = "101";
const char *str_alpha = "89";
const char *str_beta = "618";*/
mpz_t hash_num;
mpz_t p, n, Alpha, Beta;
mpz_t zero, one, two, three;
mpz_t reminder;
//alpha: g, beta: y, n: q

const unsigned int k[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2,
};

unsigned int W[1024];
unsigned int M[16];

SHA256 sha256 = SHA256();
Message_Digest mes_dig;

void SHA256::init(){
    msg_dig.h[0] = 0x6a09e667;
    msg_dig.h[1] = 0xbb67ae85;
    msg_dig.h[2] = 0x3c6ef372;
    msg_dig.h[3] = 0xa54ff53a;
    msg_dig.h[4] = 0x510e527f;
    msg_dig.h[5] = 0x9b05688c;
    msg_dig.h[6] = 0x1f83d9ab;
    msg_dig.h[7] = 0x5be0cd19;
}

Message_Digest SHA256::hash(unsigned int M[16]){
    unsigned int t1 = 0, t2 = 0;
    unsigned int W[64] = {0};
    unsigned int a, b, c, d, e, f, g, h;

    a = msg_dig.h[0];
    b = msg_dig.h[1];
    c = msg_dig.h[2];
    d = msg_dig.h[3];
    e = msg_dig.h[4];
    f = msg_dig.h[5];
    g = msg_dig.h[6];
    h = msg_dig.h[7];

    for(int i = 0; i < 16; i ++){
        W[i] = M[i];
    }

    for(int i = 16; i < 64; i ++){
        W[i] = ssigma_1(W[i - 2]) + W[i - 7] + ssigma_0(W[i - 15]) + W[i - 16];
    }

    for(int i = 0; i < 64; i ++){
        t1 = h + lsigma_1(e) + Ch(e, f, g) + k[i] + W[i];
        t2 = lsigma_0(a) + maj(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;
    }

    msg_dig.h[0] = (msg_dig.h[0] + a) & 0xFFFFFFFF;
    msg_dig.h[1] = (msg_dig.h[1] + b) & 0xFFFFFFFF;
    msg_dig.h[2] = (msg_dig.h[2] + c) & 0xFFFFFFFF;
    msg_dig.h[3] = (msg_dig.h[3] + d) & 0xFFFFFFFF;
    msg_dig.h[4] = (msg_dig.h[4] + e) & 0xFFFFFFFF;
    msg_dig.h[5] = (msg_dig.h[5] + f) & 0xFFFFFFFF;
    msg_dig.h[6] = (msg_dig.h[6] + g) & 0xFFFFFFFF;
    msg_dig.h[7] = (msg_dig.h[7] + h) & 0xFFFFFFFF;
 
    return msg_dig;
}

unsigned int SHA256::rotr(unsigned int W, int n){
    return ((W >> n) & 0xFFFFFFFF) | (W) << (32 - (n));
}

unsigned int SHA256::rotl(unsigned int W, int n){
    return ((W << n) & 0xFFFFFFFF) | (W) >> (32 - (n));
}

unsigned int SHA256::shr(unsigned int W, int n){
    return ((W >> n) & 0xFFFFFFFF);
}

int padding(char msg[], unsigned int mes[]){
	int len = strlen(msg);
	for(int i = 0; i < len; i ++)  mes[i] = msg[i];

	int i = 1;
	if(len % 64 != 56){//字符转化为二进制
		mes[len] = 0x80;
		for(;(i + len) % 64 != 56; i ++){
			mes[i + len] = 0x0;
		}
	}

	bitset<64> temp(len * 8);
	string pad = temp.to_string();
	int j = 0;
	for(; j < 8; j ++){//添加数据长度
		bitset<8> t(pad, j * 8, 8);
		mes[len + i + j] = t.to_ulong();
	}

	len = len + i + j; 

	unsigned int block[64];
	int block_cnt = len / 64;
	int cnt1 = 0, cnt2 = 0;

	for(i = 0; i < block_cnt; i ++){
		for(j = 0; j < 64; j ++){
			block[j] = mes[cnt1 ++];
		}
		for(j = 0; j < 64; j += 4){
			bitset<32> t1(block[j]), t2(block[j + 1]), t3(block[j + 2]), t4(block[j + 3]);
			t1 = t1 << 24;
			t2 = t2 << 16;
			t3 = t3 << 8 ;
			bitset<32> ans = t1 ^ t2 ^ t3 ^ t4;
			//printf("%lx\n", ans.to_ulong());
			W[cnt2 ++] = ans.to_ulong();
		} 
	}
	
	return block_cnt;
}

void cal(int block_cnt){
    for(int i = 0; i < block_cnt; i ++){//分组处理
        for(int j = 0; j < 16; j ++){
            M[j] = W[(i * 16) + j];
        }
        mes_dig = sha256.hash(M);
        if (i == block_cnt - 1){
            unsigned int h[8];
            string int_to_mpz;
                
            for(int j = 0; j < 8; j ++){
                h[j] = mes_dig.h[j];
            }
            for(int j = 0; j < 8; j ++){
                bitset<32> temp(h[j]);
                int_to_mpz.append(temp.to_string());
            }
            const char *temp_char = int_to_mpz.c_str();

            mpz_init_set_str(hash_num, temp_char, 2);
            //cout << "加密后的学号为: " << endl;
            //gmp_printf("%Zx\n", hash_num);
        }
    }
}

void get_alpha(){
    mpz_t q, mod, g, pow, p_q, cons;
    //mpz_init_set_str(hash_num, str, 16);
    mpz_init_set_str(p, str_p, 10);
    mpz_init_set_str(q, str_q, 10);
    mpz_init_set_str(p_q, str_p_q, 10);
    mpz_init_set_str(cons, str_cons, 10);
    mpz_init(mod);
    mpz_init(Alpha);
    mpz_init(pow);
    //gmp_printf("%Zx\n", hash_num);
    //gmp_printf("%Zd\n", p);

    mpz_mul(pow, p_q, cons);
    //gmp_printf("%Zd\n", pow);
    mpz_mod(mod, hash_num, p);
    //gmp_printf("%Zd\n", mod);
    mpz_powm(Alpha, mod, pow, p);//m = cipher ^ d mod n
    cout << "g: ";
    gmp_printf("%Zd\n", Alpha);
}

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
    char msg[128] = "20337211";
	unsigned int mes[128];
    int block_cnt = padding(msg, mes);
	cal(block_cnt);

    get_alpha();

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
    double duration;
    start = clock();
    xab = f(one, zero, zero);
	xab_ = f(xab.x, xab.a, xab.b);
    //gmp_printf("xab: %Zd %Zd %Zd\n", xab.x, xab.a, xab.b);
    //gmp_printf("xab_: %Zd %Zd %Zd\n", xab_.x, xab_.a, xab_.b);
    //gmp_printf("xab:\n%Zd %Zd %Zd\n", xab.x, xab.a, xab.b);
    //gmp_printf("xab_:\n%Zd %Zd %Zd\n", xab_.x, xab_.a, xab_.b);


    int cnt = 1000000, i = 1;
    while(mpz_cmp(xab.x, xab_.x) != 0){
    //while(cnt --){
		xab = f(xab.x, xab.a, xab.b);
		xab_ = f(xab_.x, xab_.a, xab_.b);
		xab_ = f(xab_.x, xab_.a, xab_.b);
        //gmp_printf("xab: %Zd %Zd %Zd\n", xab.x, xab.a, xab.b);
        //gmp_printf("xab_: %Zd %Zd %Zd\n", xab_.x, xab_.a, xab_.b);
        //gmp_printf("xab:\n%Zd\n%Zd\n%Zd\n\n\n", xab.x, xab.a, xab.b);
        //gmp_printf("xab_:\n%Zd\n%Zd\n%Zd\n\n\n", xab_.x, xab_.a, xab_.b);
        i ++;
	}
    finish = clock();
    duration = (double)(finish - start) / CLOCKS_PER_SEC;
    printf("time: %.8f seconds\n", duration);
    cout << "count: " << i << endl;

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
		gmp_printf("result: %Zd\n", output);
	}
    system("pause");
}