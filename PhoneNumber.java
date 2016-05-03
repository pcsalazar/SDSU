/*  Carlo Alejandro Salazar
    masc0381
*/

import java.util.Iterator;
import data_structures.*;

public class PhoneNumber implements Comparable<PhoneNumber> {  
    String areaCode, prefix, number;
    String phoneNumber;
    
    public PhoneNumber(String n){
        verify(n);
        this.phoneNumber = n;
        this.areaCode = n.substring(0,3);
        this.prefix = n.substring(4,7);
        this.number = n.substring(8);        
    }
     
    public int compareTo(PhoneNumber n){   
        return phoneNumber.compareTo(n.phoneNumber);
    }
       
    public int hashCode(){  
        return phoneNumber.hashCode() ;
    }
   
    private void verify(String n){
        if(n.charAt(3)!='-' || n.charAt(7)!='-')
            throw new IllegalArgumentException();
        
        for(int i=0; i<3; i++){
            char c = n.charAt(i);
            if(!(c>='0' && c<='9') || !Character.isDigit(c)){
                throw new IllegalArgumentException();
            }
        }
        
        for(int i=4; i<7; i++){
            char c = n.charAt(i);
            if(!(c>='0' && c<='9') || !Character.isDigit(c)){
                throw new IllegalArgumentException();
            }
        }
        
        for(int i=8; i<12; i++){
            char c = n.charAt(i);
            if(!(c>='0' && c<='9') || !Character.isDigit(c)){
                throw new IllegalArgumentException();
            }
        }       
    }
       
    public String getAreaCode(){
        return areaCode;
    }
            
    public String getPrefix(){
        return prefix;
    }
       
    public String getNumber(){
        return number;
    }
    
    public String toString(){
        return phoneNumber;
    }
}            