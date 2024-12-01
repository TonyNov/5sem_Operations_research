#include <iostream>
#include <cmath>
#include <windows.h>
double f(double x)
{
   return pow(x, 5) - 3 * pow(x, 4) - 51.01 * pow(x, 3) + 14.193 * pow(x, 2) + 323.2809 * x + 242.0409;
   // return -pow(x, 2) - 10 * x + 5;
}
double calcByGold(double a, double b, double eps,bool min)
{
   double x1, x2, f1, f2;
   const double k = (sqrt(5) - 1) / 2;
   x1 = b - k * (b - a);
   x2 = a + k * (b - a);
   f1 = f(x1);
   f2 = f(x2);
   while ((b - a) >= eps)
   {
      if ((f1 < f2)==min)
      {
         b = x2;
         x2 = x1;
         f2 = f1;
         x1 = b - k * (b - a);
         f1 = f(x1);
      }
      else
      {
         a = x1;
         x1 = x2;
         f1 = f2;
         x2 = a + k * (b - a);
         f2 = f(x2);
      }
   }
   return (a + b) / 2;
}
int main()
{
   SetConsoleCP(1251);
   SetConsoleOutputCP(1251);
   double eps, a0, b0;
   std::cout << "‘ункци€ f(x)=x^5-3x^4-51.01x^3+14.193x^2+323.2809x+242.0409\n";
   std::cout << "¬ведите a и b:\n";
   std::cin >> a0 >> b0;
   while (b0 <= a0)
   {
      std::cout << "b дложно быть больше a\n¬ведите a и b:\n";
      std::cin >> a0 >> b0;
   }
   std::cout << "¬ведите eps:\n";
   std::cin >> eps;
   double min=calcByGold(a0,b0,eps,true);
   double max=calcByGold(a0,b0,eps,false);
   std::cout << "min: " << min << '\n';
   std::cout << "f(min): " << f(min) << '\n';
   std::cout << "max: " << max << '\n';
   std::cout << "f(max): " << f(max) << '\n';
   system("PAUSE");
}
