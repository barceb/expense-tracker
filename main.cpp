#include <iostream>
#include <string>
#include <ctime>
#include <vector>
#include <unordered_map>
#include <functional>
#include <iomanip>
#include <format>
#include <fstream>

/*
    TODO:
    remove global variable expenses
    currency handling
    add total to budget
*/

using namespace std;

struct expense {
    double amount;
    string name;
    time_t timestamp;

};
vector<expense> expenses = {};
string currency; 

void addExpense() {
    string name; double amount; string input_amount;
    cout << "Name of expense: "; getline(cin, name);
    cout << "Amount: "; getline(cin, input_amount); 
    try {
        expense new_expense {stod(input_amount), name, time(NULL)};
        expenses.push_back(new_expense);
    } catch (invalid_argument e){
        cout << "Invalid amount.\n";
    }
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
        << setw(14) << (format("{:.2f}", expenses[id].amount) + currency)
        << "\n";
    }
}

void listCurrencies() {
    ifstream currencyfile;
    currencyfile.open("currencies.csv");

    if (!currencyfile.is_open()) {
        cerr << "Currency list cannot be obtained.\n";
        return;
    }
    

    string line;
    bool header = true;
    while (getline(currencyfile, line)) {
        stringstream ss(line);
        string cell;
        int i = 0; int column_lengths[3] = {11,60,5}; // maybe better way for this

        while (getline(ss, cell, ',')) {
            cout << left << setw(column_lengths[i]) << cell;
            i++;
        }
        cout << "\n";
        if (header) { // this is horrible probably?
            header = false;
            cout << string(76, '-') << "\n";
        }
        
    }


    

    currencyfile.close();
}

unordered_map<string, function<void()>> commands = {
    {"add", addExpense},
    {"view", viewExpenses},
    {"currency list", listCurrencies}//,
    //{"currency set", setCurrency}
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
    cout << "Expense tracker v0.2.0 -- Enter \"help\" for help.\n";
    string command ;
    while (command != "exit") {
        cout << "$exp> ";
        getline(cin, command);
        if (commands.contains(command)) {
            commands[command]();
        } else {
            cout << "Invalid command. Enter \"help\" for help.\n";
        }
    }
    return 0;
}