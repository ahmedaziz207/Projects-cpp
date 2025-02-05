#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>

using namespace std;
void ShowManageUsersMenu();
void Login();
void ShowMainMenu();
void Transactions();
void ManageUsers();

const string ClientsFileName = "Clients.txt";
const string UsersFileName = "Users.txt";


struct sClient
{
	string AccountNumber;
	string Name;
	string Phone;
	string PinCode;
	double AccountBalance = 0;


};

struct sUser
{
	string Username;
	string Password;
	short Permission;
};

sUser CurrentUser;

void GoBackToManageUsersScreen()
{
	cout << "\n\nPress any key to go back to Manage Users Menu...";
	system("pause>0");
	ManageUsers();
}
void GoBackToTransactions()
{
	cout << "\n\nPress any key to go back to Transaction Menu...";
	system("pause>0");
	Transactions();
}
void GoBackToMainMenue()
{
	cout << "\n\nPress any key to go back to Main Menu...";
	system("pause>0");
	ShowMainMenu();
}


bool IsHasPermission(short permission)
{

	short userpermission = CurrentUser.Permission;
	// Admin (-1) has full access
	if (userpermission == -1)
	{
		return true;
	}
	// Check if the required permission bit(s) are set
	return ((userpermission & permission) == permission);
}

void NOPermission()
{
		cout << "\n------------------------------------------\n";
		cout << "Access Denied,\n";
		cout << "You don't have Permission to Do This,\n";
		cout << "Please Contact your Admin.\n";
		cout << "------------------------------------------\n";
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
		S1.erase(0, pos + Delim.length());
	}
	if (S1 != "")
	{
		vString.push_back(S1); // it adds last word of the string.
	}
	return vString;
}

sClient ConvertLinetoRecord(string Line, string Seperator = "#//#")
{
	sClient Client;
	vector<string> vClientData;
	vClientData = SplitString(Line, Seperator);
	Client.AccountNumber = vClientData[0];
	Client.PinCode = vClientData[1];
	Client.Name = vClientData[2];
	Client.Phone = vClientData[3];
	Client.AccountBalance = stod(vClientData[4]);

	return Client;
}

vector <sClient> LoadCleintsDataFromFile(string FileName)
{
	vector <sClient> vClients;
	fstream MyFile;
	MyFile.open(FileName, ios::in);//read Mode
	if (MyFile.is_open())
	{
		string Line;
		sClient Client;
		while (getline(MyFile, Line))
		{
			Client = ConvertLinetoRecord(Line);
			vClients.push_back(Client);
		}
		MyFile.close();
	}
	return vClients;
}

void PrintClientRecord(sClient Client)
{
	cout << "| " << setw(15) << left << Client.AccountNumber;
	cout << "| " << setw(10) << left << Client.PinCode;
	cout << "| " << setw(40) << left << Client.Name;
	cout << "| " << setw(12) << left << Client.Phone;
	cout << "| " << setw(12) << left << Client.AccountBalance;
}

//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
bool IsAccountNumberExist(string FileName, string AccountNumber)
{
	vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
	fstream MyFile;
	MyFile.open(FileName, ios::in);//read Mode
	if (MyFile.is_open())
	{
		string Line;
		sClient Client;
		while (getline(MyFile, Line))
		{
			Client = ConvertLinetoRecord(Line);
			vClients.push_back(Client);
		}

		for (sClient& C : vClients)
		{
			if (C.AccountNumber == AccountNumber)
			{
				MyFile.close();
				return 1;
			}
		}
		return 0;
		MyFile.close();
	}
}

sClient ReadNewClient()
{
	sClient Client;
	cout << "Enter Account Number? ";
	getline(cin >> ws, Client.AccountNumber);

	while (IsAccountNumberExist(ClientsFileName, Client.AccountNumber))
	{
		cout << "\n\nClient with (" << Client.AccountNumber << ") already exists, ";
		cout << "Enter another Account Number? ";
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

string ConvertRecordToLine(sClient Client, string Seperator = "#//#")
{
	string stClientRecord = "";
	stClientRecord += Client.AccountNumber + Seperator;
	stClientRecord += Client.PinCode + Seperator;
	stClientRecord += Client.Name + Seperator;
	stClientRecord += Client.Phone + Seperator;
	stClientRecord += to_string(Client.AccountBalance);
	return stClientRecord;
}

void AddDataLineToFile(string FileName, string stDataLine)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out | ios::app);
	if (MyFile.is_open())
	{
		MyFile << stDataLine << endl;
		MyFile.close();
	}
}

void EnterClientData()
{
	sClient Client;
	Client = ReadNewClient();
	AddDataLineToFile(ClientsFileName, ConvertRecordToLine(Client));
}
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
void ShowThisClient(sClient Client)
{
	cout << "\nThe following are the Client Details:\n";
	cout << "------------------------------------\n";
	cout << "Account Number :" << Client.AccountNumber << endl;
	cout << "PinCode        :" << Client.PinCode << endl;
	cout << "Name           :" << Client.Name << endl;
	cout << "Phone          :" << Client.Phone << endl;
	cout << "Account Balance:" << Client.AccountBalance << endl;
	cout << "------------------------------------\n";

}

sClient FindClient(string FileName, string AccountNumber)
{
	char confirm;
	vector <sClient> vClients;
	fstream MyFile;
	MyFile.open(FileName, ios::in);//read Mode
	if (MyFile.is_open())
	{
		string Line;
		sClient Client;
		while (getline(MyFile, Line))
		{
			Client = ConvertLinetoRecord(Line);
			vClients.push_back(Client);
		}


		while (!(IsAccountNumberExist(ClientsFileName, AccountNumber)))
		{
		
			cout << "\nClient not Exist(" << AccountNumber << "), Do you want try again ? [Y / N] ";
			cin >> confirm;
			if (confirm == 'y' || confirm == 'Y')
			{
				cout << "\n\nPlease Enter Account Number?";
				cin >> AccountNumber;
			}
			else
			{
				MyFile.close();
				GoBackToMainMenue();
			}
		}
		for (sClient C : vClients)
		{
			if (C.AccountNumber == AccountNumber)
				Client = C;
		}

		MyFile.close();
		return Client;
	}

}

//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

void PrintMiniInfo(sClient Client)
{
	cout << "| " << setw(15) << left << Client.AccountNumber;

	cout << "| " << setw(40) << left << Client.Name;

	cout << "| " << setw(12) << left << Client.AccountBalance;
}

void ChangeBalance(sClient Client, double Amount)
{
	fstream MyFile;
	vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
	for (sClient& C : vClients)
	{
		if (C.AccountNumber == Client.AccountNumber)
		{
			C.AccountBalance = C.AccountBalance + Amount;
			break;
		}
	}
	MyFile.open(ClientsFileName, ios::out);

	if (MyFile.is_open())
	{
		for (sClient& C : vClients)
		{
			MyFile << C.AccountNumber << "#//#"
				<< C.PinCode << "#//#"
				<< C.Name << "#//#"
				<< C.Phone << "#//#"
				<< C.AccountBalance << endl;
		}
		MyFile.close();
	}

}


void TotalBalances()
{
	double Total = 0;
	vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);

	cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).";
	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
	cout << "| " << left << setw(15) << "Accout Number";

	cout << "| " << left << setw(40) << "Client Name";

	cout << "| " << left << setw(12) << "Balance";
	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;

	if (vClients.size() == 0)
		cout << "\t\t\t\tNo Clients Available In the System!";
	else
		for (sClient& Client : vClients)
		{
			PrintMiniInfo(Client);
			Total = Total + Client.AccountBalance;
			cout << endl;
		}
	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
	cout << "\t\t\t\tTotal Balances = " << Total;


}

void WithDraw()
{
	char confirm = 'n';
	double withdraw;
	sClient Client;
	string AccountNumber;
	cout << "----------------------------------------\n";
	cout << "\tWithDraw Client Screen\n";
	cout << "----------------------------------------\n";
	cout << "\nPlease Enter Account Number?";
	cin >> AccountNumber;
	Client = FindClient(ClientsFileName, AccountNumber);
	ShowThisClient(Client);
	cout << "Please Enter WithDraw Amount? ";
	cin >> withdraw;

	while (withdraw > Client.AccountBalance)
	{
		cout << "\n\nAmout exceed the balance , you can withdraw uo to : " << Client.AccountBalance;
		cout << "\nPlease Enter WithDraw Amount? ";
		cin >> withdraw;
	}

	cout << "\n\nAre you sure you want to perform this transaction ? (y/n) ";
	cin >> confirm;

	if (confirm == 'y' || confirm == 'Y')
	{

		ChangeBalance(Client, -withdraw);
	}

}

void Deposit()
{
	char confirm = 'n';
	double deposit;
	sClient Client;
	string AccountNumber;
	cout << "----------------------------------------\n";
	cout << "\tDeposit Client Screen\n";
	cout << "----------------------------------------\n";
	cout << "\nPlease Enter Account Number?";
	cin >> AccountNumber;
	Client = FindClient(ClientsFileName, AccountNumber);
	ShowThisClient(Client);
	cout << "Please Enter Deposit Amount? ";
	cin >> deposit;
	cout << "\n\nAre you sure you want to perform this transaction ? (y/n) ";
	cin >> confirm;
	if (confirm == 'y' || confirm == 'Y')
	{
		ChangeBalance(Client, deposit);
	}


}

void ChooseAction()
{
	int option = 0;
	cout << "Choose what do you want to do? [1 to 4]? ";
	cin >> option;

	switch (option)
	{
	case 1:
		system("cls");
		Deposit();
		GoBackToTransactions();
		break;
	case 2:
		system("cls");
		WithDraw();
		GoBackToTransactions();
		break;
	case 3:
		system("cls");
		TotalBalances();
		GoBackToTransactions();
		break;
	case 4:
		system("cls");
		ShowMainMenu();

		break;
	default:
		cout << "\nInvalid option! Please try again.\n\n\n";
		system("pause");
		Transactions();
	}
}

void Transactions()
{
	if (!IsHasPermission(32))
	{
		NOPermission();
		return;
	}
	system("cls");
	cout << "=================================================\n";
	cout << "\t\tTranactions Menue Screen\n";
	cout << "=================================================\n";
	cout << "\t[1] Deposit.\n";
	cout << "\t[2] Withdraw.\n";
	cout << "\t[3] Tatal Balances.\n";
	cout << "\t[4] Main menu.\n";
	cout << "=================================================\n";
	ChooseAction();

}

//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

void FindClient()
{
	if (!IsHasPermission(16))
	{
		NOPermission();
		return;
	}
	sClient Client;
	string AccountNumber;
	cout << "----------------------------------------\n";
	cout << "\tFind Client Screen\n";
	cout << "----------------------------------------\n";
	cout << "\nPlease Enter Account Number?";
	cin >> AccountNumber;
	Client = FindClient(ClientsFileName, AccountNumber);
	ShowThisClient(Client);
}

void UpdateClient()
{
	if (!IsHasPermission(8))
	{
		NOPermission();
		return;
	}
	fstream MyFile;
	vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
	sClient Client;
	char confirm = 'n';
	string AccountNumber;
	cout << "----------------------------------------\n";
	cout << "\tUpdate Client Screen\n";
	cout << "----------------------------------------\n";
	cout << "\nPlease Enter Account Number?";
	cin >> AccountNumber;
	Client = FindClient(ClientsFileName, AccountNumber);
	ShowThisClient(Client);
	cout << "\n\nAre you sure you want to update this client? [Y / N] ";
	cin >> confirm;
	if (confirm == 'y' || confirm == 'Y')
	{
		for (sClient& C : vClients)
		{
			if (C.AccountNumber == Client.AccountNumber)
			{
				cout << "\nEnter PinCode: ";
				cin >> C.PinCode;
				cout << "Enter Name: ";
				getline(cin >> ws, C.Name);
				cout << "Enter Phone: ";
				cin >> C.Phone;
				cout << "Enter Account Balance: ";
				cin >> C.AccountBalance;
				cout << "\n\nClient Update Successfully:\n";
				break;
			}

		}

	}
	MyFile.open(ClientsFileName, ios::out);

	if (MyFile.is_open())
	{
		for (sClient& C : vClients)
		{
			MyFile << C.AccountNumber << "#//#"
				<< C.PinCode << "#//#"
				<< C.Name << "#//#"
				<< C.Phone << "#//#"
				<< C.AccountBalance << endl;
		}
		MyFile.close();
	}

}

void DeleteClient()
{
	if (!IsHasPermission(4))
	{
		NOPermission();
		return;
	}
	fstream MyFile;
	vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
	sClient Client;
	char confirm = 'n';
	string AccountNumber;
	cout << "----------------------------------------\n";
	cout << "\tDelete Client Screen\n";
	cout << "----------------------------------------\n";
	cout << "\nPlease Enter Account Number?";
	cin >> AccountNumber;
	Client = FindClient(ClientsFileName, AccountNumber);
	ShowThisClient(Client);
	cout << "\n\nAre you sure to delete this client? [Y / N] ";
	cin >> confirm;
	if (confirm == 'y' || confirm == 'Y')
	{
		for (int i = 0; i < vClients.size(); i++)
		{
			if (vClients[i].AccountNumber == Client.AccountNumber)
			{
				vClients.erase(vClients.begin() + i);
				cout << "\n\nClient Deleted successfully.\n";
			}
		}
	}

	MyFile.open(ClientsFileName, ios::out);

	if (MyFile.is_open())
	{
		for (sClient& C : vClients)
		{
			MyFile << C.AccountNumber << "#//#"
				<< C.PinCode << "#//#"
				<< C.Name << "#//#"
				<< C.Phone << "#//#"
				<< C.AccountBalance << endl;
		}
		MyFile.close();
	}

}

void AddNewClient()
{
	if (!IsHasPermission(2))
	{
		NOPermission();
		return;
	}
	char AddMore = 'Y';
	cout << "----------------------------------------\n";
	cout << "\tAdd New Client Screen\n";
	cout << "----------------------------------------\n";

	do
	{
		cout << "\nAdding New Client: \n\n";
		EnterClientData();
		cout << "\nClient Added Successfully, do you want to addmore clients ? Y / N ? ";
		cin >> AddMore;

	} while (toupper(AddMore) == 'Y');
}

void ShowClientList()
{
	if (!IsHasPermission(1))
	{
		NOPermission();
		return;
	}

	vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);

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
		for (sClient Client : vClients)
		{
			PrintClientRecord(Client);
			cout << endl;
		}
	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
}

void ChooseOption()
{
	int option = 0;
	cout << "Choose what do you want to do? [1 to 8]? ";
	cin >> option;

	switch (option)
	{
	case 1:
		system("cls");
		ShowClientList();
		GoBackToMainMenue();
		break;
	case 2:
		system("cls");
		AddNewClient();
		GoBackToMainMenue();
		break;
	case 3:
		system("cls");
		DeleteClient();
		GoBackToMainMenue();
		break;
	case 4:
		system("cls");
		UpdateClient();
		GoBackToMainMenue();
		break;
	case 5:
		system("cls");
		FindClient();
		GoBackToMainMenue();
		break;
	case 6:
		system("cls");
		Transactions();
		break;
	case 7:
		system("cls");
		ManageUsers();
		break;
	case 8:
		system("cls");
		Login();
		break;
	default:
		cout << "\nInvalid option! Please try again.\n\n\n";
		system("pause>0");
		ShowMainMenu();
	}
}

void ShowMainMenu()
{
	system("cls");
	cout << "===========================================\n";
	cout << "\t\tMain Menue Screen\n";
	cout << "===========================================\n";
	cout << "\t[1] Show Client List.\n";
	cout << "\t[2] Add New Client.\n";
	cout << "\t[3] Delete Client.\n";
	cout << "\t[4] Update Client Info.\n";
	cout << "\t[5] Find Client.\n";
	cout << "\t[6] Tranactions.\n";
	cout << "\t[7] Manage Users.\n";
	cout << "\t[8] Logout.\n";
	cout << "===========================================\n";
	ChooseOption();

}
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//000000000000000000000000000000000000000000000000000000000

vector<string> SplitUserString(string S1, string Delim)
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
		S1.erase(0, pos + Delim.length());
	}
	if (S1 != "")
	{
		vString.push_back(S1); // it adds last word of the string.
	}
	return vString;
}

sUser ConvertUserLinetoRecord(string Line, string Seperator = "#//#")
{
	sUser User;
	vector<string> vUserData;
	vUserData = SplitUserString(Line, Seperator);
	User.Username = vUserData[0];
	User.Password = vUserData[1];
	User.Permission = stod(vUserData[2]);

	return User;
}

vector <sUser> LoadUsersDataFromFile(string FileName)
{
	vector <sUser> vUsers;
	fstream MyFile;
	MyFile.open(FileName, ios::in);//read Mode
	if (MyFile.is_open())
	{
		string Line;
		sUser User;
		while (getline(MyFile, Line))
		{
			User = ConvertUserLinetoRecord(Line);
			vUsers.push_back(User);
		}
		MyFile.close();
	}
	return vUsers;
}

bool IsUserAndPasswordCorrect(string FileName, string Username, string Password)
{
	vector <sUser> vUsers = LoadUsersDataFromFile(UsersFileName);
	fstream MyFile;
	MyFile.open(FileName, ios::in);//read Mode
	if (MyFile.is_open())
	{
		string Line;
		sUser User;
		while (getline(MyFile, Line))
		{
			User = ConvertUserLinetoRecord(Line);
			vUsers.push_back(User);
		}

		for (sUser& C : vUsers)
		{
			if (C.Username == Username && C.Password == Password)
			{
				MyFile.close();
				return 1;
			}
		}
		return 0;
		MyFile.close();
	}
}

bool IsUserExist(string FileName, string Username)
{
	vector <sUser> vUsers = LoadUsersDataFromFile(UsersFileName);
	fstream MyFile;
	MyFile.open(FileName, ios::in);//read Mode
	if (MyFile.is_open())
	{
		string Line;
		sUser User;
		while (getline(MyFile, Line))
		{
			User = ConvertUserLinetoRecord(Line);
			vUsers.push_back(User);
		}

		for (sUser& C : vUsers)
		{
			if (C.Username == Username)
			{
				MyFile.close();
				return 1;
			}
		}
		return 0;
		MyFile.close();
	}
}

sUser GetUserDirect(string FileName, string Username)
{
	vector <sUser> vUsers;
	fstream MyFile;
	MyFile.open(FileName, ios::in);//read Mode
	if (MyFile.is_open())
	{
		string Line;
		sUser User;
		while (getline(MyFile, Line))
		{
			User = ConvertUserLinetoRecord(Line);
			vUsers.push_back(User);
		}

		for (sUser &C : vUsers)
		{
			if (C.Username == Username)
				User = C;
		}

		MyFile.close();
		return User;
	}

}

short GetPermission()
{
	short permission = 0;
	char answer = ' ';
	cout << "\nDo you want to Give Full Access? Y/N? ";
	cin >> answer;

	if (toupper(answer) == 'Y')
		return -1;

	cout << "\n\nDo you to give access to:";
	cout << "\n\nShow Client List? Y/N? ";
	cin >> answer;
	if (toupper(answer) == 'Y')
		permission = (permission + 1);
	cout << "\nAdd New CLinet? Y/N? ";
	cin >> answer;
	if (toupper(answer) == 'Y')
		permission = (permission + 2);
	cout << "\nDelete Client? Y/N? ";
	cin >> answer;
	if (toupper(answer) == 'Y')
		permission = (permission + 4);
	cout << "\nUpdate Client? Y/N? ";
	cin >> answer;
	if (toupper(answer) == 'Y')
		permission = (permission + 8);
	cout << "\nFind Client? Y/N? ";
	cin >> answer;
	if (toupper(answer) == 'Y')
		permission = (permission + 16);
	cout << "\nTransactions? Y/N? ";
	cin >> answer;
	if (toupper(answer) == 'Y')
		permission = (permission + 32);
	cout << "\nManage Users? Y/N? ";
	cin >> answer;
	if (toupper(answer) == 'Y')
		permission = (permission + 64);

	return permission;
}

bool FindUserByUsernameAndPassword(string Username, string Password, sUser& User)
{
	vector <sUser> vUsers = LoadUsersDataFromFile(UsersFileName);
	for (sUser U : vUsers)
	{
		if (U.Username == Username && U.Password == Password)
		{
			User = U;
			return true;
		}
	}
	return false;
}

void ShowThisUser(sUser User)
{
	cout << "\nThe Following are the User Details:\n";
	cout << "------------------------------------\n";
	cout << "Username     :" << User.Username << endl;
	cout << "Password     :" << User.Password << endl;
	cout << "Permission   :" << User.Permission << endl;
	cout << "------------------------------------\n";

}

sUser FindUser(string FileName, string Username)
{
	char confirm;
	vector <sUser> vUsers;
	fstream MyFile;
	MyFile.open(FileName, ios::in);//read Mode
	if (MyFile.is_open())
	{
		string Line;
		sUser User;
		while (getline(MyFile, Line))
		{
			User = ConvertUserLinetoRecord(Line);
			vUsers.push_back(User);
		}


		while (!(IsUserExist(UsersFileName, Username)))
		{
			
			cout << "\nUser not Exist(" << Username << "), Do you want try again ? [Y / N] ";
			cin >> confirm;
			if (confirm == 'y' || confirm == 'Y')
			{
				cout << "\n\nPlease Enter Username?";
				cin >> Username;
			}
			else
			{
				MyFile.close();
				GoBackToManageUsersScreen();
			}
		}
		for (sUser C : vUsers)
		{
			if (C.Username == Username)
				User = C;
		}

		MyFile.close();
		return User;
	}
}


//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

void FindUser()
{
	sUser User;
	string Username;
	cout << "----------------------------------------\n";
	cout << "\tFind User Screen\n";
	cout << "----------------------------------------\n";
	cout << "\nPlease Enter Username?";
	cin >> Username;
	User = FindUser(ClientsFileName, Username);
	ShowThisUser(User);
}

void UpdateUser()
{
	fstream MyFile;
	vector <sUser> vUsers = LoadUsersDataFromFile(UsersFileName);
	sUser User;
	char confirm = 'n';
	string Username;
	cout << "----------------------------------------\n";
	cout << "\tUpdate User Screen\n";
	cout << "----------------------------------------\n";
	cout << "\nPlease Enter Username?";
	cin >> Username;
	User = FindUser(ClientsFileName, Username);
	ShowThisUser(User);
	cout << "\n\nAre you sure you want to update this user? [Y / N] ";
	cin >> confirm;
	if (confirm == 'y' || confirm == 'Y')
	{
		for (sUser& C : vUsers)
		{
			if (C.Username == User.Username)
			{
				cout << "\nEnter Password: ";
				cin >> C.Password;
				User.Permission = GetPermission();
				cout << "\n\nUser Updated Successfully:\n";
				break;
			}

		}

	}
	MyFile.open(UsersFileName, ios::out);

	if (MyFile.is_open())
	{
		for (sUser& C : vUsers)
		{
			MyFile << C.Username << "#//#"
				<< C.Password << "#//#"
				<< C.Permission << endl;
		}
		MyFile.close();
	}
}

void DeleteUser()
{
	fstream MyFile;
	vector <sUser> vUsers = LoadUsersDataFromFile(UsersFileName);
	sUser User;
	char confirm = 'n';
	string Username;
	cout << "----------------------------------------\n";
	cout << "\tDelete User Screen\n";
	cout << "----------------------------------------\n";
	cout << "\nPlease Enter Username?";
	cin >> Username;

	if (Username == "Admin")
	{
		cout << "You can't delete this user.\n";
		GoBackToManageUsersScreen();
	}

	User = FindUser(UsersFileName, Username);
	ShowThisUser(User);
	cout << "\n\nAre you sure to delete this user? [Y / N] ";
	cin >> confirm;
	if (confirm == 'y' || confirm == 'Y')
	{
		for (int i = 0; i < vUsers.size(); i++)
		{
			if (vUsers[i].Username == User.Username)
			{
				vUsers.erase(vUsers.begin() + i);
				cout << "\n\nUser Deleted successfully.\n";
			}
		}
	}

	MyFile.open(ClientsFileName, ios::out);

	if (MyFile.is_open())
	{
		for (sUser& C : vUsers)
		{
			MyFile << C.Username << "#//#"
				<< C.Password << "#//#"
				<< C.Permission << endl;
		}
		MyFile.close();
	}
}

sUser ReadNewUser()
{
	sUser User;
	cout << "Enter Username? ";
	getline(cin >> ws, User.Username);

	while (IsUserExist(UsersFileName, User.Username))
	{
		cout << "\n\nUser with (" << User.Username << ") already exists, ";
		cout << "Enter another Username? ";
		getline(cin, User.Username);
	}

	cout << "Enter Password? ";
	getline(cin, User.Password);
	User.Permission = GetPermission();
	return User;
}

string ConvertUserRecordToLine(sUser User, string Seperator = "#//#")
{
	string stUserRecord = "";
	stUserRecord += User.Username + Seperator;
	stUserRecord += User.Password + Seperator;
	stUserRecord += to_string(User.Permission);

	return stUserRecord;
}

void AddUserDataLineToFile(string FileName, string stDataLine)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out | ios::app);
	if (MyFile.is_open())
	{
		MyFile << stDataLine << endl;
		MyFile.close();
	}
}

void EnterUserData()
{
	sUser User;
	User = ReadNewUser();
	AddUserDataLineToFile(UsersFileName, ConvertUserRecordToLine(User));
}

void AddNewUser()
{
	char AddMore = 'Y';
	cout << "----------------------------------------\n";
	cout << "\tAdd New User Screen\n";
	cout << "----------------------------------------\n";

	do
	{
		cout << "\nAdding New User: \n\n";
		EnterUserData();
		cout << "\nUser Added Successfully, do you want to addmore users ? Y / N ? ";
		cin >> AddMore;

	} while (toupper(AddMore) == 'Y');
}

void PrintUserRecord(sUser User)
{
	cout << "| " << setw(20) << left << User.Username;
	cout << "| " << setw(15) << left << User.Password;
	cout << "| " << setw(15) << left << User.Permission;
}

void ShowListUsers()
{
	vector <sUser> vUsers = LoadUsersDataFromFile(UsersFileName);

	cout << "\n\t\t\t\t\tUsers List (" << vUsers.size() << ") User(s).";
	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
	cout << "| " << left << setw(20) << "User Name";
	cout << "| " << left << setw(15) << "PassWord";
	cout << "| " << left << setw(15) << "Permission";
	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;

	if (vUsers.size() == 0)
		cout << "\t\t\t\tNo Clients Available In the System!";
	else
		for (sUser User : vUsers)
		{
			PrintUserRecord(User);
			cout << endl;
		}
	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
}

void ChooseUserOption()
{

	int option = 0;
	cout << "Choose what do you want to do? [1 to 6]? ";
	cin >> option;

	switch (option)
	{
	case 1:
		system("cls");
		ShowListUsers();
		GoBackToManageUsersScreen();
		break;
	case 2:
		system("cls");
		AddNewUser();
		GoBackToManageUsersScreen();
		break;
	case 3:
		system("cls");
		DeleteUser();
		GoBackToManageUsersScreen();
		break;
	case 4:
		system("cls");
		UpdateUser();
		GoBackToManageUsersScreen();
		break;
	case 5:
		system("cls");
		FindUser();
		GoBackToManageUsersScreen();
		break;
	case 6:
		system("cls");
		ShowMainMenu();
		break;
	default:
		cout << "\nInvalid option! Please try again.\n\n\n";
		system("pause>0");
		ShowManageUsersMenu();
	}
}

void ShowManageUsersMenu()
{
	if (!IsHasPermission(64))
	{
		NOPermission();
		return;
	}
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
	ChooseUserOption();
}

void ManageUsers()
{

	ShowManageUsersMenu();
}

bool LoadUserInfo(string Username, string Password)
{
	if (FindUserByUsernameAndPassword(Username, Password,CurrentUser))
		return true;
	else
		return false;
}

void Login()
{
	bool LoginFaild = false;
	string Username, Password;
	do
	{
		system("cls");
		cout << "\n---------------------------------\n";
		cout << "\tLogin Screen";
		cout << "\n---------------------------------\n";
		if (LoginFaild)
		{
			cout << "Invlaid Username/Password!\n";
		}
		cout << "Enter Username? ";
		cin >> Username;
		cout << "Enter Password? ";
		cin >> Password;
		LoginFaild = !LoadUserInfo(Username, Password);
	} while (LoginFaild);
	ShowMainMenu();
}

int main()
{
	Login();

	return 0;
}
