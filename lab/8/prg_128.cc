#include <string>
#include <chrono>
#include <limits>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;
#define n 10

string dezero(string);
int judge(string, string);
string minu(string, string);
string mul(string, string);
string div(string, string);
string mod(string, string);
string modq(string, string, string);

int main()
{
    string p = "38276975027249116051285900801331379141255433763584992333552216611485936638623";
    string g = "15593026659801183422297598290145472779507237111936928372786422955200978648323";
    string q = "170141183460469231731688582683058187039";
    string s[513];
    s[0] = "20337251";
    string z;
    int num0 = 0, num1 = 0;
    auto start = chrono::high_resolution_clock::now();
    for (int i = 0; i < 512; i++)
    {
        s[i + 1] = mod(modq(g, s[i], p), q);
        z = mod(s[i + 1], "2");
        if (z == "0")
            num0++;
        if (z == "1")
            num1++;
        cout << z;
    }
    cout << "\n0: " << num0 << "\n1: " << num1 << endl;
    auto end = chrono::high_resolution_clock::now();
    std::chrono::duration<double> fp_ms = end - start;
    cout << "\nTime: " << fp_ms.count() << endl;
}

string dezero(string a)
{
    long int i;
    for (i = 0; i < a.length(); i++)
    {
        if (a.at(i) > 48)
        {
            break;
        }
    }
    if (i == a.length())
    {
        return "0";
    }
    a.erase(0, i);
    return a;
}

int judge(string a, string b)
{
    if (a.length() > b.length())
        return 1;
    if (a.length() < b.length())
        return -1;
    unsigned long long i;
    for (i = 0; i < a.length(); i++)
    {
        if (a.at(i) > b.at(i))
            return 1;
        if (a.at(i) < b.at(i))
            return -1;
    }
    return 0;
}

string minu(string a, string b)
{
    a = dezero(a);
    b = dezero(b);
    unsigned long long i, j = 0;
    string c = "0";
    string c1, c2;
    string d = "-";
    if (judge(a, b) == 0)
        return c;
    if (judge(a, b) == 1)
    {
        c1 = a;
        c2 = b;
    }
    if (judge(a, b) == -1)
    {
        c1 = b;
        c2 = a;
        j = -1;
    }
    reverse(c1.begin(), c1.end());
    reverse(c2.begin(), c2.end());
    for (i = 0; i < c2.length(); i++)
    {
        if (c2.at(i) >= 48 && c2.at(i) <= 57)
            c2.at(i) -= 48;
        if (c2.at(i) >= 97 && c2.at(i) <= 122)
            c2.at(i) -= 87;
    }
    for (i = 0; i < c1.length(); i++)
    {
        if (c1.at(i) >= 48 && c1.at(i) <= 57)
            c1.at(i) -= 48;
        if (c1.at(i) >= 97 && c1.at(i) <= 122)
            c1.at(i) -= 87;
    }
    for (i = 0; i < c2.length(); i++)
    {
        c1.at(i) = c1.at(i) - c2.at(i);
    }
    for (i = 0; i < c1.length() - 1; i++)
    {
        if (c1.at(i) < 0)
        {
            c1.at(i) += n;
            c1.at(i + 1)--;
        }
    }
    for (i = c1.length() - 1; i >= 0; i--)
    {
        if (c1.at(i) > 0)
            break;
    }
    c1.erase(i + 1, c1.length());
    for (i = 0; i < c1.length(); i++)
    {
        if (c1.at(i) >= 10)
            c1.at(i) += 87;
        if (c1.at(i) < 10)
            c1.at(i) += 48;
    }
    reverse(c1.begin(), c1.end());
    if (j == -1)
        c1.insert(0, d);
    return c1;
}

string mul(string a, string b)
{
    long int i, j, k, yao = 0, kai;
    string c1, c2;
    string c3 = a + b;
    if (a.at(0) == '-')
    {
        a.erase(0, 1);
        yao++;
    }
    if (b.at(0) == '-')
    {
        b.erase(0, 1);
        yao++;
    }
    a = dezero(a);
    b = dezero(b);
    if (a.at(0) == 48 || b.at(0) == 48)
        return "0";
    if (a.length() > b.length())
    {
        c1 = a;
        c2 = b;
    }
    else
    {
        c1 = b;
        c2 = a;
    }
    reverse(c1.begin(), c1.end());
    reverse(c2.begin(), c2.end());
    for (i = 0; i < c2.length(); i++)
    {
        if (c2.at(i) >= 48 && c2.at(i) <= 57)
            c2.at(i) -= 48;
        if (c2.at(i) >= 97 && c2.at(i) <= 122)
            c2.at(i) -= 87;
    }
    for (i = 0; i < c1.length(); i++)
    {
        if (c1.at(i) >= 48 && c1.at(i) <= 57)
            c1.at(i) -= 48;
        if (c1.at(i) >= 97 && c1.at(i) <= 122)
            c1.at(i) -= 87;
    }
    for (i = 0; i < c3.length(); i++)
        c3.at(i) = 0;
    for (i = 0; i < c2.length(); i++)
    {
        for (j = 0; j < c1.length(); j++)
        {
            kai = c2.at(i) * c1.at(j);
            c3.at(i + j + 1) += kai / n;
            c3.at(i + j) += kai % n;
            for (k = i + j; k < c3.length() - 1; k++)
            {
                if (c3.at(k) >= n)
                {
                    c3.at(k + 1) += c3.at(k) / n;
                    c3.at(k) = c3.at(k) % n;
                }
                else
                {
                    break;
                }
            }
        }
    }
    for (i = c3.length() - 1; i >= 0; i--)
    {
        if (c3.at(i) > 0)
            break;
    }
    c3.erase(i + 1, c3.length());
    for (i = 0; i < c3.length(); i++)
    {
        if (c3.at(i) >= 10)
            c3.at(i) += 87;
        if (c3.at(i) < 10)
            c3.at(i) += 48;
    }
    reverse(c3.begin(), c3.end());
    if (yao == 1)
        c3 = "-" + c3;
    return c3;
}

string div(string a, string b)
{
    if (b.length() == 1 && b.at(0) == 48)
        return "error";
    long int i, j;
    string c1, c2, d, e;
    if (judge(a, b) == 0)
        return "1";
    if (judge(a, b) == -1)
    {
        return "0";
    }
    c1 = dezero(a);
    c2 = dezero(b);
    d = "";
    e = "";
    for (i = 0; i < c1.length(); i++)
    {
        j = 0;
        d = d + c1.at(i);
        d = dezero(d);
        while (judge(d, b) >= 0)
        {
            d = minu(d, b);
            d = dezero(d);
            j++;
        }
        e = e + "0";
        e.at(i) = j;
    }
    for (i = 0; i < e.length(); i++)
    {
        if (e.at(i) >= 10)
            e.at(i) += 87;
        if (e.at(i) < 10)
            e.at(i) += 48;
    }
    e = dezero(e);
    return e;
}

string mod(string a, string b)
{
    long int i, j = 0;
    string c1, c2, c3, d;
    if (a.at(0) == '-')
        j = 1;
    if (judge(a, b) == 0)
        return "0";
    if (judge(a, b) == -1)
    {
        return a;
    }
    c1 = dezero(a);
    c2 = dezero(b);
    d = "";
    for (i = 0; i < c1.length(); i++)
    {
        d = d + c1.at(i);
        while (judge(d, b) >= 0)
        {
            d = minu(d, b);
            d = dezero(d);
        }
    }
    if (j == 1)
        d = minu(b, d);
    return d;
}

string modq(string a, string b, string m)
{
    string ans = "1";
    a = mod(a, m);
    while (b != "0")
    {
        if (mod(b, "2") == "1")
        {
            ans = mul(ans, a);
            ans = mod(ans, m);
        }
        b = div(b, "2");
        a = mul(a, a);
        a = mod(a, m);
    }
    return ans;
}
