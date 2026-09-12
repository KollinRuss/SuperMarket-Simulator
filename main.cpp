#include <iostream>
#include <iomanip>
#include <fstream>
#include "supermarket_simulator_program_3.h"

using namespace std;

int main()
{
    Supermarket myStore;

    cout << "===== Supermarket Simulator =====" << endl;
    cout << endl;

    fstream fileInput;
    processSupermarketInformation(fileInput, myStore);

    // If the file prompt was exited, stop the program.
    if (myStore.szName.empty())
    {
        cout << "No supermarket data was loaded." << endl;
        return 0;
    }

    int choice = 0;

    while (choice != -1)
    {
        cout << endl;
        cout << "===== " << myStore.szName << " =====" << endl;
        cout << "1. Display Supermarket Information" << endl;
        cout << "2. Display Aisles" << endl;
        cout << "3. Display Items in an Aisle" << endl;
        cout << "4. Customer Checkout" << endl;
        cout << "5. Display Total Funds" << endl;
        cout << "6. Calculate Total Sales" << endl;
        cout << "7. Pay Employees" << endl;
        cout << "8. Clear Members" << endl;
        cout << "-1. Exit" << endl;
        cout << "Enter choice: ";

        cin >> choice;
        cin.ignore(10000, '\n');

        if (choice == 1)
        {
            displaySupermarketInfo(myStore);
        }
        else if (choice == 2)
        {
            displayAisles(myStore);
        }
        else if (choice == 3)
        {
            int aisleIndex;
            displayAisles(myStore);
            cout << "Enter aisle index: ";
            cin >> aisleIndex;
            cin.ignore(10000, '\n');

            if (aisleIndex >= 0 && aisleIndex < 40 &&
                !myStore.aislesArr[aisleIndex].szName.empty())
            {
                displayItems(myStore, aisleIndex);
            }
            else
            {
                cout << "Invalid aisle index." << endl;
            }
        }
        else if (choice == 4)
        {
            CustomerPurchase purchase = checkout(myStore);

            if (myStore.iCurrentCustomers < 100)
            {
                myStore.customersArr[myStore.iCurrentCustomers] = purchase;
                myStore.iCurrentCustomers++;
            }
        }
        else if (choice == 5)
        {
            displayTotalFunds(myStore);
        }
        else if (choice == 6)
        {
            cout << "Total Sales: $"
                 << fixed << setprecision(2)
                 << calculateTotalSales(myStore) << endl;
        }
        else if (choice == 7)
        {
            if (payEmployees(myStore))
                cout << "Employees paid successfully." << endl;
            else
                cout << "Not enough funds to pay employees." << endl;
        }
        else if (choice == 8)
        {
            clearMembers(myStore);
        }
        else if (choice == -1)
        {
            cout << "Goodbye!" << endl;
        }
        else
        {
            cout << "Invalid choice. Please try again." << endl;
        }
    }

    return 0;
}
