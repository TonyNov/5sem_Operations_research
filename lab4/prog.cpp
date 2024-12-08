#include <iostream>
#include <fstream>
#include <vector>
#include <windows.h>
#include "TMath.cpp"
// #define output outStream
#define output std::cout
int main()
{
    std::ofstream outStream("TMath_output.txt");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    TMath math;
    std::vector<std::vector<double>> matrix = {
        {6, 9},
        {2, 1},
        {1, 0},
        {0, 1}};
    std::vector<double> rightSide = {41, 13, 0, 0};
    std::vector<int> signs = {1, 1, -1, -1};
    std::vector<double> func = {5, 7};
    bool toMax = true;
    if (!math.loadMatrix(matrix, signs, rightSide, func, toMax))
    {
        output << "Ошибка при создании матрицы!\n";
        return 0;
    }
    math.printSystem(true);
    auto result = math.branchAndBoundaryMethodV(true, false, false);
    if (result.size() == 0)
    {
        output << "Решение не найдено!\n";
        return 0;
    }
    output << "\nЛучшее целочисленное решение:\n";
    for (int i = 0; i < math.getNonBaseVarsCount(); i++)
        output << "X" << i + 1 << "= " << result[i] << std::endl;
    for (int i = 0; i < result.size() - math.getNonBaseVarsCount(); i++)
        func.push_back(0);
    output << "F = " << math.solveFunc(func, result) << std::endl;
}