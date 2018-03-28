// #include <bits/stdc++.h>
#include <vector>
#include <iostream>
#include <complex>
using namespace std;
typedef complex<double> cd;
typedef vector<cd> vcd;

vcd fft(const vcd &as);
void reverseFFT(int n, vector<complex<double> > &coef, vector<complex<double> > &roots);
void multFFT(vector<int> &a, vector<int> &b, vector<int> &c);

extern "C" {
  void fft_interface(complex<double> *data, unsigned long n) {
    cout << "beginning native fourier" << endl;
    vcd input(n);
    for (size_t i = 0; i < n; ++i) {
      input[i] = data[i];
    }
    vcd output = fft(input);
    for (size_t i = 0; i < n; ++i) {
      data[i] = output[i];
    }
    cout << "finished native fourier" << endl;
  }
}
