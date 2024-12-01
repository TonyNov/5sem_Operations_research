#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <windows.h>
#include <fstream>
#define outFile std::cout
// #define outFile outStream
class TMath
{
private:
    std::ofstream outStream;
    std::vector<std::vector<double>> data;
    const double eps = 1e-20;
    bool toMax;
    std::vector<double> optimal;
    std::vector<int> baseVarsPlace;
    int nonbaseVarCount = 0;
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
            outFile << "Решение невозможно!" << std::endl;
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
    int findColumn(const std::vector<double> table)
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
    int findRow(const std::vector<std::vector<double>> table, int column)
    {
        int row = -1;
        double minRatio = INFINITY;
        for (int i = 0; i < table.size() - 1; i++)
            if (table[i][column] > eps)
            {
                double ration = table[i].back() / table[i][column];
                if (ration < minRatio && ration > 0)
                {
                    minRatio = ration;
                    row = i;
                }
            }
        return row;
    }
    std::vector<double> solveSimplex(std::vector<std::vector<double>> table, bool visibleSteps)
    {
        if (!table.size())
        {
            outFile << "Пустая матрица!\n";
            return {};
        }
        do
        {
            if (visibleSteps)
                printMat(table);
        } while (simplexStep(table));
        std::vector<double> values;
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
            values.push_back(value);
        }
        return values;
    }
    void printSystem(std::vector<std::vector<double>> matrix, bool hideBaseVars)
    {
        for (int i = 0; i < matrix.size() - 1; i++)
        {
            if (isInteger(matrix[i][0]))
            {
                switch ((int)matrix[i][0])
                {
                case 1:
                    outFile << 'a';
                    break;
                case -1:
                    outFile << "-a";
                    break;
                case 0:
                    outFile << "";
                    break;
                default:
                    outFile << matrix[i][0] << 'a';
                    break;
                }
            }
            else
                outFile << matrix[i][0] << 'a';
            if (hideBaseVars)
            {
                for (int j = 1; j < nonbaseVarCount; j++)
                    if (matrix[i][j])
                        if (matrix[i][j] < 0)
                            if (abs(matrix[i][j] != 1))
                                outFile << matrix[i][j] << (char)('a' + j);
                            else
                                outFile << (char)('a' + j);
                        else if (abs(matrix[i][j] != 1))
                            outFile << " + " << matrix[i][j] << (char)('a' + j);
                        else
                            outFile << " + " << (char)('a' + j);
                if (baseVarsPlace[i] == 0)
                    outFile << " = " << matrix[i].back() << std::endl;
                else if (baseVarsPlace[i] < 0)
                {
                    outFile << " >= " << matrix[i].back() << std::endl;
                }
                else
                    outFile << " <= " << matrix[i].back() << std::endl;
            }
            else
            {
                for (int j = 1; j < matrix[0].size() - 1; j++)
                    if (matrix[i][j])
                        if (matrix[i][j] < 0)
                            outFile << (abs(matrix[i][j]) != 1 ? matrix[i][j] : ' ') << (char)('a' + j);
                        else
                            outFile << " + " << (abs(matrix[i][j]) != 1 ? matrix[i][j] : ' ') << (char)('a' + j);
                outFile << " = " << matrix[i].back() << std::endl;
            }
        }
        outFile << "f = ";
        if (toMax)
        {
            outFile << -matrix.back()[0] << 'a';
            for (int j = 1; j < matrix[0].size() - 1; j++)
                if (matrix.back()[j])
                    if (matrix.back()[j] > 0)
                        outFile << -matrix.back()[j] << (char)('a' + j);
                    else
                        outFile << " + " << -matrix.back()[j] << (char)('a' + j);
            outFile << " -> max\n";
        }
        else
        {
            outFile << matrix.back()[0] << 'a';
            for (int j = 1; j < matrix[0].size() - 1; j++)
                if (matrix.back()[j])
                    if (matrix.back()[j] < 0)
                        outFile << matrix.back()[j] << (char)('a' + j);
                    else
                        outFile << " + " << matrix.back()[j] << (char)('a' + j);
            outFile << " -> min\n";
        }
    }
    void separateBranch(std::vector<std::vector<double>> table, std::string curBranch, bool findMax, bool visible, bool visibleSimplex, bool hideBaseVars)
    {
        if (visible)
        {
            outFile << "\n==Ветка " << curBranch << "==" << std::endl;
            if (visible)
            {
                outFile << std::endl;
                printSystem(table, hideBaseVars);
            }
        }
        std::vector<double> values;
        if (visibleSimplex)
            values = solveSimplex(table, true);
        else
            values = solveSimplex(table, false);
        auto f = solveFunc(table[table.size() - 1], values) * (toMax ? -1 : 1);
        int nonInteger = -1;
        for (int i = 0; i < values.size() - 1; i++)
            if (!isInteger(values[i]))
            {
                nonInteger = i;
                break;
            }
        if (nonInteger == -1)
        {
            if (visible)
            {
                for (int i = 0; i < nonbaseVarCount; i++)
                    outFile << "x" << (i + 1) << " = " << values[i] << std::endl;
                outFile << "F=" << f << std::endl;
                outFile << "Целочисленное решение.\n";
            }
            auto cur = solveFunc(table[table.size() - 1], optimal) * (toMax ? -1 : 1);
            if (!optimal.size())
            {
                for (int i = 0; i < nonbaseVarCount; i++)
                    optimal.push_back(values[i]);
                if (visible)
                    outFile << "Новое значение лучше предыдущего (oldF = " << solveFunc(data.back(), optimal) << "). Сохраняем.\n";
            }
            else if ((f > cur) == findMax)
            {
                for (int i = 0; i < nonbaseVarCount; i++)
                    optimal[i] = values[i];
                if (visible)
                    outFile << "Новое значение лучше предыдущего (oldF = " << solveFunc(data.back(), optimal) << "). Сохраняем.\n";
            }
        }
        else
        {
            if ((ceil(values[nonInteger]) == table[0].back() && table[0][table[0].size() - 2] == -1) || ceil(values[nonInteger]) - 1 == table[0].back() && table[0][table[0].size() - 2] == 1)
            {
                outFile << "Нет решения!\n";
                return;
            }
            std::vector<double> temp = std::vector<double>(table[0].size() - 1, 0);
            for (int i = 0; i < table.size(); i++)
            {
                table[i].push_back(table[i][table[i].size() - 1]);
                table[i][table[i].size() - 2] = 0;
            }
            temp[nonInteger] = 1;
            temp.push_back(-1);
            temp.push_back(ceil(values[nonInteger]));
            if (visible)
                for (int i = 0; i < nonbaseVarCount; i++)
                    outFile << "x" << (i + 1) << " = " << values[i] << std::endl;
            if (visible)
                outFile << "F=" << f << std::endl;
            else
            {
                outFile << "Нет решения!\n";
                return;
            }
            outFile << "Нецелочисленное решение. Разветвляемся\n";
            table.insert(table.begin(), temp);
            baseVarsPlace.insert(baseVarsPlace.begin(), (temp.size() - 2) * temp[temp.size() - 2]);
            separateBranch(table, curBranch + '1', findMax, visible, visibleSimplex, hideBaseVars);
            outFile << "\n==Откат в ветка " << curBranch << "==\n";
            table[0][table[0].size() - 2] = 1;
            table[0].back()--;
            baseVarsPlace[0] *= -1;
            separateBranch(table, curBranch + '2', findMax, visible, visibleSimplex, hideBaseVars);
            outFile << "\n==Откат==\n";
            table.erase(table.begin());
            baseVarsPlace.erase(baseVarsPlace.begin());
        }
    }

public:
    TMath()
    {
        outStream.open("TMath_output.txt");
    }
    /// @brief Загрузка матрицы для обработки (симплекс, метод ветвей и границ)
    /// @param matrix В каждой строке одно ограничение.Если переменная есть в другом ограничении или в целевой функции, то ставить ноль
    /// @param signs Количествоо элементов соответствует количеству строк matrix. 1 - "<=", -1 - ">=", 0 - "=="
    /// @param rightSide Значение в правой стороне
    /// @param func Количество элементов равно количеству элементов строки matrix минус 1 - с правой стороны значения нет.
    /// @param toMax true, если стремится к максимуму, false - к минимуму.
    bool loadMatrix(std::vector<std::vector<double>> matrix, std::vector<int> signs, std::vector<double> rightSide, std::vector<double> func, bool toMax = true)
    {
        for (auto elem : data)
            elem.clear();
        data.clear();
        baseVarsPlace.clear();
        if (matrix.size() != signs.size() || matrix[0].size() != func.size())
        {
            outFile << "Неверные параметры!";
            return false;
        }
        data = matrix;
        this->toMax = toMax;
        nonbaseVarCount = matrix.size();
        int baseVarCount = 0;
        int tempCounter = 0;
        for (int i = 0; i < signs.size(); i++)
            if (signs[i] != 0)
                baseVarCount++;
        for (int i = 0; i < data.size(); i++)
        {
            for (int j = 0; j < baseVarCount; j++)
                if (j == tempCounter && signs[i] != 0)
                {
                    data[i].push_back(signs[i]);
                    baseVarsPlace.push_back(signs[i] * data[i].size() - 1);
                }
                else
                    data[i].push_back(0);
            if (signs[i] != 0)
                tempCounter++;
            else
                baseVarsPlace.push_back(0);
            data[i].push_back(rightSide[i]);
        }
        if (toMax)
        {
            data.push_back({});
            for (int i = 0; i < func.size(); i++)
                data.back().push_back(-func[i]);
        }
        else
            data.push_back(func);
        for (int i = 0; i < baseVarCount + 1; i++)
            data.back().push_back(0);
        return true;
    }
    /// @brief Обработать мутрицу методом ветвей и границ. Без вывода в консоль
    std::vector<double> branchAndBoundaryMethod(bool findMax)
    {
        if (data.size())
        {
            separateBranch(data, "1", findMax, false, false, true);
            return optimal;
        }
        outFile << "Пустая матрица\n";
        return {};
    }
    /// @brief Обработать мутрицу методом ветвей и границ. С выводом в консоль
    std::vector<double> branchAndBoundaryMethodV(bool findMax, bool visibleSimplex, bool hideBaseVars = true)
    {
        if (data.size())
        {
            separateBranch(data, "1", findMax, true, visibleSimplex, hideBaseVars);
            return optimal;
        }
        outFile << "Пустая матрица\n";
        return {};
    }
    /// @brief Проверка, является ли double целым
    /// @param num дробное число
    /// @return true, если число четное
    bool isInteger(double num)
    {
        return std::abs(num - static_cast<int>(num)) < eps;
    }
    /// @brief Возвращает число число в числе Фибоначчи
    /// @param n номер искомого числа
    /// @return число
    double fib(int n)
    {
        return (pow((1 + sqrt(5)) / 2, n) - pow((1 - sqrt(5)) / 2, n)) / sqrt(5);
    }
    /// @brief Поиск экстремум функции используя метод Фибоначчи
    /// @param a левая граница
    /// @param b правая граница
    /// @param eps точность ответа
    /// @param min Ищем минимум?
    /// @param f функция float f(float)
    /// @return max/min в зависимости от парметра min
    double calcByFib(double a, double b, double eps, bool min, double (*f)(float))
    {
        int n = 1;
        while ((1.0 / fib(n)) > eps / b - a)
            n++;
        double x1 = a + (double)fib(n - 2) / fib(n) * (b - a);
        double x2 = a + (double)fib(n - 1) / fib(n) * (b - a);
        double f1 = f(x1);
        double f2 = f(x2);
        for (int k = n - 1; k > 1; k--)
            if ((f1 < f2) == min)
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
    /// @brief Поиск экстремум функции используя метод золотого сечения
    /// @param a левая граница
    /// @param b правая граница
    /// @param eps точность ответа
    /// @param min Ищем минимум?
    /// @param f функция float f(float)
    /// @return max/min в зависимости от парметра min
    double calcByGold(double a, double b, double eps, bool min, double (*f)(float))
    {
        double x1, x2, f1, f2;
        const double k = (sqrt(5) - 1) / 2;
        x1 = b - k * (b - a);
        x2 = a + k * (b - a);
        f1 = f(x1);
        f2 = f(x2);
        while ((b - a) >= eps)
        {
            if ((f1 < f2) == min)
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
    /// @brief Вывести пользовательскую матрицу красиво
    /// @param matrix
    void printMat(const std::vector<std::vector<double>> matrix)
    {
        if (matrix.size())
        {
            for (const auto &row : matrix)
            {
                for (double value : row)
                    outFile << std::setw(7) << value << "\t";
                outFile << std::endl;
            }
            outFile << std::endl;
        }
        else
            outFile << "Матрица пуста\n";
    }
    /// @brief Вывести матрицу красиво
    /// @param matrix
    void printMat()
    {
        if (data.size())
        {
            for (const auto &row : data)
            {
                for (double value : row)
                    outFile << std::setw(7) << value << "\t";
                outFile << std::endl;
            }
            outFile << std::endl;
        }
        else
            outFile << "Матрица пуста\n";
    }
    /// @brief Вывести систему равенств/неравенств
    void printSystem(bool hideBaseVars)
    {
        printSystem(data, hideBaseVars);
    }

    /// @brief Вывести равенство красиво. Последний жлемент массива - значение с правой стороны
    /// @param func
    void printFunc(const std::vector<double> func)
    {
        if (func.size() < 2)
            return;
        outFile << func[0] << ' ';
        for (int i = 1; i < func.size() - 1; i++)
        {
            if (func[i] < 0)
                outFile << func[i] << ' ';
            else
                outFile << " + " << func[i] << ' ';
        }
        outFile << "= " << func[func.size() - 1] << ' ';
    }
    /// @brief Расчет уравнения с одной неизвестной.
    /// @param func Уравнение вида ax+b+c+d..=0. При вводе неизвестного операнда вводится только коэффициент при нем.
    /// @param unknownVarIndex Номен неизвестного операнда
    /// @return Ответ
    double solveOneVarFunc(std::vector<double> func, int unknownVarIndex)
    {
        double solve = 0;
        for (int i = 0; i < func.size(); i++)
            if (i != unknownVarIndex)
                solve -= func[i];
        return solve / func[unknownVarIndex];
    }
    /// @brief Расчет функции вида ax1+bx2+cx3...zxn
    /// @param func Коэффициенты функции
    /// @param vars значения переменных
    /// @return Ответ
    double solveFunc(std::vector<double> func, std::vector<double> vars)
    {
        double result = 0;
        for (int i = 0; i < vars.size(); i++)
            result += func[i] * vars[i];
        return result;
    }
    /// @brief Обработать мутрицу симплексным методом. Без вывода
    std::vector<double> solveSimplex()
    {
        return solveSimplex(data, false);
    }
    /// @brief Обработать мутрицу симплексным методом. С выводом в консоль
    std::vector<double> solveSimplexV()
    {
        return solveSimplex(data, true);
    }
    int getNonBaseVarsCount()
    {
        return nonbaseVarCount;
    }
};
