#include <iostream>
#include <string>
#include <ctime>
#include <vector>
#include <unordered_map>
#include <functional>
#include <iomanip>
#include <format>
#include <fstream>
#include <nlohmann/json.hpp>

/*
    TODO:
    fix view alignment
    automatically create new config file if not existing
*/

using namespace std;
using nlohmann::json;

struct expense {
    double amount;
    string name;
    time_t timestamp;

};
vector<expense> expenses = {};

double getTotal() {
    double total = 0;
    for (int id = 0; id < expenses.size(); id++) {
        total += expenses[id].amount;
    }
    return total;
}

string getCurrency() {
    ifstream f("config.json");
    json data = json::parse(f);
    return data["currency"];
}

void setConfig(string key, string value) {
    fstream f;
    f.open("config.json", ios::in);
    if (!f.is_open()) {
        cerr << "Error opening config file.";
    }
    json data = json::parse(f);
    f.close();
    data[key] = value;
    string newData = data.dump();

    f.open("config.json", ios::out | ios::trunc);
    f << newData;
    f.close();
}

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
    int maxid = expenses.size();
    if (maxid <= 0) {
        cout << "There are no expenses.\n";
        return;
    }
    
    string currency = getCurrency();
    // display table rows
    cout << left
    << setw(7) << "ID"
    << setw(22) << "Date and Time"
    << setw(20) << "Name" 
    << setw(14) << ("Amount")
    << "\n";
    cout << string(63, '-') << "\n"; 

    

    for (int id = 0; id < maxid; id++) {
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

    // display total
    cout << string(63, '-') << "\n";
    cout << left << setw(7) << "TOTAL: " << getTotal() << currency << "\n"; 
}

void removeExpense() {
    string id_input;
    int maxid = expenses.size();
    if (maxid <= 0) {
        cout << "There are no expenses.\n";
        return;
    }
    cout << "Enter expense ID (1-" << maxid << ")\n";
    getline(cin, id_input);

    int id = stoi(id_input);
    if (id > maxid) {
        cout << "Expense ID is not in range.";
        return;
    }

    // display expense to be removed
    cout << "Remove this expense?: (y/n)\n";

    string currency = getCurrency();

    // display table rows
    cout << left
    << setw(7) << "ID"
    << setw(22) << "Date and Time"
    << setw(20) << "Name" 
    << setw(14) << ("Amount")
    << "\n";
    cout << string(63, '-') << "\n"; 


    // time_t to human format
    tm* date = localtime(&expenses[id].timestamp);
    cout << left
        << setw(6) << id
        << put_time(date, "%Y.%m.%d %H:%M:%S") << "   " // whitespace for alignment 
        << setw(20) << expenses[id-1].name
        << setw(14) << (format("{:.2f}", expenses[id-1].amount) + currency)
        << "\n";
    
    string confirm_input;
    char confirm;
    getline(cin, confirm_input);
    confirm = confirm_input[0];
    if (confirm == 'y' || confirm == 'Y') {
        expenses.erase(expenses.begin() + id - 1);
        cout << "The expense has been successfully removed.\n";
    } else {
        cout << "No expenses have been removed.\n";
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

void setCurrency() {
    string new_code;
    cout << "Enter currency code: ";
    getline(cin, new_code);
    ifstream currencyFile;
    currencyFile.open("currencies.csv");

    if (!currencyFile.is_open()) {
        cerr << "Currency list cannot be obtained.\n";
        return;
    }


    string line;
    while (getline(currencyFile, line)) {
        
        stringstream ss(line);
        string cell;

        int i = 0;
        string values[3];
        while (getline(ss, cell, ',')) {
            values[i] = cell;
            i++;
        }
        if (values[0] == new_code) {
            setConfig("currency", values[2]);
            cout << "Currency set to: " << new_code << " (" << values[2] << ")\n";
            return;
        }
    }
    cout << "Currency code not found. Use currency list to view supported currencies.\n";
    return;

}

void help() {
    cout <<
    "help - displays help\n" << 
    "add - add an expense\n" <<
    "rm - removes an expense (id)\n" <<
    "view - displays all expenses\n" <<
    "total - sums up all expenses\n" <<
    "exit - exit\n";
}

unordered_map<string, function<void()>> commands = {
    {"add", addExpense},
    {"view", viewExpenses},
    {"remove", removeExpense},
    {"currency list", listCurrencies},
    {"help", help},
    {"currency set", setCurrency}
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
    cout << "Currency: " << getCurrency() << "\n";
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