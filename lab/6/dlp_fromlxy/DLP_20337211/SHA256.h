#ifndef _SHA_256_H
#define _SHA_256_H
#include<iostream>
using namespace std;

//逻辑函数定义
#define Ch(x, y, z) ((x & y) ^ ((~x) & z))
#define maj(x, y, z) ((x & y) ^ (x & z) ^ (y & z))
#define lsigma_0(x) (rotl(x, 30) ^ rotl(x, 19) ^ rotl(x, 10))
#define lsigma_1(x) (rotl(x, 26) ^ rotl(x, 21) ^ rotl(x, 7))
#define ssigma_0(x) (rotl(x, 25) ^ rotl(x, 14) ^ shr(x, 3))
#define ssigma_1(x) (rotl(x, 15) ^ rotl(x, 13) ^ shr(x, 10))

struct Message_Digest{//信息摘要
    unsigned int h[8];
};

class SHA256{
    private:
        Message_Digest msg_dig;
    public:
        SHA256(){init();};
        ~SHA256(){};
        Message_Digest hash(unsigned int W[16]);
        void init();
    private:
        unsigned int rotr(unsigned int W,int n);
        unsigned int rotl(unsigned int W,int n);
        unsigned int shr(unsigned int W,int n);
};

#endif