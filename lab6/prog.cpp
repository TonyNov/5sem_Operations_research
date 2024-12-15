#include <iostream>
auto max = [](int a, int b) { return a > b ? a : b; };
int f(int w)
{
    return w <= 0 ? 0 : max(f(w - 1) + 70, max(f(w - 2) + 20, f(w - 3) + 40));
}
int main()
{
    std::cout << "Maximum profit: " << f(6) << std::endl;
}