/*  Carlo Salazar, Stephen Summy, Matan Gandelman, Michael Kagan
    masc0897
    team Ohio
    prog2
    CS530, Spring 2016
    opcodetab.cc
*/

#include <map>
#include <iostream>

#include "opcodetab.h"
#include "opcode_error_exception.h"

using namespace std;

opcodetab::opcodetab(){
    for(int i=0; i<59; i++){
        ocTab.insert(pair<string,struct oc*>(mnemonic[i],new struct oc(opcode[i],format[i])));
    }
}

string opcodetab::get_machine_code(string opcode){
    map<string,struct oc*>::iterator it;
    for(unsigned int i=0; i<opcode.size(); i++) opcode.at(i) = toupper(opcode.at(i));
    string temp = opcode;
    if(opcode.empty()) throw opcode_error_exception("No opcode was provided");
    
    if(opcode.at(0)=='+') opcode.erase(0,1);
    
    it = ocTab.find(opcode);
    if(it == ocTab.end()) throw opcode_error_exception("Unable to find opcode: " +opcode);
    else if(temp.at(0)=='+' && it->second->instruction_size!=3) 
        throw opcode_error_exception("Invalid format for " +opcode);
    
    return it->second->machine_code;
}

int opcodetab::get_instruction_size(string opcode){
    map<string,struct oc*>::iterator it;
    for(unsigned int i=0; i<opcode.size(); i++) opcode.at(i) = toupper(opcode.at(i));
    string temp = opcode;
    if(opcode.empty()) throw opcode_error_exception("No opcode was provided");
    
    if(opcode.at(0)=='+') opcode.erase(0,1);
    
    it = ocTab.find(opcode);
    if(it == ocTab.end()) throw opcode_error_exception("Unable to find opcode: " +opcode);
    else if(temp.at(0)=='+' && it->second->instruction_size!=3) 
        throw opcode_error_exception("Invalid format for " +opcode);
    else if(temp.at(0)=='+') return 4;
    
    return it->second->instruction_size;
}