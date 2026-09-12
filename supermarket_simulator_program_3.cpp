#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <set>
#include <string>
#include "supermarket_simulator_program_3.h"
using namespace std;
static std::set<std::string> membershipAskedNames;


/*********************************************************************
string promptForFilename()
Purpose:
    Helper function ask the user for a valid file path
Parameters:
    -
Return Value:
    Valid filepath string
Notes:
    -
*********************************************************************/
string promptForFilename()
{
    string filename;

    while (true) {
        cout << "Please enter file path: ";
        getline(cin, filename);

        string lowercaseInput = filename;
        for (char &c : lowercaseInput) {
            c = tolower(c);
        }

        if (lowercaseInput == "exit") {
            return "EXIT";
        }

        if (filename.size() >= 4 && filename.substr(filename.size() - 4) == ".txt") {
            if (filename.find(' ') == string::npos) {
                return filename; // Valid filename
            } else {
                cout << "Please enter a valid file path" << endl;
            }
        } else {
            cout << "Please enter a valid file path" << endl;
        }
    }
}

/*********************************************************************
void processSupermarketInformation(fstream& fileInput, Supermarket& myStore)
Purpose:
    Function to read supermarket text file and process the information
    into a supermarket structure
Parameters:
    I/O fstream& fileInput       File stream to read supermarket info
    I/O Supermarket& myStore     Supermarket structure to populate
Return Value:
    -
Notes:
    This function does not validate the file structure it is provided
*********************************************************************/
void processSupermarketInformation(fstream& fileInput, Supermarket& myStore){
    string filename;

    while (true) {
        filename = promptForFilename();
        if (filename == "EXIT") {
            cout << "Exiting function due to early exiting of file prompt\n";
            return;
        }

        fileInput.open(filename, ios::in);
        if (fileInput.is_open()) {
            break;
        } else {
            cout << "Error: Unable to open file. Please try again.\n";
        }
    }

    getline(fileInput, myStore.szName);
    getline(fileInput, myStore.szStoreHours);
    fileInput >> myStore.dTotalFunds;
    fileInput.ignore();
    fileInput >> myStore.dMembershipFee;
    fileInput.ignore();

    string line;
    while (getline(fileInput, line)) {
        if (line == "*Aisle Information*") {
            while (getline(fileInput, line) && line != "*Employee Information*") {
                if (line.find("Aisle") != string::npos) {
                    int aisleIndex = stoi(line.substr(6, line.find(':') - 6));
                    myStore.aislesArr[aisleIndex].szName = line.substr(line.find(':') + 2);

                    while (getline(fileInput, line) && line != "############################") {
                        istringstream itemStream(line);
                        Item item;

                        itemStream >> item.szName >> item.dWholesale >> item.dRegularPrice >> item.dMembersPrice >> item.iQuantity;

                        for (char& ch : item.szName) {
                            if (ch == '_') {
                                        ch = ' ';
                            }
                        }

                        for (int i = 0; i < 100; ++i) {
                            if (myStore.aislesArr[aisleIndex].itemArr[i].szName.empty()) {
                                myStore.aislesArr[aisleIndex].itemArr[i] = item;
                                break;
                            }
                        }
                    }
                }
            }
        }

        if (line == "*Employee Information*") {
            while (getline(fileInput, line)) {
                istringstream empStream(line);
                Employee employee;

                empStream >> employee.szName >> employee.szID >> employee.dSalary;

                for (char& ch : employee.szName) {
                    if (ch == '_') {
                        ch = ' ';
                    }
                }

                for (int i = 0; i < 50; ++i) {
                    if (myStore.employeesArr[i].szName.empty()) {
                        myStore.employeesArr[i] = employee;
                        ++myStore.iCurrentEmployees;
                        break;
                    }
                }
            }
        }
    }

    fileInput.close();
}

/*********************************************************************
void displayMenu(string szMenuName, string szChoicesArr[], int iChoices)
Purpose:
    Function to display the menu choices of a provided menu
Parameters:
    I   string szMenuName       Title of the displayed menu
    I   string szChoicesArr     Menu choices to be displayed
    I   int iChoices            Number of menu choices
Return Value:
    -
Notes:
    Menu options are displayed starting at 1
    The last menu option should always be displayed as -1
*********************************************************************/
void displayMenu(string szMenuName, string szChoicesArr[], int iChoices)
{

    cout << szMenuName << endl;
    cout << "*****************************" << endl;

    for (int i = 0; i < iChoices - 1; ++i) {
        // For the other items, display them normally
        cout << (i + 1) << ". " << szChoicesArr[i] << endl;
    }

    cout << "-1. " << szChoicesArr[iChoices - 1] << endl;

    cout << "*****************************" << endl;
}
/*********************************************************************
void displaySupermarketInfo(const Supermarket myStore)
Purpose:
    Function to display basic supermarket information
Parameters:
    I   Supermarket myStore   Populated Supermarket info
Return Value:
    -
Notes:
    -
*********************************************************************/
void displaySupermarketInfo(const Supermarket myStore)
{
    cout << endl; // Add blank line before output
    cout << myStore.szName << "'s Information" << endl;
    cout << "*****************************" << endl;

    cout << "Hours: " << myStore.szStoreHours << endl;
    cout << "Membership fee: $" << fixed << setprecision(2) << myStore.dMembershipFee << endl;
    cout << "Total employees: " << myStore.iCurrentEmployees << endl;

    cout << "*****************************" << endl;
}

/*********************************************************************
void displayAisles(const Supermarket myStore)
Purpose:
    Function to display all aisles in the supermarket
Parameters:
    I   Supermarket myStore   Populated Supermarket info
Return Value:
    -
Notes:
    Movies are displayed starting at 0
*********************************************************************/
void displayAisles(const Supermarket myStore)
{
    cout << myStore.szName << "'s Aisles" << endl;
    cout << "*****************************" << endl;

    bool hasAisles = false; // Flag to check if there are aisles
    for (int i = 0; i < 40; ++i) {
        if (!myStore.aislesArr[i].szName.empty()) {
            bool isFilled = false;

            for (const auto& item : myStore.aislesArr[i].itemArr) {
                if (!item.szName.empty()) { // Assuming `name` stores the item's name
                    isFilled = true;
                    break;
                }
            }

            if (isFilled) {
                cout << "Aisle " << i << ": " << myStore.aislesArr[i].szName << endl;
                hasAisles = true;
            }
        }
    }

    if (!hasAisles) {
        cout << "No aisles found in the supermarket." << endl;
    }

    cout << "*****************************" << endl;
}

/*********************************************************************
void displayItems(const Supermarket myStore, int iAisleIndex)
Purpose:
    Function to display all aisles in the supermarket
Parameters:
    I   Supermarket myStore     Populated Supermarket info
    I   int iAisleIndex         Index of the aisle to display items
Return Value:
    -
Notes:
    Movies are displayed starting at 0
*********************************************************************/
void displayItems(const Supermarket myStore, int iAisleIndex)
{
    const Aisle& aisle = myStore.aislesArr[iAisleIndex];
    cout << "Aisle " << iAisleIndex << ": " << aisle.szName << endl;
    cout << "*****************************" << endl;

    bool hasItems = false; // Flag to check if there are valid items
    for (int i = 0; i < 100; ++i) {
        const Item& item = aisle.itemArr[i];

        if (!item.szName.empty()) {
            cout << endl;
            cout << item.szName << endl;
            cout << "Item Quantity: " << item.iQuantity << endl;
            cout << "Regular Price: $" << fixed << setprecision(2) << item.dRegularPrice << endl;
            cout << "Member Price: $" << fixed << setprecision(2) << item.dMembersPrice << endl;


            hasItems = true;
        }
    }

    if (!hasItems) {
        cout << "No items found in this aisle." << endl;
    }

    cout << "*****************************" << endl;
}

/*********************************************************************
CustomerPurchase checkout(Supermarket& myStore)
Purpose:
    Function to handle customer buying store items
Parameters:
    I/O Supermarket& myStore   Populated Supermarket info
Return Value:
    Populated CustomerPurchase if transaction was successful
    Empty CustomerPurchase if transaction was unsuccessful
Notes:
    -
*********************************************************************/
CustomerPurchase checkout(Supermarket& myStore) {
    bool membershipAsked;
    CustomerPurchase purchase;

    cout << "Please enter your name: ";
    cin >> purchase.szName;

    cin.ignore();

    if (membershipAskedNames.find(purchase.szName) != membershipAskedNames.end()) {
        if (purchase.bBoughtMembership == true) {
            cout << "Welcome back " << purchase.szName << endl;
        } else {
            cout << "Would you like to become a member (y/n)? ";
            string membershipChoice;
            cin >> membershipChoice;
            cin.ignore();

            if (membershipChoice == "y" ) {
                purchase.bBoughtMembership = true;
                purchase.dPurchaseCost += myStore.dMembershipFee;
                myStore.iCurrentMembers++;
                myStore.dTotalFunds += myStore.dMembershipFee;
            }
        }
    } else {
        cout << "Would you like to become a member (y/n)? ";
        string membershipChoice;
        cin >> membershipChoice;
        cin.ignore();  // Optional: Clear any remaining newline from the input buffer

        if (membershipChoice == "y" || membershipChoice == "yes") {
            purchase.bBoughtMembership = true;
            purchase.dPurchaseCost += myStore.dMembershipFee;
            myStore.iCurrentMembers++;
            myStore.dTotalFunds += myStore.dMembershipFee;
        }
        else if (membershipChoice == "n" || membershipChoice == "no"){
            purchase.bBoughtMembership = false;
        }

        membershipAskedNames.insert(purchase.szName);
    }

    cout << "Current cart total: " << fixed << setprecision(2) << purchase.dPurchaseCost << endl;
    cout << endl;

    while (true) {
        cout << myStore.szName << "'s Aisles" << endl;
        cout << "*****************************" << endl;
        for (int i = 0; i < 40; ++i) {
            if (!myStore.aislesArr[i].szName.empty()) {
                cout << "Aisle " << i << ": " << myStore.aislesArr[i].szName << endl;
            }
        }
        cout << "*****************************" << endl;

        cout << "Please enter aisle index (-1 to exit): " << endl;
        int aisleChoice;
        cin >> aisleChoice;
        cin.ignore();

        if (aisleChoice == -1) {
            membershipAsked = false;
            break;
        } else if (aisleChoice >= 0 && aisleChoice < 40 && !myStore.aislesArr[aisleChoice].szName.empty()) {
            cout << "Aisle " << aisleChoice << ": " << myStore.aislesArr[aisleChoice].szName << endl;
            cout << "*****************************" << endl;
            for (int i = 0; i < 100; ++i) {
                const Item& item = myStore.aislesArr[aisleChoice].itemArr[i];
                if (!item.szName.empty()) {
                    cout << endl;
                    cout << item.szName << endl;
                    cout << "Item Quantity: " << item.iQuantity << endl;
                    cout << "Regular Price: " << fixed << setprecision(2) << item.dRegularPrice << endl;
                    cout << "Member  Price: " << fixed << setprecision(2) << item.dMembersPrice << endl;
                }
            }
            cout << "*****************************" << endl;

            cout << "Enter item to buy: ";
            string itemName;
            getline(cin, itemName);

            bool itemFound = false;
            for (int i = 0; i < 100; ++i) {
                Item& item = myStore.aislesArr[aisleChoice].itemArr[i];
                if (item.szName == itemName) {
                itemFound = true;

                if (item.iQuantity == 0) {
                cout << itemName << " out of stock" << endl;
                cout << endl;
                break;
            }

        // Process item purchase logic if it is in stock
            cout << "Please enter valid quantity: " << endl;
            int quantity;
            cin >> quantity;
            cin.ignore();

            if (item.iQuantity >= quantity) {
                double price = purchase.bBoughtMembership ? item.dMembersPrice : item.dRegularPrice;
                double profit = (price - item.dWholesale) * quantity;
                myStore.dTotalFunds += profit;
                item.iQuantity -= quantity;
                purchase.dPurchaseCost += price * quantity;
            } else {
                cout << "Not enough stock for " << itemName << ". Available quantity: " << item.iQuantity << endl;
            }
            break;
            }
}
            if (!itemFound) {
                cout << "No item " << itemName << "found" << endl;
            }

            cout << "Current cart total: " << fixed << setprecision(2) << purchase.dPurchaseCost << endl;
            cout << endl;
        } else {
            cout << "Invalid aisle selection. Please try again." << endl;
        }
    }

    purchase.dPurchaseCost *= (1 + dSALES_TAX);

    cout << "Your total is $" << fixed << setprecision(2) << purchase.dPurchaseCost << endl;
    return purchase;
}
/*********************************************************************
void displayTotalFunds(const Supermarket& myStore)
Purpose:
    Function to display the total funds of the store
Parameters:
    I   Supermarket myStore   Populated Supermarket info
Return Value:
    -
Notes:
    This function should only be called by an employee
*********************************************************************/
void displayTotalFunds(const Supermarket& myStore)
{
    cout << myStore.szName << "'s Total Funds" << endl;
    cout << "*****************************" << endl;
    cout << "Funds: $" << fixed << setprecision(2) << myStore.dTotalFunds << endl;
    cout << "*****************************" << endl;
}

/*********************************************************************
double calculateTotalSales(const Supermarket& myStore)
Purpose:
    Function to calculate the total sales of the supermarket
Parameters:
    I   Supermarket myStore   Populated Supermarket info
Return Value:
    Total sales
Notes:
    This function should only be called by an employee
*********************************************************************/
double calculateTotalSales(const Supermarket& myStore){
    double totalSales = 0.0;
    for (int i = 0; i < myStore.iCurrentCustomers; ++i) {
        totalSales += myStore.customersArr[i].dPurchaseCost;
    }
    return totalSales / 1.08;
}

/*********************************************************************
bool payEmployees(Supermarket& myStore)
Purpose:
    Function to calculate the total sales of the supermarket
Parameters:
    I/O   Supermarket myStore   Populated Supermarket info
Return Value:
    True if funds were successfully subtracted, else false.
Notes:
    This function should only be called by an employee
*********************************************************************/
bool payEmployees(Supermarket& myStore)
{
    double totalSalaries = 0.0;

    for (int i = 0; i < myStore.iCurrentEmployees; ++i) {
        const Employee& employee = myStore.employeesArr[i];
        totalSalaries += employee.dSalary;
    }

    if (myStore.dTotalFunds < totalSalaries) {
        return false; // Payment unsuccessful
    }

    for (int i = 0; i < myStore.iCurrentEmployees; ++i) {
        Employee& employee = myStore.employeesArr[i];
        myStore.dTotalFunds -= employee.dSalary;
    }

    return true; // Payment successful
}

/*********************************************************************
void clearMembers(Supermarket& myStore);
Purpose:
    Function to clear the members from the supermarket
Parameters:
    I/O   Supermarket myStore   Populated Supermarket info
Return Value:
    -
Notes:
    A cleared member should set back to a default state.
    This function should only be called by an employee.
    This does not clear the stored text file.
*********************************************************************/
void clearMembers(Supermarket& myStore)
{
    myStore.iCurrentMembers = 0;
    cout << "All members cleared!" << endl;
}

void extraCredit(Supermarket& myStore)
{
    cout << "Not implemented yet" << endl;
}
