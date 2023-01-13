// 孙高锦的计网作业
#include <iostream>
using namespace std;

long long f(int n) {
    long long  res = 1;
    for(int i = 1; i < n; ++ i) {
        res *= (i + 1);
    }

    return res;
}

long long c(int n) {
    if(n == 0) {
        return 1;
    }
    else {
        long long a = f(n);
        long long b = 1;
        for(int i = 0; i < n; ++ i) {
            b *= (35 - i);
        }
        return b / a;
    }
}

double ff(double a, int n) {
    if(n == 0) return 1;
    double res = a;
    for(int i = 1; i < n; ++ i) {
        res *= a;
    }
    return res;
}

int main() {
    double res = 0;
    for(int i = 0; i < 11; ++ i) {
        res += (c(i) * ff(0.9, 35 - i) * ff(0.1, i));
    }
    cout << res;
}