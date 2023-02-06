#include <iostream>
#include <cmath>
#include <vector>
#include<cassert>

using namespace std;

vector<int> x_extract(int x, int r)
{
    vector<int> x_arr;
    int a;
    int b;
    do
    {
        a = x / r;
        b = x % r;
        x = a;
        x_arr.emplace_back(b);
    } while (a != 0);

    return x_arr;
}

int mod_inv(int a, int r, int fai_r)
{
    // 如果m是质数
    int temp = pow(a, fai_r - 1);
    return temp % r;
}

int Montgomery(int r, int n, int x, int y)
{
    assert( (x < n) && (y < n) );
    int n_zero = n % r;
    int fai_r = 4; // fai_r = 10 x (1-1/2) x (1-1/5) = 4;
    int n_zero_inv = mod_inv(n_zero, r, fai_r);
    // info: (r- n[0])',关于模r的逆，与 r- n[0]'关于模r的逆结果是一样的
    int p = r - n_zero_inv;
    vector<int> x_arr;
    x_arr = x_extract(x, r);
    int s = 0;
    for (int i = 0; i < x_arr.size(); i++)
    {
        int q = (((s % r) + x_arr[i] * (y % r)) * p) % r;
        s = s + x_arr[i] * y + q * n;
        s = s / r;
        if (s >= n)
        {
            s -= n;
            if (s >= n)
            {
                s -= n;
            }
        }
    }
    return s;
}
int get_R(int r, int n)
{
    int R = r;
    while (R < n)
    {
        R *= r;
    }
    return R;
}
int mod_mul(int r, int n, int x, int y)
{
    assert( (x < n) && (y < n) );
    int R = get_R(r, n);
    int R_mul_R_mod_N = R * R % n;
    int x_pie = Montgomery(r, n, x, R_mul_R_mod_N);
    int y_pie = Montgomery(r, n, y, R_mul_R_mod_N);
    int z_pie = Montgomery(r, n, x_pie, y_pie);
    int z = Montgomery(r, n, z_pie, 1);
    return z;
}

int mod_exp(int r, int n, int a, int e)
{
    int R = get_R(r, n);
    int a_pie = Montgomery(r, n, a, ((R % n) * (R % n)) % n);
    int x = R % n;
    vector<int> e_arr = x_extract(e, 2);
    for (int i = e_arr.size() - 1; i >= 0; i--)
    {
        x = Montgomery(r, n, x, x);
        if(e_arr[i] == 1){
            x = Montgomery(r, n, x, a_pie);
        }
    }
    x = Montgomery(r, n, x, 1);
    return x;
}

int main()
{
    // int r = 10, n = 79, x = 12, y = 22;
    int r = 10, n = 79, x = 17, y = 26;
    int result = mod_mul(r, n, x, y);
    cout << result << endl;

    x = 17, y = 21;
    result = mod_mul(r, n, x, y);
    cout << result << endl;

    int a = 17, e = 26;
    result = mod_exp(r, n, a, e);
    cout << result << endl;
}
