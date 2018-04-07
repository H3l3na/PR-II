#include <iostream>
#include <cstring>
#include <new>
#include <memory>
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
#pragma warning(disable:4996)

char *crt = "\n-------------------------------------------\n";

enum eNacinStudiranja { REDOVAN=0, DL };
enum eRazred { PRVI = 1, DRUGI, TRECI, CETVRTI };
char *Razredi[]={"PRVI", "DRUGI", "TRECI", "CETVRTI"};
char *NacinStudiranja[]={"REDOVAN", "DL"};


struct DatumVrijeme {
int *_dan, *_mjesec, *_godina, *_sati, *_minuti;
void Unos(int dan = 1, int mjesec = 1, int godina = 2000, int sati = 0, int minuti = 0) {
    _dan = new int(dan);
    _mjesec = new int(mjesec);
    _godina = new int(godina);
    _sati = new int(sati);
    _minuti = new int(minuti);
}
void Dealociraj() {
    delete _dan; _dan = nullptr;
    delete _mjesec; _mjesec = nullptr;
    delete _godina; _godina = nullptr;
    delete _sati; _sati = nullptr;
    delete _minuti; _minuti = nullptr;
}
void Ispis() {
    cout << *_dan << "." << *_mjesec << "." << *_godina << " " << *_sati << ":" << *_minuti << endl;
}
};

const DatumVrijeme rokZaPrijavu = { new int(5), new int(7), new int(2017), new int(12), new int(30) };

bool DaLiJeIstekao(DatumVrijeme d){
    if (*d._godina>*rokZaPrijavu._godina){
        return true;
    }
    if (*d._godina==*rokZaPrijavu._godina){
        if (*d._mjesec>*rokZaPrijavu._mjesec){
            return true;
        }
    }
    if (*d._godina==*rokZaPrijavu._godina && *d._mjesec==*rokZaPrijavu._mjesec){
        if (*d._dan>*rokZaPrijavu._dan){
            return true;
        }
    }
    if (*d._godina==*rokZaPrijavu._godina && *d._mjesec==*rokZaPrijavu._mjesec && *d._dan==*rokZaPrijavu._dan){
        if (*d._sati>*rokZaPrijavu._sati){
            return true;
        }
    }
    if (*d._godina==*rokZaPrijavu._godina && *d._mjesec==*rokZaPrijavu._mjesec && *d._dan==*rokZaPrijavu._dan && *d._sati==*rokZaPrijavu._sati){
        if (*d._minuti>*rokZaPrijavu._minuti){
            return true;
        }
    }
    return false;
}

struct Predmet {
    char * _naziv;
    int _ocjena;
    DatumVrijeme * _datumUnosa;
    void Unos(char * naziv, int ocjena, DatumVrijeme datumUnosa) {
        int vel = strlen(naziv)+1;
        _naziv = new char[vel];
        strcpy(_naziv, naziv);
        _ocjena = ocjena;
        _datumUnosa=new DatumVrijeme;
        _datumUnosa->Unos(*datumUnosa._dan, *datumUnosa._mjesec, *datumUnosa._godina, *datumUnosa._sati, *datumUnosa._minuti);
}
void Dealociraj() {
    delete[] _naziv;
    _naziv = nullptr;
    _datumUnosa->Dealociraj();
    delete _datumUnosa;
}

void Ispis() {
    cout << "Naziv predmeta: " << _naziv << endl;
//kreirati funkciju GetDatumKaoNizKaraktera() koja vraca vrijednosti atributa strukture datum kao niz karaktera
//    cout << _naziv << " (" << _ocjena << ") " << _datumUnosa->GetDatumKaoNizKaraktera() << endl;
}
};

struct Uspjeh {
    eRazred _razred;
    Predmet * _predmeti;
    int _brojPredmeta;
    void Unos(eRazred razred) {
        _razred = razred;
        _predmeti = nullptr;
        _brojPredmeta = 0;
}
void Dealociraj() {
    for (size_t i = 0; i < _brojPredmeta; i++)
        _predmeti[i].Dealociraj();
    delete[] _predmeti;
    _predmeti = nullptr;
}

void Ispis() {
    cout << crt << "Razred -> " << Razredi[((int)_razred)-1] << crt;
    for (size_t i = 0; i < _brojPredmeta; i++)
        _predmeti[i].Ispis();
    }
};

struct Kandidat {
    eNacinStudiranja _nacinStudiranja;
    char * _imePrezime;
    shared_ptr<Uspjeh> _uspjeh[4];

void Unos(eNacinStudiranja nacinStudiranja, char * imePrezime) {
    int vel = strlen(imePrezime) + 1;
    _imePrezime = new char[vel];
    strcpy(_imePrezime, imePrezime);
    _nacinStudiranja = nacinStudiranja;
    for (size_t i = 0; i < 4; i++)
        _uspjeh[i] = nullptr;

    }
    bool DodajPredmet(eRazred r, Predmet p){
        bool postoji=false;
        for (int i=1; i<=4; i++){
            if ((int)r==i){
                postoji=true;
            }
        }
        if (postoji==false){
            return false;
        }
        postoji=false;
        if (_uspjeh[((int)r)-1]!=nullptr){
            for (int i=0; i<_uspjeh[((int)r)-1]->_brojPredmeta; i++){
                if (strcmp(p._naziv, _uspjeh[((int)r)-1]->_predmeti[i]._naziv)==0){
                    postoji=true;
                }
            }
        }
        if (postoji==true){
            return false;
        }
        if (DaLiJeIstekao(*p._datumUnosa)==true){
            return false;
        }
        _uspjeh[((int)r)-1]=make_shared<Uspjeh>();
        if (_uspjeh[((int)r-1)]->_brojPredmeta==0){
              _uspjeh[((int)r-1)]->_predmeti=new Predmet;
              _uspjeh[((int)r-1)]->_predmeti->Unos(p._naziv, p._ocjena, *p._datumUnosa);
              _uspjeh[((int)r-1)]->_brojPredmeta++;
        }else {
            Predmet *temp=new Predmet[_uspjeh[((int)r-1)]->_brojPredmeta];
            for (int i=0; i<_uspjeh[((int)r-1)]->_brojPredmeta; i++){
                temp[i]=_uspjeh[((int)r-1)]->_predmeti[i];
            }
            temp[_uspjeh[((int)r-1)]->_brojPredmeta].Unos(p._naziv, p._ocjena, *p._datumUnosa);
            for (int i=0; i<_uspjeh[((int)r)-1]->_brojPredmeta; i++){
                delete [] _uspjeh[((int)r-1)]->_predmeti;
            }
            _uspjeh[((int)r-1)]->_predmeti=nullptr;
            _uspjeh[((int)r-1)]->_predmeti=new Predmet[_uspjeh[((int)r-1)]->_brojPredmeta];
            for (int i=0; i<_uspjeh[((int)r-1)]->_brojPredmeta; i++){
                _uspjeh[((int)r-1)]->_predmeti[i]=temp[i];
            }
            _uspjeh[((int)r-1)]->_brojPredmeta++;
        }
        //_uspjeh[((int)r)-1]->_predmeti=new Predmet;
        return true;
    }
    void Dealociraj() {
    delete[] _imePrezime;
    _imePrezime = nullptr;
    for (size_t i = 0; i < 4; i++)
    if (_uspjeh[i]!=nullptr){
        _uspjeh[i]->Dealociraj();
    }
    }
void Ispis() {
    cout << crt << _imePrezime << " " << NacinStudiranja[_nacinStudiranja];
    //for (size_t i = 0; i < 4; i++)
        //_uspjeh[i]->Ispis();
    }
};

Kandidat *rekNajboljaOcjena(Kandidat *niz, int brojKandidata, int indexKandidata, char* naziv, int najvecaOcjena){
    if (brojKandidata<=0){
        Kandidat *pok=&niz[indexKandidata];
        return pok;
    }

    for (int i=0; i<4; i++){
       if (niz[brojKandidata-1]._uspjeh[i]!=nullptr){
       for (int j=0; j<niz[brojKandidata-1]._uspjeh[i]->_brojPredmeta; j++){
        if (strcmp(niz[brojKandidata-1]._uspjeh[i]->_predmeti[j]._naziv, naziv)==0){
            if (niz[brojKandidata-1]._uspjeh[i]->_predmeti[j]._ocjena>najvecaOcjena){
                najvecaOcjena=niz[brojKandidata-1]._uspjeh[i]->_predmeti[j]._ocjena;
                indexKandidata=brojKandidata-1;
            }
        }
       }
       }
    }
    rekNajboljaOcjena(niz, brojKandidata-1, indexKandidata, naziv, najvecaOcjena);
}

int main()
{
    DatumVrijeme datum19062017_1015, datum20062017_1115, datum30062017_1215, datum05072017_1231;
    datum19062017_1015.Unos(19, 6, 2017, 10, 15);
    datum20062017_1115.Unos(20, 6, 2017, 11, 15);
    datum30062017_1215.Unos(30, 6, 2017, 12, 15);
    datum05072017_1231.Unos(5, 7, 2017, 12, 31);

   // cout << datum19062017_1015.GetDatumKaoNizKaraktera() << endl;//9.6.2017 10:15

    Predmet Matematika, Fizika, Hemija, Engleski;
//2 - ocjena na predmetu; datum - datum evidentiranja uspjeha na predmetu jer postoji krajnji rok za evidentiranje
    Matematika.Unos("Matematika", 2, datum19062017_1015);
    Fizika.Unos("Fizika", 5, datum20062017_1115);
    Hemija.Unos("Hemija", 2, datum20062017_1115);
    Engleski.Unos("Engleski", 5, datum05072017_1231);

    int brojKandidata = 3;

    Kandidat * prijave2017 = new Kandidat[brojKandidata];
    prijave2017[0].Unos(DL, "Jasmin Azemovic");
    prijave2017[1].Unos(REDOVAN, "Indira Hamulic");
    prijave2017[2].Unos(REDOVAN, "Alma Djedovic");

/*
uspjeh (tokom srednjoskolskog obrazovanja) se dodaje za svaki predmet na nivou razreda.
prilikom dodavanja onemoguciti:
- dodavanje predmeta za razrede koji nisu definisani enumeracijom,
- dodavanje istoimenih predmeta na nivou jednog razreda,
- dodavanje predmeta nakon dozvoljenog roka za prijavu (rokZaPrijavu).
razredi (predmeti ili uspjeh) ne moraju biti dodavani sortiranim redoslijedom (npr. prvo se moze dodati uspjeh za II razred,
 pa onda za I razred i sl.). Funkcija vraca true ili false u zavisnosti od (ne)uspjesnost izvrsenja
*/

    if (prijave2017[0].DodajPredmet(DRUGI, Engleski))//ne bi trebao dodati jer je prosao postavljeni rok za dodavanje predmeta
    cout << "Predmet uspjesno dodan! Engleski, razred DRUGI" << crt;
    if (prijave2017[0].DodajPredmet(DRUGI, Matematika))
    cout << "Predmet uspjesno dodan! Matematika, razred DRUGI" << crt;
    if (prijave2017[0].DodajPredmet(PRVI, Fizika))
    cout << "Predmet uspjesno dodan! Fizika, razred PRVI" << crt;
    if (prijave2017[0].DodajPredmet(PRVI, Hemija))
    cout << "Predmet uspjesno dodan! Hemija, razred PRVI" << crt;
    Matematika._ocjena=10;
    if (prijave2017[2].DodajPredmet(PRVI, Matematika)){
        cout << "Predmet uspjesno dodan! Matematika, razred PRVI" << crt;
    }

    Matematika._ocjena = 5;
    Hemija._ocjena = 3;

    if (prijave2017[1].DodajPredmet(PRVI, Matematika))
    cout << "Predmet uspjesno dodan! Matematika, razred PRVI" << crt;
    if (prijave2017[1].DodajPredmet(PRVI, Matematika))//ne bi trebalo ponovo dodati Matematiku!
    cout << "Predmet uspjesno dodan! Matematika, razred, PRVI" << crt;
    if (prijave2017[1].DodajPredmet(TRECI, Hemija))
    cout << "Predmet uspjesno dodan! Hemija, razred TRECI" << crt;
    if (prijave2017[1].DodajPredmet(DRUGI, Engleski))
    cout << "Predmet uspjesno dodan! Engleski, razred DRUGI" << crt;

/*
koristeci Lambda izraz kreirati funkciju koja ce vratiti uspjeh kandidata koji je ostvario najveci prosjek (na nivou razreda, a ne ukupni prosjek).
ukoliko vise kandidata ima isti prosjek funkcija vraca uspjeh (najboljeg razreda) prvog pronadjenog kandidata
*/
    auto najboljiUspjeh = [prijave2017, brojKandidata]()->shared_ptr<Uspjeh>{
        double najboljiProsjek=0;
        double pom=0;
        for (int i=0; i<4; i++){
            if (prijave2017[0]._uspjeh[i]!=nullptr){
                for (int j=0; j<prijave2017[0]._uspjeh[i]->_brojPredmeta; j++){
                    pom+=prijave2017[0]._uspjeh[i]->_predmeti[j]._ocjena;
                }
                pom/=(double)prijave2017[0]._uspjeh[i]->_brojPredmeta;
                break;
            }
        }
        int indeks=0;
        int indeks2=0;
        for (int i=0; i<4; i++){
            for (int j=0; j<brojKandidata; j++){
            najboljiProsjek=0;
            if (prijave2017[j]._uspjeh[i]!=nullptr){
                for (int k=0; k<prijave2017[j]._uspjeh[i]->_brojPredmeta; k++){
                    najboljiProsjek+=prijave2017[j]._uspjeh[i]->_predmeti[k]._ocjena;
                }
                najboljiProsjek/=(double)prijave2017[j]._uspjeh[i]->_brojPredmeta;
                if (najboljiProsjek>pom){
                    pom=najboljiProsjek;
                    indeks=j;
                    indeks2=i;
                }
            }
            }
        }
        return prijave2017[indeks]._uspjeh[indeks2];
    };
    shared_ptr<Uspjeh> najbolji = najboljiUspjeh();
    najbolji->Ispis();
    cout << endl;

/*
napisati rekurzivnu funkciju koja ce vratiti pokazivac na kandidata sa najvecom ocjenom na predmetu koji je proslijedjen kao parametar.
 ukoliko je vise kandidata ostvarilo istu ocjenu, funkcija treba da vrati onog kandidata koji je prvi evidentirao tu ocjenu
(ako je isto vrijeme evidentiranja, onda funkcija vraca kandidata koji je prvi u nizu).	u slucaju da niti jedan kandidat nije evidentirao
trazeni predmet funkcija vraca nullptr. u nastavku je prikazan primjer poziva rekurzivne funkcije, a ostale parametre dodajte po potrebi.
*/

    cout << "Kandidat sa najboljom ocjenom iz predmeta " << Matematika._naziv << endl;
    Kandidat * kandidatSaNajboljomOcjenom = rekNajboljaOcjena(prijave2017, brojKandidata, 0, "Matematika", 0);
    if (kandidatSaNajboljomOcjenom==nullptr){
        cout << "NEMA" << endl;
    } else {
        kandidatSaNajboljomOcjenom->Ispis();
    }
    cout << endl << endl;
    cout << "Ovde vrsimo ispis i dealokaciju svih kandidata, a to su: " << endl;
    for (int i = 0; i < brojKandidata; i++)
    {
        prijave2017[i].Ispis();
        prijave2017[i].Dealociraj();
    }
    cout << endl;
    delete[] prijave2017;
    prijave2017 = nullptr;

    return 0;
}











