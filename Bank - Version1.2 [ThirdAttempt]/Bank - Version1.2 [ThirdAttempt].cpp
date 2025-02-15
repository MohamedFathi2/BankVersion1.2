#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

const string ClientsFileName = "Clients.txt";
const string UsersFileName = "Users.txt";


struct stUsers
{
    string UserName;
    string Password;
    int Permissions = 0;
    bool MarkForDelete = false;
};

struct stClients
{
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance;
    bool MarkForDelete = false;
};

enum enUserPermissions
{
    enShowClientList = 1, enAddNewClient = 2, enDeleteClient = 4, enUpdateClientInfo = 8, enFindClient = 16, enTransactions = 32, enManageUsers = 64
};

enum enTransactionsMenueOptions { eDeposit = 1, eWithdraw = 2, eShowTotalBalance = 3, eShowMainMenue = 4 };

enum enMainMenueOptions
{
    eListClients = 1, eAddNewClient = 2, eDeleteClient = 3, eUpdateClient = 4, eFindClient = 5, eShowTransactionsMenue = 6, eManageUsers = 7, eLogout = 8
};

enum enManageUserOptions
{
    enListUsers = 1, enAddNewUser, enDeleteUser, enUpdateUser, enFindUser, enMainMenue
};

void PerformManageUserOption(enManageUserOptions Option, stUsers User);
void Login();
void ShowTransactionsMenue(stUsers User);
void ShowMainMenue(stUsers User);
void ShowManageUsersScreen(stUsers User);
void GoBackToManageUsersScreenMenue();






bool CheckUserPermission(int UserPermissions, enUserPermissions PermissionToCheck)
{
    return UserPermissions == -1 ? true : (UserPermissions & int(PermissionToCheck));
}


int ReadPermissionsToSet()
{
    int Permissions = 0;
    bool GiveAllPermissions = false;
    cout << "Do you want to assign all permissions for the user? (1 for true, 0 for false) ";
    cin >> GiveAllPermissions;
    if (GiveAllPermissions)
        return -1; // Can do all the functions of the user
    for (int i = 0; i < 7; i++)
    {
        cout << "Permission " << i + 1 << " (" <<
            (i == 0 ? "Show Client List" :
                i == 1 ? "Add New Client" :
                i == 2 ? "Delete Client" :
                i == 3 ? "Update Client Info" :
                i == 4 ? "Find Client" :
                i == 5 ? "Transactions" : "Manage User")
            << "): ";
        short input;
        cin >> input;
        if (input) Permissions |= (1 << i); // set the bit [left shift]
    }
    return Permissions;
}


vector<string> SplitString(string S1, string Delim)
{

    vector<string> vString;

    short pos = 0;
    string sWord; // define a string variable  

    // use find() function to get the position of the delimiters  

    while ((pos = S1.find(Delim)) != std::string::npos)
    {
        sWord = S1.substr(0, pos); // store the word   
        if (sWord != "")
        {
            vString.push_back(sWord);
        }

        S1.erase(0, pos + Delim.length());  /* erase() until positon and move to next word. */
    }

    if (S1 != "")
    {
        vString.push_back(S1); // it adds last word of the string.
    }

    return vString;

}

stClients ConvertLinetoRecordForClients(string Line, string Seperator = "#//#")
{

    stClients Client;
    vector<string> vClientData;

    vClientData = SplitString(Line, Seperator);

    Client.AccountNumber = vClientData[0];
    Client.PinCode = vClientData[1];
    Client.Name = vClientData[2];
    Client.Phone = vClientData[3];
    Client.AccountBalance = stod(vClientData[4]);//cast string to double


    return Client;

}
string ConvertRecordToLineForUsers(stUsers User, string Seperator = "#//#")
{
    string stUserRecord = "";

    stUserRecord += User.UserName + Seperator;
    stUserRecord += User.Password + Seperator;
    stUserRecord += to_string(User.Permissions);

    return stUserRecord;
}
stUsers ConvertLineToRecordForUsers(string Line, string Separator = "#//#")
{
    stUsers User;
    vector <string> vUserData;

    vUserData = SplitString(Line, Separator);

    User.UserName = vUserData[0];
    User.Password = vUserData[1];
    User.Permissions = stoi(vUserData[2]);

    return User;
}
string ConvertRecordToLine(stClients Client, string Seperator = "#//#")
{

    string stClientRecord = "";

    stClientRecord += Client.AccountNumber + Seperator;
    stClientRecord += Client.PinCode + Seperator;
    stClientRecord += Client.Name + Seperator;
    stClientRecord += Client.Phone + Seperator;
    stClientRecord += to_string(Client.AccountBalance);

    return stClientRecord;

}

bool ClientExistsByAccountNumber(string AccountNumber, string FileName)
{

    vector <stClients> vClients;

    fstream MyFile;
    MyFile.open(FileName, ios::in);//read Mode

    if (MyFile.is_open())
    {

        string Line;
        stClients Client;

        while (getline(MyFile, Line))
        {

            Client = ConvertLinetoRecordForClients(Line);
            if (Client.AccountNumber == AccountNumber)
            {
                MyFile.close();
                return true;
            }


            vClients.push_back(Client);
        }

        MyFile.close();

    }

    return false;


}

stClients ReadNewClient()
{
    stClients Client;

    cout << "Enter Account Number? ";

    // Usage of std::ws will extract allthe whitespace character
    getline(cin >> ws, Client.AccountNumber);

    while (ClientExistsByAccountNumber(Client.AccountNumber, ClientsFileName))
    {
        cout << "\nClient with [" << Client.AccountNumber << "] already exists, Enter another Account Number? ";
        getline(cin >> ws, Client.AccountNumber);
    }


    cout << "Enter PinCode? ";
    getline(cin, Client.PinCode);

    cout << "Enter Name? ";
    getline(cin, Client.Name);

    cout << "Enter Phone? ";
    getline(cin, Client.Phone);

    cout << "Enter AccountBalance? ";
    cin >> Client.AccountBalance;

    return Client;

}

vector <stClients> LoadClientsDataFromFile(string FileName)
{

    vector <stClients> vClients;

    fstream MyFile;
    MyFile.open(FileName, ios::in);//read Mode

    if (MyFile.is_open())
    {

        string Line;
        stClients Client;

        while (getline(MyFile, Line))
        {

            Client = ConvertLinetoRecordForClients(Line);

            vClients.push_back(Client);
        }

        MyFile.close();

    }

    return vClients;

}

vector <stUsers> LoadUsersDataFromFile(string FileName)
{
    vector <stUsers> vUsers;

    fstream MyFile;
    MyFile.open(FileName, ios::in); // ReadMode

    if (MyFile.is_open())
    {
        string Line;
        stUsers User;

        while (getline(MyFile, Line))
        {
            User = ConvertLineToRecordForUsers(Line);
            vUsers.push_back(User);
        }

        MyFile.close();

    }

    return vUsers;
}




void PrintClientRecordLine(stClients Client)
{

    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(10) << left << Client.PinCode;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.Phone;
    cout << "| " << setw(12) << left << Client.AccountBalance;
}


void PrintClientRecordBalanceLine(stClients Client)
{

    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.AccountBalance;

}

void ShowAllClientsScreen()
{


    vector <stClients> vClients = LoadClientsDataFromFile(ClientsFileName);

    cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    cout << "| " << left << setw(15) << "Accout Number";
    cout << "| " << left << setw(10) << "Pin Code";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Phone";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    if (vClients.size() == 0)
        cout << "\t\t\t\tNo Clients Available In the System!";
    else

        for (stClients Client : vClients)
        {

            PrintClientRecordLine(Client);
            cout << endl;
        }

    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

}

void ShowTotalBalances()
{

    vector <stClients> vClients = LoadClientsDataFromFile(ClientsFileName);

    cout << "\n\t\t\t\t\tBalances List (" << vClients.size() << ") Client(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    cout << "| " << left << setw(15) << "Accout Number";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    double TotalBalances = 0;

    if (vClients.size() == 0)
        cout << "\t\t\t\tNo Clients Available In the System!";
    else

        for (stClients Client : vClients)
        {

            PrintClientRecordBalanceLine(Client);
            TotalBalances += Client.AccountBalance;

            cout << endl;
        }

    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
    cout << "\t\t\t\t\t   Total Balances = " << TotalBalances;

}

void PrintClientCard(stClients Client)
{
    cout << "\nThe following are the client details:\n";
    cout << "-----------------------------------";
    cout << "\nAccout Number: " << Client.AccountNumber;
    cout << "\nPin Code     : " << Client.PinCode;
    cout << "\nName         : " << Client.Name;
    cout << "\nPhone        : " << Client.Phone;
    cout << "\nAccount Balance: " << Client.AccountBalance;
    cout << "\n-----------------------------------\n";

}
void PrintUserCard(stUsers User)
{
    cout << "\nThe following are the user's details:\n";
    cout << "-----------------------------------";
    cout << "\nUserName: " << User.UserName;
    cout << "\nPassword: " << User.Password;
    cout << "\nPermissions: " << User.Permissions;
    cout << "\n-----------------------------------\n";
}
bool FindClientByAccountNumber(string AccountNumber, vector <stClients> vClients, stClients& Client)
{

    for (stClients C : vClients)
    {

        if (C.AccountNumber == AccountNumber)
        {
            Client = C;
            return true;
        }

    }
    return false;

}

// bool FindUserByUsernameAndPassword()


stClients ChangeClientRecord(string AccountNumber)
{
    stClients Client;

    Client.AccountNumber = AccountNumber;

    cout << "\n\nEnter PinCode? ";
    getline(cin >> ws, Client.PinCode);

    cout << "Enter Name? ";
    getline(cin, Client.Name);

    cout << "Enter Phone? ";
    getline(cin, Client.Phone);

    cout << "Enter AccountBalance? ";
    cin >> Client.AccountBalance;

    return Client;

}
stUsers ChangeUserInfo(string UserName)
{
    stUsers User;

    User.UserName = UserName;

    cout << "\nEnter Password? ";
    getline(cin >> ws, User.Password);

    User.Permissions = ReadPermissionsToSet();
    return User;
}

bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector <stClients>& vClients)
{

    for (stClients& C : vClients)
    {

        if (C.AccountNumber == AccountNumber)
        {
            C.MarkForDelete = true;
            return true;
        }

    }

    return false;

}
bool MarkUserForDeleteByUserName(string UserName, vector <stUsers>& Users)
{
    for (stUsers& U : Users)
    {
        if (U.UserName == UserName)
        {
            U.MarkForDelete = true;
            return true;
        }
    }
    return false;
}
vector <stClients> SaveClientsDataToFile(string FileName, vector <stClients> vClients)
{

    fstream MyFile;
    MyFile.open(FileName, ios::out);//overwrite

    string DataLine;

    if (MyFile.is_open())
    {

        for (stClients C : vClients)
        {

            if (C.MarkForDelete == false)
            {
                //we only write records that are not marked for delete.  
                DataLine = ConvertRecordToLine(C);
                MyFile << DataLine << endl;

            }

        }

        MyFile.close();

    }

    return vClients;

}
vector <stUsers> SaveUsersDataToFile(string FileName, vector <stUsers> &vUsers)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out); // OverWrite mode.

    string DataLine;

    if (MyFile.is_open())
    {
        for (stUsers User : vUsers)
        {
            if (User.MarkForDelete == false)
            {
                // we only write records that are not marked for delte;
                DataLine = ConvertRecordToLineForUsers(User);
                MyFile << DataLine << endl;
            }
        }
        MyFile.close();
    }
    return vUsers;
}
void AddDataLineToFile(string FileName, string  stDataLine)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out | ios::app);

    if (MyFile.is_open())
    {

        MyFile << stDataLine << endl;

        MyFile.close();
    }

}

void AddNewClient()
{
    stClients Client;
    Client = ReadNewClient();
    AddDataLineToFile(ClientsFileName, ConvertRecordToLine(Client));

}

void AddNewClients()
{
    char AddMore = 'Y';
    do
    {
        //system("cls");
        cout << "Adding New Client:\n\n";

        AddNewClient();
        cout << "\nClient Added Successfully, do you want to add more clients? Y/N? ";


        cin >> AddMore;

    } while (toupper(AddMore) == 'Y');

}

bool DeleteClientByAccountNumber(string AccountNumber, vector <stClients>& vClients)
{

    stClients Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {

        PrintClientCard(Client);

        cout << "\n\nAre you sure you want delete this client? y/n ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {
            MarkClientForDeleteByAccountNumber(AccountNumber, vClients);
            SaveClientsDataToFile(ClientsFileName, vClients);

            //Refresh Clients 
            vClients = LoadClientsDataFromFile(ClientsFileName);

            cout << "\n\nClient Deleted Successfully.";
            return true;
        }

    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
        return false;
    }

}

bool UpdateClientByAccountNumber(string AccountNumber, vector <stClients>& vClients)
{

    stClients Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {

        PrintClientCard(Client);
        cout << "\n\nAre you sure you want update this client? y/n ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {

            for (stClients& C : vClients)
            {
                if (C.AccountNumber == AccountNumber)
                {
                    C = ChangeClientRecord(AccountNumber);
                    break;
                }

            }

            SaveClientsDataToFile(ClientsFileName, vClients);

            cout << "\n\nClient Updated Successfully.";
            return true;
        }

    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
        return false;
    }

}

bool DepositBalanceToClientByAccountNumber(string AccountNumber, double Amount, vector <stClients>& vClients)
{


    char Answer = 'n';


    cout << "\n\nAre you sure you want perfrom this transaction? y/n ? ";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y')
    {

        for (stClients& C : vClients)
        {
            if (C.AccountNumber == AccountNumber)
            {
                C.AccountBalance += Amount;
                SaveClientsDataToFile(ClientsFileName, vClients);
                cout << "\n\nDone Successfully. New balance is: " << C.AccountBalance;

                return true;
            }

        }


        return false;
    }

}

string ReadClientAccountNumber()
{
    string AccountNumber = "";

    cout << "\nPlease enter AccountNumber? ";
    cin >> AccountNumber;

    return AccountNumber;

}

void ShowDeleteClientScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tDelete Client Screen";
    cout << "\n-----------------------------------\n";

    vector <stClients> vClients = LoadClientsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();
    DeleteClientByAccountNumber(AccountNumber, vClients);

}

void ShowUpdateClientScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tUpdate Client Info Screen";
    cout << "\n-----------------------------------\n";

    vector <stClients> vClients = LoadClientsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();
    UpdateClientByAccountNumber(AccountNumber, vClients);

}

void ShowAddNewClientsScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tAdd New Clients Screen";
    cout << "\n-----------------------------------\n";

    AddNewClients();

}

void ShowFindClientScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tFind Client Screen";
    cout << "\n-----------------------------------\n";

    vector <stClients> vClients = LoadClientsDataFromFile(ClientsFileName);
    stClients Client;
    string AccountNumber = ReadClientAccountNumber();
    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
        PrintClientCard(Client);
    else
        cout << "\nClient with Account Number[" << AccountNumber << "] is not found!";

}

void ShowEndScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tProgram Ends :-)";
    cout << "\n-----------------------------------\n";

}

void ShowDepositScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tDeposit Screen";
    cout << "\n-----------------------------------\n";


    stClients Client;

    vector <stClients> vClients = LoadClientsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();


    while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        cout << "\nClient with [" << AccountNumber << "] does not exist.\n";
        AccountNumber = ReadClientAccountNumber();
    }


    PrintClientCard(Client);

    double Amount = 0;
    cout << "\nPlease enter deposit amount? ";
    cin >> Amount;

    DepositBalanceToClientByAccountNumber(AccountNumber, Amount, vClients);

}

void ShowWithDrawScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tWithdraw Screen";
    cout << "\n-----------------------------------\n";

    stClients Client;

    vector <stClients> vClients = LoadClientsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();


    while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        cout << "\nClient with [" << AccountNumber << "] does not exist.\n";
        AccountNumber = ReadClientAccountNumber();
    }

    PrintClientCard(Client);

    double Amount = 0;
    cout << "\nPlease enter withdraw amount? ";
    cin >> Amount;

    //Validate that the amount does not exceeds the balance
    while (Amount > Client.AccountBalance)
    {
        cout << "\nAmount Exceeds the balance, you can withdraw up to : " << Client.AccountBalance << endl;
        cout << "Please enter another amount? ";
        cin >> Amount;
    }

    DepositBalanceToClientByAccountNumber(AccountNumber, Amount * -1, vClients);

}

void ShowTotalBalancesScreen()
{

    ShowTotalBalances();

}


void GoBackToMainMenue()
{
    cout << "\n\nPress any key to go back to Main Menue...";
    system("pause>0");
}

void GoBackToTransactionsMenue(stUsers User)
{
    cout << "\n\nPress any key to go back to Transactions Menue...";
    system("pause>0");
    ShowTransactionsMenue(User);
}

short ReadTransactionsMenueOption()
{
    cout << "Choose what do you want to do? [1 to 4]? ";
    short Choice = 0;
    cin >> Choice;

    return Choice;
}

void PerfromTranactionsMenueOption(enTransactionsMenueOptions TransactionMenueOption, stUsers User)
{
    switch (TransactionMenueOption)
    {
    case enTransactionsMenueOptions::eDeposit:
    {
        system("cls");
        ShowDepositScreen();
        GoBackToTransactionsMenue(User);
        break;
    }

    case enTransactionsMenueOptions::eWithdraw:
    {
        system("cls");
        ShowWithDrawScreen();
        GoBackToTransactionsMenue(User);
        break;
    }


    case enTransactionsMenueOptions::eShowTotalBalance:
    {
        system("cls");
        ShowTotalBalancesScreen();
        GoBackToTransactionsMenue(User);
        break;
    }


    case enTransactionsMenueOptions::eShowMainMenue:
    {

        ShowMainMenue(User);

    }
    }

}

void ShowTransactionsMenue(stUsers User)
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tTransactions Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Deposit.\n";
    cout << "\t[2] Withdraw.\n";
    cout << "\t[3] Total Balances.\n";
    cout << "\t[4] Main Menue.\n";
    cout << "\n===========================================\n";
    PerfromTranactionsMenueOption((enTransactionsMenueOptions)ReadTransactionsMenueOption(), User);
}

short ReadMainMenueOption()
{
    cout << "Choose what do you want to do? [1 to 8]? ";
    short Choice = 0;
    cin >> Choice;

    return Choice;
}
short ReadManageUserMenueOption()
{
    cout << "Choose what do you want to do? [1 to 6]? ";
    short Choice = 0;
    cin >> Choice;
    return Choice;
}
void ShowAccessErrorMessage()
{
    cout << "-----------------------------------\n";
    cout << "Access Denied," << endl;
    cout << "You don't Have Permission To Do this,\n";
    cout << "Please Contact Your Admin." << endl;
    cout << "-----------------------------------";
}



// -------------------------------------------------
// This Part will contain MANAGE USERS FUNCTIONALITY


void PrintUserRecordLine(stUsers User)
{
    cout << "| " << setw(40) << left << User.UserName;
    cout << "| " << setw(12) << left << User.Password;
    cout << "| " << setw(12) << left << User.Permissions;
}

// Function Number one for ManageUsers
void ListAllUsersScreen()
{
    vector <stUsers> vUsers = LoadUsersDataFromFile(UsersFileName);
    system("cls");
    cout << "\n\t\t\t\t\tUsers List (" << vUsers.size() << ") User(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
    cout << "| " << left << setw(40) << "User Name";
    cout << "| " << left << setw(12) << "Password";
    cout << "| " << left << setw(12) << "Permissions";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    if (vUsers.size() == 0)
        cout << "\t\t\t\tNo Users Available In the System!";
    else
        for (stUsers User : vUsers)
        {
            PrintUserRecordLine(User);
            cout << endl;
        }
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
    GoBackToManageUsersScreenMenue();
}

// Function Number Two for ManageUsers
bool UserExistsByUserName(string UserName, string FileName)
{
    
    fstream MyFile;
    MyFile.open(FileName, ios::in);
    
    if (MyFile.is_open())
    {
        string Line;
        stUsers User;

        while (getline(MyFile, Line))
        {
            User = ConvertLineToRecordForUsers(Line);
            if (User.UserName == UserName)
            {
                MyFile.close();
                return true;
            }
        }
        MyFile.close();
    }
    return false;
}
void GetBackUserInfo(stUsers User, stUsers &UserInfo)
{
    UserInfo.UserName = User.UserName;
    UserInfo.Password = User.Password;
    UserInfo.Permissions = User.Permissions;
    UserInfo.MarkForDelete = User.MarkForDelete;
}
bool UserExistsByUserName(string UserName, string FileName, stUsers& UserInfo)
{
    fstream MyFile;
    
    MyFile.open(FileName, ios::in);

    if (MyFile.is_open())
    {
        string Line;
        stUsers User;

        while (getline(MyFile, Line))
        {
            User = ConvertLineToRecordForUsers(Line);
            if (User.UserName == UserName)
            {
                GetBackUserInfo(User, UserInfo);
                MyFile.close();
                return true;
            }
        }
        MyFile.close();
    }
    return false;

}

stUsers ReadNewUser()
{
    stUsers User;
    cout << "Enter Username: ";
    getline(cin >> ws, User.UserName);

    while (UserExistsByUserName(User.UserName, UsersFileName))
    {
        cout << "User with [" << User.UserName << "] already exists, Enter another Username? ";
        getline(cin >> ws, User.UserName);
    }
    cout << "Enter Password? ";
    getline(cin, User.Password);

    User.Permissions = ReadPermissionsToSet();

    return User;
}
void AddNewUserScreen()
{
    system("cls");
    cout << "----------------------------------------------\n";
    cout << "\t\tAdd New User Screen\n";
    cout << "----------------------------------------------\n";
}
void AddNewUser()
{
    stUsers User;
    AddNewUserScreen();
    User = ReadNewUser();
    AddDataLineToFile(UsersFileName, ConvertRecordToLineForUsers(User));
}



// Function Number 3 for manage users
bool DeleteUserByUserName(string UserName, vector <stUsers>& vUsers)
{
    stUsers User;
    char Answer = 'n';
    if (UserExistsByUserName(UserName, UsersFileName, User))
    {
        PrintUserCard(User);
        
        cout << "\n\nAre you sure you want to delete this user? y/n? ";
        cin >> Answer;

        if (Answer == 'y' || Answer == 'Y')
        {
            MarkUserForDeleteByUserName(UserName, vUsers);
            SaveUsersDataToFile(UsersFileName, vUsers);
            
            // Refresh Users
            vUsers = LoadUsersDataFromFile(UsersFileName);

            cout << "\n\nUser Deleted Successfully.";
            return true;
        }
    }
    else
    {
        cout << "\nUser with User Name (" << UserName << ") is Not Found!";
        return false;
    }
}

void ShowDeleteUserScreeen()
{
    system("cls");
    cout << "\n-----------------------------------\n";
    cout << "\tDelete User Screen";
    cout << "\n-----------------------------------\n";
    vector <stUsers> vUsers = LoadUsersDataFromFile(UsersFileName);
    string UserName = "";
    cout << "Enter User Name: ";
    getline(cin >> ws, UserName);
    if (UserName == "Admin")
    {
        cout << "\nYou Cannot Delete This User.\n";
        GoBackToManageUsersScreenMenue();
    }
    else
    {
        DeleteUserByUserName(UserName, vUsers);
        GoBackToManageUsersScreenMenue();
    }
}


// Function Number 4 for Manage User Screen


void UpdateUserByUserName(string UserName, vector <stUsers>& vUsers)
{
    stUsers User;

    char Answer = 'n';

    if (UserExistsByUserName(UserName, UsersFileName, User))
    {
        PrintUserCard(User);
        cout << "\n\nAre you sure you want to update this user? y/n? ";
        cin >> Answer;
        
        if (Answer == 'y' || Answer == 'Y')
        {
            for (stUsers &U : vUsers)
            {
                if (U.UserName == UserName)
                {
                    U = ChangeUserInfo(UserName);
                    break;
                }
            }
            SaveUsersDataToFile(UsersFileName, vUsers);

            cout << "\n\nUser Updated Successfully.";
        }
    }
    else
    {
        cout << "\nClient with User Name (" << UserName << ") is Not Found!";
        GoBackToManageUsersScreenMenue();
    }
}




void ShowUpdateUserScreen(stUsers User)
{
    system("cls");
    cout << "\n-----------------------------------\n";
    cout << "\tUpdate User Info Screen";
    cout << "\n-----------------------------------\n";
    vector <stUsers> vUsers = LoadUsersDataFromFile(UsersFileName);
    string UserName = "";
    cout << "Enter User Name : ";
    getline(cin >> ws,UserName);
    if (UserName == "Admin")
    {
        cout << "\nYou Cannot Update This User.\n";
        GoBackToManageUsersScreenMenue();
    }
    else
    {
        UpdateUserByUserName(UserName, vUsers);
        GoBackToManageUsersScreenMenue();
    }
}

// Function Number 5 for Manage User Screen Menue



void ShowFindUserScreen()
{
    system("cls");
    cout << "\n-----------------------------------\n";
    cout << "\tFind User Info Screen";
    cout << "\n-----------------------------------\n";
    vector <stUsers> vUsers = LoadUsersDataFromFile(UsersFileName);
    stUsers User;
    string UserName = "";
    cout << "Enter User Name : ";
    getline(cin >> ws, UserName);
    if (UserExistsByUserName(UserName, UsersFileName, User))
    {
        PrintUserCard(User);
        GoBackToManageUsersScreenMenue();
    }
    else
    {
        cout << "\nUser Doesn't Exists.\n";
        GoBackToManageUsersScreenMenue();
    }

}



void GoBackToManageUsersScreenMenue()
{
    cout << "\n\nPress any key to go back to Mangage Users Main Menue Screen.....";
    system("pause>0");
}

void PerformManageUserOption(enManageUserOptions Option, stUsers User)
{
    switch (Option)
    {
    case enManageUserOptions::enListUsers:
        ListAllUsersScreen();
        ShowManageUsersScreen(User);
        break;
    case enManageUserOptions::enAddNewUser:
        AddNewUser();
        GoBackToManageUsersScreenMenue();
        ShowManageUsersScreen(User);
        break;
    case enManageUserOptions::enDeleteUser:
        ShowDeleteUserScreeen();
        ShowManageUsersScreen(User);
        break;
    case enManageUserOptions::enUpdateUser:
        ShowUpdateUserScreen(User);
        ShowManageUsersScreen(User);
        break;
    case enManageUserOptions::enFindUser:
        ShowFindUserScreen();
        ShowManageUsersScreen(User);
        break;
    case enManageUserOptions::enMainMenue:
        ShowMainMenue(User);
        break;
    }
}




void ShowManageUsersScreen(stUsers User)
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tManage Users Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] List Users.\n";
    cout << "\t[2] Add New User.\n";
    cout << "\t[3] Delete User.\n";
    cout << "\t[4] Update User.\n";
    cout << "\t[5] Find User.\n";
    cout << "\t[6] Main Menue.\n";
    cout << "===========================================\n";
    PerformManageUserOption(enManageUserOptions(ReadManageUserMenueOption()), User);
}












void PerfromMainMenueOption(enMainMenueOptions MainMenueOption, stUsers User)
{

    switch (MainMenueOption)
    {
    case enMainMenueOptions::eListClients:

        system("cls");
        if (CheckUserPermission(User.Permissions, enUserPermissions::enShowClientList))
        {
            ShowAllClientsScreen();
            GoBackToMainMenue();
            ShowMainMenue(User);
        }
        else
        {
            ShowAccessErrorMessage();
            GoBackToMainMenue();
            ShowMainMenue(User);
        }
        break;

    case enMainMenueOptions::eAddNewClient:
        system("cls");
        if (CheckUserPermission(User.Permissions, enUserPermissions::enAddNewClient))
        {
            ShowAddNewClientsScreen();
            GoBackToMainMenue();
            ShowMainMenue(User);
        }
        else
        {
            ShowAccessErrorMessage();
            GoBackToMainMenue();
            ShowMainMenue(User);
        }
        break;

    case enMainMenueOptions::eDeleteClient:
        system("cls");

        if (CheckUserPermission(User.Permissions, enUserPermissions::enDeleteClient))
        {
            ShowDeleteClientScreen();
            GoBackToMainMenue();
            ShowMainMenue(User);
        }

        else
        {
            ShowAccessErrorMessage();
            GoBackToMainMenue();
            ShowMainMenue(User);
        }
        break;

    case enMainMenueOptions::eUpdateClient:
        system("cls");
        if (CheckUserPermission(User.Permissions, enUserPermissions::enUpdateClientInfo))
        {
            ShowUpdateClientScreen();
            GoBackToMainMenue();
            ShowMainMenue(User);
        }
        else
        {
            ShowAccessErrorMessage();
            GoBackToMainMenue();
            ShowMainMenue(User);
        }
        break;

    case enMainMenueOptions::eFindClient:
        system("cls");
        if (CheckUserPermission(User.Permissions, enUserPermissions::enFindClient))
        {
            ShowFindClientScreen();
            GoBackToMainMenue();
            ShowMainMenue(User);
        }
        else
        {
            ShowAccessErrorMessage();
            GoBackToMainMenue();
            ShowMainMenue(User);
        }
        break;

    case enMainMenueOptions::eShowTransactionsMenue:
        system("cls");
        if (CheckUserPermission(User.Permissions, enUserPermissions::enTransactions))
        {
            ShowTransactionsMenue(User);
        }
        else
        {
            ShowAccessErrorMessage();
            GoBackToMainMenue();
            ShowMainMenue(User);
        }
        break;

    case enMainMenueOptions::eManageUsers:
        system("cls");
        if (CheckUserPermission(User.Permissions, enUserPermissions::enManageUsers))
        {
            ShowManageUsersScreen(User);
        }
        else
        {
            ShowAccessErrorMessage();
            GoBackToMainMenue();
            ShowMainMenue(User);
        }
        break;

    case enMainMenueOptions::eLogout:
        system("cls");
        Login();
        break;
    }
}

bool FindUserByUserNameAndPassword(string UserName, string Password, int &UserPermissions)
{
    // we first need to load Users Text file into the function


    fstream MyFile;
    MyFile.open(UsersFileName, ios::in);//read Mode

    if (MyFile.is_open())
    {

        string Line;
        stUsers User;
        while (getline(MyFile, Line))
        {

            User = ConvertLineToRecordForUsers(Line);
            if (User.UserName == UserName && User.Password == Password)
            {   
                UserPermissions = User.Permissions;
                MyFile.close();
                return true;
            }

        }

        MyFile.close();

    }
    return false;
}


void ShowMainMenue(stUsers User)
{
    // Add Manage Users.
    // change exit to logout
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tMain Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Show Client List.\n";
    cout << "\t[2] Add New Client.\n";
    cout << "\t[3] Delete Client.\n";
    cout << "\t[4] Update Client Info.\n";
    cout << "\t[5] Find Client.\n";
    cout << "\t[6] Transactions.\n";
    cout << "\t[7] Manage Users.\n";
    cout << "\t[8] Logout.";
    cout << "\n===========================================\n";
    PerfromMainMenueOption((enMainMenueOptions)ReadMainMenueOption(), User);
}

/*
bool IsUserInUsers(sUser User)
{
    FindClientByAccountNumber()
}*/




void LoginScreen()
{
    cout << "-------------------------------------------\n";
    cout << "\t\tLogin Screen\n";
    cout << "-------------------------------------------\n";
}
void Login()
{
    stUsers User;
    LoginScreen();
    cout << "Enter UserName? ";
    getline(cin >> ws, User.UserName);

    cout << "Enter Password? ";
    getline(cin >> ws, User.Password);


    while (!FindUserByUserNameAndPassword(User.UserName, User.Password, User.Permissions))
    {
        system("cls");
        LoginScreen();
        cout << "Invalid UserName/Password!\n";

        cout << "Enter UserName? ";
        getline(cin >> ws, User.UserName);

        cout << "Enter Password? ";
        getline(cin >> ws, User.Password);
    }
    ShowMainMenue(User);
}


int main()
{

    Login();

    system("pause>0");
    return 0;
}