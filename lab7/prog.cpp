#include <iostream>
#include <windows.h>
#include <vector>
int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    std::vector<double> p;
    std::vector<std::string> am;
    std::vector<double> result;
    int ak, bk;
    std::cout << "Сколько альтернатив?  ";
    std::cin >> ak;
    std::cout << "Введите через Enter названия альтернатив\n";
    for (int i = 0; i < ak; i++)
    {
        std::string temp;
        std::cin >> temp;
        am.push_back(temp);
    }
    std::cout << "Сколько состояний природы?  ";
    std::cin >> bk;
    std::cout << "Введите через Enter вероятности состояний природы\n";
    for (int i = 0; i < bk; i++)
    {
        double b;
        std::cin >> b;
        p.push_back(b);
    }
    std::cout << "Введите доход при разных альтернативах и состояниях:\n";
    for (int i = 0; i < ak; i++)
    {
        std::cout << am[i]<<":\n";
        double solve = 0, temp;
        for (int j = 0; j < bk; j++)
        {
            std::cout << "\tСостояние " << j+1 << ": ";
            std::cin >> temp;
            solve += temp * p[j];
        }
        result.push_back(solve);
    }
    std::cout << "Результат:\n";
    int maxi = 0;
    for (int i = 0; i < ak; i++)
    {
        std::cout << am[i] << ": " << result[i] << std::endl;
        if (result[i] > result[maxi])
        {
            maxi = i;
        }
    }
    std::cout << "Наилучший выбор - " << am[maxi] << " с доходом " << result[maxi] << std::endl;
}