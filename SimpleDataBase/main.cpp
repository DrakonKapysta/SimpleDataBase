#include <WinSock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <Windows.h>
#include <fstream>
#include <vector>
#include <map>
#include <random>

#pragma comment(lib, "Ws2_32.lib")

using namespace std;

struct userAddressInfo {
	long long id{0};
	string pcAddress;
};

bool fileExists(const string& filename);
long long generateUniqueId();
void createDataBase(vector<userAddressInfo>* addresses);
vector<userAddressInfo> readAddresses();
void deleteAddressById(string filename, int idToDelete);
void updateDataBase(string filename, vector<userAddressInfo>* users);
void addAddressToDataBase(string filename, string address);


int main() {
	WSADATA wsa;
	int iResult, idToDelete;
	string addressToAdd;
	iResult = WSAStartup(MAKEWORD(2, 2), &wsa);
	if (iResult != 0) {
		printf("WSAStartup failed: %d\n", iResult);
		exit(EXIT_FAILURE);
	}
	vector<userAddressInfo> addresses;
	addresses.push_back({ generateUniqueId(),"DESKTOP-LT61FS4" });
	addresses.push_back({ generateUniqueId(),"DESKTOP-09499TF" });
	addresses.push_back({ generateUniqueId(),"DESKTOP-HARITONE" });

	if (fileExists("database.dat")) {
		cout << "File already exists." << endl;
	}
	else {
		createDataBase(&addresses);
	}

	vector<userAddressInfo> readedAddresses;
	readedAddresses = readAddresses();
	for (const auto& address : readedAddresses) {
		cout << "ID: " << address.id << ", pcAddress: " << address.pcAddress <<endl;
	}

	cout << "Enter id which address you want to delete: ";
	cin >> idToDelete;
	deleteAddressById("database.dat", idToDelete);

	readedAddresses = readAddresses();
	for (const auto& address : readedAddresses) {
		cout << "ID: " << address.id << ", pcAddress: " << address.pcAddress << endl;
	}

	cout << "Enter address which you want add to database: ";
	cin >> addressToAdd;
	addAddressToDataBase("database.dat", addressToAdd);

	readedAddresses = readAddresses();
	for (const auto& address : readedAddresses) {
		cout << "ID: " << address.id << ", pcAddress: " << address.pcAddress << endl;
	}
	return 0;
}
bool fileExists(const string& filename)
{
	ifstream file(filename);
	return file.good();
}
void createDataBase(vector<userAddressInfo>* addresses) {
	fstream file("database.dat", ios::in | ios::out | ios::app);

	if (!file) {
		cerr << "Failed to open the database file." << endl;
		exit(EXIT_FAILURE);
	}
	for (const auto& address : *addresses) {
		file << address.id << " " << address.pcAddress << '\n';
	}
	file.close();
}
vector<userAddressInfo> readAddresses() {
	char hostname[64];
	if (gethostname(hostname, sizeof(hostname)) == -1) {
		cout << "Cannot get hostname." << endl;
	};
	vector<userAddressInfo> users;
	userAddressInfo tempUser;
	fstream file("database.dat", ios::in | ios::out | ios::app);
	if (!file) {
		cerr << "Failed to open the database file." << endl;
		exit(EXIT_FAILURE);
	}
	while (file) {
		file >> tempUser.id >> tempUser.pcAddress;
		if (!file)
			break;
		if (tempUser.pcAddress != hostname) {
			users.push_back(tempUser);
		}
	}
	file.close();
	return users;
}
void deleteAddressById(string filename, int idToDelete) {
	ifstream inputFile(filename);
	if (!inputFile) {
		std::cerr << "Failed to open the database file." << std::endl;
		return;
	}
	vector<userAddressInfo> addresses;
	userAddressInfo tempAddress;
	while (inputFile >> tempAddress.id >> tempAddress.pcAddress) {
		if (tempAddress.id != idToDelete) {
			addresses.push_back(tempAddress);
		}
	}
	inputFile.close();
	updateDataBase(filename, &addresses);
}
void updateDataBase(string filename, vector<userAddressInfo>* addresses) {
	ofstream outputFile(filename, std::ios::trunc);
	if (!outputFile) {
		std::cerr << "Failed to open the database file for writing." << std::endl;
		return;
	}

	for (const auto& address : *addresses) {
		outputFile << address.id << ' ' << address.pcAddress << '\n';
	}
	outputFile.close();
}
void addAddressToDataBase(string filename, string address) {
	ofstream file("database.dat",  ios::app);
	if (!file.is_open()) {
		cerr << "Failed to open the database file." << endl;
		return;
	}
	file << generateUniqueId()  << " " << address << '\n';
	file.close();
	cout << "Address was successfully added to database." << endl;
}
long long generateUniqueId() {
	 std::random_device rd;
    std::mt19937_64 generator(rd());
    std::uniform_int_distribution<long long> distribution;
    return distribution(generator) % (10000000 + 1);
}