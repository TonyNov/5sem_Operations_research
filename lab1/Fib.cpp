#include <iostream>
#include <cmath>
#include <windows.h>
#include <vector>

double f(float x)
{
   // return -pow(x, 2) - 10 * x + 5;
   return pow(x, 5) - 3 * pow(x, 4) - 51.01 * pow(x, 3) + 14.193 * pow(x, 2) + 323.2809 * x + 242.0409;
}
double fib(int n)
{
   return (pow((1 + sqrt(5)) / 2, n) - pow((1 - sqrt(5)) / 2, n)) / sqrt(5);
}
double calcByFib(double a, double b, double eps, bool min)
{
   int n = 1;
   while ((1.0 / fib(n)) > eps / b-a)
      n++;
   double x1 = a + (double)fib(n - 2) / fib(n) * (b - a);
   double x2 = a + (double)fib(n - 1) / fib(n) * (b - a);
   double f1 = f(x1);
   double f2 = f(x2);
   for (int k = n - 1; k > 1; k--)
      if ((f1<f2)==min)
      {
         b = x2;
         x2 = x1;
         f2 = f1;
         x1 = a + (double)fib(k - 2) / fib(k) * (b - a);
         f1 = f(x1);
      }
      else
      {
         a = x1;
         x1 = x2;
         f1 = f2;
         x2 = a + (double)fib(k - 1) / fib(k) * (b - a);
         f2 = f(x2);
      }
   return (x1 + x2) / 2;
}

int main()
{
   SetConsoleCP(1251);
   SetConsoleOutputCP(1251);
   float a0, b0, eps;
   std::cout << "‘ункци€ f(x)=x^5-3x^4-51.01x^3+14.193x^2+323.2809x+242.0409\n";
   std::cout << "¬ведите a и b:\n";
   std::cin >> a0 >> b0;
   while (b0 <= a0)
   {
      std::cout << "b должно быть больше a\n¬ведите a и b:\n";
      std::cin >> a0 >> b0;
   }
   std::cout << "¬ведите eps:\n";
   std::cin >> eps;
   double max = calcByFib(a0, b0, eps, false);
   double min = calcByFib(a0, b0, eps, true);
   std::cout << "min: " << min << '\n';
   std::cout << "f(min): " << f(min) << '\n';
   std::cout << "max: " << max << '\n';
   std::cout << "f(max): " << f(max) << '\n';
   system("PAUSE");
}
