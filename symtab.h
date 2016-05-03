/*  Carlo Salazar, Stephen Summy, Matan Gandelman, Michael Kagan
    masc0897
    team Ohio
    prog3
    CS530, Spring 2016
    symtab.h
*/

#ifndef SYMTAB_H
#define SYMTAB_H

#include <map>

using namespace std;

class symtab{
public:
    symtab();
    void insert(string,string);
    bool contains(string);
    string get_value(string);
    void handle_fr();
    void print();
private:  
    map<string,string> sTab; 
    
    string find_fr(string ref){
        if(isdigit(ref.at(0))) return ref;
        return find_fr(get_value(ref));
    }
};

#endif 

