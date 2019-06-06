#ifndef DICT_CLASS_H_INCLUDED
#define DICT_CLASS_H_INCLUDED


// The base dictionary prototype class
class Dictionary_prototype
{
public:
    virtual Dictionary_prototype* clone() = 0;
    virtual int get_defs_per_word() = 0;
protected:
    static int dict_count;
    int def_per_word = 0;
    void set_dpw(int dpw) { def_per_word = dpw; }
};

int Dictionary_prototype::dict_count = 0;

// the two prototypes
class One_def_dict : public Dictionary_prototype
{
public:
    One_def_dict() {}
    Dictionary_prototype* clone()
    {
        return new One_def_dict(*this);
    }
    int get_defs_per_word() { return 1; }
// private:
//     const int def_per_word = 0;
};
class Mult_def_dict : public  Dictionary_prototype
{
public:
    Mult_def_dict(int dpw) { set_dpw(dpw); }
    Dictionary_prototype* clone()
    {
        return new Mult_def_dict(*this);
    }
    int get_defs_per_word()
    {
        return def_per_word;
    }
private:
    int def_per_word;
};

// a factory that generates dictionaries:
// -- three flavours -- >:D<
class Dict_factory
{
public:
    static void initialise()
    {
        single_def = new One_def_dict();
        multiple_defs_3 = new Mult_def_dict(3);
        multiple_defs_5 = new Mult_def_dict(5);
    }
    static Dictionary_prototype* get_dictionary_single()
    {
        return single_def->clone();
    }
    static Dictionary_prototype* get_dictionary_3()
    {
        return multiple_defs_3->clone();
    }
    static Dictionary_prototype* get_dictionary_5()
    {
        return multiple_defs_5->clone();
    }
    //static (***)
private:
    static Dictionary_prototype* single_def;
    static Dictionary_prototype* multiple_defs_3;
    static Dictionary_prototype* multiple_defs_5;
};

Dictionary_prototype* Dict_factory::single_def;
Dictionary_prototype* Dict_factory::multiple_defs_3;
Dictionary_prototype* Dict_factory::multiple_defs_5;

#endif // DICT_CLASS_H_INCLUDED
