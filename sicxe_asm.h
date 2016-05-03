/*  Carlo Salazar, Stephen Summy, Matan Gandelman, Michael Kagan
    masc0897
    team Ohio
    prog4
    CS530, Spring 2016
    sicxe_asm.h
*/

#include <string>	
#include <map>
#include <stdbool.h>
#include <sstream>
#include <ctype.h>

#include "file_parser.h"
#include "opcodetab.h"
#include "symtab.h"

#ifndef SICXE_ASM_H
#define SICXE_ASM_H

using namespace std;

class sicxe_asm{
    
    struct line{
        int lineNo, numComments;
        string address, label, opcode, operand, comment, machinecode;
    };
    vector<line> lis; 
    opcodetab ocTab;
    symtab symTab;
    string base_reg;
    int nixbpe[6];
    bool indexed;
    bool hasx_reg = false;
    
public:
    sicxe_asm(string);
    ~sicxe_asm();
    void pass1(file_parser);
    void pass2(string);
       
private:     
    bool strcmp(string a, string b){
        for(unsigned int i=0; i<a.size(); i++){
            a.at(i) = toupper(a.at(i));
        }
        if(a.compare(b)==0) return true;
        return false;
    }
    
    bool isDec(string a){
        for(unsigned int i=0; i<a.size(); i++){
            if(!isdigit(a[i])) return false;            
        }
        return true;
    }
    
    //Handle BYTE directive errors
    bool isHex(string a){
        for(unsigned int i=2; i<a.size()-1; i++){
            if(!isxdigit(a[i])) return false;
        }
        return true;
    }
    
    //PASS 1
    bool isValidStart(vector<line>);
    void getDir(unsigned int, string, string, string, unsigned int&, opcodetab, symtab);
    void handleDir_start(unsigned int, string, unsigned int&);
    void handleDir_byte(unsigned int, string, unsigned int&);
    void handleDir_word(unsigned int, string, string, unsigned int&, symtab);
    void handleDir_resb(unsigned int, string, unsigned int&);
    void handleDir_resw(unsigned int, string, unsigned int&);
    void handleDir_base(string);
    void handleDir_nobase();
    void handleDir_equ(string,string);
    void write_lisfile(string);
    void eHelper(string, unsigned int);
    
    //PASS 2
    string get_machinecode(int);
    string get_reg(unsigned int, string, string);
    void get_addrMode(string&,string,int);
    string f2_instr(unsigned int, string, string);
    string f3_instr(unsigned int, string, string, string);
    string f4_instr(unsigned int, string, string, string);
    string sec_mc(string,int); 
    bool isDir(string);
    bool isValidf2_instr(string,string);
    bool isValidBitFlags(int[]);
    
    //Conversion methods  
    string stoascii(string);
    string itoh(int,int);
    string itos(int);
    int stoi(string);
    int htoi(string);
};

const string dir[7] = {"START", "END", "RESB", "RESW", "BASE", "NOBASE", "EQU"};

#endif

