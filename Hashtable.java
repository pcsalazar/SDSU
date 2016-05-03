/*  Carlo Alejandro Salazar
    masc0381
*/

package data_structures;

import java.util.Iterator;
import java.util.NoSuchElementException;
import java.util.ConcurrentModificationException; 

public class Hashtable<K,V> implements DictionaryADT<K,V> {    
    private LinearListADT<Wrapper<K,V>>[] list;
    private int currentSize,maxSize,tableSize,modCounter;
    
    public Hashtable(int n){
        maxSize=n;
        tableSize=(int)(maxSize*1.3f);
        currentSize=0;
        modCounter=0;
        list = new LinearList[tableSize];   
        for(int i=0; i<tableSize; i++){
            list[i] = new LinearList<Wrapper<K,V>>();
        }
    }
    
    private class Wrapper<K,V> implements Comparable<Wrapper<K,V>>{
        K key;
        V value;
        
        public Wrapper(K key, V value){
            this.key=key;
            this.value=value;
        }
        
        public int compareTo(Wrapper<K,V> node){
            return ((Comparable<K>)key).compareTo((K)node.key);
        }
    }
    
    private int hashCode(K key){    
        return (key.hashCode() & 0x7FFFFFFF ) % tableSize;
    }
    
    public boolean contains(K key){
        return list[hashCode(key)].contains(new Wrapper<K,V>(key,null)); 
    }

    public boolean add(K key, V value){
        if(currentSize==maxSize) return false;
        if(list[hashCode(key)].contains(new Wrapper<K,V>(key,null))) return false;
        list[hashCode(key)].addLast(new Wrapper<K,V>(key,value));
        currentSize++;
        modCounter++;
        return true;
    }

    public boolean delete(K key){
        if(contains(key)){
            list[hashCode(key)].remove(new Wrapper<K,V>(key,null));
            currentSize--;
            modCounter++;
            return true;
        }           
        modCounter++;
        return false;
    }

    public V getValue(K key){
        Wrapper<K,V> temp = list[hashCode(key)].find(new Wrapper<K,V>(key,null));
        if(temp==null) return null;
        return temp.value;
    }

    public K getKey(V value){
        for(int i=0; i<tableSize; i++){
            for(Wrapper<K,V> w : list[i]){
                if(((Comparable<V>)value).compareTo(w.value)==0) return w.key;
            }
        }
        return null;
    }

    public int size(){
        return currentSize;
    }

    public boolean isFull(){
        if(currentSize==maxSize) return true;
        return false;
    }

    public boolean isEmpty(){
        if(currentSize==0) return true;
        return false;
    }

    public void clear(){
        for(int i=0; i<tableSize; i++){
            list[i].clear();
        }
        currentSize=0;
        modCounter=0;
    }

    public Iterator<K> keys(){
        return new KeyIteratorHelper<K>();
    }

    public Iterator<V> values(){
        return new ValueIteratorHelper<V>();
    }
    
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    abstract class IteratorHelper<E> implements Iterator<E>{
        protected Wrapper<K,V> [] nodes;
        protected int index;
        protected long modCheck;
        
        public IteratorHelper(){
            nodes = new Wrapper[currentSize];
            index=0;
            int j=0;
            modCheck=modCounter;
            for(int i=0; i<tableSize; i++)
                for(Wrapper n : list[i]) 
                    nodes[j++]=n;
            nodes = (Wrapper<K,V>[]) shellSort(nodes);            
        }
        
        public boolean hasNext(){
            if(modCheck!=modCounter) throw new ConcurrentModificationException();
            return index < currentSize;
        }
        
        public abstract E next();
        
        public void remove(){
            throw new UnsupportedOperationException();
        }
        
        private Wrapper<K,V>[] shellSort(Wrapper<K,V>[] array){
        Wrapper<K,V>[] n = array;
        int in,out,h=1;
        int size=array.length;
        Wrapper<K,V> temp;
        
        while(h<=size/3)
            h=h*3+1;
        while(h>0){
            for(out=h; out<size; out++){
                temp=n[out];
                in=out;
                while(in>h -1 && ((Comparable<K>) n[in-h].key).compareTo(temp.key)>=0){
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
    
    class KeyIteratorHelper<K> extends IteratorHelper<K>{
        public KeyIteratorHelper(){
            super();
        }
        public K next(){
            return (K) nodes[index++].key;
        }        
    }
    
    class ValueIteratorHelper<V> extends IteratorHelper<V>{
        public ValueIteratorHelper(){
            super();
        }
        public V next(){
            return (V) nodes[index++].value;
        }
    }
}
