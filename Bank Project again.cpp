#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>

using namespace std;

void ShowMainMenu();
void Transactions();

const string ClientsFileName = "Clients.txt";

struct sClient
{
	string AccountNumber;
	string Name;
	string Phone;
	string PinCode;
	double AccountBalance = 0;


};

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
			MyFile.close();
			cout << "\nClient not Exist("<< AccountNumber<<"), Do you want try again ? [Y / N] ";
			cin >> confirm;
			if (confirm == 'y' || confirm == 'Y')
			{
				cout << "\n\nPlease Enter Account Number?";
				cin >> AccountNumber;
			}
			else
			{
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

void ChangeBalance(sClient Client,double Amount)
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


void MainMenu()
{
	ShowMainMenu();
}

void TotalBalances()
{
	double Total=0;
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
		for (sClient &Client : vClients)
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
	cout << "Choose what do you want to do? [1 to 7]? ";
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
		MainMenu();
		
		break;
	default:
		cout << "\nInvalid option! Please try again.\n\n\n";
		system("pause");
		GoBackToTransactions();
	}
}

void Transactions()
{
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

void EndScreen()
{
	cout << "----------------------------------------\n";
	cout << "\t\tEnd Screen\n";
	cout << "----------------------------------------\n";
}

void FindClient()
{
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
	cout << "Choose what do you want to do? [1 to 7]? ";
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
		EndScreen();
		break;
	default:
		cout << "\nInvalid option! Please try again.\n\n\n";
		system("pause");
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
	cout << "\t[7] Exit.\n";
	cout << "===========================================\n";
	ChooseOption();
}

int main()
{
	ShowMainMenu();

	return 0;
}

