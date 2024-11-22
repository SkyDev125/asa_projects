#include <iomanip>
#include <iostream>
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

string decrypt(vector<vector<int>> &cypher, vector<int> &equation,
               int &solution) {
    // Print cypher
    cout << "<================= Operation Table ==================>" << endl;
    printTable(cypher);

    // Print equation
    cout << "<===================== Equation =====================>" << endl;
    printTable(equation);

    // Print solution
    cout << "<===================== Solution =====================>" << endl;
    cout << solution << endl;

    return "Not implemented";
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
    for (auto row : cypher) {
        for (auto col : row) {
            cin >> col;
        }
    }

    // Equation values
    vector<int> equation(num);
    for (auto value : equation) {
        cin >> value;
    }

    // Desired solution
    int solution;
    cin >> solution;

    cout << decrypt(cypher, equation, solution) << endl;

    return 0;
}