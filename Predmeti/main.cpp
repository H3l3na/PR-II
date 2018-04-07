//narednu liniju code-a ignorisite, osim u slucaju da vam bude predstavljala smetnje u radu
#include <iostream>
#include <fstream>
#include <cstring>
#include <new>
#include <memory>

enum enumPredmeti { Matematika, Historija, Geografia, Hemija, Biologija, Engleski };
enum enumRazredi { I, II, III, IV };
char *_enumPred[] = { "Matematika", "Historija", "Geografija", "Hemija", "Biologija", "Engleski" };
char *_enumRaz[] = { "I", "II", "III", "IV" };
char *crt = "\n--------------------------------------------\n";

using namespace std;

struct Datum {
    int * _dan, *_mjesec, *_godina;
    void Unos(int d, int m, int g) {
        _dan = new int(d);
        _mjesec = new int(m);
        _godina = new int(g);
    }
    void Ispis() { cout << *_dan << "/" << *_mjesec << "/" << *_godina << endl; }
    void Dealociraj() { delete _dan; delete _mjesec; delete _godina; }
};

bool izmedju(Datum d1, Datum d2, Datum d3) {
    if (*d3._godina<*d2._godina && *d3._godina>*d1._godina) {
        return true;
    }
    if (*d3._godina == *d1._godina && *d3._mjesec >= *d1._mjesec && *d3._godina == *d2._godina && *d3._mjesec <= *d2._mjesec) {
        return true;
    }
    if (*d3._godina == *d1._godina && *d3._mjesec == *d1._mjesec && *d3._godina == *d2._godina && *d3._mjesec == *d2._mjesec && *d3._dan >= *d1._dan && *d3._dan <= *d2._dan) {
        return true;
    }
    return false;
}

struct Kurs {
    enumPredmeti _predmet;
    enumRazredi _razredi;
    Datum * _pocetak;
    Datum * _kraj;
    char * _imePredavaca;
    void Unos(enumPredmeti predmet, enumRazredi razred, Datum pocetak, Datum kraj, char * imePredavaca) {
        _pocetak = new Datum;
        _pocetak->Unos(*pocetak._dan, *pocetak._mjesec, *pocetak._godina);
        _kraj = new Datum;
        _kraj->Unos(*kraj._dan, *kraj._mjesec, *kraj._godina);
        _imePredavaca = new char[strlen(imePredavaca) + 1];
        strcpy(_imePredavaca, imePredavaca);
        _predmet = predmet;
        _razredi = razred;
    }

    void Ispis() {
        cout << "Naziv kursa: " << _enumPred[_predmet] << endl;
        cout << "Razred: " << _enumRaz[_razredi] << endl;
        cout << "Ime predavaca: " << _imePredavaca << endl;
        cout << "Pocetak kursa: ";
        _pocetak->Ispis();
        cout << "Kraj kursa: ";
        _kraj->Ispis();
    }
    void Dealociraj() {
        delete[] _imePredavaca;
        _imePredavaca = nullptr;
        _pocetak->Dealociraj();
        _kraj->Dealociraj();
    }
};
struct Polaznik {
    Datum * _datumRodjenja;
    char * _imePrezime;
    void Unos(Datum datumRodjenja, char * imePrezime) {
        _imePrezime = new char[strlen(imePrezime) + 1];
        strcpy(_imePrezime, imePrezime);
        _datumRodjenja = new Datum;
        _datumRodjenja->Unos(*datumRodjenja._dan, *datumRodjenja._mjesec, *datumRodjenja._godina);
    }
    void Dealociraj() {
        delete[] _imePrezime;
        _imePrezime = nullptr;
    }
    void Ispis() {}
};

struct Polaganja {
    Polaznik * _polaznik;
    Kurs * _kurs;
    Datum * _datumPolaganja;
    int _ocjena; //ocjene su u rasponu od 1 do 5
    void Unos(Polaznik polaznik, Kurs kurs, Datum datumPolaganja, int uspjeh) {
        _polaznik = new Polaznik;
        _polaznik->Unos(*polaznik._datumRodjenja, polaznik._imePrezime);
        _kurs = new Kurs;
        *_kurs = kurs;
        _datumPolaganja = new Datum;
        _datumPolaganja->Unos(*datumPolaganja._dan, *datumPolaganja._mjesec, *datumPolaganja._godina);
        _ocjena = uspjeh;
    }
    void Ispis() {
        cout << "Ime polaznika: " << _polaznik->_imePrezime << endl;
        cout << "Naziv kursa: " << _enumPred[_kurs->_predmet] << endl;
        cout << "Datum polaganja: ";
        _datumPolaganja->Ispis();
        cout << "Ostvareni uspjeh: " << _ocjena << endl;
    }
    void Dealociraj() {
        _polaznik->Dealociraj();
        _kurs->Dealociraj();
        _datumPolaganja->Dealociraj();
        delete[] _polaznik;
        delete[] _kurs;
        delete[] _datumPolaganja;
        _polaznik = nullptr;
        _kurs = nullptr;
        _datumPolaganja = nullptr;
    }
};

struct EdukacijskiCentar {
    char * _nazivCentra;
    Kurs * _kursevi[50];//JEDAN EDUKACIJSKI CENTAR MOZE NUDITI NAJVISE 50 KURSEVA
    int _trenutnoKurseva;
    Polaganja * _polaganja;
    int _trenutnoPolaganja;
    void Unos(char *naziv) {
        _nazivCentra = new char[strlen(naziv) + 1];
        strcpy(_nazivCentra, naziv);
        _trenutnoKurseva = 0;
        _trenutnoPolaganja = 0;
    }
    bool DodajKurs(Kurs k) {
        if (_trenutnoKurseva == 50) {
            cout << "Dostignut maksimalni kapacitet od 50 kurseva. " << endl;
            return false;
        }
        if (_trenutnoKurseva>0) {
            for (int i = 0; i<_trenutnoKurseva; i++) {
                if (_kursevi[i]->_predmet == k._predmet) {
                    if (*_kursevi[i]->_kraj->_godina>*k._pocetak->_godina || *_kursevi[i]->_kraj->_mjesec == *k._pocetak->_mjesec && *_kursevi[i]->_kraj->_dan>*k._pocetak->_dan && *_kursevi[i]->_kraj->_godina == *k._pocetak->_godina || *_kursevi[i]->_kraj->_godina == *k._pocetak->_godina && *_kursevi[i]->_kraj->_mjesec>*k._pocetak->_mjesec) {
                        return false;
                    }
                }
            }
        }
        _kursevi[_trenutnoKurseva] = new Kurs;
        _kursevi[_trenutnoKurseva]->Unos(k._predmet, k._razredi, *k._pocetak, *k._kraj, k._imePredavaca);
        _trenutnoKurseva++;
        return true;
    }
    bool DodajPolaganje(Polaganja p) {
        bool postoji = false;
        for (int i = 0; i<_trenutnoKurseva; i++) {
            if (_kursevi[i]->_predmet == p._kurs->_predmet) {
                postoji = true;
            }
        }
        if (postoji == false) {
            return false;
        }
        if (_trenutnoPolaganja == 0) {
            _polaganja = new Polaganja;
            _polaganja->Unos(*p._polaznik, *p._kurs, *p._datumPolaganja, p._ocjena);
            _trenutnoPolaganja++;
            return true;
        }
        if (_trenutnoPolaganja>0) {
            for (int i = 0; i<_trenutnoPolaganja; i++) {
                if (p._kurs->_predmet == _polaganja[i]._kurs->_predmet) {
                    if (p._ocjena<_polaganja[i]._ocjena) {
                        return false;
                    }
                }
            }
            Polaganja* temp = new Polaganja[_trenutnoPolaganja];
            for (int i = 0; i<_trenutnoPolaganja; i++) {
                temp[i].Unos(*_polaganja[i]._polaznik, *_polaganja[i]._kurs, *_polaganja[i]._datumPolaganja, _polaganja[i]._ocjena);
            }
            for (int i = 0; i<_trenutnoPolaganja; i++) {
                _polaganja[i].Dealociraj();
            }
            delete[] _polaganja;
            _polaganja = nullptr;
            temp[_trenutnoPolaganja] = p;
            _polaganja = temp;
            _trenutnoPolaganja++;
            return true;
        }
        return false;
    }
    void Dealociraj() {
        delete[] _nazivCentra;
        _nazivCentra = nullptr;
        for (int i = 0; i<_trenutnoKurseva; i++) {
            _kursevi[i]->Dealociraj();
            delete[]_kursevi[i];
            _kursevi[i] = nullptr;
        }
        for (int i = 0; i<_trenutnoPolaganja; i++) {
            _polaganja[i].Dealociraj();
        }
        delete[] _polaganja;
        _polaganja = nullptr;
    }
    Polaganja *PolaganjaByDatumIRazred(Datum OD, Datum DO, enumRazredi r, int &brojPolaganja) {
        Polaganja *niz;
        niz = nullptr;
        if (_trenutnoPolaganja == 0) {
            return nullptr;
        }
        for (int i = 0; i<_trenutnoPolaganja; i++) {
            if (izmedju(OD, DO, *_polaganja[i]._datumPolaganja)) {
                brojPolaganja++;
            }
        }
        niz = new Polaganja[brojPolaganja + 1];
        int j = 0;
        for (int i = 0; i<_trenutnoPolaganja; i++) {
            if (izmedju(OD, DO, *_polaganja[i]._datumPolaganja)) {
                niz[j].Unos(*_polaganja[i]._polaznik, *_polaganja[i]._kurs, *_polaganja[i]._datumPolaganja, _polaganja[i]._ocjena);
                j++;
            }
        }
        return niz;
    }
    void Ispis() {
        cout << "Naziv centra: " << _nazivCentra << endl;
        cout << "HOORAY It works!" << endl;
    }
};

float PretragaRekurzivno(EdukacijskiCentar mostar, Kurs k, float prosjek, int brojPolaganja, int br) {
    if (brojPolaganja<0) {
        return prosjek / br;
    }
    else {
        if (mostar._polaganja[mostar._trenutnoPolaganja - 1]._kurs->_predmet == k._predmet) {
            br++;
            prosjek += mostar._polaganja[mostar._trenutnoPolaganja - 1]._ocjena;
            brojPolaganja = mostar._trenutnoPolaganja - 1;
            mostar._trenutnoPolaganja = brojPolaganja;
            return PretragaRekurzivno(mostar, k, prosjek, brojPolaganja - 1, br);
        }
        return PretragaRekurzivno(mostar, k, prosjek, brojPolaganja - 1, br);
    }
}

int main() {

    /*
    1. BROJ I TIPOVI PARAMETARA MORAJU BITI IDENTICNI ONIMA U TESTNOM CODE-U (OSIM UKOLIKO IMATE OPRAVDANU POTREBU ZA MODIFIKACIJOM). U SUPROTNOM SE RAD NECE BODOVATI
    2. KREIRAJTE .DOC FAJL SA VAŠIM BROJEM INDEKSA ( NPR. IB130030.DOC BEZ IMENA I PREZIMENA), TE NA KRAJU ISPITA U NJEGA KOPIRAJTE RJEŠENJA VAŠIH ZADATAKA.
    NE PREDAVATI .TXT ILI .CPP FAJLOVE. TAKOÐER, UKLONITE SVE DIJELOVE CODE-A KOJI NISU IMPLEMENTIRANI
    3. TOKOM IZRADE ISPITA NIJE DOZVOLJENO KORIŠTENJE HELP-A
    4. TOKOM IZRADE ISPITA MOGU BITI POKRENUTA SAMO TRI PROGRAMA: PDF READER (ISPITNI ZADACI), MS VISUAL STUDIO, MS WORD (U KOJI CETE KOPIRATI VAŠA RJEŠENJA)
    5. BEZ OBZIRA NA TO DA LI SU ISPITNI ZADACI URAÐENI, SVI STUDENTI KOJI SU PRISTUPILI ISPITU MORAJU PREDATI SVOJ RAD
    */

    Datum datum1, datum2, datum3, datum4, datum5, datum6;
    datum1.Unos(26, 11, 2015);
    datum2.Unos(29, 5, 2016);
    datum3.Unos(5, 6, 2016);
    datum4.Unos(15, 8, 2016);
    datum5.Unos(13, 7, 2016);
    datum6.Unos(22, 9, 2016);

    Kurs matematika, hemija, biologija, engleski;
    matematika.Unos(Matematika, III, datum1, datum2, "Nina Bijedic");
    hemija.Unos(Hemija, III, datum2, datum3, "Murat Praso");
    biologija.Unos(Biologija, IV, datum3, datum4, "Jasmin Azemovic");
    engleski.Unos(Engleski, II, datum3, datum4, "Iris Memic");

    Datum rodjenje1, rodjenje2;
    rodjenje1.Unos(22, 8, 1982);
    Polaznik denis, zanin, indira;
    denis.Unos(rodjenje1, "Denis Music");
    //zanin.Unos(rodjenje2, "Zanin Vejzovic");


    Polaganja denisMatematika, zaninHemija, denisBiologija;
    denisMatematika.Unos(denis, matematika, datum5, 3);//3 predstavlja ostvareni uspjeh/ocjenu, a uspjesno polozenim se smatra svaki kurs na kome je polaznik ostvario ocjenu vecu od 1
    //zaninHemija.Unos(zanin, hemija, datum6, 93);

    EdukacijskiCentar mostar;
    mostar.Unos("ECM - Edukacijski Center Mostar");
    //jedan edukacijski centar moze nuditi vise istih kurseva (npr. Matematika), ali se oni moraju realizovati u razlictim periodima (pocetak-kraj) tj. ne smiju se preklapati
    //novi kurs istog tipa (npr. Matematika) se mora realizovati nakon prethodno dodanog npr. ako prethodni kurs Matematika traje 01.03.2016 - 01.04.2016, onda se novi kurs Matematika moze dodati jedino ako pocinje nakon 01.04.2016
    if (mostar.DodajKurs(matematika)) {
        cout << crt << "KURS USPJESNO REGISTROVAN" << crt;
        matematika.Ispis();
        cout << crt;
    }
    //nije moguce dodati polaganje onog kursa koji nije evidentiran (registrovan, ponudjen) u tom edukacijskom centru
    //kurseve je moguce najkasnije 15 dana od dana zavrsetka (kraja) posljednje dodanog kursa
    //jedan polaznik moze vise puta polagati isti kurs, ali nova ocjena mora biti veca od prethodne ocjene na tom istom kursu
    if (mostar.DodajPolaganje(denisMatematika)) {
        cout << crt << "POLAGANJE EVIDENTIRANO" << crt;
        denisMatematika.Ispis();
    }
    Datum OD, DO;
    int brojPolaganja = 0;
    OD.Unos(1, 6, 2016);
    DO.Unos(1, 8, 2016);
    //PolaganjaByDatumIRazred - funkcija vraca niz uspjesnih polaganja koja su realizovana u periodu OD - DO, a odnose se na bilo koji predmet u definisanom razredu (npr. II)
    Polaganja * polaganjaByDatumiRazred = mostar.PolaganjaByDatumIRazred(OD, DO, II, brojPolaganja);
    cout << "U periodu od ";
    OD.Ispis();
    cout << " do ";
    DO.Ispis();
    cout << " uspjesno je polozeno " << brojPolaganja << " predmeta --->>> ";
    for (int i = 0; i < brojPolaganja; i++)
        polaganjaByDatumiRazred[i].Ispis();

    //PretragaRekurzivno - rekurzivna funkcija koja pronalazi prosjecni uspjeh koji su polaznici tokom godine (npr.2016) ostvarili na odredjenom kursu (npr. Matematika)
    cout << "Prosjecan uspjeh na kursu iz predmeta Matematika u 2016 godini je " << PretragaRekurzivno(mostar, matematika, 0, mostar._trenutnoPolaganja, 0) << endl;
    //ispisuje sve informacije o edukacijskom centru. prije ispisa sortirati sve kurseve
    mostar.Ispis();

    //izvrsiti potrebne dealokacije*/
    //system("pause");
    return 0;
}
