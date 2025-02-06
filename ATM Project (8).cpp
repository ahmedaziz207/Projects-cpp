#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
using namespace std;

const string ClientsFileName = "Clients.txt";

struct sClient
{
	string AccountNumber;
	string Name;
	string Phone;
	string PinCode;
	double AccountBalance = 0;


};

void Login();
void ShowMainMenu();
void GoBackToMainMenue()
{
	cout << "\n\n\nPress any key to go back to Main Menu...";
	system("pause>0");
	ShowMainMenu();
}


sClient CurrentClient;

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

bool FindClientByAccountNumberAndPinCode(string AccountNumber, string PinCode, sClient& Client)
{
	vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
	for (sClient C : vClients)
	{
		if (C.AccountNumber== AccountNumber && C.PinCode == PinCode)
		{
			Client = C;
			return true;
		}
	}
	return false;
}

bool LoadClientInfo(string AccountNumber, string PinCode)
{
	if (FindClientByAccountNumberAndPinCode(AccountNumber, PinCode, CurrentClient))
		return true;
	else
		return false;
}

void AddBalance(sClient& Client, double Amount)
{
	fstream MyFile;
	vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
	for (sClient& C : vClients)
	{
		if (C.AccountNumber == Client.AccountNumber)
		{
			C.AccountBalance = C.AccountBalance + Amount;
			Client.AccountBalance = Client.AccountBalance + Amount;
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

int PerformQuickWithdraw()
{
	int option = 0;
	cout << "Choose what do you want to do? [1 to 8]? ";
	cin >> option;

	switch (option)
	{
	case 1:
		return 50;
		break;
	case 2:
		return 100;
		break;
	case 3:
		return 200;
		break;
	case 4:
		return 500;
		break;
	case 5:
		return 1000;
		break;
	case 6:
		return 2000;
		break;
	case 7:
		return 3000;
		break;
	case 8:
		return 5000;
		break;
	case 9:
		ShowMainMenu();
		break;
	default:
		cout << "\nInvalid option! Please try again.\n\n";
		GoBackToMainMenue();
	}

}



void CheckBalance()
{
	cout << "==========================================\n";
	cout << "\tCheck Balance Screen";
	cout << "\n==========================================\n";
	cout << "Your Balance is: " << CurrentClient.AccountBalance;
}

void Deposit()
{
	double Amount;
	char answer = ' ';
	do
	{
		system("cls");
		cout << "==========================================\n";
		cout << "\tDeposite Screen";
		cout << "\n==========================================";
		cout << "\n\nEnter a Positive Deposit Amount? ";
		cin >> Amount;
	} while (Amount <= 0);

	cout << "\n\nAre you sure you want to perform this transaction? [y/n] ? ";
	cin >> answer;

	if (answer == 'Y' || answer == 'y')
	{
		AddBalance(CurrentClient, Amount);
		cout << "\n\nDone Successfully. New Balance is: " << CurrentClient.AccountBalance;
	}
	else
	{
		GoBackToMainMenue();
	}
}

void NormalWithdraw()
{
	int Amount;
	char answer = ' ';
	do {
		system("cls");
		cout << "==========================================\n";
		cout << "\tNormal Withdraw Screen";
		cout << "\n==========================================";
		cout << "\n\nEnter an Amount (Multiple of 50's) ? ";
		cin >> Amount;

		if (Amount > CurrentClient.AccountBalance)
		{
			cout << "\n\nThe Amount Exceeds your balance. Make another choice.";
			cout << "\nPress any key to continue...";
			system("pause>0");
		}
		else if (Amount % 50 != 0)
		{
			
		}

	} while (Amount > CurrentClient.AccountBalance || Amount % 50 != 0);


	cout << "\n\nAre you sure you want to perform this transaction? [y/n] ? ";
	cin >> answer;

	if (answer == 'Y' || answer == 'y')
	{
		AddBalance(CurrentClient, -1*Amount);
		cout << "\n\nDone Successfully. New Balance is: " << CurrentClient.AccountBalance;
	}
	else
	{
		GoBackToMainMenue();
	}
}

void QuickWithdraw()
{
	char answer = ' ';
	int Amount;
	do
	{
		system("cls");
		cout << "===========================================\n";
		cout << "\t\tQucik Withdraw\n";
		cout << "===========================================\n";
		cout << "\t[1] 50\t\t  [2] 100\n";
		cout << "\t[3] 200\t\t  [4] 500\n";
		cout << "\t[5] 1000\t  [6] 2000\n";
		cout << "\t[7] 3000\t  [8] 5000\n";
		cout << "\t[9] Exit\n";
		cout << "===========================================\n";
		cout << "Your Balance is " << CurrentClient.AccountBalance<<endl;
		
		Amount = PerformQuickWithdraw();

		if (Amount > CurrentClient.AccountBalance)
		{
			cout << "\n\nThe Amount Exceeds your balance. Make another choice.";
			cout << "\nPress any key to continue...";
			system("pause>0");
		}
	} while (Amount > CurrentClient.AccountBalance);

	cout << "\n\nAre you sure you want to perform this transaction? [y/n] ? ";
	cin >> answer;

	if (answer == 'Y' || answer == 'y')
	{
		AddBalance(CurrentClient, -1 * Amount);
		cout << "\n\nDone Successfully. New Balance is: " << CurrentClient.AccountBalance;
	}
	else
	{
		GoBackToMainMenue();
	}

}

void ChooseOption()
{
	int option = 0;
	cout << "Choose what do you want to do? [1 to 5]? ";
	cin >> option;

	switch (option)
	{
	case 1:
		system("cls");
		QuickWithdraw();
		GoBackToMainMenue();
		break;
	case 2:
		system("cls");
		NormalWithdraw();
		GoBackToMainMenue();
		break;
	case 3:
		system("cls");
		Deposit();
		GoBackToMainMenue();
		break;
	case 4:
		system("cls");
		CheckBalance();
		GoBackToMainMenue();
		break;
	case 5:
		system("cls");
		Login();
		break;
	default:
		cout << "\nInvalid option! Please try again.\n\n";
		GoBackToMainMenue();
	}
}

void ShowMainMenu()
{
	system("cls");
	cout << "===========================================\n";
	cout << "\t\tATM Main Menue Screen\n";
	cout << "===========================================\n";
	cout << "\t[1] Quick Withdraw.\n";
	cout << "\t[2] Normal Withdraw.\n";
	cout << "\t[3] Deposit.\n";
	cout << "\t[4] Check Balance.\n";
	cout << "\t[5] Logout.\n";
	cout << "===========================================\n";
	ChooseOption();
}

void Login()
{
	bool LoginFaild = false;
	string AccountNumber, PinCode;
	do
	{
		system("cls");
		cout << "\n---------------------------------\n";
		cout << "\tLogin Screen";
		cout << "\n---------------------------------\n";
		if (LoginFaild)
		{
			cout << "Invlaid Account Number/PinCode!\n";
		}
		cout << "Enter AccountNumber? ";
		cin >> AccountNumber;
		cout << "Enter PinCode? ";
		cin >> PinCode;
		LoginFaild = !LoadClientInfo(AccountNumber, PinCode);
	} while (LoginFaild);
	ShowMainMenu();
}




int main()
{
	Login();
    system("pause>0");
    return 0;
}


