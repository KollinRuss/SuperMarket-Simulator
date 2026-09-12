#ifndef SUPERMARKET_SIMULATOR_PROGRAM_3_H
#define SUPERMARKET_SIMULATOR_PROGRAM_3_H

#include <string>
#include <fstream>

const double dSALES_TAX = 0.08;

struct Item
{
    std::string szName = "";
    double dWholesale = 0.0;
    double dRegularPrice = 0.0;
    double dMembersPrice = 0.0;
    int iQuantity = 0;
};

struct Aisle
{
    std::string szName = "";
    Item itemArr[100];
};

struct Employee
{
    std::string szName = "";
    std::string szID = "";
    double dSalary = 0.0;
};

struct CustomerPurchase
{
    std::string szName = "";
    bool bBoughtMembership = false;
    double dPurchaseCost = 0.0;
};

struct Supermarket
{
    std::string szName = "";
    std::string szStoreHours = "";
    double dTotalFunds = 0.0;
    double dMembershipFee = 0.0;

    int iCurrentEmployees = 0;
    int iCurrentCustomers = 0;
    int iCurrentMembers = 0;

    Aisle aislesArr[40];
    Employee employeesArr[50];
    CustomerPurchase customersArr[100];
};

std::string promptForFilename();
void processSupermarketInformation(std::fstream& fileInput, Supermarket& myStore);
void displayMenu(std::string szMenuName, std::string szChoicesArr[], int iChoices);
void displaySupermarketInfo(const Supermarket myStore);
void displayAisles(const Supermarket myStore);
void displayItems(const Supermarket myStore, int iAisleIndex);
CustomerPurchase checkout(Supermarket& myStore);
void displayTotalFunds(const Supermarket& myStore);
double calculateTotalSales(const Supermarket& myStore);
bool payEmployees(Supermarket& myStore);
void clearMembers(Supermarket& myStore);
void extraCredit(Supermarket& myStore);

#endif
