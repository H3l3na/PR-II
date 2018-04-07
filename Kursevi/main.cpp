//narednu liniju code-a ignorisite, osim u slucaju da vam bude predstavljala smetnje u radu
//#pragma warning(disable:4996)
#include <iostream>
#include <cstring>
#include <fstream>

using namespace std;

enum enumKursevi { HtmlCSSJavaScript, SoftwareEngeneeringFundamentals, MasteringSQL, WindowsSecurity };
char *nazivi[]={"HtmlCSSJavaScript", "SoftwareEngeneeringFundamentals", "MasteringSQL", "WindowsSecurity"};
char *crt = "\n--------------------------------------------\n";

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

bool DaLiSePoklapaju(Datum d1, Datum d2){
    if (*d1._godina>*d2._godina){
        return true;
    }
    if (*d1._godina==*d2._godina){
        if (*d1._mjesec>*d2._mjesec){
            return true;
        }
    }
    if (*d1._godina==*d2._godina && *d1._mjesec==*d2._mjesec){
        if (*d1._dan>=*d2._dan){
            return true;
        }
    }
    return false;
}

int BrojDana(int mjesec){
    int dani=0;
    switch (mjesec){
        case 1: dani=31;
        break;
        case 2: dani=28;
        break;
        case 3: dani=31;
        break;
        case 4: dani=30;
        break;
        case 5: dani=31;
        break;
        case 6: dani=30;
        break;
        case 7: dani=31;
        break;
        case 8: dani=31;
        break;
        case 9: dani=30;
        break;
        case 10: dani=31;
        break;
        case 11: dani=30;
        break;
        case 12: dani=31;
        break;
    }
    return dani;
}

bool Proslo5dana(Datum d_polaganja, Datum d_zavrsetka){
    if (*d_zavrsetka._mjesec==*d_polaganja._mjesec){
        if (*d_polaganja._dan-*d_zavrsetka._dan>5){
            return true;
        }
    }
    int pomocna=0;
    int pomocna2=0;
    if (*d_polaganja._mjesec-*d_zavrsetka._mjesec==1){
            pomocna+=*d_polaganja._dan;
            pomocna2=BrojDana(*d_zavrsetka._mjesec)-*d_zavrsetka._dan;
            pomocna+=pomocna2;
            if (pomocna>5){
                return true;
            }
    }
    return false;
}

bool izmedju(Datum d,Datum OD, Datum DO){
    if (*d._godina<*OD._godina || *d._godina>*DO._godina){
        return false;
    }
    if (*d._godina==*OD._godina){
        if (*d._mjesec<*OD._mjesec){
            return false;
        }
    }
    if (*d._godina==*DO._godina){
        if (*d._mjesec>*DO._mjesec){
            return false;
        }
    }
    if (*d._godina==*OD._godina && *d._mjesec==*OD._mjesec){
        if (*d._dan<*OD._dan){
            return false;
        }
    }
    if (*d._godina==*DO._godina && *d._mjesec==*DO._mjesec){
        if (*d._dan>*DO._dan){
            return false;
        }
    }
    return true;
}

struct Kurs {
enumKursevi _kurs;
Datum _pocetak;
Datum _kraj;
char * _imePredavaca;
bool _aktivan;//SVAKI KURS JE NA POCETKU AKTIVAN
void Unos(enumKursevi k, Datum d1, Datum d2, char *imePredavaca){
    _kurs=k;
    _pocetak.Unos(*d1._dan, *d1._mjesec, *d1._godina);
    _kraj.Unos(*d2._dan, *d2._mjesec, *d2._godina);
    _imePredavaca=new char[strlen(imePredavaca)+1];
    strcpy(_imePredavaca, imePredavaca);
    _aktivan=true;
}
void Ispis(){
    cout << "Naziv kursa: " << nazivi[(int)_kurs] << endl;
    cout << "Pocetak: ";
    _pocetak.Ispis();
    //cout << endl;
    cout << "Kraj: ";
    _kraj.Ispis();
    //cout << endl;
    cout << "Ime predavaca: " << _imePredavaca << endl;
}
void Dealociraj() {
delete[] _imePredavaca; _imePredavaca = nullptr;
}
};
struct Polaznik {
int _polaznikID;
char * _imePrezime;
void Unos(int polaznikID, char * imePrezime) {
_polaznikID = polaznikID;
int vel = strlen(imePrezime) + 1;
_imePrezime = new char[vel];
strcpy(_imePrezime, imePrezime);
}
void Ispis() { cout << _polaznikID << " " << _imePrezime << endl; }
void Dealociraj() {
delete[] _imePrezime; _imePrezime = nullptr;
}
};
struct Polaganja {
Polaznik _polaznik;
Kurs _kurs;
Datum _datumPolaganja;
float _ostvareniUspjeh;
void Unos(Polaznik p, Kurs k, Datum d, float uspjeh){
    _polaznik=p;
    _kurs.Unos(k._kurs, k._pocetak, k._kraj, k._imePredavaca);
    _ostvareniUspjeh=uspjeh;
    _datumPolaganja.Unos(*d._dan, *d._mjesec, *d._godina);
}
void Ispis(){
    cout << "Naziv kursa: " << nazivi[(int)_kurs._kurs] << endl;
    cout << "Ime polaznika: " << _polaznik._imePrezime << endl;
    cout << "Datum polaganja: ";
    _datumPolaganja.Ispis();
    cout << "Ostvareni uspjeh: " << _ostvareniUspjeh << endl;
}
};

struct SkillsCentar {
char * _nazivCentra;
Kurs * _kursevi[50];//JEDAN EDUKACIJSKI CENTAR MOZE NUDITI NAJVISE 50 KURSEVA
int _trenutnoKurseva;
Polaganja * _polaganja;
int _trenutnoPolaganja;
void Unos(char *naziv){
    _nazivCentra=new char[strlen(naziv)+1];
    strcpy(_nazivCentra, naziv);
    _trenutnoKurseva=0;
    _trenutnoPolaganja=0;
}
bool DodajKurs(Kurs k){
    if (_trenutnoKurseva>0){
        for (int i=0; i<_trenutnoKurseva; i++){
            if (_kursevi[i]->_kurs==k._kurs){
                    if (DaLiSePoklapaju(_kursevi[i]->_kraj, k._pocetak)==true){
                        return false;
                    }
            }
        }
    }
    _kursevi[_trenutnoKurseva]=new Kurs;
    //_kursevi[_trenutnoKurseva]->_aktivan=true;
    _kursevi[_trenutnoKurseva]->Unos(k._kurs, k._pocetak, k._kraj, k._imePredavaca);
    _trenutnoKurseva++;
    return true;
}
bool DodajPolaganje(Polaganja p){
    bool pronadjen=false;
    for (int i=0; i<_trenutnoKurseva; i++){
        if (_kursevi[i]->_kurs==p._kurs._kurs){
            pronadjen=true;
        }
    }
    if (pronadjen==false){
        return false;
    }/*
    if (_trenutnoPolaganja>0){
        for (int i=0; i<_trenutnoPolaganja; i++){
            if (strcpy(_polaganja[i]._polaznik._imePrezime, p._polaznik._imePrezime)==0 && _polaganja[i]._kurs._kurs==p._kurs._kurs){
                if (p._ostvareniUspjeh<_polaganja[i]._ostvareniUspjeh){
                    return false;
                }
            }
        }
    }*/
    for (int i=0; i<_trenutnoKurseva; i++){
        if (_kursevi[i]->_kurs==p._kurs._kurs){
            if (_kursevi[i]->_aktivan==false){
                return false;
            }
        }
    }
    if (_trenutnoPolaganja==0){
        _polaganja=new Polaganja;
        _polaganja->Unos(p._polaznik, p._kurs, p._datumPolaganja, p._ostvareniUspjeh);
        _trenutnoPolaganja++;
        return true;
    }else {
        Polaganja *temp=new Polaganja[_trenutnoPolaganja];
        for (int i=0; i<_trenutnoPolaganja; i++){
            temp[i].Unos(_polaganja[i]._polaznik, _polaganja[i]._kurs, _polaganja[i]._datumPolaganja, _polaganja[i]._ostvareniUspjeh);
        }
        delete [] _polaganja;
        _polaganja=nullptr;
        _polaganja=new Polaganja[_trenutnoPolaganja+1];
        for (int i=0; i<_trenutnoPolaganja; i++){
            _polaganja[i].Unos(temp[i]._polaznik, temp[i]._kurs, temp[i]._datumPolaganja, temp[i]._ostvareniUspjeh);
        }
        _polaganja[_trenutnoPolaganja].Unos(p._polaznik, p._kurs, p._datumPolaganja, p._ostvareniUspjeh);
        delete[] temp;
        temp=nullptr;
        _trenutnoPolaganja++;
        return true;
    }

}
Polaganja *PolaganjaByDatum(Datum OD, Datum DO, int &brojPolaganja){
    int brojac=0;
    Polaganja *niz;
    for (int i=0; i<_trenutnoPolaganja; i++){
        if (izmedju(_polaganja[i]._datumPolaganja, OD, DO)==true){
            brojac++;
            brojPolaganja++;
        }
    }
    niz=new Polaganja[brojac];
    int j=0;
    for (int i=0; i<_trenutnoPolaganja; i++){
        if (izmedju(_polaganja[i]._datumPolaganja, OD, DO)==true){
            niz[j].Unos(_polaganja[i]._polaznik, _polaganja[i]._kurs, _polaganja[i]._datumPolaganja, _polaganja[i]._ostvareniUspjeh);
            j++;
        }
    }
    return niz;
}
void Ispis
};

float PretragaRekurzivno(SkillsCentar mostar,Kurs html,int brojPolaganja, int brojac, float uspjeh){
    if (brojPolaganja<=0){
        return uspjeh/(float)brojac;
    }else {
        if (mostar._polaganja[brojPolaganja-1]._kurs._kurs==html._kurs){
            uspjeh+=mostar._polaganja[brojPolaganja-1]._ostvareniUspjeh;
            brojac++;
        }
        return PretragaRekurzivno(mostar, html, brojPolaganja-1, brojac, uspjeh);
    }
}

int main(){

/*
1. BROJ I TIPOVI PARAMETARA MORAJU BITI IDENTICNI ONIMA U TESTNOM CODE-U (OSIM UKOLIKO IMATE OPRAVDANU POTREBU ZA MODIFIKACIJOM). U SUPROTNOM SE RAD NEĆE BODOVATI
2. KREIRAJTE .DOC FAJL SA VAŠIM BROJEM INDEKSA ( NPR. IB130030.DOC BEZ IMENA I PREZIMENA), TE NA KRAJU ISPITA U NJEGA KOPIRAJTE RJEŠENJA VAŠIH ZADATAKA. NE PREDAVATI .TXT ILI .CPP FAJLOVE. TAKOĐER, UKLONITE SVE DIJELOVE CODE-A KOJI NISU IMPLEMENTIRANI
3. TOKOM IZRADE ISPITA NIJE DOZVOLJENO KORIŠTENJE HELP-A
4. TOKOM IZRADE ISPITA MOGU BITI POKRENUTA SAMO TRI PROGRAMA: PDF READER (ISPITNI ZADACI), MS VISUAL STUDIO, MS WORD (U KOJI ĆETE KOPIRATI VAŠA RJEŠENJA)
5. BEZ OBZIRA NA TO DA LI SU ISPITNI ZADACI URAĐENI, SVI STUDENTI KOJI SU PRISTUPILI ISPITU MORAJU PREDATI SVOJ RAD
*/

Datum datum1, datum2, datum3, datum4, datum5, datum6;
datum1.Unos(26, 11, 2015);
datum2.Unos(29, 5, 2016);
datum3.Unos(5, 6, 2016);
datum4.Unos(15, 8, 2016);
datum5.Unos(13, 7, 2016);
datum6.Unos(22, 9, 2016);

Kurs softverski, html, sql, security;
softverski.Unos(SoftwareEngeneeringFundamentals, datum1, datum2, "Emina Junuz");
html.Unos(HtmlCSSJavaScript, datum2, datum3, "Larisa Tipura");
sql.Unos(MasteringSQL, datum3, datum4, "Jasmin Azemovic");
security.Unos(WindowsSecurity, datum3, datum4, "Adel Handzic");

Polaznik denis, zanin, indira;
denis.Unos(1, "Denis Music");
zanin.Unos(2, "Zanin Vejzovic");
indira.Unos(3, "Indira Hamulic");

Polaganja denisHtml, zaninHtml, indiraSql;
denisHtml.Unos(denis, html, datum5, 61);
//61 predstavlja ostvareni uspjeh/procenat, a uspjesno polozenim se smatra svaki kurs na kome je polaznik ostvari vise od 55%
//zaninHtml.Unos(zanin, html, datum6, 93);

SkillsCentar mostar;
mostar.Unos("Skills Center Mostar");
//jedan edukacijski centar moze nuditi vise istih kurseva (npr. MasteringSQL), ali se oni moraju realizovati u razlictim periodima (pocetak-kraj) tj. ne smiju se preklapati
//novi kurs istog tipa (npr. MasteringSQL) se mora realizovati nakon prethodno dodanog npr. ako prethodni kurs MasteringSQL traje 01.03.2016 - 01.04.2016, onda se novi kurs MasteringSQL moze dodati jedino ako pocinje nakon 01.04.2016
if (mostar.DodajKurs(softverski)) {
cout << crt << "KURS USPJESNO REGISTROVAN" << crt;
softverski.Ispis();
cout << crt;
}
if (mostar.DodajKurs(html)){
   cout << crt << "KURS USPJESNO REGISTROVAN" << crt;
html.Ispis();
cout << crt;
}

//nije moguce dodati polaganje onog kursa koji nije evidentiran (registrovan, ponudjen) u tom edukacijskom centru
//moguce je polagati samo aktivne kurseve i to najkasnije 5 dana od dana zavrsetka (kraja) posljednje dodanog (aktivnog) kursa iste vrste (npr. MasteringSQL)
//jedan polaznik moze vise puta polagati isti kurs, ali novi procenat mora biti veci od prethodno dodanog (na istom kursu)
if (mostar.DodajPolaganje(denisHtml)) {
cout << crt << "POLAGANJE EVIDENTIRANO" << crt;
denisHtml.Ispis();
}

Datum OD, DO;
int brojPolaganja = 0;
OD.Unos(1, 6, 2016);
DO.Unos(1, 8, 2016);

//PolaganjaByDatum – vraca niz polaganja koja su uspjesno realizovana u periodu OD-DO
Polaganja * polaganjaByDatum = mostar.PolaganjaByDatum(OD, DO, brojPolaganja);
cout << "U periodu od ";
OD.Ispis();
cout << " do ";
DO.Ispis();
cout << " uspjesno je realizovano " << brojPolaganja << " polaganja--->>>";
for (size_t i = 0; i < brojPolaganja; i++)
polaganjaByDatum[i].Ispis();

//PretragaRekurzivno - rekurzivna funkcija pronalazi prosjecni uspjeh koji su polaznici tokom godine (npr.2016) ostvarili na odredjenom kursu (npr. MasteringSQL)
cout << "Prosjecan uspjeh na kursu MasteringSQL u 2016 godini je " << PretragaRekurzivno(mostar, html, mostar._trenutnoPolaganja, 0, 0.)<<endl;
//Ispis - ispisuje sve informacije o edukacijskom centru. prije ispisa sortirati sve kurseve na osnovu pocetka odrzavanja kursa
mostar.Ispis();

//izvrsiti potrebne dealokacije
return 0;
}
