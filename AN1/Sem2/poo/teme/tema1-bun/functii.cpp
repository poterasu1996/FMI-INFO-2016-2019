#include <iostream>
#include <cmath>
#include "cod.h"

using namespace std;

///--constructor--
Polinom::Polinom()
{
    cout<<"constructor polinom\n";
    prim=NULL;
}

///--constructor cu parametrii--
//initializeaza primul element al listei
Polinom::Polinom(int gr,int coef)
{
    cout<<"constructor polinom cu parametrii\n";
    prim=new Lista;
    prim->grad=gr;
    prim->coeficient=coef;
    prim->leg=NULL;
    cout<<"polinomul are primul termen "<<coef<<"*x^";
    cout<<gr<<'\n';
}

///--copy constructor--
//deep copy(copiaza element cu element)
Polinom::Polinom(const Polinom &ob2)
{
    cout<<"constructor de copiere\n";
    if(ob2.prim==NULL)
        prim=NULL;
    else
    {
        prim=new Lista;
        prim->grad=ob2.prim->grad;
        prim->coeficient=ob2.prim->coeficient;
        Lista *curent=prim;
        Lista *next=ob2.prim->leg;
        while(next)
        {
            curent->leg=new Lista;
            curent=curent->leg;
            curent->grad=next->grad;
            curent->coeficient=next->coeficient;
            next=next->leg;
        }
        curent->leg=NULL;
    }
}

///--destructor--
Polinom::~Polinom()
{
    Lista *p=prim;
    while(p!=NULL)
    {
        p=p->leg;
        delete prim;
        prim=p;
    }
    delete prim;
    cout<<"Polinom distrus\n";
}

///--copierea unui polinom--
//deep copy(copiaza element cu element)
void Polinom::operator=(const Polinom &ob2)
{
    if(ob2.prim==NULL)
        prim=NULL;
    else
    {
        prim=new Lista;
        prim->grad=ob2.prim->grad;
        prim->coeficient=ob2.prim->coeficient;
        Lista *curent=prim;
        Lista *next=ob2.prim->leg;
        while(next)
        {
            curent->leg=new Lista;
            curent=curent->leg;
            curent->grad=next->grad;
            curent->coeficient=next->coeficient;
            next=next->leg;
        }
        curent->leg=NULL;
    }
}

///--citire--
istream &operator>>(istream &i,Polinom &ob)
{
    int nr,gr;
    int coef;
    cout<<"Dati numarul de elemente dorite: ";
    i>>nr;
    cout<<"\nDati gradele si coeficientii ca un sir de ";
    cout<<2*nr<<" numere, separate prin spatii:\n";
    for(int j=1; j<=nr; j++)
    {
        i>>gr>>coef;
        ob.inserare(gr,coef);
    }
    return i;
}

///--afisare--
ostream& operator<<(ostream &i,const Polinom &ob)
{
    if(ob.prim==NULL)
        i<<"Polinom nul.\n";
    else
    {
        i<<"p: ";
        Polinom::Lista *p;
        for(p=ob.prim; p->leg!=NULL; p=p->leg)
        {
            if(p->coeficient!=1)
                i<<p->coeficient<<"*x^"<<p->grad;
            else
                i<<"x^"<<p->grad;
            if(p->leg->coeficient>=0)
                i<<'+';
        }
        if(p->grad!=0)
        {
            if(p->coeficient!=1)
                i<<p->coeficient<<"*x^"<<p->grad<<"\n";
            else
                i<<"x^"<<p->grad<<"\n";
        }
        else
            i<<p->coeficient<<"\n";
    }
    return i;
}

///--Obtine coeficientul termenului cu gradul i--
//daca s-a gasit, altfel nu exista sau polinomul este nul
void Polinom::operator[](int i)
{
    if(this->prim==NULL)
        cout<<"Polinomul este nul.\n";
    else
    {
        Polinom::Lista *p;
        for(p=this->prim; p->leg&&p->grad>i; p=p->leg) {}
        if(p)
        {
            cout<<"Coeficientul termenului cu gradul "<<i;
            cout<<" este "<<p->coeficient<<endl;
        }
        else cout<<"Nu exista termen cu acest grad.\n";
    }
}

///--Calculul polinomului pentru un anumit x--
//se aduna termenii inmultind fiecare
//coeficient cu x-ul ridicat la gradul dat
void Polinom::calcul(int x)
{
    int suma=0;
    Lista *p;
    for(p=this->prim; p!=NULL; p=p->leg)
        suma=suma+p->coeficient*pow(x,p->grad);
    cout<<"Pentru x="<<x<<" polinomul este egal cu "<<suma;
    cout<<".\n";
}

///--inserare la inceput--
void Polinom::adaugareInceput(int gr,int coef)
{
    Lista *p=new Lista;
    p->grad=gr;
    p->coeficient=coef;
    p->leg=prim;
    prim=p;
}

///--inserare descrescator dupa grad--
//la grade egale se aduna coeficientii
void Polinom::inserare(int gr,int coef)
{
    if(coef==0)
        return;
    if(prim==NULL||prim->grad<gr)
        adaugareInceput(gr,coef);
    else
    {
        Lista *p=prim;
        while(p->leg!=NULL&&p->leg->grad>=gr)
            p=p->leg;
        if(gr==p->grad)
        {
            p->coeficient=p->coeficient+coef;
            if(p->coeficient==0&&p==prim)
            {
                p=p->leg;
                delete prim;
                prim=p;
            }
            return;
        }
        Lista *q=new Lista;
        q->grad=gr;
        q->coeficient=coef;
        q->leg=p->leg;
        p->leg=q;
    }
}

///--Obtine gradul maxim al polinomului--
//fiind pe prima pozitie a listei
int Polinom::gradul()
{
    cout<<"Gradul polinomului este ";
    if(prim==NULL)
        cout<<"nul.\n";
    else
        cout<<prim->grad<<".\n";
    return prim->grad;
}

///--adunarea a doua polinoame--
//la grade egale aduna coeficientii
//altfel insereaza cate un termen in polinomul nou pana
//ajunge la grade egale sau se termina parcurgerea polinoamelor intiale
Polinom Polinom::operator+(const Polinom &ob2)
{
    Polinom ob3;
    Lista *p1=this->prim;
    Lista *p2=ob2.prim;
    while(p1&&p2)
    {
        if(p1->grad==p2->grad)
        {
            ob3.inserare(p1->grad,p1->coeficient+p2->coeficient);
            p1=p1->leg;
            p2=p2->leg;
        }
        else if(p1->grad>p2->grad)
        {
            ob3.inserare(p1->grad,p1->coeficient);
            p1=p1->leg;
        }
        else
        {
            ob3.inserare(p2->grad,p2->coeficient);
            p2=p2->leg;
        }
    }
    while(p1)
    {
        ob3.inserare(p1->grad,p1->coeficient);
        p1=p1->leg;
    }
    while(p2)
    {
        ob3.inserare(p2->grad,p2->coeficient);
        p2=p2->leg;
    }
    return ob3;
}

///--Inmultirea a doua polinoame--
//sub forma listelor simplu inlantuite,de ex:
//a: 2|3|->1|2|->0|2->NULL
//b: 1|1->0|2->NULL
//inmultim primul element din lista b cu toate elementele listei a
//rezultatul se va salva progresiv intr-o noua lista
//adunam gradele si inmultim coeficientii
//pas1:3|3->2|2->1|2
//cand inmultim al doilea element din b cu toate din lista a
//inseram in ordine; 3|3->2|3+2->1|2+2->0|4->NULL
//devine 3|3->2|5->1|4->0|4->NULL
//parcurgem astfel toate elementele din b
Polinom Polinom::operator*(const Polinom &ob2)
{
    Polinom ob3;
    Lista *p1=this->prim;
    Lista *p2=ob2.prim;
    for(; p1; p1=p1->leg)
        for(p2=ob2.prim; p2; p2=p2->leg)
            ob3.inserare(p1->grad+p2->grad,p1->coeficient*p2->coeficient);
    return ob3;
}

///--Impartirea a doua polinoame--
//Se ordoneaza polinoamele f si g dupa puterile descrescatoare
//Se face impartirea polinomului de grad mai mare la polinomul de grad mai mic
//Se imparte primul termen al lui f la primul termen al lui g
//si se obtine astfel primul termen al catului.
//Se inmulteste rezultatul obtinut cu impartitorul g si se scade acest produs
//din deimpartitul f.Acest calcul ne da primul rest al impartirii.
//Se repeta procedeul avand deimpartitul actualizat
//Algoritmul se termina cand restul este mai mic decat impartitorul
Polinom Polinom::operator/(const Polinom &ob2)
{
    Polinom deimpartit,impartitor,cat;
    Lista *p1,*p2;
    if(this->prim->grad>=ob2.prim->grad)
    {
        deimpartit= *this;
        impartitor=ob2;
    }
    else
    {
        impartitor= *this;
        deimpartit=ob2;
    }
    p1=deimpartit.prim;
    p2=impartitor.prim;
    while(p1->grad>=p2->grad&&p1&&p2)
    {

        int x,y;
        x=p1->grad-p2->grad;
        y=(p1->coeficient)/(p2->coeficient);
        cat.inserare(x,y);
        for(p2=impartitor.prim; p2; p2=p2->leg)
            deimpartit.inserare(x+(p2->grad),(-1)*y*(p2->coeficient));
        p1=deimpartit.prim;
        p2=impartitor.prim;
    }
    cout<<"Catul este "<<cat;
    cout<<"Restul este "<<deimpartit;
    return cat;
}