//narednu liniju code-a ignorisite, osim u slucaju da vam bude predstavljala smetnje u radu
#pragma warning(disable:4996)
#include <iostream>
#include <fstream>
#include <cstring>
#include <new>
#include <memory>

using namespace std;

enum Opstina{ Opstina1, Opstina2, Opstina3, Opstina4 };
char * OpstineChar[] = { "Opstina1", "Opstina2", "Opstina3", "Opstina4" };
enum Kandidati{ Kandidat1, Kandidat2, Kandidat3, Kandidat4 };
char * KandidatiChar[] = { "Kandidat1", "Kandidat2", "Kandidat3", "Kandidat4" };

struct Datum{
int *_dan, *_mjesec, *_godina;
void Unos(int d, int m, int g){ _dan = new int(d); _mjesec =new int (m); _godina = new int (g); }
void Ispis(){ cout << _dan << "/" << _mjesec << "/" << _godina << endl; }
};

bool izmedju(Datum d1, Datum d2, Datum d3){
    if (*d3._godina<*d1._godina){
        return  false;
    }
    if (*d3._godina>*d2._godina){
        return false;
    }
    if (*d3._godina==*d1._godina && *d3._mjesec<*d1._mjesec){
        return false;
    }
    if (*d3._godina==*d2._godina && *d3._mjesec>*d2._mjesec){
        return false;
    }
    if (*d3._godina==*d1._godina && *d3._mjesec==*d1._mjesec && *d3._dan<*d1._dan){
        return false;
    }
    if (*d3._godina==*d2._godina && *d3._mjesec==*d2._mjesec && *d3._dan>*d2._dan){
        return false;
    }
    return true;
}

struct Osoba{
Datum _datumRodjenja;
char * _imePrezime;
char _JMBG[14];
Opstina _Prebivaliste;
void unos(Datum d, char * ip, char jmbg[], Opstina p){
_datumRodjenja = d;
_imePrezime = new char[strlen(ip) + 1];
strcpy(_imePrezime, ip);
strcpy(_JMBG, jmbg);
_Prebivaliste = p;
}
void Dealociraj(){ delete[] _imePrezime; _imePrezime = nullptr; }
void Info(){
_datumRodjenja.Ispis();
cout << _imePrezime << " " << _JMBG << " " <<
OpstineChar[_Prebivaliste] << endl;
}
};
struct Glas{
Osoba * _glasac;
Kandidati * _glasZa;
void Unos(Osoba o, Kandidati k){
 _glasac=new Osoba;
_glasac->unos(o._datumRodjenja, o._imePrezime, o._JMBG, o._Prebivaliste);
_glasZa = new Kandidati(k);
}
void Ispis(){
_glasac->Info();
cout << KandidatiChar[*_glasZa] << endl;
}
void Dealociraj(){ _glasac->Dealociraj(); }
};

struct Izbori2016{
    Opstina * _glasackoMjesto;
    Glas * _glasovi[1000];
    int _doSadaGlasalo;
    void Unos(Opstina o){
        _glasackoMjesto=new Opstina;
        *_glasackoMjesto=o;
        _doSadaGlasalo=0;
    }
    bool DodajGlas(Glas g){
        bool glasao=false;
        for (int i=0; i<_doSadaGlasalo; i++){
            if (strcmp(_glasovi[i]->_glasac->_imePrezime, g._glasac->_imePrezime)==0){
                glasao=true;
            }
        }
        if (glasao==true){
            return false;
        }
        if (*g._glasac->_datumRodjenja._godina>1999){
            return false;
        }
        if (*g._glasac->_datumRodjenja._godina==1999 && *g._glasac->_datumRodjenja._mjesec>6){
            return false;
        }
        _glasovi[_doSadaGlasalo]=new Glas;
        _glasovi[_doSadaGlasalo]->Unos(*g._glasac, *g._glasZa);
        _doSadaGlasalo++;
        return true;
    }

    void Ispis(){
        float niz[4]={0};
        for (int i=0; i<_doSadaGlasalo; i++){
            if (*_glasovi[i]->_glasZa==Kandidat1){
                niz[Kandidat1]++;
            }
            if (*_glasovi[i]->_glasZa==Kandidat2){
                niz[Kandidat2]++;
            }
            if (*_glasovi[i]->_glasZa==Kandidat3){
                niz[Kandidat3]++;
            }
            if (*_glasovi[i]->_glasZa==Kandidat4){
                niz[Kandidat4]++;
            }
        }
        for (int i=0; i<4; i++){
            for (int j=i+1; j<4; j++){
                if (niz[j]>niz[i]){
                    int temp=niz[i];
                    niz[i]=niz[j];
                    niz[j]=temp;
                    char *temp1;
                    temp1=new char[10];
                    temp1=KandidatiChar[i];
                    KandidatiChar[i]=KandidatiChar[j];
                    KandidatiChar[j]=temp1;
                }
            }
        }
        cout << "SORTIRANO" << endl;
        for (int i=0; i<4; i++){
             cout << i+1 << ". " << KandidatiChar[i] << " ostvario uspjeh: " << niz[i] << endl;
        }
    }

    int BrojGlasacaPoRodjenju(Datum OD, Datum DO){
        int brojac=0;
        for (int i=0; i<_doSadaGlasalo; i++){
            if (izmedju(OD, DO, _glasovi[i]->_glasac->_datumRodjenja)==true){
            brojac++;
            }
        }
        return brojac;
    }

     void Dealociraj(){
        delete _glasackoMjesto;
        _glasackoMjesto=nullptr;
        for (int i=0; i<_doSadaGlasalo; i++){
            _glasovi[i]->Dealociraj();
            delete[] _glasovi[i];
            _glasovi[i]=nullptr;
        }
    }
/*
1. Unos - na osnovu vrijednosti primljenih parametara, inicijalizovati vrijednosti atributa strukture.
2. DodajGlas - funkcija treba da omoguci dodavanje informacija o novom glasu i tom prilikom onemoguciti: da ista osoba glasa vise puta, glasanje osobama mladjim od 18 godina (uzeti u obzir samo mjesec i godinu rodjenja), glasanje osobama iz drugih opstina. U zavisnosti od uspjesnosti operacije funkcija vraca true ili false
3. Ispis - u zavisnosti od postignutog uspjeha funkcija treba ispisati listu kandidata sortiranu opadajucim redoslijedom, a uz svakog kandidata je potrebno ispisati i osvojeni procenat glasova.
Funkcija vraca opstinu u kojoj je pobjednicki kandidat ostvario najveci broj glasova, a ukoliko je pobjednicki kandidat ostvario podjednak broj glasova u vise opstina, funkcija vraca prvu koja se nalazi u nizu na koji pokazuje pokazivac izbornaMjesta
4. BrojGlasacaPoRodjenju - funkcija vraca broj glasaca koji su do tog momenta glasali, a koji su rodjeni u parametrima definisanom periodu (parametri su tipa datum, period OD i DO)
*/
};

void UpisiUBinarniFajl(Izbori2016 izbori){
    char *imeFajla;
    imeFajla=new  char[strlen(OpstineChar[*izbori._glasackoMjesto])+1];
    strcpy(imeFajla, OpstineChar[*izbori._glasackoMjesto]);
    ofstream upis(imeFajla, ios::binary);
    if (!upis.fail()){
        upis.write((char*)(&izbori), sizeof(izbori));
        upis.close();
    }else {
        cout << "Doslo je do greske prilikom upisa u " << imeFajla << endl;
    }
}

void UcitajIzBinarnogFajla(Izbori2016 &izbori, char *naziv){
    ifstream ispis(naziv, ios::binary);
    if (ispis.is_open()){
        ispis.read((char*)(&izbori), sizeof(izbori));
        ispis.close();
    }else {
        cout << "Greska prilikom ispisa iz " << naziv << endl;
    }
}

int PretragaRekurzivno(Izbori2016 izbori, Kandidati k, int brojac, int br){
    if (br<=0){
        return brojac;
    }else
    {
        if (*izbori._glasovi[br-1]->_glasZa==k){
            brojac++;
       // return PretragaRekurzivno(izbori, k, brojac, br-1);
        }
        PretragaRekurzivno(izbori, k, brojac, br-1);
    }
}
/*
1. UpisiUBinarniFajl - objekat koji je primljen kao parametar upisuje u binarni fajl. Naziv fajla treba biti identican nazivu opstine u kojoj se odrzavaju izbori
2. UcitajIzBinarnogFajla - u objekat koji je primljen kao parametar ucitava sadrzaj binarnog fajla (naziv fajla je takodjer proslijedjen kao parametar)
3. PretragaRekurzivno - rekurzivna funkcija koja treba da vrati broj glasova koje je na izborima ostvario odredjeni kandidat (kandidat se proslijedjuje kao parametar)
*/

int main(){
/*
1. BROJ I VRSTA PARAMETARA MORAJU BITI IDENTICNI KAO U PRIMJERIMA. U SUPROTNOM SE RAD NECE BODOVATI
2. STAVITE KOMENTAR NA DIJELOVE CODE-A KOJE NE BUDETE IMPLEMENTIRALI
3. KREIRAJTE .DOC FAJL SA VAŠIM BROJEM INDEKSA ( NPR. IB130030.DOC BEZ IMENA I PREZIMENA), TE NA KRAJU ISPITA U NJEGA KOPIRAJTE RJEŠENJA VAŠIH ZADATAKA. NE PREDAVATI .TXT ILI .CPP FAJLOVE
4. TOKOM IZRADE ISPITA NIJE DOZVOLJENO KORIŠTENJE HELP-A
5. TOKOM IZRADE ISPITA MOGU BITI POKRENUTA SAMO TRI PROGRAMA: PDF READER (ISPITNI ZADACI), MS VISUAL STUDIO, MS WORD (U KOJI CETE KOPIRATI VAŠA RJEŠENJA)
6. BEZ OBZIRA NA TO DA LI SU ISPITNI ZADACI URAÐENI, SVI STUDENTI KOJI SU PRISTUPILI ISPITU MORAJU PREDATI SVOJ RAD
*/

Datum datumGlasac1, datumGlasac2, datumGlasac3, datumGlasac4, datumIzbora;
datumIzbora.Unos(14, 7, 2016);
datumGlasac1.Unos(18, 5, 1990);
datumGlasac2.Unos(5, 3, 1982);
datumGlasac3.Unos(12, 8, 1958);
datumGlasac4.Unos(22, 6, 2000);

Osoba a, b, c, d, e;
a.unos(datumGlasac1, "Denis Music", "1111111111111", Opstina1);
b.unos(datumGlasac2, "Jasmin Azemovic", "1111111111112", Opstina1);
c.unos(datumGlasac3, "Indira Hamulic", "1111111111113", Opstina1);
d.unos(datumGlasac4, "Adel Handzic", "1111111111114", Opstina2);
e.unos(datumGlasac4, "Zanin Vejzovic", "1111111111115", Opstina1);


Glas g1, g2, g3, g4, g5;
g1.Unos(a, Kandidat2);
g2.Unos(b, Kandidat2);
g3.Unos(c, Kandidat1);
g4.Unos(d, Kandidat3);
g5.Unos(e, Kandidat2);

Izbori2016 izbori, izbori_FromFile;
izbori.Unos(Opstina1);

izbori.DodajGlas(g1);
izbori.DodajGlas(g2);
izbori.DodajGlas(g3);
izbori.DodajGlas(g4);
izbori.DodajGlas(g5);
izbori.Ispis();

Datum OD, DO;
DO.Unos(17, 7, 1998);
OD.Unos(17, 7, 1986);
cout<<"Glasaca u starost od 18 - 30 godina->"<<izbori.BrojGlasacaPoRodjenju(OD,DO)<<endl;
cout<<"Broj glasova za Kandidat1->" << PretragaRekurzivno(izbori, Kandidat1, 0, izbori._doSadaGlasalo) << endl;
UpisiUBinarniFajl(izbori);
UcitajIzBinarnogFajla(izbori_FromFile, "Opstina1");
izbori_FromFile.Ispis();
return 0;
}


