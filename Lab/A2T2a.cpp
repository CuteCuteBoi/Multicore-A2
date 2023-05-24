#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>

using namespace std;

vector<char> plainText;
vector<char> cipherText;
vector<char> decryptedText;

int shift;

void readFile();
void processCipher();
void outputCipherFile();
void processDecrypt();
void outputDecryptFile();

int main()
{
	readFile();
	processCipher();
	outputCipherFile();
	processDecrypt();
	outputDecryptFile();
	return 0;
}

void readFile()
{
	bool isFilePresent = false;
	string fileName = "plaintext.txt";
	fstream newFile;
	char charOne;

	newFile.open(fileName, ios::in);
	if (newFile.fail())
	{
		cout << "plaintext.txt does not exist!" << endl;
	}
	else
	{
		isFilePresent = true;
		cout << "File Present" << endl;
	}

	if (isFilePresent == true)
	{
		if (newFile.is_open())
		{
			while (newFile.get(charOne))
			{
				if (charOne >= 'a' && charOne <= 'z')
				{
					charOne = charOne - 32;
					plainText.push_back(charOne);
				}
				else
				{
					plainText.push_back(charOne);
				}
			}

			newFile.close();
			cout << "Done Reading" << endl;
		}
	}

	////Debug
	//std::cout << "Plain text:" << std::endl;
	//for (int i = 0; i < plainText.size(); i++)
	//{
	//	std::cout << plainText[i];
	//}
}

void processCipher()
{
	bool isValid = false;

	//Asks for user input for shift value and validating input
	do {
		cout << "Please enter the shift value: ";
		cin >> shift;

		if (cin.fail() == true)
		{
			cin.clear();
			cin.ignore(100, '\n');
			cout << "Invalid shift value!!" << endl;
			processCipher();
		}

		if (shift <= 26 && shift >= -26)
		{
			isValid = true;
			cout << "You've entered a shift value of " << shift << endl;
		}
	} while (isValid == false);

	char charTwo;
	for (int i = 0; i < plainText.size(); i++)
	{
		charTwo = plainText[i];

		if (charTwo >= 'a' && charTwo <= 'z')
		{
			charTwo = charTwo + shift;

			if (charTwo > 'z')
			{
				charTwo = charTwo - 26;
			}
			else if (charTwo < 'a')
			{
				charTwo = charTwo + 26;
			}
			cipherText.push_back(charTwo);
		}
		else if (charTwo >= 'A' && charTwo <= 'Z')
		{
			charTwo = charTwo + shift;

			if (charTwo > 'Z')
			{
				charTwo = charTwo - 26;
			}
			else if (charTwo < 'A')
			{
				charTwo = charTwo + 26;
			}
			cipherText.push_back(charTwo);
		}
		else
		{
			cipherText.push_back(charTwo);
		}
	}

	////Debug
	//std::cout << "\nCipher text:" << std::endl;
	//for (int i = 0; i < cipherText.size(); i++)
	//{
	//	std::cout << cipherText[i];
	//}
}

void outputCipherFile()
{
	string fileName = "ciphertext.txt";
	ofstream outFile;
	outFile.open(fileName);
	for (int i = 0; i < cipherText.size(); i++)
	{
		outFile << cipherText[i];
	}
	outFile.close();
	cout << "Cipher file written to " << fileName << endl;
}

void processDecrypt()
{
	char charThree;
	for (int i = 0; i < cipherText.size(); i++)
	{
		charThree = cipherText[i];

		if (charThree >= 'A' && charThree <= 'Z')
		{
			charThree = charThree - shift;

			if (charThree > 'Z')
			{
				charThree = charThree - 26;
			}
			else if (charThree < 'A')
			{
				charThree = charThree + 26;
			}
			decryptedText.push_back(charThree);
		}
		else
		{
			decryptedText.push_back(charThree);
		}
	}

	////Debug
	//std::cout << "\nDecrypted text:" << std::endl;
	//for (int i = 0; i < decryptedText.size(); i++)
	//{
	//	std::cout << decryptedText[i];
	//}
}

void outputDecryptFile()
{
	string fileName = "decrypted.txt";
	ofstream outFile;
	outFile.open(fileName);
	for (int i = 0; i < decryptedText.size(); i++)
	{
		outFile << decryptedText[i];
	}
	outFile.close();
	cout << "Decrypted file written to " << fileName << endl;
}