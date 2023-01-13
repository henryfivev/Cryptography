#include <bitset>
#include <string>
#include <iostream>
using namespace std;

std::string string_to_bitset(std::string str)
{
    std::string retstr;
    for (int i = 0; i < str.length(); i++)
    {
        // cout << str[i] << '\n';
        std::bitset<8> temp(str[i]);
        retstr += temp.to_string();
    }
    return retstr;
}

std::string PKCS5Padding(std::string str)
{
    if (!str.length() % 128)
        return str + "10101010101010101010101010101010";

    int i = 1;
    for (;; i++)
        if (i * 128 > str.length())
            break;
    int n = (i * 128 - str.length()) / 8;
    for (int i = 0; i < n; i++)
    {
        std::bitset<8> temp(n);
        str += temp.to_string();
    }
    return str;
}

int main()
{
    bitset<10> a('b');
    bitset<5> b(11);
    string astr = a.to_string();
    astr = "00" + astr;
    bitset<5> aa(astr);
    cout << astr.length() << '\n';

    std::string raw_IV = "20337251";
    string IV = string_to_bitset(raw_IV);
    cout << IV << '\n';
    string sub = raw_IV.substr(0, 3);
    string res = PKCS5Padding("000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
    cout << res;
    // bitset<5> c = a ^ b;
}

/*
110010001100000011001100110011001101110011001000110101
00110001
*/

class BigNum2
{
public:
    BigNum highBit;
    BigNum lowBit;
    BigNum2()
    {
        highBit = 0x0;
        lowBit = 0x0;
    }
    BigNum2(BigNum num)
    {
        highBit = 0x0;
        lowBit = num;
    }
    BigNum2(const BigNum2 &input)
    {
        highBit = input.highBit;
        lowBit = input.lowBit;
    }
    BigNum2 &operator=(const BigNum2 &input)
    {
        highBit = input.highBit;
        lowBit = input.lowBit;
        return *this;
    }
    BigNum2 operator*(const BigNum2 &input)
    {
        BigNum2 ret;
        ret.lowBit = this->lowBit * input.lowBit;
        BigNum a_lo = (__uint128_t)(this->lowBit);
        BigNum a_hi = this->lowBit >> 64;
        BigNum b_lo = (__uint128_t)(input.lowBit);
        BigNum b_hi = input.lowBit >> 64;
        BigNum a_x_b_hi = a_hi * b_hi;
        BigNum a_x_b_mid = a_hi * b_lo;
        BigNum b_x_a_mid = b_hi * a_lo;
        BigNum a_x_b_lo = a_lo * b_lo;
        BigNum carry_bit = ((BigNum)(__uint128_t)a_x_b_mid + (BigNum)(__uint128_t)b_x_a_mid + (a_x_b_lo >> 64)) >> 64;
        ret.highBit = a_x_b_hi + (a_x_b_mid >> 64) + (b_x_a_mid >> 64) + carry_bit;
        return ret;
    }
    BigNum2 &operator<<=(int value)
    {
        for (int i = 0; i < value; i++)
        {
            int msb = this->lowBit >> 127;
            this->lowBit <<= 1;
            this->highBit <<= 1;
            this->highBit |= msb;
        }
        return *this;
    }
    BigNum2 &operator>>=(int value)
    {
        for (int i = 0; i < value; i++)
        {
            BigNum lsb = this->highBit << 127;
            this->highBit >>= 1;
            this->lowBit >>= 1;
            this->lowBit |= lsb;
        }
        return *this;
    }
    BigNum2 operator<<(int value)
    {
        BigNum2 ret;
        ret.highBit = this->highBit;
        ret.lowBit = this->lowBit;
        for (int i = 0; i < value; i++)
        {
            int msb = ret.lowBit >> 127;
            ret.lowBit <<= 1;
            ret.highBit <<= 1;
            ret.highBit |= msb;
        }
        return ret;
    }
    BigNum2 operator>>(int value)
    {
        BigNum2 ret;
        ret.highBit = this->highBit;
        ret.lowBit = this->lowBit;
        for (int i = 0; i < value; i++)
        {
            BigNum lsb = ret.highBit << 127;
            ret.highBit >>= 1;
            ret.lowBit >>= 1;
            ret.lowBit |= lsb;
        }
        return ret;
    }
    BigNum2 operator-(const BigNum2 &input)
    {
        BigNum2 ret;
        ret.highBit = this->highBit - input.highBit;
        if (this->lowBit > input.lowBit)
        {
            ret.lowBit = this->lowBit - input.lowBit;
        }
        else
        {
            ret.highBit -= 1;
            ret.lowBit = 0xffffffffffffffffffffffffffffffff - (input.lowBit - this->lowBit) + 1;
        }
        return ret;
    }
    BigNum2 operator/(int div)
    {
        BigNum2 ret;
        ret.highBit = this->highBit / div;
        ret.lowBit = this->lowBit / div;
        return ret;
    }
    bool operator==(const BigNum2 &input)
    {
        if (this->highBit == input.highBit && this->lowBit == input.lowBit)
        {
            return true;
        }
        return false;
    }
    bool operator>(const BigNum2 &input)
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
    bool operator<(const BigNum2 &input)
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
    bool operator>=(const BigNum2 &input)
    {
        return (*this > input) || (*this == input);
    }
    bool operator<=(const BigNum2 &input)
    {
        return (*this < input) || (*this == input);
    }
    bool operator!=(int num)
    {
        BigNum2 a;
        a.lowBit = num;
        return !(*this == a);
    }
};