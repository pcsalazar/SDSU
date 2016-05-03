/*  Carlo Salazar, Stephen Summy, Matan Gandelman, Michael Kagan
    masc0897
    team Ohio
    prog1
    CS530, Spring 2016
    file_parser.cc
*/

#include <iostream>
#include <fstream> 
#include <sstream>
#include <vector>
#include <cstdlib>
#include <string.h>
#include <iomanip>

#include "file_parser.h"
#include "file_parse_exception.h"

using namespace std;

file_parser::file_parser(string file){
    scfile=file;    
}
file_parser::~file_parser(void){}

void file_parser::read_file(){  
    ifstream infile;
    infile.open(scfile.c_str());
    
    string line;
    int index = 0;
    while(!infile.eof()){ 
        getline(infile,line);
        struct tokens temp;
        temp.label = temp.opcode = temp.operand = temp.comment = "";
        string *pLabel = &temp.label;
        string *pOpcode = &temp.opcode;
        string *pOperand = &temp.operand;
        string *pComment = &temp.comment;
        
        index++;
        
        //Checks for tabs and replace with wspace
        line = rTabs(line);
       
        //push empty line
        if(!(line.find_first_not_of(' ')!=line.npos)){
            sc.push_back(temp);
            continue;        
        }   
        
        line = parse_label(line, pLabel, index);
        line = parse_opcode(line, pOpcode, index);
        line = parse_operand(line,pOperand,index);
        line = parse_comment(line,pComment,index);
        
        sc.push_back(temp);
        
    }
    infile.close(); 
}

string file_parser::get_token(unsigned int row, unsigned int column){
    if(column==0) return sc[row].label;
    else if(column==1) return sc[row].opcode;
    else if(column==2) return sc[row].operand;
    else if(column==3) return sc[row].comment;
}

void file_parser::print_file(){
    cout << left << setw(20) << "label" << left << setw(20) << "opcode"
         << left << setw(20) << "operand" << left << setw(20) << 
         "comment" << endl;
    for(int i=0; i<size(); i++){
        cout << left << setw(20) << sc[i].label << left << setw(20)
             << sc[i].opcode << left << setw(20) << sc[i].operand 
             << left << setw(20) << sc[i].comment << endl;
    }
}

int file_parser::size(){
    return sc.size();
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

string file_parser::parse_label(string line, string *pLabel, unsigned int index){
    string label;
    if(isalpha(line.at(0))){
        label = line.substr(0,line.find_first_of(" "));
        if(label.length()>8){
            label = *pLabel = line.substr(0,8);
            line.erase(0,label.length());
        }
        else{
            label = *pLabel = line.substr(0,line.find_first_of(" "));
            line.erase(0,label.length());
        }
        for(unsigned int i=0; i<label.length(); i++){
            if(!isalnum(label.at(i))) eHelper("Invalid label", index);
        }
    }
    else if(line.at(0)!='.' && !isalpha(line.at(0)) && !isspace(line.at(0))){
        eHelper("First character of label must be a letter", index);
    }
    return line;
}

string file_parser::parse_opcode(string line, string *pOpcode, unsigned int index){   
    line = clear_wspace(line);
    if(line.empty()) return line;
    if(line.at(0)!='.'){
        string opcode = *pOpcode = line.substr(0,line.find_first_of(" "));
        line.erase(0,opcode.length());
    }
    return line;
}

string file_parser::parse_operand(string line, string *pOperand, unsigned int index){
    line = clear_wspace(line);
    string tmp = line;
    if(line.empty()) return line;
    if(line.at(0)!='.'){
        line = clear_wspace(line);
        string operand;
        if(line.find("'")!=line.npos) operand = *pOperand = line.substr(0,line.find_last_of("'")+1);
        else operand = *pOperand = line.substr(0,line.find_first_of(" "));
        line.erase(0,operand.length());
    }
    line = clear_wspace(line);
    
    //check operand error
    int numQuote = 0;
    for(unsigned int i=0; i<tmp.size(); i++){
        if(tmp.at(i)=='\'') numQuote++;
    }
    if(numQuote>2){
        eHelper("Invalid operand", index);
    }
    return line;
}

string file_parser::parse_comment(string line, string *pComment, unsigned int index){
    line = clear_wspace(line);
    if(!line.empty() && line.at(0)!='.'){
        eHelper("Too many tokens.", index);
    }
    else *pComment = line;
    return line;
}

string file_parser::clear_wspace(string line){
    if(!line.empty() && isspace(line.at(0))){
        int next_token = line.find_first_not_of(' ');
        line.erase(0,next_token);
    }
    return line;
}

string file_parser::rTabs(string line){
    for(unsigned int i=0; i<line.length(); i++){
        if(line.at(i)=='\t'){
            line.replace(i,1,"            ");
        }
    }
    return line;
}

void file_parser::eHelper(string msg, unsigned int index){
    stringstream sInt;
    sInt << index;
    string lineNum = sInt.str();
    throw file_parse_exception("Error on line "+lineNum+": "+msg);
}