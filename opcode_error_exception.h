/*  Carlo Salazar, Stephen Summy, Matan Gandelman, Michael Kagan
    masc0897
    team Ohio
    prog2
    CS530, Spring 2016
    opcode_error_exception.h
*/

#ifndef OPCODE_ERROR_EXCEPTION_H
#define OPCODE_ERROR_EXCEPTION_H

using namespace std;

class opcode_error_exception {

public:
    opcode_error_exception(string s) {
        message = s;        
        }
        
    opcode_error_exception() {
        message = "An error has occurred";
        }
        
    string getMessage() {
        return message;
    }
    
private:
    string message;
};
#endif

