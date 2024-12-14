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
    std::cout << "������� �����������?  ";
    std::cin >> ak;
    std::cout << "������� ����� Enter �������� �����������\n";
    for (int i = 0; i < ak; i++)
    {
        std::string temp;
        std::cin >> temp;
        am.push_back(temp);
    }
    std::cout << "������� ��������� �������?  ";
    std::cin >> bk;
    std::cout << "������� ����� Enter ����������� ��������� �������\n";
    for (int i = 0; i < bk; i++)
    {
        double b;
        std::cin >> b;
        p.push_back(b);
    }
    std::cout << "������� ����� ��� ������ ������������� � ����������:\n";
    for (int i = 0; i < ak; i++)
    {
        std::cout << am[i]<<":\n";
        double solve = 0, temp;
        for (int j = 0; j < bk; j++)
        {
            std::cout << "\t��������� " << j+1 << ": ";
            std::cin >> temp;
            solve += temp * p[j];
        }
        result.push_back(solve);
    }
    std::cout << "���������:\n";
    int maxi = 0;
    for (int i = 0; i < ak; i++)
    {
        std::cout << am[i] << ": " << result[i] << std::endl;
        if (result[i] > result[maxi])
        {
            maxi = i;
        }
    }
    std::cout << "��������� ����� - " << am[maxi] << " � ������� " << result[maxi] << std::endl;
}