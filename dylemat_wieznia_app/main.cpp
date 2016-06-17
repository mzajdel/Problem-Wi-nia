#include <iostream>
#include <vector>
#include <ctime> 
#include <cstdlib>
#include <cmath>


//#define WSPOLPRACA 0
//#define ZDRADA 1

#define LICZBA_WIEZNIOW 20
int licznik = 0; //zmienna kontrolna

using namespace std;

class Osobnik
{
public:
	bool chromosom[64];
	bool poprzednie[6];
	unsigned int wyrok;
	Osobnik()
	{
		wyrok = 0;
	}

	void inicjalizuj()
	{
		for (int i = 0; i < 64; i++) {
			chromosom[i] = rand() % 2;
			cout << chromosom[i];
		}
		for (int i = 0; i < 6; i++)
			poprzednie[i] = rand() % 2;
	}

	void wyswietl_chromosom()
	{
		cout << "Chromosom: " << "[";
		for (int i = 0; i < 63; i++)
		{
			cout << chromosom[i] << ",";
		}
		cout << chromosom[63] << "]" << endl << endl;
	}
};

void inicjalizuj_populacje(vector <Osobnik> &populacja)
{
	Osobnik *wsk;
	srand(time(NULL));
	for (int i = 0; i < LICZBA_WIEZNIOW; i++)
	{
		wsk = new Osobnik;
		wsk->inicjalizuj();
		populacja.push_back(*wsk);
	}
}

int bin_to_dec(bool *bin)
{
	int dec = 0;
	int j = 0;
	for (int i = 5; i >=0; i--)
	{
		if (bin[i] == true)
			dec += pow(2, j);
		j++;
	}
	return dec;
}

void przesluchanie(Osobnik &A, Osobnik &B)
{
	licznik++;
	cout << "przesluchanie nr " << licznik << endl;
	bool a = A.chromosom[bin_to_dec(B.poprzednie)]; //wiêzieñ A podejmuje decyzjê w oparciu o strategie wiêŸnia B w poprzednich przes³uchaniach
	bool b = B.chromosom[bin_to_dec(A.poprzednie)]; //wiêzieñ B analogicznie
	
	if (a == false && b == false)
	{
		A.wyrok += 2;
		B.wyrok += 2;
	}
	else if (a == false && b == true)
	{
		A.wyrok += 10;
	}
	else if (a == true && b == false)
	{
		B.wyrok += 10;
	}
	else if (a == true && b == true)
	{
		A.wyrok += 10;
		B.wyrok += 10;
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



int main() 
{
	vector <Osobnik> populacja;	//wektor przechowuj¹cy populacjê - wszystkich wiêŸniów
	inicjalizuj_populacje(populacja);
	
	for (int i = LICZBA_WIEZNIOW - 1; i >= 0; i--)
	{
		for (int j = i - 1; j >= 0; j--)
		{
			przesluchanie(populacja[i], populacja[j]);
		}
	}


	system("pause");
	return 0;
}