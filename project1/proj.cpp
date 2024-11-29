#include <algorithm>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
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
void printTable(const vector<vector<vector<pair<int, int>>>> &table) {
    for (const auto &row : table) {
        for (const auto &col : row) {
            cout << "[";
            for (const auto &pair : col) {
                cout << "(" << pair.first << ", " << pair.second << ")";
            }
            cout << "]";
        }
        cout << endl;
    }
}

// verify if value exists in vector
bool existsInVector(const vector<pair<int, int>> &vec, int key) {
    return find_if(vec.begin(), vec.end(), [key](const pair<int, int> &p) {
               return p.first == key;
           }) != vec.end();
}

// Reconstruct the solution from the matrix
string parenthisise(vector<vector<vector<pair<int, int>>>> &matrix,
                    vector<vector<int>> &cypher, int row, int col, int sol) {
    if (row == col) {
        return to_string(sol);
    }
    for (auto &pair : matrix[row][col]) {
        if (pair.first == sol) {
            auto left = matrix[row][pair.second];
            auto right = matrix[pair.second + 1][col];

            for (auto &value_l : left) {
                for (auto &value_r : right) {
                    if (cypher[value_l.first - 1][value_r.first - 1] == sol) {
                        return "(" +
                               parenthisise(matrix, cypher, row, pair.second,
                                            value_l.first) +
                               " " +
                               parenthisise(matrix, cypher, pair.second + 1,
                                            col, value_r.first) +
                               ")";
                    }
                }
            }
        }
    }
    return "No Solution";
}

string decrypt(vector<vector<int>> &cypher, vector<int> &equation,
               int &solution) {
    int equation_size = equation.size();
    vector<vector<vector<pair<int, int>>>> matrix(
        equation_size, vector<vector<pair<int, int>>>(equation_size));

    // Fill the diagonals of the matrix with the starting values
    for (int i = 0; i < equation_size; i++) {
        matrix[i][i].push_back(make_pair(equation[i], -1));
    }

    // Fill the remaining diagonals
    for (int i = 1; i < equation_size; i++) {
        for (int row = 0, col = i; col < equation_size; row++, col++) {
            // Max limit of values to find in this cell
            int n = i;

            // find values for all K's
            for (int k = col - 1; k >= row; k--) {
                vector<pair<int, int>> &left = matrix[row][k];
                vector<pair<int, int>> &right = matrix[k + 1][col];

                for (auto &value_l : left) {
                    for (auto &value_r : right) {
                        // check if value already in matrix
                        int value =
                            cypher[value_l.first - 1][value_r.first - 1];
                        if (existsInVector(matrix[row][col], value)) {
                            continue;
                        }

                        // Add value to matrix
                        matrix[row][col].push_back(make_pair(value, k));
                        n--;

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

    ostringstream str;
    if (existsInVector(matrix[0][equation_size - 1], solution)) {
        return "1\n" +
               parenthisise(matrix, cypher, 0, equation_size - 1, solution);
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