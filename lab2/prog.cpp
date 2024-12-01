#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <windows.h>
const double eps = 1e-20;
void printMat(const std::vector<std::vector<double>> &matrix)
{
    for (const auto &row : matrix)
    {
        for (double value : row)
            std::cout << std::setw(5) << value << "\t";
        std::cout << std::endl;
    }
    std::cout << std::endl;
}
int findColumn(const std::vector<double> &table)
{
    int column = -1;
    double min = 0;
    for (int i = 0; i < table.size() - 1; i++)
        if (table[i] < min)
        {
            min = table[i];
            column = i;
        }
    return column;
}
int findRow(const std::vector<std::vector<double>> &table, int column)
{
    int row = -1;
    double minRatio = INFINITY;
    for (int i = 0; i < table.size() - 1; i++)
        if (table[i][column] > eps)
        {
            double ration = table[i].back() / table[i][column];
            if (ration < minRatio&&ration>0)
            {
                minRatio = ration;
                row = i;
            }
        }
    return row;
}
bool simplexStep(std::vector<std::vector<double>> &table)
{
    int limitCount = table.size() - 1;
    int varCount = table[0].size() - 1;
    int column = findColumn(table[limitCount]);
    if (column == -1)
        return false;
    int row = findRow(table, column);
    if (row == -1)
    {
        std::cout << "������� ����������!" << std::endl;
        return false;
    }
    double elem = table[row][column];
    for (int j = 0; j <= varCount; j++)
        table[row][j] /= elem;
    for (int i = 0; i <= limitCount; i++)
        if (i != row)
        {
            double ratio = table[i][column];
            for (int j = 0; j <= varCount; ++j)
                table[i][j] -= ratio * table[row][j];
        }
    return true;
}
void solveSimplex(std::vector<std::vector<double>> &table)
{
    while (true)
    {
        printMat(table);
        if (!simplexStep(table))
            break;
    }
    std::cout << "����������� �������:" << std::endl;
    for (int i = 0; i < table[0].size() - 1; ++i)
    {
        bool isBasic = false;
        double value = 0;
        for (int j = 0; j < table.size() - 1; ++j)
            if (abs(table[j][i] - 1) < eps)
            {
                value = table[j].back();
                break;
            }
        std::cout << "x" << (i + 1) << " = " << value << std::endl;
    }
}
int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    std::vector<std::vector<double>> table = {
        {2, -1, 0, -2, 1, 0, 16},
        {3, 2, 1, -3, 0, 0, 18},
        {-1, 3, 0, 4, 0, 1, -24},
        {-2, -3, 0, 1, 0, 0, 0}};
    solveSimplex(table);
    system("pause");
}
