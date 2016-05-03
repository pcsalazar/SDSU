/*  Carlo Alejandro Salazar
    masc0381
*/

package data_structures;

import java.util.TreeMap;
import java.util.Iterator;
import java.util.NoSuchElementException;
import java.util.ConcurrentModificationException; 
import java.util.Map.Entry;

public class BalancedTree<K,V> implements DictionaryADT<K,V> {
    TreeMap<K,V> TreeMap;
    
    public BalancedTree(){
        TreeMap=new TreeMap<K,V>();
    }
    
    public boolean contains(K key){
        return TreeMap.containsKey(key);
    }
    
    public boolean add(K key, V value){
        return TreeMap.put(key,value)==null;
    }
    
    public boolean delete(K key){
        if(TreeMap.remove(key)==null) return false;
        return true;
    }
    
    public V getValue(K key){
        return TreeMap.get(key);
    }
    
    public K getKey(V value){
        for(Entry<K,V> n : TreeMap.entrySet())
            if(((Comparable<V>)value).compareTo(n.getValue())==0)
                return n.getKey();            
        return null;
    }
    
    public int size(){
        return TreeMap.size();
    }
    
    public boolean isFull(){
        return false;
    }
    
    public boolean isEmpty(){
        return TreeMap.isEmpty();
    }
    
    public void clear(){
        TreeMap.clear();
    }
    
    public Iterator<K> keys(){
        return TreeMap.keySet().iterator();
    }
    
    public Iterator<V> values(){
        return TreeMap.values().iterator();
    }
}
