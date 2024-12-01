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
        {2, 1},
        {6, 9}};
    std::vector<double> rightSide = {13, 41};
    std::vector<int> signs = {1, 1};
    std::vector<double> func = {5, 7};
    bool toMax = true;
    if (!math.loadMatrix(matrix, signs, rightSide, func, toMax))
    {
        std::cout << "Ошибка при создании матрицы!\n";
        return 0;
    }
    math.printSystem(false);
    std::cout << "Максимальное целочисленное решение:\n";
    auto result = math.branchAndBoundaryMethodV(true, false, true);
    for (int i = 0; i < math.getNonBaseVarsCount(); i++)
        std::cout << "X" << i + 1 << "= " << result[i] << std::endl;
    for (int i = 0; i < result.size() - math.getNonBaseVarsCount(); i++)
        func.push_back(0);
    std::cout << "F = " << math.solveFunc(func, result) << std::endl;
}