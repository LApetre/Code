//Console application implementing text encryption
#include <stdio.h>
#include <tchar.h>
#include <cstring>
#include <cstdlib>
#include <string>
#include <iostream>
#include <random>
#include <fstream>

using namespace std;

static const char *alphanum ="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

//method to get a random encryption key of n characters
char* getKey(int n)
{
	random_device rd;
	char *key;
	int x = strlen(alphanum);
	key = new char[n];

	for (int i = 0; i < n; i++)
	{
		key[i] = alphanum[rd() % (x-1)];
	}
	key[n] = '\0';
	return key;
}

//Vernam encryption using bitwise OR
char* encryptVernam(const char* plaintext)
{
	int n = strlen(plaintext);
	char* key = getKey(n);
	char* cipher;
	cipher = new char[2*n];
	for (int i = 0; i < n; i++)
	{
		cipher[i] = plaintext[i] ^ key[i];
		cipher[n + i] = key[i];

	}
	cipher[2*n]= '\0';
	return cipher;
}
char* decryptVernam(const char* cipher)
{
	char *decrypted;
	int n = strlen(cipher) / 2;
	decrypted = new char[n];
	for (int i = 0; i < n; i++)
	{
		decrypted[i] = cipher[i] ^ cipher[n+i];
	}
	decrypted[n]= '\0';
	return decrypted;
}

// OneTimePad encryption done by shifting
char* OneTimePad(const char* str)
{
	int n = strlen(str);
	//get a random key of the same length as the plaintext 
	char* key = getKey(n);
	char* ciphertext = new char[2 * n];
	for (int i = 0; i < n; i++)
	{
		char c = str[i];
		char k = key[i];
		string a = alphanum;
		int ic = a.find(c);
		int ik = a.find(k);
		int size = a.size();
		if (ic != std::string::npos)
		{
			if ((ic + ik) < size )
				ciphertext[i] = alphanum[ic + ik];
			else ciphertext[i] = alphanum[(ic + ik)- size];
		}
		else ciphertext[i] = str[i];
		ciphertext[n + i] = key[i];
	}
	ciphertext[2 * n] = '\0';
	return ciphertext; 
}

char* decryptOTP(const char *str)
{
	cout << endl;
	int n = strlen(str);
	int x = n / 2;
	char* plaintext=new char[x];
	
	for (int i = 0; i < x; i++)
	{
		char c = str[i];
		char k = str[x+i];
		string a = alphanum;
		int ic = a.find(c);
		int ik = a.find(k);
		int size =a.size();
		if (ic != std::string::npos)
		{
			if ( ic > ik )
				plaintext[i] = alphanum [ic - ik];
			else plaintext[i] = alphanum[ (ic-ik) + size];
		}
		else plaintext[i] = str[i];
	}
	plaintext[n/2]= '\0';
	return plaintext;
}
//method that transposes a given string into a matrix of m columns
char* transpose(char* ciphertext, int m)
{
	int n = strlen(ciphertext);
	int t = n / m;;
	if (n % m != 0)
	{
		t++;
	}
	char** transpose;
	transpose = new char*[t];
	for (int i = 0; i < t; i++)
	{
		transpose[i] = new char[m];
	}
	for (int i = 0; i < t; i++)
	{
		for (int j = 0; j < m; j++)
		{
			if ((i*m + j)<n)
				transpose[i][j] = ciphertext[i*m + j];
			else transpose[i][j] = ' ';
		}
	}

	char* newCipher = new char[m*t];
	int ic = 0;
	for (int i = 0; i < m; i++)
		for (int j = 0; j < t; j++)
		{
			newCipher[ic] = transpose[j][i];
			ic++;
		}
	newCipher[m*t] = '\0';
	return newCipher;

}

//mixed transposition and vigenere cipher
char* encryptMixed(char* plaintext, char* key)
{
	int n = strlen(plaintext);
	int m = strlen(key);
	char* ciphertext=new char[n];
	int* k = new int[m];
	string a = alphanum;
	int size = a.length();
	for (int i = 0; i<m; i++)
	{
		k[i] = a.find(key[i]);
	}
	//first we shift the plaintext letters using a vigenere cipher and ´the key given
	for (int i = 0; i < n; i++)
	{	
		int ik = k[i % m];
		int ic = a.find(plaintext[i]);
		if (ic != std::string::npos)
		{
			if ( size > (ic + ik))
			{
				ciphertext[i] = alphanum[ic + ik];
			}
			else 
			{
				ciphertext[i] = alphanum[(ic + ik) - size];
			}
		}
		else ciphertext[i] = plaintext[i];
	}
	ciphertext[n] = '\0';

	//here is where we transpose the ciphertext in a mtrix with m colums m=key length
	char* newCipher = transpose(ciphertext, m);

	return newCipher;


}
char* decryptMixed(char* ciphertext, char* key)
{
	int n = strlen(ciphertext);
	int m = strlen(key);
	char* plaintext = new char[n];
	int* k = new int[m];
	string a = alphanum;
	int size = a.length();
	for (int i = 0; i<m; i++)
	{
		k[i] = a.find(key[i]);
	}

	//first transpose the ciphertext by using the same function but giving it a different column number
	char* newPlain = transpose(ciphertext, n / m);
	//now shift letters using the given key and vigenere cipher
	for (int i = 0; i < n; i++)
	{
		int ik = k[i % m];
		int ic = a.find(newPlain[i]);
		if (ic != std::string::npos)
		{
			if ( 0 < (ic - ik))
				plaintext[i] = alphanum[ic - ik];
			else plaintext[i] = alphanum[ size - ( ik - ic)];
		}
		else plaintext[i] = newPlain[i];
	}
	plaintext[n]= '\0'; 
	return plaintext;

}


//encrypt file using OTP
void encryptFile(string filename)
{
	int i=filename.find(".txt");
	string sx = filename;
	string fileTemp;
	fileTemp = sx.substr(0, i);
	fileTemp = fileTemp + "Temp.txt";
	ifstream f(filename);
	ofstream g(fileTemp);
	if (!f)
	{
		cout << "Invalid file name: ";
	}
	else {
		while (!f.eof())
		{
			string s;
			getline(f, s);
			const char *str = s.c_str();
			g << OneTimePad(str) << endl;
		}
		f.close();
		g.close();
		const char *str1 = filename.c_str();
		std::remove(str1); // delete file
		const char *str2 = fileTemp.c_str();
		int result = rename(str2, str1);

		if (result == 0)
			puts("File successfully encrypted.");
		else
			perror("Error renaming file.");
	}
}

//decrypt OTP encrypted file 
void decryptFile(string filename)
{
	ifstream f(filename);
	int i = filename.find(".txt");
	string sx = filename;
	string fileTemp;
	fileTemp = sx.substr(0, i);
	fileTemp = fileTemp + "Decrypted.txt";
	ofstream g(fileTemp);
	if (!f)
	{
		cout << "Invalid file name: ";
	}
	else {
		while (!f.eof())
		{
			string s;
			getline(f, s);
			const char *str = s.c_str();
			cout << decryptOTP(str) << endl;
		}
		f.close();
		g.close();
	}
}

void menu()
{
	cout << "What would you like to do? " << endl;
	cout << "(e)Encrypt file" << endl;
	cout << "(d)Read file" << endl;
	cout << "(q) Quit" << endl;
	char d;
	cin >> d;
	cout << "Give file path: " << endl;
	cin.ignore(1000, '\n');
	string s;
	getline(cin, s);
	switch (d)
	{
	case 'e':
	{
		encryptFile(s);
	}
	break;
	case 'd':
	{

		decryptFile(s);
	}
	break;
	case 'q':
	{
		cout << "Quitting now." << endl;
	}
	break;
	}



}
int main()
{
	//menu();
	char* e = encryptMixed("I got secrets and you dont know them!", "sEcreTKY");
	cout << e <<endl;
	cout << decryptMixed(e,"sEcreTKY")<<endl;

	char* v = encryptVernam("I got secrets and you dont know them!");
	cout << v << endl;
	cout << decryptVernam(v);
    return 0;
}

