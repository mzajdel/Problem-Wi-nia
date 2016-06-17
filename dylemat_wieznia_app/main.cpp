#include <iostream>
#include <vector>
#include <ctime> 
//#include <cstdlib>
#include <cmath>
#include <algorithm>


#define WSPOLPRACA 0
#define ZDRADA 1

#define LICZBA_WIEZNIOW 20
 
#define wspolczynnik_elitaryzmu 0.1

const int pozostalosc_osobnikow=(int)(wspolczynnik_elitaryzmu*LICZBA_WIEZNIOW);
int licznik = 0; //zmienna kontrolna

int bin_to_dec(bool *bin);
using namespace std;

/*****************************************************************************************************
																									OSOBNIK
*****************************************************************************************************/


class Osobnik
{
public:
	bool chromosom[64];

	bool poprzednie[6];
	unsigned int wyrok;
	int id;
	Osobnik()
	{
		static int id_stat = 0;
		id_stat++;
		id = id_stat;
		wyrok = 0;
	}

	void inicjalizuj()
	{
		for (int i = 0; i < 64; i++) {
			chromosom[i] = rand() % 2;
		}
		for (int i = 0; i < 6; i++)
			poprzednie[i] = rand() % 2;
	}

	void wyswietl_chromosom();
	void wyswietl_poprzednie();
	void wyswietl_osobnika();
};

/*************************************************************************************************
																							POPULACJA
*************************************************************************************************/

class Populacja
{
public:
	vector <Osobnik> all;	//wektor przechowuj¹cy populacjê - wszystkich wiêŸniów
	Populacja() {
		//inicjalizuj();
	}
	// ~Populacja();

	void inicjalizuj()
	{
		Osobnik *wsk;
		srand(time(NULL));
		for (int i = 0; i < LICZBA_WIEZNIOW; i++)
		{
			wsk = new Osobnik;
			wsk->inicjalizuj();
			all.push_back(*wsk);
			delete wsk; //avoid memory leak
		}
	}

	struct porownaj_wyrok {
		inline bool operator() (const Osobnik& osobnik1, const Osobnik& osobnik2); 
	};

	void sortuj() {
		sort(all.begin(), all.end(), porownaj_wyrok());
	}

	void erase_half() {
		all.erase(all.begin()+all.size()/2, all.begin()+all.size());
	}

	Osobnik get(int el) 
	{
		return all[el];
	}

	int size() {
		return all.size();
	}

	void elitaryzm( Populacja &p, Populacja &temp){

		for(int i=0;i<pozostalosc_osobnikow;i++)
			temp.all.push_back(p.all[i]);		
	}

};


/******************************************************************************************************
																												GRA
******************************************************************************************************/

void przesluchanie(Osobnik &A, Osobnik &B)
{
	licznik++;
	cout << "przesluchanie nr " << licznik << endl;
	bool a = A.chromosom[bin_to_dec(B.poprzednie)]; //wiêzieñ A podejmuje decyzjê w oparciu o strategie wiêŸnia B w poprzednich przes³uchaniach
	bool b = B.chromosom[bin_to_dec(A.poprzednie)]; //wiêzieñ B analogicznie
	
	if (a == WSPOLPRACA && b == WSPOLPRACA)
	{
		A.wyrok += 1;
		B.wyrok += 1;
	}
	else if (a == WSPOLPRACA && b == ZDRADA)
	{
		A.wyrok += 5;
	}
	else if (a == ZDRADA && b == WSPOLPRACA)
	{
		B.wyrok += 5;
	}
	else if (a == ZDRADA && b == ZDRADA)
	{
		A.wyrok += 3;
		B.wyrok += 3;
	}
	else 
		cout << "BLAD!" << endl;
	
	A.poprzednie[0] = A.poprzednie[2];
	A.poprzednie[1] = A.poprzednie[3];
	A.poprzednie[2] = A.poprzednie[4];
	A.poprzednie[3] = A.poprzednie[5];
	A.poprzednie[4] = a;	//decyzja podjêta w aktualnym przes³uchaniu przez wiêŸnia A
	A.poprzednie[5] = b;	//decyzja podjêta w aktualnym przes³uchaniu przez wiêŸnia B

	B.poprzednie[0] = B.poprzednie[2];
	B.poprzednie[1] = B.poprzednie[3];
	B.poprzednie[2] = B.poprzednie[4];
	B.poprzednie[3] = B.poprzednie[5];
	B.poprzednie[4] = b;
	B.poprzednie[5] = a;
	
}

void graj_kazy_z_kazdym(vector <Osobnik> &populacja) {
	for (int i = LICZBA_WIEZNIOW - 1; i >= 0; i--)
	{
		for (int j = i - 1; j >= 0; j--)
		{
			przesluchanie(populacja[i], populacja[j]);
		}
	}
}


/******************************************************************************************************
																								MAIN
******************************************************************************************************/


int main() 
{
	
	Populacja *populacja = new Populacja();
	populacja->inicjalizuj();

	graj_kazy_z_kazdym(populacja->all);

	populacja->sortuj();

	populacja->erase_half();

	for (int i = 0; i < populacja->size(); ++i)
	{
		populacja->get(i).wyswietl_osobnika();
	}

	Populacja *temp_populacja=new Populacja();

	populacja->elitaryzm(*populacja,*temp_populacja);

	for(int i=0;i<pozostalosc_osobnikow;i++)
		cout<<temp_populacja->all[i].wyrok<<endl;

	delete populacja;


	system("pause");
	return 0;
}






// ----------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------




int bin_to_dec(bool *bin)
{
	int dec = 0;
	int j = 0;
	for (int i = 5; i >=0; i--)
	{
		if (bin[i] == true)
			dec += pow (2.0, j);
		j++;
	}
	return dec;
}

	void Osobnik::wyswietl_chromosom()
	{
		cout << "Chromosom: " << "[";
		for (int i = 0; i < 63; i++)
		{
			cout << chromosom[i] << ",";
		}
		cout << chromosom[63] << "]" << endl;
	}

	void Osobnik::wyswietl_poprzednie() {
		cout << "Poprzednie rozgrywki: " << "[";
		for (int i = 0; i < 6; i++)
		{
			cout << poprzednie[i] << ", ";
		}
		cout << poprzednie[5] << "]" << endl;
	}

	void Osobnik::wyswietl_osobnika() {
		cout << "Osobnik: " << id << endl;
		wyswietl_chromosom();
		wyswietl_poprzednie();
		cout << "Wyrok: " << wyrok << " lat." << endl << endl;
	}

  inline bool Populacja::porownaj_wyrok::operator() (const Osobnik& osobnik1, const Osobnik& osobnik2)
  {
      return (osobnik1.wyrok < osobnik2.wyrok);
  }
