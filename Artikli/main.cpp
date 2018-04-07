#include <iostream>
#include <memory>
#include <new>
#include <cstring>

using namespace std;

char * crt = "\n----------------------------------------------------\n";
struct Datum {
int * _dan, *_mjesec, *_godina;
void Unos(int d, int m, int g) {
_dan = new int(d);
_mjesec = new int(m);
_godina = new int(g);
}
void Ispis() { cout << *_dan << "/" << *_mjesec << "/" << *_godina << endl; }
void Dealociraj() {
delete _dan; delete _mjesec; delete _godina;
_dan = _mjesec = _godina = nullptr;
}
};
struct Artikal {
char * _naziv;
float _cijena;
void Unos(char * naziv, float cijena) {
    int vel = strlen(naziv) + 1;
    _naziv = new char[vel];
    strcpy(_naziv, naziv);
    _cijena = cijena;
}
void Dealociraj() { delete[]_naziv; _naziv = nullptr; }
void Ispis() { cout << _naziv << " " << _cijena << " KM"; }
};
struct Racun {
//format broja racuna: 006005491
//osigurati automatsko generisanje broja racuna pocevsi od 000000001
    char _brojRacuna[10];
    Datum _datumKreiranja;
    Artikal * _artikli;
    int * _kolicine;//cuva informaciju o kolicini svakog artikla na racunu
    int _brojArtikala;
    void Unos(char *naziv, float cijena){
        _brojArtikala=0;
        _artikli->_naziv=new char[strlen(naziv)+1];
        strcpy(_artikli->_naziv, naziv);
        _artikli->_cijena=cijena;
        _kolicine=nullptr;
    }
    void Dealociraj(){
        delete [] _artikli;
        _artikli=nullptr;
    }
    void DodajArtikal(Artikal a, int kolicina){
        if (_brojArtikala!=0){
        for (int i=0; i<_brojArtikala; i++){
            if (strcmp(_artikli[i]._naziv, a._naziv)==0){
                _kolicine[i]+=kolicina;
            }
        }
        }
        if (_brojArtikala==0){
            _artikli=new Artikal;
            _artikli->Unos(a._naziv, a._cijena);
            _kolicine=new int;
            *_kolicine=kolicina;
            _brojArtikala++;
        }else {
            Artikal *temp=new Artikal[_brojArtikala+1];
            for (int i=0; i<_brojArtikala; i++){
                temp[i]=_artikli[i];
            }
            temp[_brojArtikala].Unos(a._naziv, a._cijena);
            _brojArtikala++;
            int *tempk=new int[_brojArtikala];
            for (int i=0; i<_brojArtikala; i++){
                tempk[i]=_kolicine[i];
            }
            delete []_artikli;
            delete [] _kolicine;
            _kolicine=nullptr;
            _artikli=nullptr;
            _artikli=new Artikal[_brojArtikala];
            _kolicine=new int[_brojArtikala];
            for (int i=0; i<_brojArtikala; i++){
                _kolicine[i]=tempk[i];
            }
            for (int i=0; i<_brojArtikala; i++){
                _artikli[i]=temp[i];
            }
            delete []tempk;
            tempk=nullptr;
        }
    }
    void Ispis(){
        cout << " RACUN " << endl;
        cout << "Datum kreiranja racuna: ";
       // _datumKreiranja.Ispis();
        cout << endl;
    }
    void Sortiraj(){
        for (int i=0; i<_brojArtikala; i++){
            for (int j=i+1; j<_brojArtikala; j++){
                if (strcmp(_artikli[i]._naziv, _artikli[j]._naziv)>0){
                    Artikal temp;
                    temp=_artikli[i];
                    _artikli[i]=_artikli[j];
                    _artikli[j]=temp;
                }
            }
        }
    }
    Artikal *BinarnaPretraga(char *naziv){
        Artikal *pok;
        Sortiraj();
        int prvi=0, srednji=0, zadnji=0;
        zadnji=_brojArtikala-1;
        srednji=(prvi+zadnji)/2;
        while (prvi<=zadnji){
            srednji=(prvi+zadnji)/2;
        if (strcmp(_artikli[srednji]._naziv, naziv)==0){
            pok=&_artikli[srednji];
            return pok;
        }
        else if(strcmp(_artikli[srednji]._naziv, naziv)<0){
            prvi=srednji+1;
        }else if (strcmp(_artikli[srednji]._naziv, naziv)>0){
            zadnji=srednji-1;
        }
        }
        return nullptr;
    }
//Potpisi funkcija trebaju odgovarati onima u main-u
//1. Unos
//2. Dealociraj
//3. DodajArtikal - dodaje novi artikal u listu artikala zajedno sa njegovom kolicinom.
//Onemoguciti ponavljanje artikala na nacin da se uvecava samo kolicina ukoliko
//korisnik vise puta pokusa dodati isti artikal.
//4. Ispis - ispisuje racun u formatu prikazanom na slici (nije obavezno da bude
//identican, ali je svakako pozeljno). Prilikom svakog ispisa, racun
//je potrebno spasiti u tekstualni fajl sa istim nazivom kao i broj racuna.
//5. BinarnaPretraga - koristeci binarnu pretragu, na osnovu naziva, pronalazi i vraca
//pokazivac na artikal. Ukoliko trazeni artikal ne postoji funkcija vraca nullptr.
};
Artikal *PronadjiNajskupljiArtikal(Racun r, int brojArtikala, int indeks, int makscijena){
    if (brojArtikala<=0){
        return &r._artikli[indeks];
    }else {
      if (r._artikli[brojArtikala-1]._cijena*r._kolicine[brojArtikala-1]>makscijena){
        makscijena=r._artikli[brojArtikala-1]._cijena*r._kolicine[brojArtikala-1];
        indeks=brojArtikala-1;
      }
    }
    return PronadjiNajskupljiArtikal(r, brojArtikala-1, indeks, makscijena);
}
//1. PronadjiNajskupljiArtikal - rekurzivna funkcija koja vraca pokazivac na artikal sa najvecom ukupnom cijenom (cijena*kolicina)
//2. GenerisiSljedeciBrojRacuna - generise i vraca naredni broj racuna

int main() {
//za eventualne nejasnoce analizirati testni program i ispise na slici
    Datum danas;
    danas.Unos(13, 6, 2016);
    Artikal cigarete, cokolada, sok;
    cigarete.Unos("Cigarete", 2.23);
    cokolada.Unos("Cokolada", 1.23);
    sok.Unos("Cappy", 2.10);
    Racun racun;
//    racun.Unos(GenerisiSljedeciBrojRacuna(), danas);
    racun.DodajArtikal(cokolada, 5);
    racun.DodajArtikal(sok, 10);
    racun.DodajArtikal(cigarete, 2);
    racun.DodajArtikal(cigarete, 5);
    racun.Ispis();
    Artikal * p1 = racun.BinarnaPretraga("Cokolada");
    cout << "Binarnom pretragom pronadjen artikal -> ";
    p1->Ispis();
    racun.Ispis();
   // Artikal * p2 = PronadjiNajskupljiArtikal(racun, racun._brojArtikala, 0, 0);
    cout << "Rekurzivno pronadjen najskuplji artikal -> ";
   // p2->Ispis();
    cout << endl;
    return 0;
//dealocirati zauzetu memoriju
}








