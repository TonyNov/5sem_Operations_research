// https://github.com/LIvanoff/Transportation-theory/blob/master/Transportation%20theory/Transportation%20theory.cpp
#include <vector>
#include <iomanip>
#include <iostream>
#include <windows.h>
using namespace std;

class Matrix
{
private:
    int size_row;
    int size_column;
    int potential_i;
    int potential_j;
    bool way = false;
    bool sign = 1;
    int minimal_element;
    vector<vector<int>> matrix;
    vector<vector<int>> free_matrix;
    vector<vector<int>> matrix_plus_minus;
    vector<int> u_vec;
    vector<int> v_vec;

public:
    vector<vector<int>> wastes;
    vector<int> stocks;
    vector<int> needs;
    Matrix(int size_row, int size_column)
    {
        this->size_row = size_row;
        this->size_column = size_column;
        for (int i = 0; i < size_row; i++)
        {
            matrix.push_back(vector<int>(size_column, 0));
            free_matrix.push_back(vector<int>(size_column, 0));
            matrix_plus_minus.push_back(vector<int>(size_column, -1));
        }
        for (int i = 0; i < size_row; i++)
            u_vec.push_back(0);
        for (int i = 0; i < size_column; i++)
            v_vec.push_back(0);
    }
    void northwestCorner()
    {
        vector<int> newNeeds = needs;
        vector<int> newStocks = stocks;
        for (int i = 0; i < this->size_row; i++)
        {
            for (int j = 0; j < this->size_column; j++)
            {
                if (newStocks.at(i) < newNeeds.at(j) && newNeeds.at(j) != 0)
                {
                    matrix[i][j] += newStocks[i];
                    newNeeds[j] -= newStocks[i];
                    newStocks[i] = 0;
                }
                else if (newStocks.at(i) == newNeeds.at(j) && newStocks.at(i) != 0 && newNeeds.at(j) != 0)
                {
                    matrix[i][j] = newNeeds[j];
                    newStocks[i] = 0;
                    newNeeds[i] = 0;
                }
                else if (newStocks.at(i) > newNeeds.at(j) && newStocks.at(i) != 0 && newNeeds.at(j) != 0)
                {
                    matrix[i][j] += newStocks.at(i) - (newStocks.at(i) - newNeeds.at(j));
                    newStocks[i] -= newNeeds[j];
                    newNeeds[j] = 0;
                }
            }
        }
    }

    void printMatrix()
    {
        cout << right << setw(4) << ' ' << setw(4) << ' ';
        for (auto elem : needs)
            cout << "\x1B[33m" << right << setw(4) << elem << "\x1B[37m";
        cout << endl
             << right << setw(4) << ' ' << setw(4) << ' ';
        for (auto elem : v_vec)
            cout << right << setw(4) << elem;
        cout << endl;
        cout << "        ";
        for (int i = 0; i < size_column; i++)
            cout << "____";
        cout << endl;
        for (int i = 0; i < size_row; i++)
        {
            cout << "\x1B[33m" << right << setw(4) << stocks[i] << "\x1B[37m" << right << setw(4) << u_vec[i] << '|';
            for (int j = 0; j < size_column; j++)
            {
                switch (matrix_plus_minus[i][j])
                {
                case -1:
                    cout << right << setw(4) << this->matrix[i][j];
                    break;
                case 1:
                    cout << "\x1B[31m" << right << setw(4) << this->matrix[i][j] << "\x1B[37m";
                    break;
                case 0:
                    cout << "\x1B[32m" << right << setw(4) << this->matrix[i][j] << "\x1B[37m";
                    break;
                }
            }
            cout << endl;
        }
        cout << endl;
    }
    void printWastes()
    {
        for (int i = 0; i < size_row; i++)
        {
            for (int j = 0; j < size_column; j++)
                cout << right << setw(4) << this->wastes[i][j];
            cout << endl;
        }
        cout << endl;
    }
    bool IsItNotTrue(vector<bool> &bool_vec)
    {
        for (auto c : bool_vec)
            if (c == false)
                return 1;
        return 0;
    }

    void target_func()
    {
        int func = 0;
        for (int i = 0; i < size_row; i++)
            for (int j = 0; j < size_column; j++)
                if (matrix[i][j] != 0)
                    func += matrix[i][j] * wastes[i][j];
        cout << " F = " << func << endl;
    }
    void calculating()
    {
        minimal_element = INT_MAX;
        for (int i = 0; i < size_row; i++)
            for (int j = 0; j < size_column; j++)
                if (matrix_plus_minus[i][j] == 1)
                    if (matrix[i][j] < minimal_element)
                        minimal_element = matrix[i][j];
        for (int i = 0; i < size_row; i++)
            for (int j = 0; j < size_column; j++)
                if (matrix_plus_minus[i][j] == 1)
                    matrix[i][j] -= minimal_element;
                else if (matrix_plus_minus[i][j] == 0)
                    matrix[i][j] += minimal_element;
        minimal_element = 0;
    }

    void start()
    {
        while (potentials())
        {
            for (int i = 0; i < size_row; i++)
                for (int j = 0; j < size_column; j++)
                    matrix_plus_minus[i][j] = -1;
            searchLoop(potential_i, potential_j);
            calculating();
            printMatrix();
        }
    }
    bool potentials() // метод потенциалов
    {
        vector<bool> boolU(size_row, false);
        vector<bool> boolV(size_column, false);
        boolU[0] = true;
        for (auto elem : u_vec)
            elem = 0;
        for (auto elem : v_vec)
            elem = 0;
        while (IsItNotTrue(boolU) || IsItNotTrue(boolV))
        {
            for (int i = 0; i < size_row; i++)
            {
                for (int j = 0; j < size_column; j++)
                {
                    if (matrix[i][j] != 0)
                    {
                        if (boolU[i])
                        {
                            v_vec[j] = wastes[i][j] - u_vec[i];
                            boolV[j] = true;
                        }
                        if (boolV[j])
                        {
                            u_vec[i] = wastes[i][j] - v_vec[j];
                            boolU[i] = true;
                        }
                    }
                }
            }
        }
        if (delta(u_vec, v_vec))
            return 1;
        return 0;
    }

    bool delta(vector<int> u_vec, vector<int> v_vec)
    {
        potential_i = 0;
        potential_j = 0;
        for (int i = 0; i < size_row; i++)
        {
            for (int j = 0; j < size_column; j++)
                if (matrix[i][j] == 0)
                {
                    this->free_matrix[i][j] = wastes[i][j] - (u_vec[i] + v_vec[j]);
                    // нахождение наибольшего отрицательного значения
                    if (free_matrix[i][j] < free_matrix[potential_i][potential_j])
                    {
                        potential_i = i;
                        potential_j = j;
                    }
                }
        }
        // проверка существование отрицательных значений
        if (free_matrix[potential_i][potential_j] < 0)
        {
            cout << "(" << potential_i + 1 << ',' << potential_j + 1 << "):" << free_matrix[potential_i][potential_j] << endl;
            return 1;
        }
        return 0;
    }
    void searchLoop(int i, int j) // поиск контура
    {
        matrix_plus_minus[i][j] = 0;
        if (j && !way)
            go_left(i, j - 1);
        if ((j != size_column - 1) && !way)
            go_right(i, j + 1);
        if (i && !way)
            go_up(i - 1, j);
        if ((i != size_row - 1) && !way)
            go_down(i + 1, j);
        way = false;
        sign = 1;
    }

    void go_down(int i, int j)
    {
        if ((i == potential_i) && (j == potential_j))
            way = true;
        if ((i != this->size_row - 1) && !way)
            go_down(i + 1, j);
        if (j && matrix[i][j] && !way)
        {
            go_left(i, j - 1);
            if (way && matrix[i][j])
            {
                matrix_plus_minus[i][j] = sign;
                sign = 1 - sign;
            }
        }
        if ((j != this->size_column - 1) && (matrix[i][j] != 0) && !way)
        {
            go_right(i, j + 1);
            if (way && matrix[i][j])
            {
                matrix_plus_minus[i][j] = sign;
                sign = 1 - sign;
            }
        }
    }

    void go_up(int i, int j)
    {
        if ((i == potential_i) && (j == potential_j))
            way = true;
        if (i && !way)
            go_up(i - 1, j);
        if (j && matrix[i][j] && !way)
        {
            go_left(i, j - 1);
            if (way && matrix[i][j])
            {
                matrix_plus_minus[i][j] = sign;
                sign = 1 - sign;
            }
        }
        if ((j != this->size_column - 1) && matrix[i][j] && !way)
        {
            go_right(i, j + 1);
            if (way && matrix[i][j])
            {
                matrix_plus_minus[i][j] = sign;
                sign = 1 - sign;
            }
        }
    }

    void go_right(int i, int j)
    {
        if ((i == potential_i) && (j == potential_j))
            way = true;
        if ((j != this->size_column - 1) && !way)
            go_right(i, j + 1);
        if (i && matrix[i][j] && !way)
        {
            go_up(i - 1, j);
            if (way && matrix[i][j])
            {
                matrix_plus_minus[i][j] = sign;
                sign = 1 - sign;
            }
        }
        if ((i != this->size_row - 1) && matrix[i][j] && !way)
        {
            go_down(i + 1, j);
            if (way && matrix[i][j])
            {
                matrix_plus_minus[i][j] = sign;
                sign = 1 - sign;
            }
        }
    }

    void go_left(int i, int j)
    {
        if ((i == potential_i) && (j == potential_j))
            way = true;
        if (j && !way)
            go_left(i, j - 1);
        if (i && matrix[i][j] && !way)
        {
            go_up(i - 1, j);
            if (way && matrix[i][j])
            {
                matrix_plus_minus[i][j] = sign;
                sign = 1 - sign;
            }
        }
        if ((i != this->size_row - 1) && matrix[i][j] && !way)
        {
            go_down(i + 1, j);
            if (way && matrix[i][j])
            {
                matrix_plus_minus[i][j] = sign;
                sign = 1 - sign;
            }
        }
    }
};

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    // Matrix D(4, 4);
    // D.wastes = {{4, 8, 1, 6}, {3, 5, 3, 4}, {2, 6, 4, 3}, {1, 4, 5, 3}};
    // D.stocks = {70, 80, 90, 80};
    // D.needs = {60, 40, 120, 100};
    Matrix D(4, 5);
    D.wastes = {{15, 1, 22, 19, 1}, {21, 18, 11, 4, 3}, {26, 29, 23, 26, 24}, {21, 10, 3, 19, 27}};
    D.stocks = {20, 20, 20, 20};
    D.needs = {19, 19, 19, 19, 4};
    cout << "\x1B[33mМетод северо-западного угла: \x1B[37m" << endl;
    D.printWastes();
    D.northwestCorner();
    D.printMatrix();
    D.target_func();
    cout << "\x1B[33mМетод потенциалов: \x1B[37m" << endl;
    D.start();
    D.target_func();
    // system("pause");
}