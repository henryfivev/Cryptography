#include <iostream>
#include <string>
#include <vector>
#include <map>
using namespace std;

string hill_cipher_encrypt(string &, vector<vector<int>> &); //希尔密码加密
string hill_cipher_decrypt(string &, vector<vector<int>> &); //希尔密码解密
void affine_hill(string &, string &);
void hill_cipher_calculate_key(string &, string &);										//希尔密码求秘钥（仅3阶矩阵）
void print_matrix(vector<vector<int>> &);												//打印矩阵
vector<vector<int>> matrix_mod26_mutiply(vector<vector<int>> &, vector<vector<int>> &); //矩阵模26乘运算
int matrix_mod26_mutiply_sub(vector<vector<int>> &, vector<vector<int>> &, int, int);	//矩阵模26乘运算子函数（v1的第i行乘v2的第j列）
int inverse_matrix_mod26_sub(int i, int j, vector<vector<int>> &v);						//求3阶矩阵在模26下的逆	子函数	求代数余子式
vector<vector<int>> inverse_matrix_mod26(vector<vector<int>> &v);						//求3阶矩阵在模26下的逆

int main()
{
	//明文
	string plaintext = "adisplayedequation";
	//密文
	string ciphertext = "DSRMSIOPLXLJBZULLM";

	for (int i = 0; i < 18; i++)
	{
		cout << plaintext[i] - 'a' << ' ';
	}
	cout << '\n';
	for (int i = 0; i < 18; i++)
	{
		cout << ciphertext[i] - 'A' << ' ';
	}
	cout << '\n';

	//求解秘钥
	affine_hill(plaintext, ciphertext);
}

//打印矩阵
void print_matrix(vector<vector<int>> &v)
{
	for (auto a : v)
	{
		for (auto b : a)
			cout << b << " ";
		cout << '\n';
	}
}

//矩阵模26乘运算
vector<vector<int>> matrix_mod26_mutiply(vector<vector<int>> &v1, vector<vector<int>> &v2)
{
	vector<vector<int>> v;
	for (int i = 0; i < 3; i++)
	{
		vector<int> v_tp;
		for (int j = 0; j < 3; j++)
		{
			int tp = matrix_mod26_mutiply_sub(v1, v2, i, j);
			v_tp.push_back(tp);
		}
		v.push_back(v_tp);
	}
	return v;
}

//矩阵模26乘运算子函数（v1的第i行乘v2的第j列）
int matrix_mod26_mutiply_sub(vector<vector<int>> &v1, vector<vector<int>> &v2, int i, int j)
{
	int res = 0;
	for (int k = 0; k < 3; k++)
		res += v1[i][k] * v2[k][j];
	res = (res % 26 + 26) % 26;
	return res;
}

//求3阶矩阵在模26下的逆
vector<vector<int>> inverse_matrix_mod26(vector<vector<int>> &v)
{
	//求模26行列式值
	int A = v[0][0] * inverse_matrix_mod26_sub(0, 0, v) + v[0][1] * inverse_matrix_mod26_sub(0, 1, v) + v[0][2] * inverse_matrix_mod26_sub(0, 2, v);
	A = (A % 26 + 26) % 26;
	map<int, int> inverse;
	//模A可逆情况，其他情况不可逆
	inverse.insert(pair<int, int>(1, 1));
	inverse.insert(pair<int, int>(3, 9));
	inverse.insert(pair<int, int>(5, 21));
	inverse.insert(pair<int, int>(7, 15));
	inverse.insert(pair<int, int>(9, 3));
	inverse.insert(pair<int, int>(11, 19));
	inverse.insert(pair<int, int>(15, 7));
	inverse.insert(pair<int, int>(17, 23));
	inverse.insert(pair<int, int>(19, 11));
	inverse.insert(pair<int, int>(21, 5));
	inverse.insert(pair<int, int>(23, 17));
	inverse.insert(pair<int, int>(25, 25));
	if (inverse.find(A) == inverse.end())
	{
		cout << "矩阵模26不可逆" << '\n';
		return v;
	}

	int rate = inverse[A];
	cout << "行列式: " << A << '\n';
	vector<vector<int>> v_mod26_inverse;
	for (int i = 0; i < 3; i++)
	{
		vector<int> v_tp;
		for (int j = 0; j < 3; j++)
		{
			int tp = (rate * inverse_matrix_mod26_sub(j, i, v) % 26 + 26) % 26;
			v_tp.push_back(tp);
		}
		v_mod26_inverse.push_back(v_tp);
	}
	cout << '\n'
		 << "逆矩阵: " << '\n';
	print_matrix(v_mod26_inverse);
	cout << '\n'
		 << "检验结果: " << '\n';
	vector<vector<int>> muti = matrix_mod26_mutiply(v, v_mod26_inverse);
	print_matrix(muti);
	return v_mod26_inverse;
}

//求3阶矩阵在模26下的逆的子函数
int inverse_matrix_mod26_sub(int i, int j, vector<vector<int>> &v)
{
	int i1 = 0;
	int i2 = 0;
	int j1 = 0;
	int j2 = 0;
	switch (i)
	{
	case 0:
		i1 = 1;
		i2 = 2;
		break;
	case 1:
		i1 = 0;
		i2 = 2;
		break;
	case 2:
		i1 = 0;
		i2 = 1;
		break;
	}
	switch (j)
	{
	case 0:
		j1 = 1;
		j2 = 2;
		break;
	case 1:
		j1 = 0;
		j2 = 2;
		break;
	case 2:
		j1 = 0;
		j2 = 1;
		break;
	}
	//求代数余子式
	int res = (i + j) % 2 == 0 ? 1 : -1;
	res *= (v[i1][j1] * v[i2][j2] - v[i1][j2] * v[i2][j1]);
	return res;
}

//希尔密码求秘钥
void hill_cipher_calculate_key(string &plaintext, string &chipertext)
{
	vector<vector<int>> v1, v2;
	vector<int> tp1, tp2;
	for (int i = 0; i < 10; i++)
	{

		if (i % 3 == 0 && i != 0)
		{
			v1.push_back(tp1);
			v2.push_back(tp2);
			tp1.clear();
			tp2.clear();
		}
		tp1.push_back(plaintext[i] - 'a');
		tp2.push_back(chipertext[i] - 'A');
	}
	vector<vector<int>> v0 = inverse_matrix_mod26(v1);
	vector<vector<int>> v = matrix_mod26_mutiply(v0, v2);
	cout << "秘钥: " << '\n';
	print_matrix(v);
}

//希尔密码加密
string hill_cipher_encrypt(string &text, vector<vector<int>> &v)
{
	if (text == "")
		return "error";

	string target = "";
	cout << "希尔密码明文:" << '\n';
	cout << text << '\n'
		 << '\n';
	cout << "希尔密码秘钥:" << '\n';
	print_matrix(v);
	int counter = 0;
	cout << '\n';
	for (unsigned int i = 0; i < text.size(); i += 3)
	{
		char c1 = text.at(i) - 97;
		char c2 = text.at(i + 1) - 97;
		char c3 = text.at(i + 2) - 97;
		int a1 = c1;
		int a2 = c2;
		int a3 = c3;
		for (int j = 0; j < 3; j++)
		{
			int tp = c1 * v[0][j] + c2 * v[1][j] + c3 * v[2][j];
			tp = (tp % 26 + 26) % 26 + 65;
			char c = tp;
			target += c;
		}
	}
	cout << "希尔密码加密求得密文：" << '\n';
	cout << target << '\n';
	return target;
}

//希尔密码解密
string hill_cipher_decrypt(string &text, vector<vector<int>> &v)
{
	if (text == "")
	{
		return "error";
	}

	string target = "";
	cout << "希尔密码密文:" << '\n' << text << '\n' << '\n'
		 << "希尔密码秘钥逆矩阵:" << '\n';
	print_matrix(v);
	int counter = 0;
	for (unsigned int i = 0; i < text.size(); i += 3)
	{
		char c1 = text.at(i) - 65;
		char c2 = text.at(i + 1) - 65;
		char c3 = text.at(i + 2) - 65;
		for (int j = 0; j < 3; j++)
		{
			int tp = c1 * v[0][j] + c2 * v[1][j] + c3 * v[2][j];
			tp = (tp % 26 + 26) % 26 + 97;
			char c = tp;
			target += c;
		}
	}
	cout << "希尔密码解密求得明文: " << '\n';
	cout << target << '\n';
	return target;
}

//仿射希尔密码求密钥
void affine_hill(string &plaintext, string &chipertext)
{
	string pt, ct;
	string pt1 = plaintext.substr(0, 9);
	string pt2 = plaintext.substr(9, 9);
	string ct1 = chipertext.substr(0, 9);
	string ct2 = chipertext.substr(9, 9);
	for (int i = 0; i < 9; i++)
	{
		int p = pt2[i] - pt1[i] + 52;
		int c = ct2[i] - ct1[i] + 52;
		const char np = (p % 26) + 'a';
		const char nc = (c % 26) + 'A';
		pt += np;
		ct += nc;
	}
	cout << pt << '\n';
	hill_cipher_calculate_key(pt, ct);
}