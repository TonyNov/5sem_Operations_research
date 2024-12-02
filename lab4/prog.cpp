#include <iostream>
#include <fstream>
#include <vector>
#include <windows.h>
#include "TMath.cpp"
int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    TMath math;
    std::vector<std::vector<double>> matrix = {
        {2, 5},
        {4, 2}};
    std::vector<double> rightSide = {9, 9};
    std::vector<int> signs = {1, 1};
    std::vector<double> func = {3, 2};

    /* std::vector<std::vector<double>> matrix = {
        {0, 1, 0, 0, 0, 1},
        {1, 0, 0, 0, -1, 0},
        {2, 5, 1, 0, 0, 0},
        {4, 2, 0, 1, 0, 0}};
    std::vector<double> rightSide = {0, 2, 9, 9};
    std::vector<int> signs = {0, 0, 0, 0};
    std::vector<double> func = {3, 2, 0, 0, 0, 0}; */
    //     0 + b + f = 0
    // a -1e = 2
    // 2a + 5b + c = 9
    // 4a + 2b + d = 9
    // f = 3a + 2b -> max
    bool toMax = true;
    if (!math.loadMatrix(matrix, signs, rightSide, func, toMax))
    {
        std::cout << "Ошибка при создании матрицы!\n";
        return 0;
    }
    math.printSystem(false);
    // auto result = math.branchAndBoundaryMethodV(true, true, false);
    auto result = math.branchAndBoundaryMethod(true);
    if (result.size() == 0)
    {
        std::cout << "Решение не найдено!\n";
        return 0;
    }
    std::cout << "Лучшее целочисленное решение:\n";
    for (int i = 0; i < math.getNonBaseVarsCount(); i++)
        std::cout << "X" << i + 1 << "= " << result[i] << std::endl;
    for (int i = 0; i < result.size() - math.getNonBaseVarsCount(); i++)
        func.push_back(0);
    std::cout << "F = " << math.solveFunc(func, result) << std::endl;
}