/*  Carlo Salazar, Stephen Summy, Matan Gandelman, Michael Kagan
    masc0897
    team Ohio
    prog3
    CS530, Spring 2016
    symtab.cc
*/

#include <map>
#include <string>
#include <iostream>
#include <vector>
#include <utility>
#include <sstream>
#include <iomanip>
#include <ctype.h>

#include "symtab.h"
#include "symtab_exception.h"

using namespace std;

symtab::symtab(){};

void symtab::insert(string label, string val){
    if(sTab.find(label)==sTab.end()){
        if(val.length()!=5){
            stringstream ss;
            ss << setfill('0') << setw(5) << val;
            val = ss.str();
        }
        sTab.insert(pair<string,string>(label,val));
    }
    else throw symtab_exception("Unable to use duplicate token: " + label);
}

bool symtab::contains(string label){
    if(sTab.find(label)==sTab.end()) return false;
    return true;
}

string symtab::get_value(string label){
    if(sTab.find(label)==sTab.end()) 
        throw symtab_exception("User defined token: " + label + "does not exist");
    return sTab.find(label)->second;
}

void symtab::handle_fr(){
    map<string,string >::iterator it;
    for(it=sTab.begin(); it!=sTab.end();++it){
        if(!isdigit(it->second.at(0))) it->second=find_fr(it->second);
    }
}

//Test symtab
void symtab::print(){
    map<string,string >::iterator it;
    for(it=sTab.begin(); it!=sTab.end();++it){
        cout << "Label: " << it->first << " Value: " << it->second << endl;
    }
    cout << "# symbols: " << sTab.size() << endl;
}
