#include <iostream>
#include <vector>
#include <ctime> 
//#include <cstdlib>
#include <cmath>
#include <algorithm>


#define WSPOLPRACA 0 //MILCZENIE
#define ZDRADA 1 // SYPANIE

#define LICZBA_WIEZNIOW 1000
#define LICZBA_ITERACJI 1
 
#define wspolczynnik_elitaryzmu 0.2
#define prawdop_mutacji 0.3

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
	unsigned int ilosc_przesluchan;
	Osobnik()
	{
		static int id_stat = 0;
		id_stat++;
		id = id_stat;
		wyrok = 0;
		ilosc_przesluchan = 0;
		
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
	int sredni_wyrok;
	unsigned int srednia()
	{
		sredni_wyrok = wyrok / ilosc_przesluchan;
		return sredni_wyrok;
	}
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
		for(int i = 0; i < all.size(); i++) {
			all[i].srednia();
		}
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

	void mutacja(Osobnik &o){

		int gen = rand()%64;

		if(o.chromosom[gen]==WSPOLPRACA)
			o.chromosom[gen]==ZDRADA;
		else
			o.chromosom[gen]==WSPOLPRACA;
	}

	void krzyzowanie(Populacja &p, Populacja &temp){

		int i1;
		int i2;
		int punkt_krzyzowania;
		Osobnik nowy_osobnik1;
		Osobnik nowy_osobnik2;

		for (int i=pozostalosc_osobnikow; i<LICZBA_WIEZNIOW;i=i+2){

			i1=rand()%LICZBA_WIEZNIOW;
			i2=rand()%LICZBA_WIEZNIOW;
			punkt_krzyzowania=rand()%63+1;

			//krzyzowanie dla nowy_osobnik1
			for(int i=0;i<punkt_krzyzowania;i++)
				nowy_osobnik1.chromosom[i]=p.all[i1].chromosom[i];

			// nowy_osobnik1.wyrok = (p.all[i1].wyrok + p.all[i2].wyrok) / 2; nowy_osobnik2.wyrok = (p.all[i1].wyrok + p.all[i2].wyrok) / 2;
	
			for(int i=punkt_krzyzowania;i<64;i++)
				nowy_osobnik1.chromosom[i]=p.all[i2].chromosom[i];

			//krzyzowanie dla nowy_osobnik2
			for(int i=0;i<punkt_krzyzowania;i++)
				nowy_osobnik2.chromosom[i]=p.all[i2].chromosom[i];
	
			for(int i=punkt_krzyzowania;i<64;i++)
				nowy_osobnik2.chromosom[i]=p.all[i1].chromosom[i];

			//mutacja
			if(((float)rand() / RAND_MAX)<0.2)
				mutacja(nowy_osobnik1);

			if(((float)rand() / RAND_MAX)<0.2)
				mutacja(nowy_osobnik2);

			temp.all.push_back(nowy_osobnik1);
			temp.all.push_back(nowy_osobnik2);
		}
	}

};


/******************************************************************************************************
																												GRA
******************************************************************************************************/

void przesluchanie(Osobnik &A, Osobnik &B)
{
	licznik++;
	//cout << "przesluchanie nr " << licznik << endl;
	bool a = A.chromosom[bin_to_dec(B.poprzednie)]; //wiêzieñ A podejmuje decyzjê w oparciu o strategie wiêŸnia B w poprzednich przes³uchaniach
	bool b = B.chromosom[bin_to_dec(A.poprzednie)]; //wiêzieñ B analogicznie

	A.ilosc_przesluchan++;
	B.ilosc_przesluchan++;

	if (a == WSPOLPRACA && b == WSPOLPRACA)
	{
		A.wyrok += 10;
		B.wyrok += 10;
	}
	else if (a == WSPOLPRACA && b == ZDRADA)
	{
		A.wyrok += 50;
	}
	else if (a == ZDRADA && b == WSPOLPRACA)
	{
		B.wyrok += 50;
	}
	else if (a == ZDRADA && b == ZDRADA)
	{
		A.wyrok += 30;
		B.wyrok += 30;
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

	Populacja *temp_populacja=new Populacja();


for(int i=0;i<LICZBA_ITERACJI;i++){
	graj_kazy_z_kazdym(populacja->all);

	populacja->sortuj();

	/*for (int i = 0; i < populacja->size(); ++i)
	{
		populacja->get(i).wyswietl_osobnika();
	}*/

	//populacja->all[0].wyswietl_osobnika();
	//cout<<populacja->all[0].wyrok;

	//cout<<endl<<endl<<endl<<endl;
	populacja->elitaryzm(*populacja,*temp_populacja);
	populacja->krzyzowanie(*populacja,*temp_populacja);

	/*for(int i=0;i<pozostalosc_osobnikow;i++)
		cout<<temp_populacja->all[i].wyrok<<endl;
	cout<<endl<<endl;*/	
	/*for(int i=0;i<LICZBA_WIEZNIOW;i++){
//conflict
	for(int i=0;i<pozostalosc_osobnikow;i++)
		cout<<temp_populacja->all[i].srednia()<<endl;



	for(int i=0;i<LICZBA_WIEZNIOW;i++){
//conflict
		cout<< i << ") ";
		temp_populacja->all[i].wyswietl_chromosom();
	}*/

	for(int i=0;i<LICZBA_WIEZNIOW;i++)
		populacja->all[i]=temp_populacja->all[i];
	temp_populacja->all.clear();
}

	populacja->all[0].wyswietl_osobnika();
	cout<<populacja->all[0].wyrok;


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
		cout << "\"Chromosom\": " << "[";
		for (int i = 0; i < 63; i++)
		{
			cout << chromosom[i] << ",";
		}
		cout << chromosom[63] << "], " << endl;
	}

	void Osobnik::wyswietl_poprzednie() {
		cout << "\"Poprzednie rozgrywki\": " << "[";
		for (int i = 0; i < 6; i++)
		{
			cout << poprzednie[i] << ", ";
		}
		cout << poprzednie[5] << "], " << endl;
	}

	void Osobnik::wyswietl_osobnika() {
		cout << "{" << "\"Osobnik\": " << id << ", " << endl;
		wyswietl_chromosom();
		wyswietl_poprzednie();
		cout << "\"Sredni_wyrok\": " << srednia() << ", " << endl <<

		"\"liczba_wiezniow\": " << LICZBA_WIEZNIOW << ", " << endl <<
		"\"liczba_iteracji\": " << LICZBA_ITERACJI << ", " << endl <<
		"\"wspolczynnik_elitaryzmu\": " << wspolczynnik_elitaryzmu << ", " << endl << 
		"\"prawdopodobienstwo_mutacji\": " << prawdop_mutacji <<
		"}" << endl << endl;
	}

  inline bool Populacja::porownaj_wyrok::operator() (const Osobnik& osobnik1, const Osobnik& osobnik2)
  {
      return osobnik1.sredni_wyrok < osobnik2.sredni_wyrok;
  }

