#include <bitset>
#include <vector>
#include <iostream>
#include <chrono>
#include <numeric>
#include <algorithm>
using namespace std;
typedef chrono::steady_clock STEADY_CLOCK;
#define P 131
#define RSTR "10000000000111"

class gf
{
private:
    bitset<P> value;

public:
    void set(int);
    void set(bitset<P>);
    bitset<P> returnValue();
    bitset<P> add(gf);
    bitset<P> mul(bitset<P>);
    bitset<P> squ();
    bitset<P> in1();
    bitset<P> in2();
    bitset<P> mod(bitset<P * 2>, bitset<P>);
    bitset<P> exp(bitset<P>);
    int maxPower(bitset<P + 1>);
};

void printTime(vector<chrono::duration<int, nano>>);

int main()
{
    /*
    // int a = 9; // 1001    // int b = 1; // 0001  // cout << (a ^ b);
    // int t = 001111; //以0开头表示八进制 // cout << t;
    // bitset<P> a("11111110");
    // gf b, c;
    // b.set(a);
    // b.set(b.mul(b.returnValue()));
    // b.set(b.squ());
    // b.set(2);
    // c.set(2);
    // b.set(b.in1());
    // b.set(b.mul(c.returnValue()));
    // b.set(2);
    // cout << b.in2() << '\n';
    // cout << b.returnValue(); // a = 10, cout 01
    */
    gf a, b;
    a.set(20000000);
    b.set(30000000);
    vector<chrono::duration<int, nano>> timestorer;

    for (int i = 0; i < 50000; i++)
    {
        STEADY_CLOCK::time_point t1 = STEADY_CLOCK::now();
        a.set(a.add(b));
        STEADY_CLOCK::time_point t2 = STEADY_CLOCK::now();
        chrono::duration<int, nano> dTimeSpan = chrono::duration<int, nano>(t2 - t1);
        timestorer.push_back(dTimeSpan);
    }
    cout << "加法运算表现如下: " << '\n';
    printTime(timestorer);
    timestorer.clear();

    for (int i = 0; i < 50000; i++)
    {
        STEADY_CLOCK::time_point t1 = STEADY_CLOCK::now();
        a.set(a.mul(b.returnValue()));
        STEADY_CLOCK::time_point t2 = STEADY_CLOCK::now();
        chrono::duration<int, nano> dTimeSpan = chrono::duration<int, nano>(t2 - t1);
        timestorer.push_back(dTimeSpan);
    }
    cout << "乘法运算表现如下: " << '\n';
    printTime(timestorer);
    timestorer.clear();

    for (int i = 0; i < 50000; i++)
    {
        STEADY_CLOCK::time_point t1 = STEADY_CLOCK::now();
        a.set(a.squ());
        STEADY_CLOCK::time_point t2 = STEADY_CLOCK::now();
        chrono::duration<int, nano> dTimeSpan = chrono::duration<int, nano>(t2 - t1);
        timestorer.push_back(dTimeSpan);
    }
    cout << "平方运算表现如下: " << '\n';
    printTime(timestorer);
    timestorer.clear();

    for (int i = 0; i < 50000; i++)
    {
        STEADY_CLOCK::time_point t1 = STEADY_CLOCK::now();
        a.in1();
        STEADY_CLOCK::time_point t2 = STEADY_CLOCK::now();
        chrono::duration<int, nano> dTimeSpan = chrono::duration<int, nano>(t2 - t1);
        timestorer.push_back(dTimeSpan);
    }
    cout << "基于扩展欧几里得算法的求逆运算表现如下: " << '\n';
    printTime(timestorer);
    timestorer.clear();

    for (int i = 0; i < 50000; i++)
    {

        STEADY_CLOCK::time_point t1 = STEADY_CLOCK::now();
        b.in2();
        STEADY_CLOCK::time_point t2 = STEADY_CLOCK::now();
        chrono::duration<int, nano> dTimeSpan = chrono::duration<int, nano>(t2 - t1);
        timestorer.push_back(dTimeSpan);
    }
    cout << "基于费马小定理的求逆运算表现如下: " << '\n';
    printTime(timestorer);
    timestorer.clear();

    // 20337251
    bitset<P> xh("00100000001100110111001001010001");
    bitset<P> ex(21214928);
    a.set(xh);
    cout << "学号为" << xh << '\n';
    cout << "逆为" << a.in1() << '\n';
    cout << "学号^21214928为" << a.exp(ex);
}

void printTime(vector<chrono::duration<int, nano>> timestorer)
{
    chrono::duration<int, nano> sum(0);
    sort(timestorer.begin(), timestorer.end());
    for (int i = 0; i < 50000; i++)
    {
        sum += timestorer[i];
    }
    cout << "下四分位: " << timestorer[12500].count() << "ns" << '\n';
    cout << "中位数: " << timestorer[25000].count() << "ns" << '\n';
    cout << "均值: " << (sum / 50000).count() << "ns" << '\n';
    cout << "上四分位: " << timestorer[37500].count() << "ns" << '\n';
}

// 模函数
bitset<P> gf::mod(bitset<P * 2> a, bitset<P> r)
{
    bitset<P> ret(0);
    bitset<P * 2> temp;
    bitset<P * 2> _extend(r.to_ulong());
    for (int i = P * 2 - 1; i >= P; i--)
    {
        if (a[i])
        {
            temp = _extend << (i - P);
            a ^= temp;
            a[i] = 0;
        }
    }
    for (int i = 0; i < P; i++)
    {
        ret[i] = a[i];
    }
    return ret;
}

/*
bitset有不少成员函数,
其中有一个<<=左移操作, 可以把0000001变成0010000, 可能用于实现乘法?
*/

// int set value
void gf::set(const int n)
{
    this->value = n;
}

// bitset<P> set value
void gf::set(bitset<P> n)
{
    this->value = n;
}

// return value
bitset<P> gf::returnValue()
{
    return this->value;
}

bitset<P> gf::add(gf a)
{
    return this->value ^ a.returnValue();
}

bitset<P> gf::mul(bitset<P> a)
{
    bitset<P * 2> _ret(0);
    bitset<P> ret(0);
    bitset<P> r(RSTR);
    bitset<P * 2> _extend(this->value.to_string());
    for (int i = 0; i < P; i++)
    {
        if (a[i])
        {
            _ret ^= (_extend << i);
        }
    }
    ret = mod(_ret, r);
    return ret;
}

bitset<P> gf::squ()
{
    return this->mul(this->value);
}

int gf::maxPower(bitset<P + 1> a)
{
    for (int i = P; i > -1; i--)
    {
        if (a[i])
        {
            return i;
        }
    }
    return 0;
}

// from Extend Euclidean...
bitset<P> gf::in1()
{
    bitset<P + 1> poly(RSTR);
    poly[131] = 1;
    bitset<P + 1> k1(this->value.to_string());
    bitset<P + 1> k2(poly.to_string());
    bitset<P + 1> ktemp;
    bitset<P> x1(0);
    bitset<P> x2(0);
    bitset<P> temp;
    x1[0] = 1;
    int difPower;
    while (maxPower(k1))
    {
        difPower = maxPower(k1) - maxPower(k2);
        if (difPower < 0)
        {
            difPower = -difPower;
            ktemp = k1;
            k1 = k2;
            k2 = ktemp;
            temp = x1;
            x1 = x2;
            x2 = temp;
        }
        k1 ^= (k2 << difPower);
        x1 ^= (x2 << difPower);
    }
    bitset<P> r(RSTR);
    return x1;
}

// from fermat...
bitset<P> gf::in2()
{
    // a = 2^P - 2
    bitset<P> a("11111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111110");
    return this->exp(a);
}

// 快速幂算法算指数
bitset<P> gf::exp(bitset<P> e)
{
    gf ret;
    ret.set(1);
    if (!e.any())
    {
        return ret.returnValue();
    }
    while (e.any())
    {
        if (e[0] & 1)
        {
            ret.set(ret.mul(this->value));
        }
        this->set(this->squ());
        e >>= 1;
    }
    return ret.returnValue();
}