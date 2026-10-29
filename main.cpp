#include <iostream>
#include <string>
#include <ctime>
#include <vector>
#include <unordered_map>
#include <functional>
#include <iomanip>

/*
    TODO:
    remove global variable expenses
    better currrency storing
    wrong input handling
    getline instead of cin
*/

using namespace std;

struct expense {
    double amount;
    string name;
    time_t timestamp;

};
vector<expense> expenses = {};

const string currency = "zł";

void addExpense() {
    string name; double amount;
    cout << "Name of expense: "; cin >> name;
    cout << "Amount: "; cin >> amount;
    expense new_expense {amount, name, time(NULL)};
    expenses.push_back(new_expense);
}

void viewExpenses() {

    // display table rows
    cout << left
    << setw(6) << "ID"
    << setw(22) << "Date and Time"
    << setw(20) << "Name"
    << setw(14) << ("Amount (" + currency + ")")
    << "\n";
    cout << string(62, '-') << "\n";

    

    for (int id = 0; id < expenses.size(); id++) {
        // time_t to human format
        tm* date = localtime(&expenses[id].timestamp);
        // display info of expense
        cout << left
        << setw(6) << id+1
        << put_time(date, "%Y.%m.%d %H:%M:%S") << "   " // whitespace for alignment 
        << setw(20) << expenses[id].name
        << setw(14) << (to_string(expenses[id].amount) + currency)
        << "\n";
    }
}

unordered_map<string, function<void()>> commands = {
    {"add", addExpense},
    {"view", viewExpenses}
};



/* 
    help - displays help
    add - add an expense
    rm - removes an expense (id)
    view - displays all expenses
    total - sums up all expenses
    exit - exit
*/

int main() {
    cout << "Expense tracker v0.1.0 -- Enter \"help\" for help.\n";
    string command ;
    while (command != "exit") {
        cout << "$exp> ";
        cin >> command;
        commands[command]();
    }
    return 0;
}