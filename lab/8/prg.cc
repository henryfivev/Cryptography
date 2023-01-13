#include <chrono>
#include <limits>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

class BigNum
{
public:
    uint64_t highBit;
    uint64_t lowBit;
    BigNum()
    {
        highBit = 0x0;
        lowBit = 0x0;
    }
    BigNum(unsigned long long num)
    {
        highBit = 0x0;
        lowBit = num;
    }
    BigNum(const BigNum &input)
    {
        highBit = input.highBit;
        lowBit = input.lowBit;
    }
    BigNum &operator=(const BigNum &input)
    {
        highBit = input.highBit;
        lowBit = input.lowBit;
        return *this;
    }
    BigNum operator*(const BigNum &input)
    {
        BigNum ret;
        ret.lowBit = this->lowBit * input.lowBit;
        uint64_t a_lo = (uint32_t)(this->lowBit);
        uint64_t a_hi = this->lowBit >> 32;
        uint64_t b_lo = (uint32_t)(input.lowBit);
        uint64_t b_hi = input.lowBit >> 32;
        uint64_t a_x_b_hi = a_hi * b_hi;
        uint64_t a_x_b_mid = a_hi * b_lo;
        uint64_t b_x_a_mid = b_hi * a_lo;
        uint64_t a_x_b_lo = a_lo * b_lo;
        uint64_t carry_bit = ((uint64_t)(uint32_t)a_x_b_mid + (uint64_t)(uint32_t)b_x_a_mid + (a_x_b_lo >> 32)) >> 32;
        ret.highBit = a_x_b_hi + (a_x_b_mid >> 32) + (b_x_a_mid >> 32) + carry_bit;
        return ret;
    }
    BigNum &operator<<=(int value)
    {
        for (int i = 0; i < value; i++)
        {
            int msb = this->lowBit >> 63;
            this->lowBit <<= 1;
            this->highBit <<= 1;
            this->highBit |= msb;
        }
        return *this;
    }
    BigNum &operator>>=(int value)
    {
        for (int i = 0; i < value; i++)
        {
            uint64_t lsb = this->highBit << 63;
            this->highBit >>= 1;
            this->lowBit >>= 1;
            this->lowBit |= lsb;
        }
        return *this;
    }
    BigNum operator<<(int value)
    {
        BigNum ret;
        ret.highBit = this->highBit;
        ret.lowBit = this->lowBit;
        for (int i = 0; i < value; i++)
        {
            int msb = ret.lowBit >> 63;
            ret.lowBit <<= 1;
            ret.highBit <<= 1;
            ret.highBit |= msb;
        }
        return ret;
    }
    BigNum operator>>(int value)
    {
        BigNum ret;
        ret.highBit = this->highBit;
        ret.lowBit = this->lowBit;
        for (int i = 0; i < value; i++)
        {
            uint64_t lsb = ret.highBit << 63;
            ret.highBit >>= 1;
            ret.lowBit >>= 1;
            ret.lowBit |= lsb;
        }
        return ret;
    }
    BigNum operator-(const BigNum &input)
    {
        BigNum ret;
        ret.highBit = this->highBit - input.highBit;
        if (this->lowBit > input.lowBit)
        {
            ret.lowBit = this->lowBit - input.lowBit;
        }
        else
        {
            ret.highBit -= 1;
            ret.lowBit = 0xffffffffffffffff - (input.lowBit - this->lowBit) + 1;
        }
        return ret;
    }
    BigNum operator/(int div)
    {
        BigNum ret;
        ret.highBit = this->highBit / div;
        ret.lowBit = this->lowBit / div;
        return ret;
    }
    bool operator==(const BigNum &input)
    {
        if (this->highBit == input.highBit && this->lowBit == input.lowBit)
        {
            return true;
        }
        return false;
    }
    bool operator>(const BigNum &input)
    {
        if (this->highBit > input.highBit)
        {
            return true;
        }
        else if (this->highBit < input.highBit)
        {
            return false;
        }
        else
        {
            if (this->lowBit > input.lowBit)
            {
                return true;
            }
            return false;
        }
    }
    bool operator<(const BigNum &input)
    {
        if (this->highBit < input.highBit)
        {
            return true;
        }
        else if (this->highBit > input.highBit)
        {
            return false;
        }
        else
        {
            if (this->lowBit < input.lowBit)
            {
                return true;
            }
            return false;
        }
    }
    bool operator>=(const BigNum &input)
    {
        return (*this > input) || (*this == input);
    }
    bool operator<=(const BigNum &input)
    {
        return (*this < input) || (*this == input);
    }
    bool operator!=(int num)
    {
        BigNum a;
        a.lowBit = num;
        return !(*this == a);
    }
};

bool mod2(uint64_t);
BigNum modulo(BigNum, uint64_t);
BigNum powm(BigNum, BigNum, uint64_t);

int main()
{
    auto start = chrono::high_resolution_clock::now();
    vector<int> z;
    uint64_t p = 0xc00010000040000b;
    BigNum g(12332102632472395673ULL);
    BigNum s(20337250ULL);
    for (int i = 0; i < 512; i++)
    {
        s = powm(g, s, p);
        z.push_back(mod2(s.lowBit));
    }
    for (int i : z)
    {
        cout << i;
    }
    cout << "\nthe num if 0 is " << count(z.begin(), z.end(), 0);
    cout << "\nthe num of 1 is " << count(z.begin(), z.end(), 1);
    auto end = chrono::high_resolution_clock::now();
    std::chrono::duration<double> totalTime = end - start;
    cout << "\nand we use " << totalTime.count() << "s\n";
}

bool mod2(uint64_t num)
{
    return num & 0x1;
}

BigNum modulo(BigNum A, uint64_t mod)
{
    BigNum B;
    B.lowBit = mod;
    BigNum X = B;
    while (X <= A / 2)
    {
        X <<= 1;
    }
    while (A >= B)
    {
        if (A >= X)
        {
            A = A - X;
        }
        X >>= 1;
    }
    return A;
}

BigNum powm(BigNum base, BigNum exp, uint64_t mod)
{
    BigNum ret;
    ret.lowBit = 0x1;
    BigNum temp = modulo(base, mod);
    while (exp != 0)
    {
        if (exp.lowBit & 0x1)
        {
            ret = modulo((ret * temp), mod);
        }
        exp >>= 1;
        temp = modulo((temp * temp), mod);
    }
    return ret;
}