/*  Carlo Salazar, Stephen Summy, Matan Gandelman, Michael Kagan
    masc0897
    team Ohio
    prog3
    CS530, Spring 2016
    symtab_exception.h
*/

#ifndef SYMTAB_EXCEPTION_H
#define SYMTAB_EXCEPTION_H

using namespace std;

class symtab_exception {
public:
    symtab_exception(string s) {
        message = s;        
        }
        
    symtab_exception() {
        message = "An error has occurred";
        }
        
    string getMessage() {
        return message;
    }
    
private:
    string message;
};
#endif

