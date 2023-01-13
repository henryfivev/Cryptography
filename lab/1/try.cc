#include<iostream>
#include<string>
using namespace std;

int main()
{
    string a = "abc";
    string b = "bcd";
    int c = b[0] - a[0];
    int mod = -2 % 26;
    cout << mod;
}