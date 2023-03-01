#include <string>
#include <vector>
#include <iostream>
using namespace std;

// 维吉尼亚解密_获得密钥长度
int getKeyLength(string c)
{
    int klen = 1;          //密钥长度
    int clen = c.length(); //密文的长度
    while (1)
    {
        float IC[klen];                //重合指数
        float avgIC = 0;               //平均重合指数
        for (int i = 0; i < klen; i++) //统计分组字母个数
        {
            int out[26] = {0}; //盛放字母个数的数组
            for (int j = 0; i + j * klen < clen; j++)
            {
                out[(int)(c[i + j * klen] - 'A')]++;
            }
            float e = 0.000f;
            int L = 0;
            for (int k = 0; k < 26; k++) //子串密文长度
            {
                L += out[k];
            }
            L *= (L - 1);
            for (int k = 0; k < 26; k++) //分组计算重合指数IC
            {
                if (out[k] != 0)
                {
                    e = e + ((float)out[k] * (float)(out[k] - 1)) / (float)L;
                }
            }
            IC[i] = e;
        }
        for (int i = 0; i < klen; i++)
        {
            avgIC += IC[i];
        }
        avgIC /= klen; //求IC的平均值
        if (avgIC >= 0.06)
        {
            break;
        } //判断退出条件，重合指数的平均值是否大于0.06
        else
        {
            klen++;
        }
    }
    cout << "密钥长度为: " << klen << endl;
    return klen;
}

// 维吉尼亚解密_获得密钥
string getKey(string c, int klen)
{
    int clen = c.length();
    float p[] = {0.082, 0.015, 0.028, 0.043, 0.127, 0.022, 0.02, 0.061, 0.07, 0.002, 0.008, 0.04, 0.024, 0.067, 0.075, 0.019, 0.001, 0.06, 0.063, 0.091, 0.028, 0.01, 0.023, 0.001, 0.02, 0.001};
    int key[100] = {0};            //存放密钥
    for (int i = 0; i < klen; i++) //统计分组字母个数
    {
        int g = 0; //密文移动g个位置
        for (int t = 0; t < 26; t++)
        {
            float x = 0.000f;  //拟重合指数
            int out[26] = {0}; //盛放字母个数的数组
            for (int j = 0; i + j * klen < clen; j++)
            {
                out[(int)(c[i + j * klen] - 'A')]++;
            }
            int L = 0;
            for (int k = 0; k < 26; k++) //子串密文长度
            {
                L += out[k];
            }
            for (int k = 0; k < 26; k++)
            {
                x = x + p[k] * out[(k + g) % 26];
            }
            if (x / L > 0.055)
            {
                key[i] = g;
                break;
            }
            else
            {
                g++;
            }
        }
    }
    string ret;
    cout << "加密密钥为: ";
    for (int i = 0; i < klen; i++) //输出密钥字
    {
        cout << char('A' + key[i]);
        ret += ('A' + key[i]);
        // string的+=可将char塞入string, append不行
    }
    cout << '\n';
    return ret;
}

// 维吉尼亚加密_
string eVernam(string str, string key)
{
    //仅处理str全为大写的情况
    string ret;
    for (int i = 0; i < str.length(); i++)
    {
        for (int j = 0; j < key.length(); j++, i++)
        {
            if (i >= str.length())
            {
                return ret;
            }
            else
            {
                ret += ((str[i] + (key[j] - 'A') - 'A') % 26) + 'A';
            }
        }
    }
    return ret;
}

// 维吉尼亚解密_
void dVernam(string str)
{
    string key = getKey(str, getKeyLength(str));
    ;
    string plaintext;
    int cont = 0;
    int c_len = str.length();
    int k_len = key.length();
    for (int i = 0; i < c_len; i++)
    {
        if (!isalpha(str[i]))
        {
            cont++;
            plaintext += str[i];
        }
        else
        {
            int j = (i - cont) % k_len;
            int n = key[j] - 'A';
            plaintext += (str[i] - 'A' + 26 - n) % 26 + 'A';
        }
    }
    cout << plaintext << '\n';
}

int main()
{
    string afterVernam = "KCCPKBGUFDPHQTYAVINRRTMVGRKDNBVFDETDGILTXRGUDDKOTFMBPVGEGLTGCKQRACQCWDNAWCRXIZAKFTLEWRPTYCQKYVXCHKFTPONCQQRHJVAJUWETMCMSPKQDYHJVDAHCTRLSVSKCGCZQQDZXGSFRLSWCWSJTBHAFSIASPRJAHKJRJUMVGKMITZHFPDISPZLVLGWTFPLKKEBDPGCEBSHCTJRWXBAFSPEZQNRWXCVYCGAONWDDKACKAWBBIKFTIOVKCGGHJVLNHIFFSQESVYCLACNVRWBBIREPBBVFEXOSCDYGZWPFDTKFQIYCWHJVLNHIQIBTKHJVNPIST";
    vector<vector<int>> hkey = {{1, 2, 3}}; // 错误密钥, 应kxk大小
    dVernam(afterVernam);
    cout << eVernam("AAA", "ABC") << '\n';
    // cout << 'b'-'a'; 单引号和双引号不同
}

// 希尔密码note
// key一般 kxk 大小
// 计算矩阵的逆, 即逆矩阵
// 解密需要明文, 密文, k, 用这三个解出key