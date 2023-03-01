#include <cmath>
#include <string>
#include <vector>
#include <time.h>
#include <iostream>
#include <windows.h>
using namespace std;

// string *makeGroup(string beforeText, int step)
// {
//     string *afterText = new string[step];
//     long length = beforeText.length();
//     for (long i = 0; i < length; i++)
//     {
//         afterText[i % step] += beforeText[i];
//     }
//     return afterText;
// };

// //求密钥长度，根据不同密钥长度对应的重合指数，正常文本为0.0635左右
// int GetKeyLength(string cipherText)
// {
//     //尝试1-20的密钥长度
//     double smallest = 1;
//     int keyLength = 0;
//     for (int i = 1; i < 20; i++)
//     {
//         double index = 0;
//         string *afterText = makeGroup(cipherText, i);
//         for (int j = 0; j < i; j++)
//         {
//             double num = 0;
//             // ASCII码97-122对应a-z
//             for (int p = 97; p < 123; p++)
//             {
//                 int count = 0;
//                 for (int q = 0; q < afterText[j].length(); q++)
//                 {
//                     if (afterText[j][q] == p)
//                     {
//                         count++;
//                     }
//                 }
//                 num += (double(count) / afterText[j].length()) * ((double(count) - 1) / (afterText[j].length() - 1));
//             }
//             index += num;
//         }
//         cout << "密钥长度为" << i << "时，重合指数=" << index / i << endl;
//     }
//     return keyLength;
// }

// //频率分析,确定e对应的密钥
// string frequencyCount(string cipherText, int step)
// {
//     string *afterText = makeGroup(cipherText, step);
//     string key;
//     for (int i = 0; i < step; i++)
//     {
//         // ASCII码97-122对应a-z
//         int E = 97;
//         double max = 0;
//         for (int j = 97; j < 123; j++)
//         {
//             int count = 0;
//             for (int k = 0; k < afterText[i].length(); k++)
//             {
//                 if (afterText[i][k] == j)
//                 {
//                     count++;
//                 }
//             }
//             cout << char(j) << "的频率=" << double(100 * count) / afterText[i].length() << endl;
//             if ((double(100 * count) / afterText[i].length()) > max)
//             {
//                 max = (double(100 * count) / afterText[i].length());
//                 E = j;
//             }
//         }
//         key += char(E);
//     }
//     return key;
// }

// //获取密钥
// string GetKey(string cipherText, int keyLength)
// {
//     string key = frequencyCount(cipherText, keyLength);
//     for (int i = 0; i < key.length(); i++)
//     {
//         key[i] = (key[i] - 101 + 26) % 26 + 'a';
//     }
//     cout << "根据频率分析法密钥最可能为：" << key << endl;
//     return key;
// }

// //密文解密，参数key为密钥
// void decrypt(string cipherText, string key)
// {
//     //分组
//     string *afterText = makeGroup(cipherText, key.length());
//     //对照密码表倒推替换
//     for (int i = 0; i < key.length(); i++)
//     {
//         for (int j = 0; j < afterText[i].length(); j++)
//         {
//             afterText[i][j] = (afterText[i][j] - key[i] + 26) % 26 + 'a';
//         }
//     }
//     //组装明文内容
//     string plainText;
//     for (int j = 0; j < afterText[0].length(); j++)
//     {
//         for (int i = 0; i < key.length() && j < afterText[i].length(); i++)
//         {
//             plainText += afterText[i][j];
//         }
//     }
//     cout << plainText << endl;
// }

// Hill /////////////////////////////////////////////

//用指针动态生成二维矩阵很
int **makeMatrix(int param, int param1)
{
    int **mat = new int *[param];
    for (int i = 0; i < param; i++)
    {
        mat[i] = new int[param1];
    }
    for (int i = 0; i < param; i++)
    {
        for (int j = 0; j < param1; j++)
        {
            mat[i][j] = 0;
        }
    }
    return mat;
}
//矩阵与矩阵相乘
std::string multiply(int **param, int **param1, int param2, int param3)
{
    std::string sum = "";
    int **mat3 = makeMatrix(param2, param3);
    for (int i = 0; i < param2; i++)
    {
        for (int j = 0; j < param3; j++)
        {
            for (int k = 0; k < param3; k++)
            {

                mat3[i][j] += param[i][k] * param1[k][j];
            }

            mat3[i][j] = mat3[i][j] % 26;
            sum += (int)mat3[i][j] + 'A';
        }
    }
    return sum;
}
//数字与矩阵相乘
int **multiply(int param, int **param1)
{

    // std::string sum="";
    int col = _msize(param1[1]) / sizeof(int);
    int **mat = makeMatrix(col, col);
    for (int i = 0; i < col; i++)
    {
        for (int j = 0; j < col; j++)
        {
            mat[i][j] = ((param1[i][j] * param) % 26 + 26) % 26;
        }
    }

    return mat;
}
//判断生成的矩阵是否合理
void judge(bool &param, int **param1, int param2)
{
    int det = caculate(param1, param2);
    if (det % 2 == 0 || det % 13 == 0 || det < 0)
    {
        param = true;
    }
    else
    {
        param = false;
    }
}

//加密函数入口
void getCode()
{
    std::string orignal;
    int num, zhong;
    bool flag = true;
    std::cout << "请输入原文：" << std::endl;
    std::cin >> orignal;
    std::cout << "请选择生成的密钥矩阵的阶数" << std::endl;
    std::cin >> num;
    int len = orignal.length() % num == 0 ? orignal.length() / num : orignal.length() / num + 1;
    int **mat1 = makeMatrix(num, num); //密码矩阵
    int **mat2 = makeMatrix(len, num); //明文矩阵
    //循环创建符合密码矩阵要求的秘钥
    while (flag)
    {
        Sleep(500);
        srand(time(NULL));
        for (int i = 0; i < num; i++)
        {
            for (int j = 0; j < num; j++)
            {
                zhong = rand() % 20;
                mat1[i][j] = zhong == 0 ? 1 : zhong;
            }
        }
        judge(flag, mat1, num);
    }
    //补充不能够形成矩阵的字符
    int k = 0, le = num - orignal.length() % num;
    for (int m = 0; m < le; m++)
    {
        orignal += "A";
    }
    //将字符转化为原文矩阵
    for (int i = 0; i < len; i++)
    {
        for (int j = 0; j < num; j++)
        {
            mat2[i][j] = (orignal[k] - 'A');
            k++;
        }
    }
    //矩阵相乘
    std::string mtext = multiply(mat2, mat1, len, num);
    std::cout << "加密矩阵" << std::endl;
    for (int i = 0; i < num; i++)
    {
        for (int j = 0; j < num; j++)
        {
            std::cout << mat1[i][j] << " ";
        }
        std::cout << "" << std::endl;
    }
    std::cout << "密文：" << mtext << std::endl;
}
// 注释：param参数为行列式的值mod26得到的值

int gradeMode(int param)
{

    for (int i = 1;;)
    {
        if ((26 * i + 1) % param == 0)
        {
            return (26 * i + 1) / param;
            break;
        }
        i += 1;
    }
}

int caculate(int **param, int param1)
{
    int row = param1;
    if (row == 2)
    {
        return param[0][0] * param[1][1] - param[0][1] * param[1][0];
    }
    else
    {
        int sum = 0;
        for (int i = 0; i < row; i++)
        {
            sum += param[0][i] * surplus(param, 0, i, param1);
        }
        return sum;
    }
}

int surplus(int **param, int x, int y, int param1)
{
    if (param1 == 2)
    {
        return pow(-1, x + y + 2) * param[1 - x][1 - y];
    }
    else
    {

        int row = param1, col = param1;
        int xflag = 0, yflag = 0;
        int **mat = makeMatrix(row - 1, col - 1);
        for (int i = 0; i < row; i++)
        {
            if (i != x)
            {
                yflag = 0;
                for (int j = 0; j < col; j++)
                {
                    if (j != y)
                    {
                        mat[xflag][yflag] = param[i][j];
                        yflag++;
                    }
                }
                xflag++;
            }
        }
        return pow(-1, x + y + 2) * caculate(mat, param1 - 1);
    }
}

void putcode()
{
    int num;
    std::string dense;
    std::cout << "请输入密文" << std::endl;
    std::cin >> dense;
    std::cout << "请输入阶数：" << std::endl;
    std::cin >> num;
    int **mat1 = makeMatrix(num, num);                  //接受加密矩阵
    int **mat2 = makeMatrix(dense.length() / num, num); //密文矩阵
    int **mat3 = makeMatrix(num, num);                  //

    std::cout << "请输入加密矩阵：" << std::endl;
    //获取加密矩阵
    for (size_t i = 0; i < num; i++)
    {
        for (size_t j = 0; j < num; j++)
        {
            std::cin >> mat1[i][j];
        }
    }

    //转换密文维密文矩阵
    int k = 0;
    for (size_t i = 0; i < dense.length() / num; i++)
    {
        for (size_t j = 0; j < num; j++)
        {
            mat2[i][j] = (dense[k] - 'A');
            k++;
        }
    }

    //算密码矩阵的伴随矩阵
    for (int k = 0; k < num; k++)
    {
        for (int j = 0; j < num; j++)
        {
            mat3[k][j] = surplus(mat1, j, k, num);
        }
    }

    int num1 = caculate(mat1, num) % 26;          //计算行列式
    int **mat4 = multiply(gradeMode(num1), mat3); //解码矩阵

    for (int k = 0; k < num; k++)
    {
        for (int j = 0; j < num; j++)
        {
            std::cout << mat4[k][j] << " ";
        }
        std::cout << "" << std::endl;
    }

    std::string original = multiply(mat2, mat4, dense.length() / num, num); //原文

    std::cout << "解密结果为：" << original;
}

/////////////////////////////////////////////

int main()
{
    // string hillText = "KCCPKBGUFDPHQTYAVINRRTMVGRKDNBVFDETDGILTXRGUDDKOTFMBPVGEGLTGCKQRACQCWDNAWCRXIZAKFTLEWRPTYCQKYVXCHKFTPONCQQRHJVAJUWETMCMSPKQDYHJVDAHCTRLSVSKCGCZQQDZXGSFRLSWCWSJTBHAFSIASPRJAHKJRJUMVGKMITZHFPDISPZLVLGWTFPLKKEBDPGCEBSHCTJRWXBAFSPEZQNRWXCVYCGAONWDDKACKAWBBIKFTIOVKCGGHJVLNHIFFSQESVYCLACNVRWBBIREPBBVFEXOSCDYGZWPFDTKFQIYCWHJVLNHIQIBTKHJVNPIST";
    // int klength = GetKeyLength(hillText);
    // string key = GetKey(hillText, klength);
    // decrypt(hillText, key);
    putcode();
}