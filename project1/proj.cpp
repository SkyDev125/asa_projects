#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <tuple>
#include <vector>

using namespace std;

// Print all the values of the table with consistent spacing
// Overload for vector<int>
void printTable(const vector<int> &table) {
    for (const auto &col : table) {
        cout << setw(4) << col;
    }
    cout << endl;
}

// Overload for vector<vector<int>>
void printTable(const vector<vector<int>> &table) {
    for (const auto &row : table) {
        for (const auto &col : row) {
            cout << setw(4) << col;
        }
        cout << endl;
    }
}

// Overload for vector<vector<vector<pair<int,int>>>>
void printTable(
    const vector<vector<vector<tuple<int, int, int, int>>>> &table) {
    for (const auto &row : table) {
        for (const auto &col : row) {
            cout << "[";
            for (const auto &tuple : col) {
                cout << "(" << get<0>(tuple) << ", " << get<1>(tuple) << ", "
                     << get<2>(tuple) << ", " << get<3>(tuple) << ")";
            }
            cout << "]";
        }
        cout << endl;
    }
}

// verify if value exists in vector
tuple<int, int, int, int> existsInVector(
    const vector<tuple<int, int, int, int>> &vec, int key) {
    for (auto &value : vec) {
        if (get<0>(value) == key) {
            return value;
        }
    }
    return make_tuple(-1, -1, -1, -1);
}

// Reconstruct the solution from the matrix
string parenthisise(vector<vector<vector<tuple<int, int, int, int>>>> &matrix,
                    int row, int col, tuple<int, int, int, int> tuple) {
    if (row == col) {
        return to_string(get<0>(tuple));
    }
    int k = get<1>(tuple);
    auto left = matrix[row][k];
    auto right = matrix[k + 1][col];

    return "(" +
           parenthisise(matrix, row, k, existsInVector(left, get<2>(tuple))) +
           " " +
           parenthisise(matrix, k + 1, col,
                        existsInVector(right, get<3>(tuple))) +
           ")";
}

string decrypt(vector<vector<int>> &cypher, vector<int> &equation,
               int &solution) {
    int equation_size = equation.size();
    int cypher_size = cypher.size();
    vector<vector<vector<tuple<int, int, int, int>>>> matrix(
        equation_size,
        vector<vector<tuple<int, int, int, int>>>(equation_size));

    // Fill the diagonals of the matrix with the starting values
    for (int i = 0; i < equation_size; i++) {
        matrix[i][i].push_back(make_tuple(equation[i], -1, -1, -1));
    }

    // Fill the remaining diagonals
    for (int i = 1; i < equation_size; i++) {
        for (int row = 0, col = i; col < equation_size; ++row, ++col) {
            // Max limit of values to find in this cell
            int n = cypher_size;
            vector<bool> checker(cypher_size, false);

            // find values for all K's
            for (int k = col - 1; k >= row; --k) {
                vector<tuple<int, int, int, int>> &left = matrix[row][k];
                vector<tuple<int, int, int, int>> &right = matrix[k + 1][col];

                for (auto &value_l : left) {
                    for (auto &value_r : right) {
                        // check if value already in matrix
                        int value =
                            cypher[get<0>(value_l) - 1][get<0>(value_r) - 1];
                        if (checker[value]) {
                            continue;
                        }

                        // Add value to matrix
                        matrix[row][col].push_back(make_tuple(
                            value, k, get<0>(value_l), get<0>(value_r)));
                        checker[value] = true;
                        --n;

                        // Verify if we have found all possible values for
                        // cell
                        if (n == 0) break;
                    }
                    // Verify if we have found all possible values for cell
                    if (n == 0) break;
                }
                // Verify if we have found all possible values for cell
                if (n == 0) break;
            }
        }
    }

    // // Print cypher
    // cout << "<================= Operation Table ==================>" << endl;
    // printTable(cypher);

    // // Print matrix
    // cout << "<================= Solution Table ==================>" << endl;
    // printTable(matrix);

    // // Print equation
    // cout << "<===================== Equation =====================>" << endl;
    // printTable(equation);

    // // Print solution
    // cout << "<===================== Solution =====================>" << endl;
    // cout << solution << endl;

    auto result = existsInVector(matrix[0][equation_size - 1], solution);
    if (result != make_tuple(-1, -1, -1, -1)) {
        return "1\n" + parenthisise(matrix, 0, equation_size - 1, result);
    } else {
        return "0";
    }
}

int main() {
    // Matrix size
    int matrix_size;
    cin >> matrix_size;

    // Number of values in equation
    int num;
    cin >> num;

    // Operations Matrix
    vector<vector<int>> cypher(matrix_size, vector<int>(matrix_size));
    for (auto &row : cypher) {
        for (auto &col : row) {
            cin >> col;
        }
    }

    // Equation values
    vector<int> equation(num);
    for (auto &value : equation) {
        cin >> value;
    }

    // Desired solution
    int solution;
    cin >> solution;

    cout << decrypt(cypher, equation, solution) << endl;

    return 0;
}