#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <cstdlib>
#include "des.h"
#include "locale.h"

using namespace std;



void StrFromBlock(char * str, const Block & block)
{
	memset(str, 0, 8);
	for (size_t i = 0; i < block.size(); ++i)
	{
		if (true == block[i])
			*((unsigned char *)(str)+i / 8) |= (1 << (7 - i % 8));
	}
}

void BlockFromStr(Block & block, const char * str)
{
	for (size_t i = 0; i < block.size(); ++i)
	{
		if (0 != (*((unsigned char *)(str)+i / 8) & (1 << (7 - i % 8))))
			block[i] = true;
		else 	block[i] = false;
	}
}


int main()
{
	setlocale(LC_ALL, "Russian");
	char c = 0;
	Method method;
	string infile,outfile="output.txt";
	while (true){
		cout << "Чтобы зашифровать файл введите e, чтобы расшифровать d" << endl;
		cin >> c;
		
		switch (c)
		{
		case 'e':
			method = e;
			break;
		case 'd':
			method = d;
			break;

		default:
			cout << "Некорректный ввод!" << endl;
			continue;
			break;
		}
		break;
	}

	cout << "Введите ключ: (8 символов)" << endl;
	char key[32] = "";
	while (true){
		cin >> key;
		if (strlen(key) < 8 || strlen(key)>8){
			cout << "Неверная длина ключа" << endl;
			continue;
		}
		break;
	}
	cout << "Введите имя входного файла: " << endl;
	cin >> infile;
	
	int add_count;
	ifstream srcFile(infile);
	ofstream tarFile(outfile);
	if (!srcFile || !tarFile) cout << "Ошибка открытия файла" << endl;

	Block block, bkey;
	BlockFromStr(bkey, key);
	char buffer[8];
	int n=0;
	while (true)
	{
		if (srcFile.eof()){ 

			if (method == 0){
				for (int i = 0; i < n; i++) buffer[i] = '0';
				buffer[7] = n + 8 + '0';

				BlockFromStr(block, buffer);//выделение блока из строки
				des(block, bkey, method);  //шифрование блока



				StrFromBlock(buffer, block);//блок в строку
				tarFile.write(buffer, 8);// запись в файл
			}
			
			break; };
		memset(buffer, 0, 8);//заполнение 0ми
		srcFile.read(buffer, 8);//чтение по 8байт 
		
		if (srcFile.gcount() == 0) break;
		n = 8 - srcFile.gcount();
		if (srcFile.gcount() != 8)
		{
			for (int i = 0; i<n; i++) buffer[7 - i] = '0'; //дополнение если считалось не 8 байт
		}

		BlockFromStr(block, buffer);//выделение блока из строки
		des(block, bkey, method);  //шифрование блока
		
		

		StrFromBlock(buffer, block);//блок в строку
		tarFile.write(buffer, 8);// запись в файл

		
	}
	
	
	srcFile.close();
	tarFile.close();

	if (method == 1){

		std::ofstream outFile("tmp_outfile.txt");

		std::string line;
		

		ifstream inFile("output.txt", ios::in);
		inFile.seekg(-1, inFile.end);
		char ch = inFile.get();

		n = ch - '0';
		inFile.close();

		inFile.open("output.txt");
		string buf;
		inFile >> buf;
		buf.erase(buf.size()-n, n);
		outFile << buf;


		inFile.close();
		outFile.close();

		std::remove("output.txt");
		std::rename("tmp_outfile.txt", "output.txt");

		
	};
	cout << "Выполнено! см. Файл "<< outfile << endl;
	return 0;
}
