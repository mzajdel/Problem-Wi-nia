# Problem Wieźnia

## Opis problemu

[Artykuł na Wikipedii](https://pl.wikipedia.org/wiki/Dylemat_wi%C4%99%C5%BAnia)

## Pojęcia

* gra - podjęcie przez dwóch graczy decyzji - zdradzić czy współpracować w celu uzyskania dla siebie jak najmniejszego wyroku
* gracz / wiezień / osobnik - jedna osoba która gra o jak nakrótszy wyrok z innym więźniem, może podjąć decyzje - zdradzić lub współpracować
* chromosom - tablica strategii (64 bity) oraz stanu trzech poprzednich rozgrywek (6 bitów)
* populacja - ustalony zbiór graczy, który aktualnie podejmuje ze sobą rozgrywki
* iteracja - etap wykonywanego zadania, który pozwala na przeprowadzenie rozgrywki i stworzenie nowej populacji


## Dane

* tablica poprzednich rozgrywek - 6 bitów, które zapisują 3 poprzednie rozgrywki więźnia, np [ZW|ZW|ZW] oznacza, że w poprzednich trzech rozgrywkach gracz 1 zawsze zdradzał, a gracz dwa zawsze współpracował. Możliwych kombinacji tych strategii jest 2^6 = 64
* tablica strategii - 64 bity, które kodują decyzje więźnia w zależności od tablicy poprzednich rozgrywek jego przeciwnika. Jeśli potraktujemy 3 poprzednie liczby jako liczbę zapisaną dwójkowo [ZZ|ZZ|ZZ] -> 0, [WW|WW|WW] -> 63 to możemy użyć jej jako indeksu tablicy strategii do podjęcia aktualnej decyzji - zdradzić czy współpracować.
* osobnik - składa się z chromosomu (tablica strategii oraz tablica 3 poprzednich rozgrywek) oraz wyroku, potrafi na podstawie 

## Algorytm

1. tworzymy populację o zadanej wielkości z losowymi wartościami chromosomów
1. przeprowadzamy iteracje według następujących kroków
    * gracze z obecnej populacji w parach, grają o najkrótszy wyrok, wynik zostaje dopisany do ich punktów (wyroku)
    * populacja jest sortowana według wyników - gracz z najkrótszym wyrokiem jest najlepszym osobnikiem
    * gorsza część graczy zostaje odzucona, lepszą poddajemy krzyżowaniu wg wybranej strategii elitaryzmu
    * wprowadzamy losowe zmiany do części chromosomów
    * ze skrzyżowanych osobników tworzymy nową populację
1. wybieramy najlepszego osobnika i decydujemy czy wyrok jest satysfakcjonujący
1. kończymy dla satysfakcjinującego wyroku, w przeciwnym wypadku powtarzamy iteracje


## !!!Problemy!!!
* jak ma działać krzyżowanie - jeśli rozetniemy 70 bitów i skleimy to prawie zawsze niezmienne pozostanie ostatnie 6 bitów (tablica poprzednich trzech rozgrywek), może trzeba ją przeciąż odzielnie? albo za każdym razem losować od nowa? czy przesunąć? tego nie wiem
