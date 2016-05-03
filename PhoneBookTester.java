import java.util.Iterator;
import java.util.ConcurrentModificationException;
import data_structures.*;


public class PhoneBookTester {
    public static void main(String [] args){
        PhoneBook p = new PhoneBook(100000);
        p.load("p4_data.txt");
        System.out.println(p.nameLookup("Wilson, Winnie"));
        
        
    }
   
    
}
