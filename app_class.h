#ifndef APP_CLASS_H_INCLUDED
#define APP_CLASS_H_INCLUDED

#include <conio.h>
#include <fstream>
#include <iostream>

using namespace std;

class App
{
public:
    // for the singleton implementation
    static App* get_instance()
    {
        static bool is_instance = false;
        if (is_instance)
            return instance;
        else
        {
            instance = new App;
            is_instance = true;
        }

    }
    //App(App const&)               = delete;
    //void operator=(App const&)  = delete;
    //

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
            cout << "browse words" << '\n';
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
                    continue_with_message("No 'dictionary' is selected!");
                break;
            case 1:
                if (app_path[0])
                    browse_dictionary(app_path);
                else
                    continue_with_message("No 'dictionary' is selected!");
                break;
            case 2:
                if (app_path[0])
                    add_words(app_path);
                else
                    continue_with_message("No 'dictionary' is selected!");
                break;
            case 3:
                add_dictionary();
                break;
            case 4:
                if (app_path[0])
                {
                    delete_dictionary(app_path);
                    app_path[0] = 0;
                    cpy(doc, init);
                    length[0] = 20 + leng(doc);
                }
                else
                    continue_with_message("No 'dictionary' is selected!");
                break;
            }
        }
        while (pos != (mod - 1) && ch != ESCAPE);
        show_console_cursor(true);
    }
private:
    // the pointer type is used becuase Dictionary_prototype is a pure virtual class,
    Dictionary_prototype* dictionary; // we only use it for virtualization
    // we declare a private 'setter' for the Dictionary_prototye
    void set_dictionary(int dpw)
    {
        switch (dpw)
        {
            case 1:
                dictionary = Dict_factory::get_dictionary_single();
                break;
            case 3:
                dictionary = Dict_factory::get_dictionary_3();
                break;
            case 5:
                dictionary = Dict_factory::get_dictionary_5();
                break;
        }

    }
    // for the singleton implementation
    static App* instance;
    App() {}
    //

    void continue_with_message(const char *message)
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
            continue_with_message("The word is already in the dictionary!");
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
        // first get the type of dictionary:
        char dict_type_path[MAX_PATH], dict_type_filename[] = "\\_dict_type.txt";
        strcpy(dict_type_path, dict_path);
        strcpy(dict_type_path + leng(dict_type_path), dict_type_filename);
          // cout << dict_type_path << endl;
          // cin.get();
          // system("cls");
        file_stream.open(dict_type_path, ios_base::in);
        int dict_type_value;
        file_stream >> dict_type_value;
        // as to instantiate this.dictionnary :
            this->set_dictionary(dict_type_value);
        //
        file_stream.close();
        //
        char word[52], word_path[MAX_PATH], *ptr1, *ptr2, dottxt[] = ".txt";
        word[0] = '\\';
        ptr1 = word + 1;
        cpy(word_path, dict_path);
        int word_path_length = leng(word_path);
        ptr2 = word_path + word_path_length;
        int num_words = 0;
        do
        {
            cout << "word:    <ENTER, when void, TO STOP>" << '\n';
            cin.getline(ptr1, 51);
            int lgw = leng(ptr1);
            if (lgw && word_exists(ptr1, dict_path) == false)
            {
                num_words++;
                cpy(ptr1 + lgw, dottxt);
                cpy(ptr2, word);
                file_stream.open(word_path, ios_base::out);
                char def[251], num_defs = 1;
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
                while (def[0] && num_defs < dictionary->get_defs_per_word()); // (*)
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
        char id_dictionary[51];
        cpy(id_dictionary, docs_path + pos + 1);
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
            if (cmp(line, id_dictionary) == 0)
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

    bool id_dictionary(char *dict_name)
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
        while (f1.getline(line, 51) && cmp(line, dict_name) == -1)
        {
            f2 << line << '\n';
            f1 >> number;
            f1.get();
            f2 << number << '\n';
        }
        if (line[0] && cmp(line, dict_name) == 0)
        {
            continue_with_message("There already is a 'dictionnary' with the same name!");
            return true;
        }
        f2 << dict_name << '\n' << '0' << '\n';
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

    void add_dictionary()
    {
        system("cls");
        show_console_cursor(true);
        // (**)
        // cout << "Type of the 'dictionary':    <ANY NUMBER, but 1, 3, and 5 and STANDARD :)>" << endl;
        // //
        // int dpw = 0;
        // cin >> dpw;
        // while (dpw <= 0)
        // {
        //     cout << "Try a positive number of definitions :).." << endl;
        //     cout << "Retry: ";
        // }
        // cout << "You chose <" << dpw << ">. that means you'll be able to add as mush definitions to any word!" << endl;
        // // we for now store the value inside the dictionary
        // this->set_dictionary(dpw);
        // and we will store it, later, inside the file before opening the _words.txt
        //
        cout << "Name of the 'dictionary':    <ENTER TO EXIT>" << '\n';
        char name[51];
        cin.getline(name, 51);
        if (name[0])
        {
            /// creez folderul
            char p[MAX_PATH];
            cpy(p, path());
            if (id_dictionary(name) == false)
            {
                // we add the dict to the filesystem
                char bslash[] = "\\";
                cpy(p + leng(p), bslash);
                cpy(p + leng(p), name);
                CreateDirectory(p, NULL);
                add_words(p);
                // // and then we add the _words.txt file to the filesystem (***)
                // char dict_type_path[MAX_PATH], dict_type_txt[] = "\\_dict_type.txt";
                // cpy(dict_type_path, p);
                // cpy(dict_type_path + leng(dict_type_path), dict_type_txt);
                // ofstream aux_fs(dict_type_path);
                // aux_fs << dpw;
                // aux_fs.close();
                // // and closing the file we close a deal
            }
        }
        show_console_cursor(false);
        system("cls");
    }

    void delete_dictionary(char *dict_path)
    {
        system("cls");
        fstream f1, f2;
        char docs_path[MAX_PATH], aux_path[MAX_PATH], _docs[] = "\\_docs.txt",
                _aux[] = "\\_aux.txt";

        cpy(docs_path, dict_path);
        cpy(aux_path, dict_path);
        int pos = leng(docs_path);
        while (docs_path[pos] != '\\')
            pos--;
        char dict_name[MAX_PATH];
        cpy(dict_name, docs_path + pos + 1);
        cpy(docs_path + pos, _docs);
        cpy(aux_path + pos, _aux);
        int number;
        char line[51];
        f1.open(docs_path, ios_base::in);
        f2.open(aux_path, ios_base::out);
        f1 >> number;
        f1.get();
        f2 << (number - 1) << '\n';
        while (f1.getline(line, 51))
        {
            if (cmp(line, dict_name) == 0)
                f1.getline(line, 51);
            else
                f2 << line << '\n';
        }
        f1.close();
        f2.close();
        f1.open(aux_path, ios_base::in);
        f2.open(docs_path, ios_base::out);
        while (f1.getline(line, 51))
            f2 << line << '\n';
        f1.close();
        f2.close();
        char words_path[MAX_PATH], word_path[MAX_PATH],
             _words[] = "\\_words.txt", dottxt[] = ".txt", bslash[] = "\\";
        int lengPath = leng(dict_path);
        cpy(words_path, dict_path);
        cpy(words_path + lengPath, _words);
        cpy(word_path, dict_path);
        cpy(word_path + lengPath, bslash);
        f1.open(words_path, ios_base::in);
        while (f1.getline(line, 51))
        {
            cpy(word_path + lengPath + 1, line);
            cpy(word_path + leng(word_path), dottxt);
            DeleteFileA(word_path);
        }
        f1.close();
        char p[MAX_PATH];
        cpy(p, dict_path);
        int p_length = leng(p);
        cpy(p + p_length, _words);
        DeleteFileA(p);
        cpy(p + p_length, _aux);
        DeleteFileA(p);
        RemoveDirectoryA(dict_path);
        system("cls");
    }

    void show_defs(char *word, char *dict_path)
    {
        system("cls");
        char word_path[MAX_PATH], dottxt[] = ".txt";
        int p_lengthath = leng(dict_path);
        cpy(word_path, dict_path);
        word_path[p_lengthath] = '\\';
        cpy(word_path + p_lengthath + 1, word);
        cpy(word_path + leng(word_path), dottxt);
        ifstream f(word_path);
        char def[251];
        cout << word << endl;
        while (f.getline(def, 251))
            cout << "  = " << def << endl;
        f.close();
        cout << "Press ENTER to return to browse_dictionary!";
        cin.get();
        system("cls");
    }

    int found_defs(char *word, char words[][51], char *dict_path)
    {
        char p[MAX_PATH], _words[] = "\\_words.txt";
        cpy(p, dict_path);
        cpy(p + leng(p), _words);
        ifstream f(p);
        int num_words = 0, word_length = leng(word);
        while (f.getline(words[num_words], 51))
            if (ncmp(words[num_words], word, word_length) == true)
                break;
            else
                continue;
        char w[51];
        do
            if (ncmp(words[num_words], word, word_length) == true)
            {
                cpy(w, words[num_words]);
                num_words++;
                f.getline(words[num_words], 51);
            }
            else
                break;
        while (words[num_words][0] && num_words < 25);
        gotoxy(1, 0);
        for (int i = 1; i <= 25; i++)
            cout << "                                                  \n";
        gotoxy(1, 0);
        for (int i = 0; i < num_words; i++)
            cout << "    " << words[i] << endl;
        f.close();
        return num_words;
    }

    void browse_dictionary(char *Path)
    {
        system("cls");
        show_console_cursor(true);
        int word_length = 0, num_words = 0;
        char word[51], words[25][51], ch;
        cout << "word: ";
        do
        {
            word[word_length] = 0;
            num_words = found_defs(word, words, Path);
            gotoxy(0, 6);
            if (word_length)
                cout << word;
            ch = getch();
            if (ch == BACKSPACE)
            {
                if (word_length > 0)
                {
                    word[--word_length] = 0;
                    gotoxy(0, 6 + word_length);
                    cout << ' ';
                }
            }
            else if (ch == KEYS)
            {
                ch = getch();
                if (ch == DOWN && num_words)
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
                                    pos = num_words;
                                if (pos > num_words)
                                    pos = 1;
                            }
                        }
                        else if (ch == ENTER)
                        {
                            show_defs(words[pos - 1], Path);
                            cout << "word: ";
                            gotoxy(0, 6);
                            if (word_length)
                                cout << word;
                            found_defs(word, words, Path);
                        }
                    }
                    while (ch != ESCAPE);
                    if (ch == ESCAPE)
                        ch = 0;
                    show_console_cursor(true);
                }
            }
            if (ch >= 'a' && ch <= 'z')
                word[word_length++] = ch;
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
                add_dictionary();
            system("cls");
            return false;
        }
        else
        {
            char docs_path[MAX_PATH], aux_path[MAX_PATH], _docs[] = "\\_docs.txt",
                    _aux[] = "\\_aux.txt";
            cpy(docs_path, path());
            cpy(aux_path, docs_path);
            int lengPath = leng(docs_path);
            cpy(docs_path + lengPath, _docs);
            cpy(aux_path + lengPath, _aux);
            fstream f1, f2;
            f1.open(docs_path, ios_base::in);
            f2.open(aux_path, ios_base::out);
            int nrdict, lengths[25], num_words[25];
            char line[51];
            f1 >> nrdict;
            f1.get();
            for (int i = 0; i < nrdict; i++)
            {
                f1.getline(line, 51);
                lengths[i] = leng(line);
                f1 >> num_words[i];
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
                cout << " <" << num_words[pos] << " words>";
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
            f1.open(aux_path, ios_base::in);
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

// definition of static member, used for singleton:
App *App::instance = NULL;

#endif // APP_CLASS_H_INCLUDED
