/*  Carlo Salazar, Stephen Summy, Matan Gandelman, Michael Kagan
    masc0897
    team Ohio
    prog4
    CS530, Spring 2016
    sicxe_asm.cpp 
*/

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <fstream>
#include <string.h>

#include "file_parser.h"
#include "file_parse_exception.h"
#include "opcodetab.h"
#include "opcode_error_exception.h"
#include "symtab.h"
#include "symtab_exception.h"
#include "sicxe_asm.h"

using namespace std;

sicxe_asm::sicxe_asm(string file){
    try{
        file_parser parser(file);
        pass1(parser);
        pass2(file);
        write_lisfile(file);
    }
    catch(file_parse_exception excpt) {
        cout << excpt.getMessage() << endl;
    }
    catch(opcode_error_exception excpt){
        cout << excpt.getMessage() << endl;
    }
    catch(symtab_exception excpt){
        cout << excpt.getMessage() << endl;
    }
}

sicxe_asm::~sicxe_asm(void){};

void sicxe_asm::pass1(file_parser parser){
    parser.read_file();
    
    unsigned int lineNo=0;
    unsigned int curAddr = 0; //curAddr in DEC
    bool foundEnd = false; //Avoids parsing file past END directive
    
    while(!foundEnd){
        struct line temp;
        temp.lineNo = lineNo+1;
        temp.address = itoh(curAddr,5);
        temp.label = parser.get_token(lineNo,0);
        temp.opcode = parser.get_token(lineNo,1);
        temp.operand = parser.get_token(lineNo,2);
        temp.comment = parser.get_token(lineNo,3);
        
        if(strcmp(temp.opcode,"LDX")) hasx_reg = true;
        
        if(!(temp.opcode.empty())){ 
            getDir(lineNo, temp.label, temp.opcode, temp.operand, curAddr, ocTab, symTab);
            if(strcmp(temp.opcode,"END")) foundEnd = true;
        }
        if(!temp.label.empty() && !strcmp(temp.opcode,"EQU")) symTab.insert(temp.label,temp.address);
       
        lis.push_back(temp);
        lineNo++;
    }
    
    if(!isValidStart(lis))
        throw opcode_error_exception("Only blank lines and comments may occur before the START directive.");  
}

////////////////////////////////////////////////////////////////////////////////

void sicxe_asm::getDir(unsigned int lineNo, string label, string opcode, string operand, unsigned int& curAddr, opcodetab ocTab, symtab symTab){
    if(strcmp(opcode,"START")) handleDir_start(lineNo, operand, curAddr);
    else if(strcmp(opcode,"BYTE")) handleDir_byte(lineNo, operand, curAddr);  
    else if(strcmp(opcode,"WORD")) handleDir_word(lineNo, label,operand, curAddr, symTab);
    else if(strcmp(opcode,"RESB")) handleDir_resb(lineNo, operand, curAddr);
    else if(strcmp(opcode,"RESW")) handleDir_resw(lineNo, operand, curAddr);
    else if(strcmp(opcode,"BASE")) handleDir_base(operand);
    else if(strcmp(opcode,"NOBASE")) handleDir_nobase();
    else if(strcmp(opcode,"EQU")) handleDir_equ(label, operand);
    else if(!opcode.empty() && !strcmp(opcode,"END")){
        curAddr+= ocTab.get_instruction_size(opcode);
    }
    else return;
}

void sicxe_asm::handleDir_start(unsigned int lineNo, string operand, unsigned int& curAddr){ 
    if(isDec(operand)) curAddr += stoi(operand);
    else if(operand.at(0)=='#' && operand.at(1)=='$'){
        operand.erase(0,2);
        curAddr = htoi(operand);
    }
    else if(operand.at(0)=='#'){
        operand.erase(0,1);
        curAddr += stoi(operand);
    }
    else if(operand.at(0)=='$'){
        operand.erase(0,1);
        curAddr = htoi(operand);
    }
    else eHelper("Invalid operand used with START directive: " + operand, lineNo);
}

void sicxe_asm::handleDir_byte(unsigned int lineNo, string operand, unsigned int& curAddr){
    if(toupper(operand.at(0))=='C') curAddr+= (operand.length()-3);
    else if(toupper(operand.at(0))=='X' && isHex(operand)){
        if((operand.length()-3)%2 != 0) 
            eHelper("Invalid hexadecimal value used with BYTE directive: " + operand, lineNo);
        else curAddr+= ((operand.length()-3)/2);
    }
    else eHelper("Invalid operand used with BYTE directive: " + operand, lineNo);
}

void sicxe_asm::handleDir_word(unsigned int lineNo, string label, string operand, unsigned int& curAddr, symtab symTab){
    if(isalpha(operand.at(0)) && !symTab.contains(operand)){
        eHelper("Invalid operand used with WORD directive: " + operand, lineNo);
    }
    else if(operand.at(0)=='$' && operand.size()>7)
        eHelper("Invalid hexadecimal value used with WORD directive: " + operand, lineNo);
    curAddr+= 3;
}

void sicxe_asm::handleDir_resb(unsigned int lineNo, string operand, unsigned int& curAddr){
    if(isDec(operand)){
        curAddr += stoi(operand);
    }
    else if(operand.at(0)=='$'){
        operand.erase(0,1);
        curAddr += htoi(operand);
    }  
    else eHelper("Invalid operand used with RESB directive: " + operand, lineNo);
}

void sicxe_asm::handleDir_resw(unsigned int lineNo, string operand, unsigned int& curAddr){
    if(isDec(operand)){
        curAddr += stoi(operand)*3;
    }
    else if(operand.at(0)=='$'){
        operand.erase(0,1);
        curAddr += htoi(operand)*3;
    }
    else eHelper("Invalid operand used with RESW directive: " + operand, lineNo);
}

void sicxe_asm::handleDir_base(string operand){
    base_reg = operand;
    return;
}
void sicxe_asm::handleDir_nobase(){
    base_reg = "";
    return;
}

void sicxe_asm::handleDir_equ(string label, string operand){
    if(isDec(operand)){
        int val = stoi(operand);
        symTab.insert(label,itoh(val,5));
    }
    else if(operand.at(0)=='#' && operand.at(1)=='$'){
        operand.erase(0,2);
        symTab.insert(label,operand);
    }
    else if(operand.at(0)=='#'){
        operand.erase(0,1);
        int val = stoi(operand);
        symTab.insert(label,itoh(val,5));
    }
    else if(operand.at(0)=='$'){
        operand.erase(0,1);
        symTab.insert(label,operand);
    } 
    else symTab.insert(label,operand);
}

void sicxe_asm::write_lisfile(string file){
    string lisfile = file.substr(0,file.find_first_of(".")) + ".lis";
    
    ofstream ofs;
    ofs.open(lisfile.c_str(), ios::out);
    
    ofs << right << setw(12) << "**" << file << "**" << endl;
    ofs << "Line#     Address     Label        Opcode       Operand   Machine Code\n"
        << "=====     =======     =====        ======       =======   ============" << endl;
    
    for(unsigned int i=0; i<lis.size(); i++){
        ofs << right << setw(5) << lis[i].lineNo << right << setw(12)
            << lis[i].address << "     " << left << setw(13) << lis[i].label 
            << left << setw(13) << lis[i].opcode << left << setw(10)
            << lis[i].operand << lis[i].machinecode << endl;
    }
    ofs.close();
}

bool sicxe_asm::isValidStart(vector<line> lis){
    bool found_start = false;
    vector<line>::iterator it = lis.begin();
    for(it; it!=lis.end(); it++){ 
        if(strcmp(it->opcode,"START")) found_start = true;
        if(((!(it->label).empty() || !(it->opcode).empty() || !(it->operand).empty())) 
                && found_start==false)
            return false;
        if(found_start) break; //Avoids iterating through entire vector
    }
    
    return true;
}

void sicxe_asm::eHelper(string msg, unsigned int index){
    stringstream sInt;
    sInt << index;
    string lineNum = sInt.str();
    throw opcode_error_exception("***Error on line "+lineNum+": "+msg);
}

////////////////////////////////////////////////////////////////////////////////

void sicxe_asm::pass2(string file){
    symTab.handle_fr(); 
    for(unsigned int i=0; i<lis.size(); i++){
        string opcode = lis[i].opcode;
        if(!opcode.empty()) lis[i].machinecode = get_machinecode(i); 
        else lis[i].machinecode = "";
    }
}

string sicxe_asm::get_machinecode(int lineNo){
    string label = lis[lineNo].label;
    string opcode = lis[lineNo].opcode;
    string operand = lis[lineNo].operand;
    
    if(isDir(opcode)) return "";
    
    if(strcmp(opcode, "BYTE")){
        if(toupper(operand.at(0))=='C')
            return stoascii(operand.substr(2,operand.size()-3));
        
        else if(toupper(operand.at(0))=='X'){
            string tmp = operand.substr(2,operand.size()-3);
            transform(tmp.begin(), tmp.end(), tmp.begin(), ::toupper);
            return tmp;
        }
    } 
    
    if(strcmp(opcode, "WORD")){
        if(operand.at(0)=='$'){
            operand.erase(0,1);
            stringstream ss;
            ss << setfill('0') << setw(6) << operand;
            return ss.str();
        }
        else{   
            int val = stoi(operand);
            string tmp = itoh(val,6);
            return tmp;
        }
    }
    
    else{
        int instrSize = ocTab.get_instruction_size(opcode);
        switch(instrSize){
            case(1): return ocTab.get_machine_code(opcode);
            case(2): return f2_instr(lineNo, opcode, operand);
            case(3): return f3_instr(lineNo, label, opcode, operand);
            case(4): return f4_instr(lineNo, label, opcode, operand);
        }
    }
    
    return "";
}

string sicxe_asm::f2_instr(unsigned int lineNo, string opcode, string operand){
    if(!isValidf2_instr(opcode,operand))
        eHelper("Invalid format 2 instruction: " +opcode+" "+operand, lineNo+1);
    string tmp = ocTab.get_machine_code(opcode);
    
    if(strcmp(opcode,"SHIFTL") || strcmp(opcode,"SHIFTR")){        
        string r1 = operand.substr(0,1);
        tmp = get_reg(lineNo, tmp, r1);
        
        int n = stoi(operand.substr(2,operand.size())) - 1; //r2 = n-1
        if(n+1>15) eHelper("Bit shift cannot be greater than 15", lineNo+1);
        string r2 = itoh(n,1);        
        tmp.append(r2);
        return tmp;
    } 
    
    else if(strcmp(opcode,"SVC")){
        if(!isDec(operand))
            eHelper("Invalid operand used with SVC instruction", lineNo+1);        
        int n =stoi(operand.substr(0,operand.size()));
        if(n>15) eHelper("r1 cannot be greater than 15", lineNo+1);
        tmp.append(itoh(n,1));
    }
    
    else tmp = get_reg(lineNo, tmp, operand);
    
    while(tmp.size()!=4) tmp.append("0");
    return tmp;
}

string sicxe_asm::f3_instr(unsigned int lineNo, string label, string opcode, string operand){
    if(strcmp(opcode,"RSUB")) return "4F0000";
    
    string tmp = ocTab.get_machine_code(opcode);
   
    indexed = false;
    if(operand.find(',') != operand.npos) {
        indexed = true;
        nixbpe[2] = 1; 
    }
    
    if(operand.at(0)=='#'){        
        nixbpe[0] = 0;
        nixbpe[1] = 1;
        
        operand.erase(0,1);
        tmp.append(sec_mc(tmp,1));
        
        if(isDec(operand)){
            tmp.append("0");
            tmp.append(itoh(stoi(operand.substr(0,operand.length())),3));
        }
        else{
            get_addrMode(tmp,operand,lineNo);
        }
    }
    
    else if(operand.at(0)=='@'){
        nixbpe[0] = 1;
        nixbpe[1] = 0;
        
        operand.erase(0,1);
        tmp.append(sec_mc(tmp,2));
        
        if(isDec(operand)){
            tmp.append("0");
            tmp.append(itoh(stoi(operand.substr(0,operand.length())),3));
        }
        else{
            get_addrMode(tmp,operand,lineNo);
        }
    }
    
    else{
        nixbpe[0] = nixbpe[1] = 1;
        
        tmp.append(sec_mc(tmp,3));
        
        if(isDec(operand)){
            if(indexed) tmp.append("8");
            else tmp.append("0");
            tmp.append(itoh(stoi(operand.substr(0,operand.length())),3));            
        }
        else{
            get_addrMode(tmp,operand,lineNo);
        }
    }
   
    if(!isValidBitFlags(nixbpe)) 
        eHelper("Invalid machine instruction!", lineNo+1);
    
    memset(nixbpe,0,sizeof(nixbpe));
        
    tmp.erase(0,2);
    return tmp;
}

string sicxe_asm::f4_instr(unsigned int lineNo, string label, string opcode, string operand){
    if(strcmp(opcode,"+RSUB")) return "4F0000";
    
    string tmp = ocTab.get_machine_code(opcode);
    nixbpe[5] = 1;
    
    indexed = false;
    if(operand.find(',') != operand.npos) {
        indexed = true;
        nixbpe[2] = 1; 
    }
        
    if(operand.at(0)=='#'){
        nixbpe[0] = 0;
        nixbpe[1] = 1;
        
        operand.erase(0,1);
        tmp.append(sec_mc(tmp,1));
        
        tmp.append("1"); 
        
        if(isDec(operand)){
            //m indicates memory addr or const value > 4095
            if(stoi(operand)<=4095)
                eHelper("Invalid operand used with format 4 instruction: "+operand,lineNo+1);
            tmp.append(itoh(stoi(operand.substr(0,operand.length())),5));
        }
        else{
            tmp.append(symTab.get_value(operand));
        }
    }
    
    else if(operand.at(0)=='@'){
        nixbpe[0] = 1;
        nixbpe[1] = 0;
        
        operand.erase(0,1);
        tmp.append(sec_mc(tmp,2));
        
        tmp.append("1");
        
        if(isDec(operand)){
            //m indicates memory addr or const value > 4095
            if(stoi(operand)<=4095)
                eHelper("Invalid operand used with format 4 instruction: "+operand,lineNo+1);
            tmp.append(itoh(stoi(operand.substr(0,operand.length())),5));
        }
        else{
            tmp.append(symTab.get_value(operand));
        }
    }
    
    else{
        nixbpe[0] = nixbpe[1] = 1;
        
        tmp.append(sec_mc(tmp,3));
        
        if(isDec(operand)){
            //m indicates memory addr or const value > 4095
            if(stoi(operand)<=4095)
                eHelper("Invalid operand used with format 4 instruction: "+operand,lineNo+1);
            if(indexed) tmp.append("9");
            else tmp.append("1");
            tmp.append(itoh(stoi(operand.substr(0,operand.length())),5));            
        }
        else{
            if(indexed) tmp.append("9");
            else tmp.append("1");
            
            tmp.append(symTab.get_value(operand.substr(0,operand.find(','))));
        }
    }  
    
    if(!isValidBitFlags(nixbpe)) 
        eHelper("Invalid machine instruction!", lineNo+1);
    
    memset(nixbpe,0,sizeof(nixbpe));
    
    tmp.erase(0,2);
    return tmp;
}

string sicxe_asm::sec_mc(string tmp, int addrType){
    string sec = tmp.substr(1,1);           // addrType:
    int code = htoi(sec) + addrType;        // 1 = immediate (#)
    sec = itoh(code,1);                     // 2 = indirect (@)
    tmp.erase(1,1);                         // 3 = simple
    tmp.append(sec);
    
    return tmp;
}

void sicxe_asm::get_addrMode(string& tmp, string operand, int lineNo){
    string dest = symTab.get_value(operand.substr(0,operand.find(',')));
    int disp = htoi(dest) - (htoi(lis[lineNo].address)+3);
    
    //PC relative addr
    if(-2048 <= disp && disp <= 2047){
        nixbpe[3] = 0;
        nixbpe[4] = 1;
        
        if(indexed) tmp.append("A");
        else tmp.append("2");
        tmp.append(itoh(disp,3));
    }
    //BASE relative addr
    else if(0 <= disp && disp <= 4095){
        nixbpe[3] = 1;
        nixbpe[4] = 0;
        
        if(indexed) tmp.append("C");
        else tmp.append("4");
        disp = htoi(dest) - htoi(symTab.get_value(base_reg));
        //if(lineNo==20) cout << htoi(dest) << " - " << htoi(symTab.get_value(base_reg));
        //if(disp==0) eHelper("Invalid BASE register value used for addressing", lineNo+1);
        tmp.append(itoh(disp,3));
        
    }
    else{
        eHelper("Addressing disp " +itos(disp)+" out of bounds!", lineNo+1);
    }
}

string sicxe_asm::get_reg(unsigned int lineNo, string tmp, string operand){
    for(unsigned int i=0; i<operand.size(); i++){
        switch(toupper(operand.at(i))){
            case 'A': 
                tmp.append("0");
                break;
            case 'X': 
                tmp.append("1");
                break;
            case 'L': 
                tmp.append("2");
                break;
            case 'B': 
                tmp.append("3");
                break;
            case 'S': 
                tmp.append("4");
                break;
            case 'T': 
                tmp.append("5");
                break;
            case 'F': 
                tmp.append("6");
                break;
            case '*': 
                tmp.append("7");
                break;
            case 'P': break;
            case 'C': 
                if(operand.at(i-1)!='P') eHelper("Invalid register used", lineNo+1); 
                tmp.append("8");
                break;
            case 'W': 
                if(operand.at(i-1)!='S') eHelper("Invalid register used", lineNo+1);  
                tmp.erase(i,1);
                tmp.append("9");
                break;
            case ',': break;
            default:
                eHelper("Invalid register used", lineNo+1);            
        }
    }
    return tmp;
}

bool sicxe_asm::isValidBitFlags(int f[]){
    int n = f[0];
    int i = f[1];
    int x = f[2];
    int b = f[3];
    int p = f[4];
    int e = f[5];
    
    if(x==1 && (n!=1 || i!=1) || e==1 && (b!=0 || p!=0) || (b==1 && p==1))
        return false;  
    else if(x==1 && !hasx_reg) return false;
    return true;
}

bool sicxe_asm::isDir(string opcode){
    for(unsigned int i=0; i<7; i++){
        if(strcmp(opcode,dir[i])) return true;
    }
    return false;
}

bool sicxe_asm::isValidf2_instr(string opcode, string operand){
    if(operand.empty()) return false;
    else if((strcmp(opcode,"CLEAR") || strcmp(opcode,"SVC") || strcmp(opcode,"TIXR"))
            && operand.find(',')!=operand.npos) return false;
    else if((!strcmp(opcode,"CLEAR") && !strcmp(opcode,"SVC") && !strcmp(opcode,"TIXR"))
            && operand.find(',')==operand.npos) return false;
    
    return true;
}

string sicxe_asm::stoascii(string operand){
    stringstream ss;
    for(unsigned int i=0; i<operand.size();i++){
        ss << hex << (int)operand[i];
    }
    string tmp = ss.str();
    transform(tmp.begin(), tmp.end(), tmp.begin(), ::toupper);
    return tmp;
}

string sicxe_asm::itoh(int a, int width){
    string tmp;
    stringstream ss;
    ss << setfill('0') << setw(width) << hex << a;
    tmp = ss.str();
    transform(tmp.begin(), tmp.end(), tmp.begin(), ::toupper);
    if(a<0) tmp.erase(0,5); 
    return tmp;
}

string sicxe_asm::itos(int a){
    stringstream ss;
    ss << a;
    return ss.str();
}

int sicxe_asm::stoi(string a){
    int x;
    stringstream ss(a);
    ss >> x;
    return x;
}

int sicxe_asm::htoi(string a){
    int x;
    stringstream stream(a);
    stream >> hex >> x;
    return x;    
}

////////////////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[]){
    if(argc != 2) {
        cout << "***Error, you must supply the name of the file " <<
        "to process at the command line." << endl;
        exit(1);
    }
    new sicxe_asm(argv[1]);
    return 0;
}