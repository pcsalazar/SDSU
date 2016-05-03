/*  Carlo Alejandro Salazar
    masc0381
*/

import data_structures.*;
import java.util.Iterator;
import java.io.*;

public class PhoneBook {
    private Hashtable directory;
    //private BinarySearchTree BST;
    //private BalancedTree BT;
   
    public PhoneBook(int maxSize){
        directory = new Hashtable<PhoneNumber,String>(maxSize);
    }          
                  
    public void load(String filename){
         String line;
         try{
             BufferedReader in = new BufferedReader(new FileReader(filename));
             while((line=in.readLine())!=null){
                 PhoneNumber key = new PhoneNumber(line.substring(0,12));
                 String value = line.substring(13);
                 if(!addEntry(key, value))
                     throw new RuntimeException("Error, duplicate");
             }
         }
        catch(FileNotFoundException e){
             e.printStackTrace();
         } catch(IOException e){
             e.printStackTrace();
         };         
    }
     
    public String numberLookup(PhoneNumber number){ 
        return (String)directory.getValue(number);
    }
     
    public PhoneNumber nameLookup(String name){
        return (PhoneNumber)directory.getKey(name);
    }
           
    public boolean addEntry(PhoneNumber number, String name){
        return directory.add(number, name);
    }
        
    public boolean deleteEntry(PhoneNumber number){
        return directory.delete(number);        
    }
       
    public void printAll(){
        Iterator<PhoneNumber> keys = directory.keys();
        Iterator<String> values = directory.values();
        
        while(keys.hasNext()){
            System.out.println(keys.next() + " " + values.next());
        }
    }
   
    public void printByAreaCode(String code){
        Iterator<PhoneNumber> keys = directory.keys();
        while(keys.hasNext()){
            PhoneNumber n = keys.next();
            String p = n.areaCode;
            if(((Comparable<String>)p).compareTo(code)==0)
                System.out.println(n);
        }
    }       
       
    public void printNames(){
        int size=0;
        Iterator<PhoneNumber> keys = directory.keys();
        Iterator<String> values = directory.values();
        
        while(keys.hasNext()){
            keys.next();
            size++;            
        }
        String[] names = new String[size];
        for(int i=0; i<size; i++){
            names[i] = values.next();
        }
        shellSort(names);
        
        for(int i=0; i<names.length; i++){
            System.out.println(names[i]);
        }   
    }
    
    private String[] shellSort(String[] array){
        String[] n = array;
        int in,out,h=1;
        int size=array.length;
        String temp;
        
        while(h<=size/3)
            h=h*3+1;
        while(h>0){
            for(out=h; out<size; out++){
                temp=n[out];
                in=out;
                while(in>h -1 && ((Comparable<String>) n[in-h]).compareTo(temp)>=0){
                    n[in] = n[in-h];
                    in -= h;
                }
                n[in]=temp;
            }
            h=(h-1)/3;
        }        
        return n;
    }
}