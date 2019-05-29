#include <iostream>
#include <fstream>

#include <windows.h>
#include <conio.h>

using namespace std;

const char ENTER = 13, UP = 72, DOWN = 80, LEFT = 75, RIGHT = 77;

void ShowConsoleCursor(bool showFlag);
void gotoxy( short x, short y );

/*
 * functions for working with char* variables:
*/
char* trim(char *a);
int leng(char *a)
{
    int lg = 0;
    while(a[lg])
        lg++;
    return lg;
}
void cpy(char *a, char *b)
{
    int i = 0;
    do
        a[i] = b[i];
    while(b[i++] != 0);
}

/*
 * definition struct and how to instantiate one:
*/
struct definition
{
    char *def;
    definition *nxt;
};
definition* defi(char *d)
{
    definition *q = new definition;
    char *d2 = new char[leng(d)];
    cpy(d2, d);
    q->def = d2;
    return q;
}

/* 
 * as to make a dictionary, we define a trie (reTRIEval tree)
 */
class trie
{
private:
    trie *t[26]; // limitare memorie? (1)
    int nrdefs;
    definition *d;
    bool isword;
public:
    void allc(int p) // allocate
    {
        trie *tr;
        tr = new trie;
        this->t[p] = tr;
    }
    void init()
    {
        //t = new trie*[26]; (1)
        for(int i = 0; i < 26; i++)
            t[i] = NULL;
        d = NULL;
        nrdefs = isword = 0;
    }
    trie* addr(int p)
    {
        return t[p];
    }
    void word()
    {
        isword = 1;
    }
    void defs(definition *def)
    {
        this->d = def;
    }
    void addw(char *s)
    {
        system("cls"); /// It remains to be seen
        int lg = 0, w[51]; /// schimbat lungimea
        while(s[lg] != 0)
            w[lg] = s[lg] - 'a', lg++;
        int pz = 0;
        trie *ptr = this;
        while(pz < lg)
        {
            if(ptr->t[w[pz]] != NULL)
                ptr = ptr->t[w[pz]], pz++;
            else
                while(pz < lg)
                {
                    ptr->allc(w[pz]);
                    ptr = ptr->t[w[pz]];
                    ptr->init();
                    pz++;
                }
        }
        ptr->word();
        ShowConsoleCursor(false); ///
        cout << "Adaugati definitii?\n";  // row 0
        cout << "   DA\n";              // row 1
        cout << "   NU\n";              // row 2
        int row = 1;
        char ch;
        gotoxy(row, 6);
        cout << '*';
        do
        {

            ch = getch();
            if(ch == -32)
            {
                ch = getch();
                if(ch == UP && row == 2)
                {
                    /// subprogram
                    gotoxy(row, 6);
                    cout << ' ';
                    gotoxy(--row, 6);
                    cout << '*';
                }
                if (ch == DOWN && row == 1)
                {
                    /// subprogram
                    gotoxy(row, 6);
                    cout << ' ';
                    gotoxy(++row, 6);
                    cout << '*';
                }
            }
        } /// FOR NOW
        while(ch != ENTER);
        ShowConsoleCursor(true); ///
        if (row == 1) // DA
        {
            gotoxy(1, 3);
            cout << '\t' ;
            gotoxy(2, 3);
            cout << '\t' ;
            gotoxy(0, 0);
            cout << s << ": \t\t\t\t\n";
            ptr->addd();
        }
        else
            cin.get(); /// where this "carriage return" from
        system("cls"); /// It remains to be seen
    }
    void addd()
    {
        cin.get(); /// where this "carriage return" from
        int nrd = 0;
        definition *definitions, *ptr = definitions; /// FIRSTLY, ptr = definitions is useless.
        definitions = new definition;
        ptr = definitions;
        char df[101]; // d is already taken for the CLASS
        do
        {
            cout << '\t';
            cin.getline(df, 101); /// length
            if(df[0])
            {
                nrd++;
                ptr->nxt = defi(df);
                ptr = ptr->nxt;
                continue;
            }
            else break;
        }
        while(df[0] != 0); /// mai ramane de verificat cu alte taste.
        if(ptr != NULL) /// WHY
            ptr->nxt = NULL; /// THE
        if(nrd > 0) ///definitions != NULL) /// HELL
            {
                this->defs(definitions->nxt); /// ????
                this->nrdefs += nrd;
            }
        else
            delete definitions;
    }
    void list()
    {
        trie *a[50];
        int w[51];
        w[0] = -1;
        int p = 0;
        a[p] = this;
        while(p >= 0)
        {
            int ok = 1;
            for(int i = w[p] + 1; i < 26; i++)
                if(a[p]->t[i] != NULL)
                {
                    w[p] = i;
                    p++;
                    w[p] = -1;
                    a[p] = a[p - 1]->t[i];
                    ok = 0;
                    break;
                }
            if(ok != 0) // in ordinea inversa (fata de afisare), se afiseaza de doua ori, logically
                p--;
            else    /// merge modificat??
                if(a[p]->isword == 1)
                {
                    for(int i = 0; i < p; i++)
                        cout << char(w[i] + 'a');
                    cout << endl;
                    definition *ptr = a[p]->d; /// I am the most stupid guy, I wrote this->d instead of a[p]->d
                    while(ptr != NULL)
                    {
                        cout << "  = " << ptr->def << endl;
                        ptr = ptr->nxt;
                    }
                }
        }
    }
    void rite(char *doc1, char *doc2) /// read() si rite utilizeaza doua variante dintre care trebuie sa aleg doar una!!!
    {
        ofstream f1(doc1);
        ofstream f2(doc2);
        trie *a[50];
        int w[51];
        w[0] = -1;
        int p = 0;
        a[p] = this;
        while(p >= 0)
        {
            int ok = -1; ///  modif
            for(int i = w[p] + 1; i < 26; i++)
                if(a[p]->t[i] != NULL)
                {
                    w[p] = i;
                    p++;
                    w[p] = -1;
                    a[p] = a[p - 1]->t[i];
                    ok = i;
                    break;
                }
            if(ok == -1)
            {
                p--;
                f1 << ok << '\n';/// modif
            }
            else
            {
                f1 << ok << ' ' << a[p]->isword << '\n'; /// modif
                if(a[p]->isword)
                {
                    f2 << a[p]->nrdefs << '\n';
                    definition *ptr = a[p]->d;
                    while(ptr != NULL)
                    {
                        f2 << ptr->def << '\n';
                        ptr = ptr->nxt;
                    }
                }
            }
        }
        f1.close();
        f2.close();
    }
    void nrdf(int nrd)
    {
        this->nrdefs = nrd;
    }
    void read(char *doc1, char *doc2)
    {
        ifstream f1(doc1);
        ifstream f2(doc2);
        trie *a[50];
        unsigned char w[51]; /// NOT USED . . .
        int p = 0, next, word;
        this->init();
        a[p] = this;
        while(p >= 0)
        {
            f1 >> next;
            if(next >= 0)
            {
                f1 >> word;
                w[p] = 'a' + next; ///
                a[p]->allc(next);
                p++;
                a[p] = a[p - 1]->addr(next);
                a[p]->init();
                if(word != 0) a[p]->word();
                w[p] = 0; ///
                if (word)
                {
                    //cout << w << '\n'; ///
                    /// allocating a new "definition"; need to think about the names of the variables
                    definition *definitions, *ptr = definitions; /// FIRSTLY, ptr = definitions is useless.
                    int nrdfs;
                    f2 >> nrdfs;
                    if(nrdfs > 0)
                    {
                        f2.get();
                        a[p]->nrdf(nrdfs);

                        definitions = new definition;
                        ptr = definitions;
                        do
                        {
                            char d[101]; /// the length needs to be modified, or not
                            f2.getline(d, 101);
                            ptr->nxt = defi(d);
                            ptr = ptr->nxt;
                        }
                        while(--nrdfs);
                    }
                    if(ptr != NULL) /// WHY
                        ptr->nxt = NULL; /// THE
                    if(definitions != NULL) /// HELL
                        a[p]->defs(definitions->nxt); /// ?
                }
            }
            else
                p--;
        }
        f1.close();
        f2.close();
    }
};

int main()
{
    
    return 0;
}

char* trim(char *a)
{
    char *b = new char[leng(a)];
    cpy(b, a); // b <- a
    return b;
}

void gotoxy( short x, short y )
{
    COORD p = { y, x };
    SetConsoleCursorPosition( GetStdHandle( STD_OUTPUT_HANDLE ), p );
}

void ShowConsoleCursor(bool showFlag)
{
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_CURSOR_INFO     cursorInfo;

    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = showFlag; // set the cursor visibility
    SetConsoleCursorInfo(out, &cursorInfo);
}