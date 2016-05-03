public class GSTester {
    public static void main(String [] args){
        Person[] p = new Person[6];
        p[0] = new Person("e");
        p[1] = new Person("a");
        p[2] = new Person("d");
        p[3] = new Person("c");
        p[4] = new Person("b");
        p[5] = new Person("f");
        
        p=GenericSorter.insertionSort(p);
        //p = GenericSorter.shellSort(p);
        //p = GenericSorter.quickSort(p);
        
        for(int i=0; i<p.length; i++){
            System.out.println(p[i]);
        }
        
    }

    

    public static class Person implements Comparable<Person> {  
    String Person;
        public Person(String n){
            this.Person= n;
        }
        public int compareTo(Person n){   
            return Person.compareTo(n.Person);
        }      
        public String toString(){
            return Person;
        }
    }
        
    
}
