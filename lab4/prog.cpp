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
    auto optimal = math.branchAndBoundaryMethodV();
    for (int i = 0; i < optimal.size(); i++)
        std::cout << "X" << i + 1 << "= " << optimal[i] << std::endl;
    math.solveFunc(func, optimal); 
}