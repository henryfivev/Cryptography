#include <bitset>
#include <iostream>
using namespace std;

int main()
{
    bitset<100> str1('b');
    bitset<100> str2("00110001001100010011000100110010");
    bitset<100> str3(1);
    bitset<100> str4("0");
    string str = "";
    
    for (int i = 0; i < 4; i++)
    {
        str += (str2.to_ulong());
    }
    cout << "str1 " << str1 << '\n';
    cout << "str2 " << str2.to_string() << '\n';
    cout << "str3 " << str3 << '\n';
    cout << "str4 " << str4 << '\n';
    cout << "str " << str << '\n';
}