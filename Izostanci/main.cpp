#include <iostream>
#include <cstring>
#include <memory>
#include <new>
#include <functional>
#include <fstream>

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

bool ispred(Datum d1, Datum d2){
    if (*d1._mjesec<*d2._mjesec){
        return true;
    }
    if (*d1._mjesec==*d2._mjesec && *d1._dan<*d2._dan){
        return true;
    }
    return false;
}


struct Izostanak {
char * _razlogIzostanka;
Datum _datumIzostanka;
int _brojSati;
bool _opravdano;//pravdanje zahtijeva da se evidentira i razlog izostanka
void Unos(Datum datumIzostanka, int brojSati) {
_datumIzostanka.Unos(*datumIzostanka._dan, *datumIzostanka._mjesec, *datumIzostanka._godina);
_brojSati = brojSati;
_opravdano = false;
_razlogIzostanka = new char;
_razlogIzostanka[0] = '\0';
}
void Dealociraj() { delete[]_razlogIzostanka; _razlogIzostanka = nullptr; }
void Ispis() { _datumIzostanka.Ispis(); cout << _razlogIzostanka << " " << _razlogIzostanka; }
void Opravdaj(char * razlogIzostanka) {
if (_razlogIzostanka != nullptr)
delete _razlogIzostanka;
int vel = strlen(razlogIzostanka) + 1;
_razlogIzostanka = new char[vel];
strcpy(_razlogIzostanka, razlogIzostanka);
_opravdano = true;
}
};
struct Student{
//format broja indeksa: IB150051
//osigurati automatsko generisanje broja indeksa pocevsi od IB150001
char _brojIndeksa[9];
char * _imePrezime;
Izostanak * _izostanci;
int _brojIzostanaka;
void Unos(char *gen_indeks, char *ime){
    strcpy(_brojIndeksa, gen_indeks);
    _imePrezime=new char[strlen(ime)+1];
    strcpy(_imePrezime, ime);
    _izostanci=nullptr;
    _brojIzostanaka=0;
}
void Dealociraj(){
    delete [] _imePrezime;
    _imePrezime=nullptr;
    for (int i=0; i<_brojIzostanaka; i++){
        _izostanci[i].Dealociraj();
    }
    delete [] _izostanci;
    _izostanci=nullptr;
}
void DodajIzostanak(Izostanak iz){
    if (_brojIzostanaka>0){
        for (int i=0; i<_brojIzostanaka; i++){
            if (*_izostanci[i]._datumIzostanka._dan==*iz._datumIzostanka._dan && *_izostanci[i]._datumIzostanka._mjesec==*iz._datumIzostanka._mjesec && *_izostanci[i]._datumIzostanka._godina==*iz._datumIzostanka._godina){
                _izostanci[i]._brojSati+=iz._brojSati;
            }
        }
    }
    if (_brojIzostanaka==0){
        _izostanci=new Izostanak;
        _izostanci->Unos(iz._datumIzostanka, iz._brojSati);
        _brojIzostanaka++;
    }else{
        Izostanak *izTemp=new Izostanak[_brojIzostanaka];
        for (int i=0; i<_brojIzostanaka; i++){
            izTemp[i]=_izostanci[i];
        }
        delete [] _izostanci;
        _izostanci=nullptr;
        _izostanci=new Izostanak[_brojIzostanaka+1];
        for (int i=0; i<_brojIzostanaka; i++){
            _izostanci[i]=izTemp[i];
        }
        _izostanci[_brojIzostanaka].Unos(iz._datumIzostanka, iz._brojSati);
        _brojIzostanaka++;
        delete [] izTemp;
        izTemp=nullptr;
    }

}
void Ispis(){
    char imeFajla[16];
    strcpy(imeFajla, _brojIndeksa);
    strcat(imeFajla, ".txt");
    ofstream upis(imeFajla, ios::app);
    if (!upis.fail()){
        cout << "Broj indeksa: \t Ime i prezime:\t\tDatum izostanka: \t\tBroj sati: " << endl;
        upis << "Broj Indeksa: \tIme i Prezime: " << endl;
        for (int i = 0; i <_brojIzostanaka; i++)
            {
                cout << _brojIndeksa << "\t\t" << _imePrezime << "\t\t\t" << *_izostanci[i]._datumIzostanka._dan << "." << *_izostanci[i]._datumIzostanka._mjesec << "." << *_izostanci[i]._datumIzostanka._godina << "\t\t\t" << _izostanci[i]._brojSati << endl;
                upis << _brojIndeksa << "\t\t" << _imePrezime << "\t\t\t" << *_izostanci[i]._datumIzostanka._dan << "." << *_izostanci[i]._datumIzostanka._mjesec << "." << *_izostanci[i]._datumIzostanka._godina << "\t\t\t" << _izostanci[i]._brojSati << endl;
                if (_izostanci[i]._opravdano == true)
                {
                    cout << "Razlog izostanka: " << endl;
                    cout << _izostanci[i]._razlogIzostanka << endl;
                }
            }
            upis.close();
    }
}
void Sortiraj(){
    for (int i=0; i<_brojIzostanaka; i++){
        for (int j=i+1; j<_brojIzostanaka; j++){
            if (*_izostanci[i]._datumIzostanka._mjesec > *_izostanci[j]._datumIzostanka._mjesec || *_izostanci[i]._datumIzostanka._mjesec == *_izostanci[j]._datumIzostanka._mjesec && *_izostanci[i]._datumIzostanka._dan > *_izostanci[j]._datumIzostanka._dan){
                swap(_izostanci[i], _izostanci[j]);
            }
        }
    }
}


Izostanak *BinarnaPretraga(Datum d){
    Sortiraj();
    int prvi=0, zadnji=_brojIzostanaka-1;
    int srednji=0;
    while (prvi<=zadnji){
        srednji=(prvi+zadnji)/2;
        if (*_izostanci[srednji]._datumIzostanka._dan==*d._dan && *_izostanci[srednji]._datumIzostanka._mjesec==*d._mjesec){
            return &_izostanci[srednji];
        }
        if (ispred(_izostanci[srednji]._datumIzostanka, d)==true){
            prvi=srednji+1;
        }
        if (ispred(_izostanci[srednji]._datumIzostanka, d)==false){
            zadnji=srednji-1;
        }
    }
    return nullptr;
}
//Potpisi funkcija trebaju odgovarati onima u main-u
//1. Unos
//2. Dealociraj
/*3. DodajIzostanak - dodaje novi izostanak u listu izostanaka.	Onemoguciti ponavljanje izostanaka na nacin da se uvecava samo broj sati ukoliko
korisnik (profesor) vise puta pokusa dodati izostanak na isti dan.*/
//4. Ispis - ispisuje izostanke u formatu prikazanom na slici (nije obavezno da ispis bude identican, ali je svakako pozeljno). Prilikom svakog ispisa, izostanke je potrebno spasiti u tekstualni fajl sa istim nazivom kao i broj indeksa.
//5. BinarnaPretraga - koristeci binarnu pretragu, na osnovu datuma, pronalazi i vraca pokazivac na izostanak. Ukoliko trazeni izostanak ne postoji funkcija vraca nullptr.
};

Izostanak *PronadjiNajveciNeopravdaniIzostanak(Student s, int _brojIzostanaka, int indeks){
    if (_brojIzostanaka<0){
        return &s._izostanci[indeks];
    }else {
        if (s._izostanci[_brojIzostanaka]._brojSati>s._izostanci[indeks]._brojSati){
            indeks=_brojIzostanaka;
            return PronadjiNajveciNeopravdaniIzostanak(s, _brojIzostanaka-1, indeks);
        }
        return PronadjiNajveciNeopravdaniIzostanak(s, _brojIzostanaka-1, indeks);
    }
}

/*int indbr;

char *GenerisiSljedeciBrojIndeksa(){

}*/
//1. PronadjiNajveciNeopravdaniIzostanak - rekurzivna funkcija koja vraca pokazivac na neopravdani izostanak koji ima najveci broj sati
//2. GenerisiSljedeciBrojIndeksa - generise i vraca naredni broj indeksa

int main() {
Datum jucer, prije5Dana, prije10Dana;
jucer.Unos(12, 6, 2016);
prije5Dana.Unos(8, 6, 2016);
prije10Dana.Unos(3, 6, 2016);

Student denis;
denis.Unos("IB160317", "Denis Music");

Izostanak izostanakJucer, izostanakPrije5Dana, izostanakPrije10Dana;

izostanakJucer.Unos(jucer, 5);
denis.DodajIzostanak(izostanakJucer);

izostanakPrije5Dana.Unos(prije5Dana, 3);
izostanakPrije5Dana.Opravdaj("Odsutan zbog bolesti - gripa");
denis.DodajIzostanak(izostanakPrije5Dana);
izostanakPrije5Dana.Unos(prije5Dana, 2);
denis.DodajIzostanak(izostanakPrije5Dana);

izostanakPrije10Dana.Unos(prije10Dana, 1);
denis.DodajIzostanak(izostanakPrije10Dana);
izostanakPrije10Dana.Unos(prije10Dana, 1);
denis.DodajIzostanak(izostanakPrije10Dana);

denis.Ispis();
Izostanak * p = denis.BinarnaPretraga(jucer);
cout << "Binarnom pretragom pronadjen izostanak -> ";
p->Ispis();
denis.Ispis();
Izostanak * p2 = PronadjiNajveciNeopravdaniIzostanak(denis, denis._brojIzostanaka-1, 0);
cout << "Rekurzivno pronadjen najveci neopravdani izostanak -> ";
p2->Ispis();
cout << endl;
return 0;
//dealocirati zauzetu memoriju
}
