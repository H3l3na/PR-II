#include <iostream>
#include <cstring>
#include <new>
#include <memory>
#include <fstream>

using namespace std;

enum VrstaObaveze { Seminarski, Parcijalni1, Parcijalni2, Integralni, Prakticni };
char *enumV[]={"Seminarski", "Parcijalni1", "Parcijalni2", "Integralni", "Prakticni"};
struct Datum {
int * _dan, *_mjesec, *_godina;
void Unos(int d, int m, int g) {
    _dan = new int(d);
    _mjesec = new int(m);
    _godina = new int(g);
}
void Ispis() {
    cout << *_dan << "/" << *_mjesec << "/" << *_godina << endl;
    }
void Dealociraj() {
     delete _dan;
     delete _mjesec;
     delete _godina; }
};

struct ObavezeNaPredmetu {
    VrstaObaveze * _vrstaObaveze;
    Datum * _datumIzvrsenja;
    char * _napomena;
    int _ocjena; // 5 - 10
    void Unos(VrstaObaveze vrsta, Datum  datum, int ocjena, char * napomena) {
        _vrstaObaveze=new VrstaObaveze;
        *_vrstaObaveze = vrsta;
        _datumIzvrsenja = new Datum;
        _datumIzvrsenja->Unos(*datum._dan, *datum._mjesec, *datum._godina);
        _ocjena = ocjena;
        _napomena = new char[strlen(napomena)+1];
        strcpy(_napomena, napomena);
}
void Ispis() {
    cout << enumV[*_vrstaObaveze] << " " << _ocjena << " " << _napomena;
    _datumIzvrsenja->Ispis();
    cout << endl;
}
void Dealociraj() {
    _datumIzvrsenja->Dealociraj();
    delete _vrstaObaveze; _vrstaObaveze = nullptr;
    delete _datumIzvrsenja; _datumIzvrsenja = nullptr;
    delete[] _napomena; _napomena = nullptr;
}
};


struct PolozeniPredmet {
    Datum _datumPolaganja;//datum koji ce se evidentirati kao datum kada je predmet polozen tj. kada je formirana konacna ocjena
    char * _nazivPredmeta;
    ObavezeNaPredmetu * _listaIzvrsenihObaveza[10];
    int _trenutnoIzvrsenihObaveza;
    int _konacnaOcjena; //formira se na osnovu ocjena izvrsenih obaveza
    void Unos(char *naziv){
        _nazivPredmeta=new char[strlen(naziv)+1];
        strcpy(_nazivPredmeta, naziv);
        _trenutnoIzvrsenihObaveza=0;
    }
    bool DodajIzvrsenuObavezu(VrstaObaveze obaveza, Datum d, int ocjena, char *napomena){
        if (_trenutnoIzvrsenihObaveza>=10){
            return false;
        }
        bool postoji=false;
        for (int i=0; i<_trenutnoIzvrsenihObaveza; i++){
            if (*_listaIzvrsenihObaveza[i]->_vrstaObaveze==obaveza && obaveza!=Seminarski){
                postoji=true;
            }
        }
        if (postoji==true){
            return false;
        }
        _listaIzvrsenihObaveza[_trenutnoIzvrsenihObaveza]=new ObavezeNaPredmetu;
        _listaIzvrsenihObaveza[_trenutnoIzvrsenihObaveza]->Unos(obaveza, d, ocjena, napomena);
        _trenutnoIzvrsenihObaveza++;
        return true;
    }
    int FormirajKonacnuOcjenu(){
        int ocjena=5;
        bool pao=false;
        bool pao_parc1=false, pao_parc2=false, pao_integ=false;
        for (int i=0; i<_trenutnoIzvrsenihObaveza; i++){
            if (*_listaIzvrsenihObaveza[i]->_vrstaObaveze==Parcijalni1 && _listaIzvrsenihObaveza[i]->_ocjena==5){
                pao_parc1=true;
            }
            if (*_listaIzvrsenihObaveza[i]->_vrstaObaveze==Parcijalni2 && _listaIzvrsenihObaveza[i]->_ocjena==5){
                pao_parc2=true;
            }
            if (*_listaIzvrsenihObaveza[i]->_vrstaObaveze==Integralni && _listaIzvrsenihObaveza[i]->_ocjena==5){
                pao_integ=true;
            }
        }
        if ((pao_parc1 || pao_parc2) && pao_integ){
            return ocjena;
        }
        int brojac=0;
        ocjena=0;
        int indeks;
        for (int i=0; i<_trenutnoIzvrsenihObaveza; i++){
            if (_listaIzvrsenihObaveza[i]->_ocjena>5){
            ocjena+=_listaIzvrsenihObaveza[i]->_ocjena;
            brojac++;
            indeks=i;
            }
        }
        _datumPolaganja.Unos(*_listaIzvrsenihObaveza[indeks]->_datumIzvrsenja->_dan, *_listaIzvrsenihObaveza[indeks]->_datumIzvrsenja->_mjesec, *_listaIzvrsenihObaveza[indeks]->_datumIzvrsenja->_godina);
        float dec=float(ocjena)/float(brojac);
        int de=ocjena/brojac;
        float d=dec-de;
        ocjena/=brojac;
        if (d>=0.5){
            ocjena+=1;
        }
        _konacnaOcjena=ocjena;
        return ocjena;
    }
    void Ispis(){
        cout << "Naziv predmeta: " << _nazivPredmeta << endl;
        cout << "Datum polaganja: ";
        _datumPolaganja.Ispis();
        for (int i=0; i<_trenutnoIzvrsenihObaveza; i++){
            cout << "Naziv obaveze: " << enumV[*_listaIzvrsenihObaveza[i]->_vrstaObaveze] << "\t" << "Ocjena: " << _listaIzvrsenihObaveza[i]->_ocjena << endl;
        }
        cout << "Konacna ocjena: " << FormirajKonacnuOcjenu() << endl;
    }
    void Dealociraj(){
        delete [] _nazivPredmeta;
        _nazivPredmeta=nullptr;
        for (int i=0; i<_trenutnoIzvrsenihObaveza; i++){
            _listaIzvrsenihObaveza[i]->Dealociraj();
            delete[]_listaIzvrsenihObaveza[i];
            _listaIzvrsenihObaveza[i]=nullptr;
        }
        delete [] _listaIzvrsenihObaveza;
    }
/*
1. Unos - na osnovu vrijednosti primljenog parametra izvrsiti inicijalizaciju odgovarajucih atributa
2. DodajIzvrsenuObavezu – na osnovu vrijednosti primljenih parametara osigurati dodavanje novoizvrsene obaveze na predmetu. Potrebno je onemoguciti dodavanje identicnih obaveza, a izmedju izvrsenja pojedinih obaveza mora proci najmanje 7 dana. Identicna vrsta obaveze se moze dodati samo u slucaju da se radi o Seminarskom ili je prethodno dodana obaveza (identicne vrste)
imala ocjenu 5. Ukoliko je moguce, osigurati prosirenje niza na nacin da prihvati vise od 10 obaveza.
3. FormirajKonacnuOcjenu - konacna ocjene predstavlja prosjecnu ocjenu na predmetu, a za njeno formiranje student mora posjedovati polozen integralni ili dva parijcalna ispita. Ukoliko je ispit polozen putem parcijalnih ispita, student takodjer mora imati pozitivno (ocjenom vecom od 5) ocijenjena najmanje dva seminarska rada. U slucaju da bilo koji od navedenih uslova nije zadovoljenkonacna ocjena treba biti postavljena na vrijednost 5. Konacna ocjena, takodjer, ne smije biti formirana u slucaju da u napomeni od dvije obaveze stoji rijec 'prepisivao' ili 'izbacen'. Ukoliko su ispunjeni svi uslovi, osim formiranja konacne ocjene, kao datum polaganja je potrebno postaviti na vrijednost datuma posljednje izvrsene obaveze koja je usla u formiranje ocjene.
4. Ispis – ispsuje vrijednosti svih atributa strukture
*/
};

int PretragaRekurzivno(PolozeniPredmet p, int brojObaveza, int br_neg){
    if (brojObaveza==0){
        return br_neg;
    }else
    {
        if (p._listaIzvrsenihObaveza[brojObaveza-1]->_ocjena==5){
            br_neg++;
            return PretragaRekurzivno(p, brojObaveza-1, br_neg);
        }
        return PretragaRekurzivno(p, brojObaveza-1, br_neg);
    }
}
void UpisiUBinarniFajl(PolozeniPredmet p, int k){
    char *imeFajla;
    if (p._konacnaOcjena!=10){
    imeFajla=new char[strlen(p._nazivPredmeta)+2];
    strcpy(imeFajla, p._nazivPredmeta);
    }else if (p._konacnaOcjena==10){
        imeFajla=new char[strlen(p._nazivPredmeta)+3];
        strcpy(imeFajla, p._nazivPredmeta);
    }
    int brojac=0;
    int j=0;
    while (p._nazivPredmeta[j]!='\0'){
        brojac++;
        j++;
    }
    if (p._konacnaOcjena==5){
        imeFajla[brojac]='5';
    }
    if (p._konacnaOcjena==6){
        imeFajla[brojac]='6';
    }
    if (p._konacnaOcjena==7){
        imeFajla[brojac]='7';
    }
    if (p._konacnaOcjena==8){
        imeFajla[brojac]='8';
    }
    if (p._konacnaOcjena==9){
        imeFajla[brojac]='9';
    }
    if (p._konacnaOcjena==10){
        imeFajla[brojac]='1';
        imeFajla[brojac+1]='0';
    }
    ofstream upis(imeFajla);
    if (!upis.fail()){
        upis.write((char*)(&p), sizeof(p));
        /*upis << "Naziv predmeta: " << p._nazivPredmeta << endl;
        upis << "Konacna ocjena: " << p._konacnaOcjena << endl;
        upis << "Datum polaganja: " << *p._datumPolaganja._dan << "/" << *p._datumPolaganja._mjesec << "/" << *p._datumPolaganja._godina << endl;*/
        upis.close();
    }
}

void UcitajIzBinarnogFajla(PolozeniPredmet &p, char *naziv){
    ifstream ispis(naziv, ios::binary);
    if (!ispis.fail()){
        ispis.read((char*) (&p), sizeof(p));
    }

}
/*
1. UpisiUBinarniFajl - objekat koji je primljen kao parametar upisuje u binarni fajl. Naziv fajla treba biti sastavljen od naziva predmeta i konacne ocjena npr. "Programiranje II_6"
2. UcitajIzBinarnogFajla - u objekat koji je primljen kao parametar ucitava sadrzaj binarnog fajla (naziv fajla je takodjer proslijedjen kao parametar)
3. PretragaRekurzivno - rekurzivna funkcija koja treba da vrati broj obaveza kod koji je student ostvario negativnu ocjenu
*/
int main() {
/*
1. BROJ I VRSTA PARAMETARA MORAJU BITI IDENTICNI KAO U PRIMJERIMA. U SUPROTNOM SE RAD NECE BODOVATI
2. STAVITE KOMENTAR NA DIJELOVE CODE-A KOJE NE BUDETE IMPLEMENTIRALI
3. KREIRAJTE .DOC FAJL SA VAŠIM BROJEM INDEKSA ( NPR. IB130030.DOC BEZ IMENA I PREZIMENA), TE NA KRAJU ISPITA U NJEGA KOPIRAJTE RJEŠENJA VAŠIH ZADATAKA. NE PREDAVATI .TXT ILI .CPP FAJLOVE
4. TOKOM IZRADE ISPITA NIJE DOZVOLJENO KORIŠTENJE HELP-A
5. TOKOM IZRADE ISPITA MOGU BITI POKRENUTA SAMO TRI PROGRAMA: PDF READER (ISPITNI ZADACI), MS VISUAL STUDIO, MS WORD (U KOJI CETE KOPIRATI VAŠA RJEŠENJA)
6. BEZ OBZIRA NA TO DA LI SU ISPITNI ZADACI URAÐENI, SVI STUDENTI KOJI SU PRISTUPILI ISPITU MORAJU PREDATI SVOJ RAD
*/

Datum datumSeminarski1, datumSeminarski2, datumParcijalni1, datumParcijalni2,
datumIntegralni;
datumSeminarski1.Unos(10, 6, 2016);
datumSeminarski2.Unos(18, 6, 2016);
datumParcijalni1.Unos(22, 3, 2016);
datumParcijalni2.Unos(22, 6, 2016);

//polozeni predmet

PolozeniPredmet prII, prII_FromFile;
prII.Unos("Programiranje II");
if (prII.DodajIzvrsenuObavezu(Parcijalni1, datumParcijalni1, 9, "uslovno polozen"))
cout << "Parcijalni 1...dodan" << endl;
if (prII.DodajIzvrsenuObavezu(Parcijalni2, datumParcijalni2, 9, "previse gresaka"))
cout << "Parcijalni 2...dodan" << endl;
if (prII.DodajIzvrsenuObavezu(Seminarski, datumSeminarski1, 9, "implementirani svi algoritmi"))
cout << "Seminarski1...dodan" << endl;
if (prII.DodajIzvrsenuObavezu(Seminarski, datumSeminarski2, 9, "rad slican kao kod studenta IB150388"))
cout << "Seminarski2...dodan" << endl;

cout << "Konacna ocjena iz predmeta PRII je: " << prII.FormirajKonacnuOcjenu() << endl;

datumParcijalni2.Unos(22, 7, 2016);
if (prII.DodajIzvrsenuObavezu(Parcijalni2, datumParcijalni2, 6, ""))
cout << "Parcijalni 2...dodan" << endl;
cout << "Konacna ocjena iz predmeta PRII je: " << prII.FormirajKonacnuOcjenu() << endl;
//rekurzija
cout << "Broj negativnih ocjena je: " << PretragaRekurzivno(prII, prII._trenutnoIzvrsenihObaveza, 0) << endl;
//manipulacija fajlovima*/
int k=10;
UpisiUBinarniFajl(prII, k);
prII.Ispis();
UcitajIzBinarnogFajla(prII_FromFile, "Programiranje II_6");
prII_FromFile.Ispis();
prII.Dealociraj();
return 0;
}

