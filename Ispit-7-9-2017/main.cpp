#include <iostream>
#include <cstring>
#include <new>
#include <fstream>
using namespace std;

/*
1. BROJ I VRSTA PARAMETARA MORAJU BITI IDENTICNI KAO U PRIMJERIMA. U SUPROTNOM SE RAD NEĆE BODOVATI
2. STAVITE KOMENTAR NA DIJELOVE CODE-A KOJE NE BUDETE IMPLEMENTIRALI
3. KREIRAJTE .DOC FAJL SA VAŠIM BROJEM INDEKSA ( NPR. IB160061.DOC BEZ IMENA I PREZIMENA), TE NA KRAJU ISPITA U NJEGA KOPIRAJTE RJEŠENJA VAŠIH ZADATAKA. NE PREDAVATI .TXT ILI .CPP FAJLOVE
4. TOKOM IZRADE ISPITA NIJE DOZVOLJENO KORIŠTENJE HELP-A
5. TOKOM IZRADE ISPITA MOGU BITI POKRENUTA SAMO TRI PROGRAMA: PDF READER (ISPITNI ZADACI), MS VISUAL STUDIO, MS WORD (U KOJI ĆETE KOPIRATI VAŠA RJEŠENJA)
6. BEZ OBZIRA NA TO DA LI SU ISPITNI ZADACI URAĐENI, SVI STUDENTI KOJI SU PRISTUPILI ISPITU MORAJU PREDATI SVOJ RAD
*/
//narednu liniju code-a ignorisite, osim u slucaju da vam bude predstavljala smetnje u radu
//#pragma warning(disable:4996)

/*
u nastavku je prikazan primjer ispitnog zapisnika (izvještaj o održanom ispitu) sa detaljima/stavkama

Datum ispita: 07.09.2017
Predmet: Programiranje II
IZVJESTAJ O ODRZANOM ISPITU
Br indeksa Ime prezime Komisijsko*** Ocjena
16001	A. Joldic	DA	5
15251	D. Music	NE	7 Ponisteno
16014	E. Babovic	DA	8
UKUPNO POLOZILO:	2
PROSJECNA OCJENA:	6.67
*** komisijski ispit: ako student pristupa četvrti put i dalje (računajući i poništene rezultate)
*/

char * crt = "\n-------------------------------------------\n";
struct Datum {
int _dan;
int _mjesec;
int _godina;
void Unos(int d, int m, int g) {
_dan = d;
_mjesec = m;
_godina = g;
}
void Ispis() {
cout << _dan << "." << _mjesec << "." << _godina << " ";
}
};
struct Student {
char _imePrezime[20];
int _brojIndeksa;
int _polozioECTS;

void Unos(char* naziv, int brIndeksa) {
strcpy(_imePrezime, naziv);
_brojIndeksa = brIndeksa;
_polozioECTS = 0;
}
void DodajECTS(float ects) {
_polozioECTS += ects;
}
void UmanjiECTS(float ects) {
_polozioECTS -= ects;
}
void Ispis() {
cout << _imePrezime << ", Indeks: " << _brojIndeksa << ", ECTS: " << _polozioECTS << " " << endl;
}
};

struct ZapisnikStavka {
int _ocjena;
Student* _student; //ZapisnikStavka će pokazivati na objekat tipa student koji se nalazi u instanci tipa Fakultet, pa nije potrebno vršiti nove alokacije
bool _ponisteno;
bool _komisijskoPolaganje=false;

void Unos(Student* proizvod, int ocjena, bool komisijskoPolaganje) {
_ocjena = ocjena;
_student = proizvod;
_komisijskoPolaganje = false;//komisijskoPolaganje;
_ponisteno = false;
}

void Ispis() {
cout << _student->_imePrezime << "\t" << _ocjena << "\t" << _student->_brojIndeksa << "\t";
if (_ponisteno)
cout << " Ponisteno";
cout << endl;
}
};
struct IspitniZapisnik {
Datum _datum;
ZapisnikStavka ** _stavke;//svaki put se dodaje po jedna stavka
int _brojStavki;
int _ects;
char _predmet[20];
void Unos(Datum datum, char* predmet, int ects) {
_datum = datum;
_brojStavki = 0;
_stavke = nullptr;
_ects = ects;
strcpy(_predmet, predmet);
}
/*Prosjecnu ocjenu izračunati koristeći lambda funkciju koja se poziva koristeći objekat
tipa auto. u prosjek računati i poništene rezultate (stavke)*/
float GetProsjecnaOcjena() {
return 0;
}
};
const int Max = 100;

/*unutar objekta tipa Fakultet ce se cuvati podaci o svim studentima i ispitnim zapisnicima, a dodaju se pomocu globalnih funkcija*/
struct Fakultet {
//umjesto brojaca koristiti default.ne vrijednosti
Student* Studenti[Max] = { NULL };
IspitniZapisnik* Zapisnici[Max] = { NULL };

void Dealociraj() {
for (size_t i = 0; i < Max; i++) {
delete Studenti[i]; Studenti[i] = NULL;
}
for (size_t i = 0; i < Max; i++) {
delete Zapisnici[i]; Zapisnici[i] = NULL;
}
}
};
void dodaj_student(Fakultet &f, char *imeP, int brIndeksa){
    int indeks=0;
    for (int i=0; i<Max; i++){
        if (f.Studenti[i]==nullptr){
            indeks=i;
            break;
        }
    }
    f.Studenti[indeks]=new Student;
    f.Studenti[indeks]->Unos(imeP, brIndeksa);
}

void kreiraj_zapisnik(Fakultet &f, int dan, int mjesec, int godina, char *nazivPredmeta, int ects){
    int indeks=0;
    for (int i=0; i<Max; i++){
        if (f.Zapisnici[i]==nullptr){
            indeks=i;
            break;
        }
    }
    Datum d;
    d._dan=dan;
    d._mjesec=mjesec;
    d._godina=godina;
    f.Zapisnici[indeks]=new IspitniZapisnik;
    f.Zapisnici[indeks]->Unos(d, nazivPredmeta, ects);
}

bool dodaj_stavku_na_zapisnik(Fakultet &f, int brIndeksa, int ocjena){
    int indeks1=0;
    for (int i=0; i<Max; i++){
        if (f.Zapisnici[i]==nullptr){
            indeks1=i;
            break;
        }
    }
    int indeks2=0;
    if (f.Zapisnici[indeks1]!=nullptr){
    for (int i=0; i<f.Zapisnici[indeks1]->_brojStavki; i++){
        if (f.Zapisnici[indeks1]->_stavke[i]==nullptr){
            indeks2=i;
            break;
        }
    }
    }

    if (f.Zapisnici[indeks1-1]!=nullptr){
        if (f.Zapisnici[indeks1-1]->_brojStavki==0){
        f.Zapisnici[indeks1-1]->_stavke=new ZapisnikStavka*;
        *f.Zapisnici[indeks1-1]->_stavke=new ZapisnikStavka;
        bool pom1=false;
        for (int i=0; i<Max; i++){
            if (f.Studenti[i]!=nullptr){
                if (f.Studenti[i]->_brojIndeksa==brIndeksa){
                    pom1=true;
                    (*f.Zapisnici[indeks1-1]->_stavke)->_student=new Student;
                    (*f.Zapisnici[indeks1-1]->_stavke)->_student->Unos(f.Studenti[i]->_imePrezime, f.Studenti[i]->_brojIndeksa);
                }
            }
        }
        if (pom1==false){
            return false;
        }else {
        (*f.Zapisnici[indeks1-1]->_stavke)->_ocjena=ocjena;
        f.Zapisnici[indeks1-1]->_brojStavki++;
        return true;
        }
    }
    else {
        for (int i=0; i<f.Zapisnici[indeks1-1]->_brojStavki; i++){
            if (f.Zapisnici[indeks1-1]->_stavke[i]->_student->_brojIndeksa==brIndeksa){
                f.Zapisnici[indeks1-1]->_stavke[i]->_ocjena=ocjena;
                return false;
            }
        }
        ZapisnikStavka **temp;
        temp=new ZapisnikStavka*[f.Zapisnici[indeks1-1]->_brojStavki];
        for (int i=0; i<f.Zapisnici[indeks1-1]->_brojStavki; i++){
            temp[i]=new ZapisnikStavka;
            temp[i]->_student=new Student;
        }
        for (int i=0; i<f.Zapisnici[indeks1-1]->_brojStavki; i++){
            temp[i]->_student->Unos(f.Zapisnici[indeks1-1]->_stavke[i]->_student->_imePrezime, f.Zapisnici[indeks1-1]->_stavke[i]->_student->_brojIndeksa);
            temp[i]->_ocjena=f.Zapisnici[indeks1-1]->_stavke[i]->_ocjena;
        }
        for (int i=0; i<f.Zapisnici[indeks1-1]->_brojStavki; i++){
            delete f.Zapisnici[indeks1-1]->_stavke[i]->_student;
            //f.Zapisnici[indeks1-1]->_stavke[i]->_student=nullptr;
            delete f.Zapisnici[indeks1-1]->_stavke[i];
            //f.Zapisnici[indeks1-1]->_stavke[i]=nullptr;
        }
        delete [] f.Zapisnici[indeks1-1]->_stavke;
        //f.Zapisnici[indeks1-1]->_stavke=nullptr;

        f.Zapisnici[indeks1-1]->_stavke=new ZapisnikStavka*[f.Zapisnici[indeks1-1]->_brojStavki+1];
        for (int i=0; i<f.Zapisnici[indeks1-1]->_brojStavki+1; i++){
            f.Zapisnici[indeks1-1]->_stavke[i]=new ZapisnikStavka;
            f.Zapisnici[indeks1-1]->_stavke[i]->_student=new Student;
        }

        for (int i=0; i<f.Zapisnici[indeks1-1]->_brojStavki; i++){
            f.Zapisnici[indeks1-1]->_stavke[i]->_student->Unos(temp[i]->_student->_imePrezime, temp[i]->_student->_brojIndeksa);
            f.Zapisnici[indeks1-1]->_stavke[i]->_ocjena=temp[i]->_ocjena;
        }
        bool pom=false;
        for (int i=0; i<Max; i++){
            if (f.Studenti[i]!=nullptr){
                if (f.Studenti[i]->_brojIndeksa==brIndeksa){
                    pom=true;
                    f.Zapisnici[indeks1-1]->_stavke[f.Zapisnici[indeks1-1]->_brojStavki]->_student->Unos(f.Studenti[i]->_imePrezime, f.Studenti[i]->_brojIndeksa);
                }
            }
        }

        if (pom==false){
            return false;
        }else {
            f.Zapisnici[indeks1-1]->_stavke[f.Zapisnici[indeks1-1]->_brojStavki]->_ocjena=ocjena;
            f.Zapisnici[indeks1-1]->_brojStavki++;
        }

        delete [] temp;
        temp=nullptr;
        return true;
}
    }
    return false;
}

bool ponisti_stavku_rezultat(Fakultet &f, int brIndeksa){
    int ind=0;
    for (int i=0; i<Max; i++){
        if (f.Zapisnici[i]==nullptr){
            ind=i;
        }
    }
    if (ind!=0){
    ind=ind-1;
    }
    if (f.Zapisnici[ind]!=nullptr){
    for (int i=0; i<Max; i++){
        if (f.Studenti[i]!=nullptr){
            if (f.Studenti[i]->_brojIndeksa==brIndeksa){
                f.Studenti[i]->UmanjiECTS(f.Zapisnici[ind]->_ects);
            }
        }
    }
    }
    int indeks=0;
    for (int i=0; i<Max; i++){
        if (f.Zapisnici[i]==nullptr){
            indeks=i;
            break;
        }
    }
    bool postoji=false;
    if (f.Zapisnici[indeks-1]!=nullptr){
        for (int i=0; i<f.Zapisnici[indeks-1]->_brojStavki; i++){
            if (f.Zapisnici[indeks-1]->_stavke[i]!=nullptr){
                cout << f.Zapisnici[indeks-1]->_stavke[i]->_student->_brojIndeksa << endl;
                if (f.Zapisnici[indeks-1]->_stavke[i]->_student->_brojIndeksa==brIndeksa){
                    postoji=true;
                }
            }
        }
    }else
    {
        return false;
    }
    if (postoji==false){
        return false;
    }
    return true;
}

bool printaj(Fakultet f){
    for (int i=0; i<Max; i++){
        if (f.Zapisnici[i]!=nullptr){
            cout << "Datum ispita: ";
            f.Zapisnici[i]->_datum.Ispis();
            cout << endl;
            cout << "Naziv predmeta: " << f.Zapisnici[i]->_predmet << endl;
            cout << "IZVJESTAJ O ODRZANOM ISPITU " << endl;
            cout << "Br indeksa\t Ime prezime\t Komisijsko***\t Ocjena\t" << endl;
            /*for (int j=0; j<f.Zapisnici[i]->_brojStavki; i++){
                if (f.Zapisnici[i]->_stavke[j]!=nullptr){
                cout << f.Zapisnici[i]->_stavke[j]->_student->_brojIndeksa << "\t" << f.Zapisnici[i]->_stavke[j]->_student->_imePrezime << "\t" << f.Zapisnici[i]->_stavke[j]->_komisijskoPolaganje << "\t" << f.Zapisnici[i]->_stavke[j]->_ocjena << endl;
            }
            }*/
        }
    }
    return true;
}

int main() {
Fakultet fakultet;

/*dodaje podatke o novom studentu u fakultet */
//fakultet, imePrezime, broj indeksa
dodaj_student(fakultet, "A. Joldic", 16001);
dodaj_student(fakultet, "D. Music", 15251);
dodaj_student(fakultet, "E. Babovic	", 16014);
dodaj_student(fakultet, "E. Junuz", 16015);
dodaj_student(fakultet, "J. Azemovic", 16014);//ne dodaje studenta jer je broj indeksa zauzet
/*kraira novi zapisnik*/
//fakultet, datum, predmet, ects krediti
kreiraj_zapisnik(fakultet, 6, 9, 2017, "Programiranje 2", 7);

/*dodaje stavku na posljednji kreiran zapisnik, a ects studenta uveća ako je student položio ispit. */

if (dodaj_stavku_na_zapisnik(fakultet, 16001, 5))
cout << "Stavka (rezultat) uspjesno dodata! 1" << endl;

/*ako je student vec dodat u zapisnik, onda se vrsi samo modifikacija njegove ocjene*/
if (!dodaj_stavku_na_zapisnik(fakultet, 16001, 6))
cout << "Student vec dodat na zapisnik. Ocjena je ispravljena!" << endl;

if (dodaj_stavku_na_zapisnik(fakultet, 15251, 5))
    cout << "Stavka (rezultat) uspjesno dodata!" << endl;
if (dodaj_stavku_na_zapisnik(fakultet, 16015, 6))
    cout << "Stavka (rezultat) uspjesno dodata!" << endl;
if (dodaj_stavku_na_zapisnik(fakultet, 16014, 8))
    cout << "Stavka (rezultat) uspjesno dodata!" << endl;

kreiraj_zapisnik(fakultet, 7, 9, 2017, "Računarske mreze", 5);
//naredne stavke se dodaju na novi zapisnik
if (dodaj_stavku_na_zapisnik(fakultet, 16015, 5))
    cout << "Stavka (rezultat) uspjesno dodata! 1" << endl;
if (dodaj_stavku_na_zapisnik(fakultet, 19001, 10))
    cout << "Stavka (rezultat) uspjesno dodata!" << endl;


/*u posljednjem zapisniku stavku za studenta 16001 ponistiti, a studentu umanjiti ects bodove.
funkcija vraca false u slucaju da se student ne nalazi na posljednjem zapisniku*/

/*if (ponisti_stavku_rezultat(fakultet, 16001)==true)
cout << "Stavka (rezultat) uspjesno ponisten!" << endl;

if (ponisti_stavku_rezultat(fakultet, 19001)==false)
cout << "Student nije pronadjen!" << endl;

/* funkcija printaj prikazuje posljednji ispitni zapisnik u formatu prikazanom na pocetku zadatka.
* pored prikazivanja, identican sadrzaj ispitnog zapisnika je potrebno je pasiti u tekstualni fajl.
* naziv fajla treba biti Predmet_datum.txt, npr. ProgramiranjeII_2017-09-01.txt. zapisnik se nece
prikazati niti sacuvati u fajl ukoliko ne posjeduje niti jednu stavku ili su sve stavke ponistene*/
if (printaj(fakultet))
cout << "Ispitni zapisnik uspjesno prikazan i spasen u fajl!" << endl;
//fakultet.Dealociraj();
return 0;
}




















