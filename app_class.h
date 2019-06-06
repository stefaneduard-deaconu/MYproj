#ifndef APP_CLASS_H_INCLUDED
#define APP_CLASS_H_INCLUDED

#include <conio.h>
#include <fstream>
#include <iostream>

using namespace std;

class App
{
public:
    static const char ENTER = 13, ESCAPE = 27, BACKSPACE = 8,
        KEYS = -32, UP = 72, DOWN = 80, LEFT = 75, RIGHT = 77;
    void start()
    {
        show_console_cursor(false);
        char suffix[] = ">";
        char app_path[MAX_PATH], doc[53], init[] = " <none>";
        app_path[0] = 0;
        cpy(doc, init);
        int mod = 6, pos = 0, length[6] = {20 + leng(doc), 13, 10, 20, 20, 5};

        char ch;
        do
        {
            system("cls");
            cout << "Choose 'dictionary'" << doc << '\n';
            cout << "Browse words" << '\n';
            cout << "Add words" << '\n';
            cout << "Create 'dictionary'" << '\n';
            cout << "Delete 'dictionary'" << '\n';
            cout << "Exit" << '\n';
            do
            {
                gotoxy(pos, length[pos]);
                cout << '*';
                ch = getch();
                if (ch == KEYS)
                {
                    ch = getch();
                    if (ch == UP || ch == DOWN)
                    {
                        gotoxy(pos, length[pos]);
                        cout << ' ';
                        if (ch == UP)
                            pos--;
                        else
                            pos++;
                        pos = (pos + mod) % mod;
                    }
                }
            }
            while (ch != ENTER);
            switch (pos)
            {
            case 0:
                if (choose_dictionary(doc + 2) == true)
                {
                    cpy(app_path, path());
                    app_path[leng(app_path) + 1] = 0;
                    app_path[leng(app_path)] = '\\';
                    cpy(app_path + leng(app_path), doc + 2);
                    cpy(doc + leng(doc), suffix);
                    length[0] = 20 + leng(doc);
                }
                else
                    continue_message("No 'dictionary' is selected!");
                break;
            case 1:
                if (app_path[0])
                    Browse(app_path);
                else
                    continue_message("No 'dictionary' is selected!");
                break;
            case 2:
                if (app_path[0])
                    add_words(app_path);
                else
                    continue_message("No 'dictionary' is selected!");
                break;
            case 3:
                newDictionary();
                break;
            case 4:
                if (app_path[0])
                {
                    delDictionary(app_path);
                    app_path[0] = 0;
                    cpy(doc, init);
                    length[0] = 20 + leng(doc);
                }
                else
                    continue_message("No 'dictionary' is selected!");
                break;
            }
        }
        while (pos != (mod - 1) && ch != ESCAPE);
        show_console_cursor(true);
    }
private:

    void continue_message(const char *message)
    {
        system("cls");
        cout << message << endl;
        cout << "Press ENTER to continue...";
        cin.get();
        system("cls");
    }
///

    bool word_exists(char *word, char *dict_path)
    {
        char _words[] = "\\_words.txt", _aux[] = "\\_aux.txt";
        char p1[MAX_PATH], p2[MAX_PATH];
        cpy(p1, dict_path);
        cpy(p1 + leng(p1), _words);
        cpy(p2, dict_path);
        cpy(p2 + leng(p2), _aux);
        fstream f1, f2;
        f1.open(p1, ios_base::in);
        f2.open(p2, ios_base::out);
        char read_word[51];
        while (f1.getline(read_word, 51) && cmp(read_word, word) == -1)
            f2 << read_word << '\n';
        if (read_word[0] && cmp(read_word, word) == 0)
        {
            continue_message("The word is already in the dictionary!");
            return true;
        }
        f2 << word << '\n';
        if (read_word[0])
            f2 << read_word << '\n';
        while (f1.getline(read_word, 51))
            f2 << read_word << '\n';
        f1.close();
        f2.close();
        f1.open(p2, ios_base::in);
        f2.open(p1, ios_base::out);
        while (f1.getline(read_word, 51))
            f2 << read_word << '\n';
        f1.close();
        f2.close();
        return false;
    }
    void add_words(char *dict_path)
    {
        system("cls");
        show_console_cursor(true);
        fstream file_stream;
        char word[52], word_path[MAX_PATH], *ptr1, *ptr2, dottxt[] = ".txt";
        word[0] = '\\';
        ptr1 = word + 1;
        cpy(word_path, dict_path);
        int word_path_length = leng(word_path);
        ptr2 = word_path + word_path_length;
        int num_words = 0;
        do
        {
            cout << "word:    <ENTER TO STOP>" << '\n';
            cin.getline(ptr1, 51);
            int lgw = leng(ptr1);
            if (lgw && word_exists(ptr1, dict_path) == false)
            {
                num_words++;
                cpy(ptr1 + lgw, dottxt);
                cpy(ptr2, word);
                file_stream.open(word_path, ios_base::out);
                char def[251];
                cout << "    definitions:    <ENTER TO STOP>" << '\n';
                do
                {
                    cout << "    ";
                    cin.getline(def, 251);
                    if (def[0])
                        file_stream << def << '\n';
                    else
                        break;
                }
                while (def[0]);
                file_stream.close();
            }
        }
        while (ptr1[0]);
        char docs_path[MAX_PATH], aux_path[MAX_PATH], _docs[] = "\\_docs.txt",
                _aux[] = "\\_aux.txt";
        cpy(docs_path, dict_path);
        cpy(aux_path, dict_path);
        int pos = word_path_length;
        while (docs_path[pos] != '\\')
            pos--;
        char dict_name[51];
        cpy(dict_name, docs_path + pos + 1);
        cpy(docs_path + pos, _docs);
        cpy(aux_path + pos, _aux);
        fstream f1, f2;
        int number;
        char line[51];
        f1.open(docs_path, ios_base::in);
        f2.open(aux_path, ios_base::out);
        f1 >> number;
        f2 << number << '\n';
        f1.get();
        while (f1.getline(line, 51))
        {
            f1 >> number;
            f1.get();
            if (cmp(line, dict_name) == 0)
                number += num_words;
            f2 << line << '\n' << number << '\n';
        }
        f1.close();
        f2.close();
        f1.open(aux_path, ios_base::in);
        f2.open(docs_path, ios_base::out);
        while (f1.getline(line, 51))
            f2 << line << '\n';
        f1.close();
        f2.close();
        show_console_cursor(false);
        system("cls");
    }

    bool isDir(char *name)
    {
        char p1[MAX_PATH], p2[MAX_PATH], _docs[] = "\\_docs.txt",
                _aux[] = "\\_aux.txt";
        int lengPath;
        cpy(p1, path());
        cpy(p2, p1);
        lengPath = leng(p1);
        cpy(p1 + lengPath, _docs);
        cpy(p2 + lengPath, _aux);
        fstream f1, f2;
        f1.open(p1, ios_base::in);
        f2.open(p2, ios_base::out);
        int number;
        char line[51];
        f1 >> number;
        f1.get();
        f2 << number << '\n';
        while (f1.getline(line, 51) && cmp(line, name) == -1)
        {
            f2 << line << '\n';
            f1 >> number;
            f1.get();
            f2 << number << '\n';
        }
        if (line[0] && cmp(line, name) == 0)
        {
            continue_message("There already is a 'dictionnary' with the same name!");
            return true;
        }
        f2 << name << '\n' << '0' << '\n';
        if (line[0])
        {
            f1 >> number;
            f1.get();
            f2 << line << '\n' << number << '\n';
        }
        while (f1.getline(line, 51))
        {

            f1 >> number;
            f1.get();
            f2 << line << '\n' << number << '\n';
        }
        f1.close();
        f2.close();
        f1.open(p2, ios_base::in);
        f2.open(p1, ios_base::out);
        f1 >> number;
        f1.get();
        f2 << (number + 1) << '\n';
        while (f1.getline(line, 51))
        {
            f1 >> number;
            f1.get();
            f2 << line << '\n' << number << '\n';
        }
        f1.close();
        f2.close();
        return false;
    }

    void newDictionary()
    {
        system("cls");
        show_console_cursor(true);
        cout << "Name of the 'dictionary':    <ENTER TO EXIT>" << '\n';
        char name[51];
        cin.getline(name, 51);
        if (name[0])
        {
            /// creez folderul
            char p[MAX_PATH];
            cpy(p, path());
            if (isDir(name) == false)
            {
                char bslash[] = "\\";
                cpy(p + leng(p), bslash);
                cpy(p + leng(p), name);
                CreateDirectory(p, NULL);
                add_words(p);
            }
        }
        show_console_cursor(false);
        system("cls");
    }

    void delDictionary(char *Path)
    {
        system("cls");
        fstream f1, f2;
        char docsPath[MAX_PATH], auxPath[MAX_PATH], _docs[] = "\\_docs.txt",
                _aux[] = "\\_aux.txt";

        cpy(docsPath, Path);
        cpy(auxPath, Path);
        int pos = leng(docsPath);
        while (docsPath[pos] != '\\')
            pos--;
        char dictname[MAX_PATH];
        cpy(dictname, docsPath + pos + 1);
        cpy(docsPath + pos, _docs);
        cpy(auxPath + pos, _aux);
        int number;
        char line[51];
        f1.open(docsPath, ios_base::in);
        f2.open(auxPath, ios_base::out);
        f1 >> number;
        f1.get();
        f2 << (number - 1) << '\n';
        while (f1.getline(line, 51))
        {
            if (cmp(line, dictname) == 0)
                f1.getline(line, 51);
            else
                f2 << line << '\n';
        }
        f1.close();
        f2.close();
        f1.open(auxPath, ios_base::in);
        f2.open(docsPath, ios_base::out);
        while (f1.getline(line, 51))
            f2 << line << '\n';
        f1.close();
        f2.close();
        char wordsPath[MAX_PATH], wordPath[MAX_PATH],
             _words[] = "\\_words.txt", dottxt[] = ".txt", bslash[] = "\\";
        int lengPath = leng(Path);
        cpy(wordsPath, Path);
        cpy(wordsPath + lengPath, _words);
        cpy(wordPath, Path);
        cpy(wordPath + lengPath, bslash);
        f1.open(wordsPath, ios_base::in);
        while (f1.getline(line, 51))
        {
            cpy(wordPath + lengPath + 1, line);
            cpy(wordPath + leng(wordPath), dottxt);
            DeleteFileA(wordPath);
        }
        f1.close();
        char p[MAX_PATH];
        cpy(p, Path);
        int lgp = leng(p);
        cpy(p + lgp, _words);
        DeleteFileA(p);
        cpy(p + lgp, _aux);
        DeleteFileA(p);
        RemoveDirectoryA(Path);
        system("cls");
    }

    void Showdefs(char *word, char *Path)
    {
        system("cls");
        char wordPath[MAX_PATH], dottxt[] = ".txt";
        int lgpath = leng(Path);
        cpy(wordPath, Path);
        wordPath[lgpath] = '\\';
        cpy(wordPath + lgpath + 1, word);
        cpy(wordPath + leng(wordPath), dottxt);
        ifstream f(wordPath);
        char def[251];
        cout << word << endl;
        while (f.getline(def, 251))
            cout << "  = " << def << endl;
        f.close();
        cout << "Press ENTER to return to Browse!";
        cin.get();
        system("cls");
    }

    int Result(char *word, char words[][51], char *Path)
    {
        char p[MAX_PATH], _words[] = "\\_words.txt";
        cpy(p, Path);
        cpy(p + leng(p), _words);
        ifstream f(p);
        int nrwords = 0, lgword = leng(word);
        while (f.getline(words[nrwords], 51))
            if (ncmp(words[nrwords], word, lgword) == true)
                break;
            else
                continue;
        char w[51];
        do
            if (ncmp(words[nrwords], word, lgword) == true)
            {
                cpy(w, words[nrwords]);
                nrwords++;
                f.getline(words[nrwords], 51);
            }
            else
                break;
        while (words[nrwords][0] && nrwords < 25);
        gotoxy(1, 0);
        for (int i = 1; i <= 25; i++)
            cout << "                                                  \n";
        gotoxy(1, 0);
        for (int i = 0; i < nrwords; i++)
            cout << "    " << words[i] << endl;
        f.close();
        return nrwords;
    }

    void Browse(char *Path)
    {
        system("cls");
        show_console_cursor(true);
        int lgword = 0, nrwords = 0;
        char word[51], words[25][51], ch;
        cout << "word: ";
        do
        {
            word[lgword] = 0;
            nrwords = Result(word, words, Path);
            gotoxy(0, 6);
            if (lgword)
                cout << word;
            ch = getch();
            if (ch == BACKSPACE)
            {
                if (lgword > 0)
                {
                    word[--lgword] = 0;
                    gotoxy(0, 6 + lgword);
                    cout << ' ';
                }
            }
            else if (ch == KEYS)
            {
                ch = getch();
                if (ch == DOWN && nrwords)
                {
                    show_console_cursor(false);
                    int pos = 1;
                    do
                    {
                        gotoxy(pos, 5 + leng(words[pos - 1]));
                        cout << '*';
                        ch = getch();
                        if (ch == KEYS)
                        {
                            ch = getch();
                            if (ch == UP || ch == DOWN)
                            {
                                gotoxy(pos, 5 + leng(words[pos - 1]));
                                cout << ' ';
                                if (ch == UP)
                                    pos--;
                                else
                                    pos++;
                                if (pos == 0)
                                    pos = nrwords;
                                if (pos > nrwords)
                                    pos = 1;
                            }
                        }
                        else if (ch == ENTER)
                        {
                            Showdefs(words[pos - 1], Path);
                            cout << "word: ";
                            gotoxy(0, 6);
                            if (lgword)
                                cout << word;
                            Result(word, words, Path);
                        }
                    }
                    while (ch != ESCAPE);
                    if (ch == ESCAPE)
                        ch = 0;
                    show_console_cursor(true);
                }
            }
            if (ch >= 'a' && ch <= 'z')
                word[lgword++] = ch;
        }
        while (ch != ENTER && ch != ESCAPE);
        show_console_cursor(false);
        system("cls");
    }

    bool choose_dictionary(char *choice)
    {
        system("cls");
        fstream f;
        char p[MAX_PATH], _docs[] = "\\_docs.txt";
        cpy(p, path());
        cpy(p + leng(p), _docs);
        f.open(p, ios_base::in);
        int number;
        f >> number;
        if (number == 0)
        {
            cout << "Right now, there are no 'dictionaries'." << endl;
            cout << "Press ENTER to create a new one, or other key to exit to menu."
                 << endl;
            char ch = getch();
            if (ch == ENTER)
                newDictionary();
            system("cls");
            return false;
        }
        else
        {
            char docsPath[MAX_PATH], auxPath[MAX_PATH], _docs[] = "\\_docs.txt",
                    _aux[] = "\\_aux.txt";
            cpy(docsPath, path());
            cpy(auxPath, docsPath);
            int lengPath = leng(docsPath);
            cpy(docsPath + lengPath, _docs);
            cpy(auxPath + lengPath, _aux);
            fstream f1, f2;
            f1.open(docsPath, ios_base::in);
            f2.open(auxPath, ios_base::out);
            int nrdict, lengths[25], nrwords[25];
            char line[51];
            f1 >> nrdict;
            f1.get();
            for (int i = 0; i < nrdict; i++)
            {
                f1.getline(line, 51);
                lengths[i] = leng(line);
                f1 >> nrwords[i];
                f1.get();
                f2 << line << '\n';
                cout << line << '\n';
            }
            f1.close();
            f2.close();
            char ch;
            int pos = 0;
            do
            {
                gotoxy(pos, lengths[pos]);
                cout << " <" << nrwords[pos] << " words>";
                ch = getch();
                if (ch == KEYS)
                {
                    ch = getch();
                    if (ch == UP || ch == DOWN)
                    {
                        gotoxy(pos, lengths[pos]);
                        cout << "             ";
                        if (ch == UP)
                            pos--;
                        else
                            pos++;
                        pos = (pos + nrdict) % nrdict;
                    }
                }
            }
            while (ch != ENTER);
            f1.open(auxPath, ios_base::in);
            while (pos >= 0)
            {
                f1.getline(line, 51);
                pos--;
            }
            cpy(choice, line);
            system("cls");
            return true;
        }
    }
};

#endif // APP_CLASS_H_INCLUDED
