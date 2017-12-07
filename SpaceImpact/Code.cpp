/*	Projekat - Space Impact
 *
 *	PMF - Programiranje II
 *
 *	Projekat je inspirisan igricom razvijenom za Nokia mobitele.
 *	Iskoristite vase oruzje da prezivite!
 *	Imate mogucnost da se krecete horizontalno te da unistavate neprijatelje.
 *	Pazite da neprijatelji ne prodju u vasu bazu!
 *
 *	Studenti:
 *		Agic Naida
 *		Ibric Dzemal
 *	Odsjek i smjer:
 *		Matematika - Teorijska kompjuterska nauka
 */

#include <iostream>
#include <vector>
#include <iomanip> //ukljuceno radi funkcije setw()
#include<windows.h>	//ukljuceno radi funkcija Sleep() i Beep()
#include<conio.h> //ukljuceno radi _kbhit
#include <algorithm>
#include <string>
#include <fstream>

enum Tijela{ //svi specificni ASCII karakteri koje cemo koristiti
	Glupi = 194,
	Jaci = 197,
	Igrac = 202,
	Bum = 248,
	UspravniRub = 186,
	VodoravniRub = 205,
	GornjiDesni = 187,
	GornjiLijevi = 201,
	DonjiDesni = 188,
	DonjiLijevi = 200
};
using namespace std;
void NoviRedovi(int n){
	for(int i = 0; i < n; i++)	cout << endl;
}
void IspisujJezivo(const string x,int j = 200){
	for(unsigned int i = 0; i < x.size(); i++){
		Sleep(j);
		cout << x[i];
	}
}
class Metak{
	int visina;
	int lokacija;
	bool ziv;
	Metak(int v, int l, bool z){visina = v; lokacija = l; ziv = z;}
	friend class Igra;
};
class Neprijatelj{
	int visina;
	int lokacija;
	bool ziv;
	char izgled;
	Neprijatelj(int v, int l, bool z,char i){visina = v; lokacija = l; ziv = z; izgled = i;}
	friend class Igra;
};
class Igra{
		static const int lijevaMargina{40};
			//pravimo razmak od lijeve margine
		static const int visina{29};
		static const int sirina{21};
			//visina i sirina polja u kojem ce se igra odvijati
		vector<vector<char> > Matrica;
			//matrica u kojoj se odvija igrica
		int skor{0};
			//skor igraca
		int DajSkor()const{ return skor; }
			//metoda vraca dosadasnji rezultat igraca
		void NapraviPocetnoPolje();
			//metoda pravi pocetno prazno polje
		Neprijatelj GenerisiNeprijatelja();

		bool PomjeriNeprijatelja(vector<Neprijatelj>& enemies);
			//metoda koja omogucava neprijateljima da 'padaju'
		void PomjeriBrodic(int stara_lokacija, int nova_lokacija);
			//metoda pomjera igraca po matrici
		void IspisiPolje() const;
			//metoda ispisuje sadrzaj matrice
		Metak Pucaj(int lokacija);
			//metoda postavlja metak u polje
		bool ProvjeriSudar(vector<Neprijatelj>& enemies,vector<Metak>& met);
			//metoda provjerava da li je doslo do sudara izmedju metaka igraca i neprijatelja
		void LetiMetak(vector<Metak>& pucpuc);
			//metoda pomjera metak igraca po matrici
		void IspisiPocetak();
			//metoda ispisuje pocetni ekran
		void IspisiKraj();
			//metoda ispisuje zavrsni ekran
		void Akcije(int lokacija);
			//metoda se brine o desavanjima tokom igre
	public:
		void Pocni();
		friend class Neprijatelj;
		friend class Metak;
};

void Igra::NapraviPocetnoPolje(){
	Matrica.resize(visina);
	for(int i = 0; i < visina; i++){
		Matrica[i].resize(sirina);
	}
	for(int i = 0; i < visina; i ++){
		for(int j = 0; j < sirina; j++){
			if( (i == 0 || i == visina - 1) && Matrica[i][j] == 0) Matrica[i][j] = VodoravniRub;
			else if( (j == 0 || j == sirina - 1) && Matrica[i][j] == 0) Matrica[i][j] = UspravniRub;
			else Matrica[i][j]=' ';
		}
	}
	Matrica[0][0] = GornjiLijevi;
	Matrica[visina - 1][sirina - 1] = DonjiDesni;
	Matrica[0][sirina - 1] = GornjiDesni;
	Matrica[visina - 1][0] = DonjiLijevi;
	Matrica[visina - 2][sirina / 2] = Igrac;
}
Neprijatelj Igra::GenerisiNeprijatelja(){
	//visina 1
	//sirina [1,sirina-1)
	int nasumicno_polje = rand() % (sirina - 2) + 1;
	if(rand()%5 == 2){
		Matrica[1][nasumicno_polje] = Jaci;
		return Neprijatelj{1, nasumicno_polje, true, char(Jaci)};
	}
	else {
		Matrica[1][nasumicno_polje] = Glupi;
		return Neprijatelj{1, nasumicno_polje, true, char(Glupi)};
	}
}
bool Igra::PomjeriNeprijatelja(vector<Neprijatelj>& enemies){
	int br{};
	for(Neprijatelj &x: enemies){
		if(x.ziv){
			if(x.visina >= visina - 3) {
			return true;
			}

			else{
				x.visina++;
				Matrica[x.visina-1][x.lokacija] = ' ';
				Matrica[x.visina][x.lokacija] = x.izgled;
			}
		}
		br++;
	}
}
void Igra::PomjeriBrodic(int stara_lokacija, int nova_lokacija){
	Matrica[visina - 2][stara_lokacija] = ' ';
	Matrica[visina - 2][nova_lokacija] = Igrac;
}
void Igra::IspisiPolje()const{
	for(int i = 0; i < visina; i ++){
		cout << right << setw(lijevaMargina);
		for(int j = 0; j < sirina; j++){
			cout << Matrica[i][j];
		}
		if(i==visina/2)cout<<" Skor: "<<skor;
		cout << endl;
	}

	cout << right << setw(55) << "Komande:" << endl;
	cout << setw(57) << "SPACEBAR - pucaj" << endl;
	cout << setw(57) << "<= - idi lijevo" << endl;
	cout << setw(57) << "=> - idi desno" << endl;
	cout << setw(57) << "X - prekini igru" << endl;
}
Metak Igra::Pucaj(int lokacija){
	Matrica[visina - 3][lokacija] = Bum;
	return{visina - 3, lokacija, true};
}
bool Igra::ProvjeriSudar(vector<Neprijatelj>& enemies,vector<Metak>& met){
	for(int i=0;i<enemies.size();i++){
		for(int j=0;j<met.size();j++){
			if((enemies[i].visina==met[j].visina || enemies[i].visina==met[j].visina+1) && enemies[i].lokacija==met[j].lokacija){
				if(enemies[i].izgled == char(Glupi)) enemies[i].ziv = false;
				else enemies[i].izgled = char(Glupi);
				met[j].ziv=false;
				return true;
			}
		}
	}
}
void Igra::LetiMetak(vector<Metak>& pucpuc){
	for(Metak &x: pucpuc){
		x.visina--;
		if(x.ziv){
			if(x.visina <= 0){
				Matrica[x.visina+1][x.lokacija] = ' ';
				x.ziv = false;
			}
			else{
				Matrica[x.visina+1][x.lokacija] = ' ';
				Matrica[x.visina][x.lokacija] = Bum;
			}
		}
	}
}
void Igra::IspisiPocetak(){
	for(int i = 0; i < 10; i++) cout << endl;
	cout << setw(62) << "S P A C E     I M P A C T";
}
void Igra::IspisiKraj(){
	system("cls");
	NoviRedovi(10);
	cout << right << setw(40);
	IspisujJezivo("G A M E  O V E R");
	cout << endl << endl; Sleep(500);
	cout << right << setw(30);
	IspisujJezivo("Vas rezultat je: ", 100);
	cout << skor << endl << endl << setw(30); Sleep(500);
	ifstream ulaz("Highscore.txt");
	if(!ulaz){
		ofstream izlaz("Highscore.txt");
		ifstream ulaz("Highscore.txt");
	}
	int highscore;
	ulaz >> highscore;
	IspisujJezivo("Dosadasnji highscore je: ", 100);
	cout << highscore << endl << setw(30);
	if(skor > highscore){
		cout << setw(30);
		IspisujJezivo("Cestitam! Pobijedili ste prethodni highscore!", 100);
		cout << endl << right << setw(30);
		IspisujJezivo("Novi highscore je: ", 100);
		cout << skor;
		ofstream izlaz("Highscore.txt");
		izlaz << skor;
	}
	cout << endl << endl;
	cout << right << setw(30);
	IspisujJezivo("Zelite li igrati ponovo?(y/n)", 100);
	cout << endl;
	int key_code;
	while(1){
		if(_kbhit()){
			key_code = _getch();
			if(key_code == 'n'){
				cout << right << setw(40);
				IspisujJezivo("Cao ??", 100);
				return;
			}
        	else if(key_code == 'y'){
        		system("cls");
        		NoviRedovi(10);
        		IspisujJezivo("Spremi se, igra uskoro pocinje...", 100);
        		cout << endl;
        		cout<<"3"<<endl; Beep(500,800);
        		cout<<"2"<<endl; Beep(500,800);
        		cout<<"1"<<endl; Beep(500,800);
        		cout<<"GO"<<endl; Beep(1000,800);
        		system("cls");
        		Matrica.clear();
        		Pocni();
        		break;
			}
		}
	}
}
void Igra::Akcije(int lokacija){
	int brojac{};
	vector<Metak> pucnjevi;
	vector<Neprijatelj> enemies;
	int nova_lokacija = lokacija;
	int key_code;
	while (1){
		brojac++;
		system("cls");
    	if (_kbhit()){
        	key_code = _getch();
        	if(key_code == 75) { //lijevo
        		nova_lokacija--;
        		if(nova_lokacija == 0) nova_lokacija++;
        		else{
					PomjeriBrodic( lokacija, nova_lokacija);
					lokacija = nova_lokacija;
				}
			}
			else if(key_code == 77){ //desno
				nova_lokacija++;
				if(nova_lokacija == sirina - 1) nova_lokacija--;
				else{
					PomjeriBrodic(lokacija, nova_lokacija);
					lokacija = nova_lokacija;
				}
			}
			else if(key_code == ' '){ //pucaj
				pucnjevi.push_back(Pucaj(lokacija));
				Beep(500,30);
			}
			else if(key_code == 'x' || key_code == 'X'){ //prekini
				system("cls");
				IspisiKraj();
				break;
			}
		}
		IspisiPolje();
		Sleep(10);
		if(rand() % 20 == 0){
			enemies.push_back(GenerisiNeprijatelja());
		}
		if(enemies.size() != 0 ){
			for(unsigned int i = 0; i < enemies.size(); i++){
				if(!enemies[i].ziv){
					Matrica[enemies[i].visina][enemies[i].lokacija] = ' ';
					enemies.erase(enemies.begin()+i);
				}
			}
			if(brojac % 5 == 0)
				if(PomjeriNeprijatelja(enemies)){
					IspisiKraj();
					break;
				}

		}
		if(ProvjeriSudar(enemies,pucnjevi)){
			Beep(1000,100);
			skor++;
		}
		if(pucnjevi.size() != 0) {
			LetiMetak( pucnjevi);
			for(unsigned int i = 0; i < pucnjevi.size(); i++)
				if(!pucnjevi[i].ziv) {
					Matrica[pucnjevi[i].visina+1][pucnjevi[i].lokacija] = ' ';
					pucnjevi.erase(pucnjevi.begin() + i);
				}
		}
}

}
void Igra::Pocni(){
	IspisiPocetak();
	Sleep(1000);
	system("cls");
	skor=0;
	NapraviPocetnoPolje();
	IspisiPolje();
	Akcije(sirina/2);
}
int main(){
	Igra Space;
	Space.Pocni();
	return 0;
}
