#include<fstream>
using namespace std;
void readData(int& curentLevel, bool& themeSound, bool& gamePlaySound) {
	ifstream fin;
	fin.open("data.dat", ios::binary);
	if (!fin) {
		ofstream fout;
		fout.open("data.dat", ios::binary);
		curentLevel = 1;
		themeSound = 0;
		gamePlaySound = 0;
		fout << curentLevel << themeSound << gamePlaySound;
		fout.close();
	}
	fin >> curentLevel >> themeSound >> gamePlaySound;
	fin.close();
}
void writeData(int curentLevel, bool themeSound, bool gamePlaySound) {
	ofstream fout;
	fout.open("data.dat", ios::binary|ios::trunc);
	fout << curentLevel << themeSound << gamePlaySound;
	fout.close();
}