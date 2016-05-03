/*  Carlo Alejandro Salazar
    masc0381
*/

package data_structures;

import java.util.Iterator;
import java.util.NoSuchElementException;
import java.util.ConcurrentModificationException;

public class BinarySearchTree<K,V> implements DictionaryADT<K,V> { 
    private class Node<K,V>{
        private K key;
        private V value;
        private Node<K,V> leftChild, rightChild;
        
        public Node(K k, V v){
            key=k;
            value=v;
            leftChild=rightChild=null;
        }
    }
    
    private Node<K,V> root,parent;  
    private int currentSize,modCounter;
    private boolean wasLeft;
    private K setKey;
    
    public BinarySearchTree(){
        root=parent=null;
        currentSize=0;
        modCounter=0;
    }
    
    public boolean contains(K key){
        if(findValue(key,root)==null) return false;
        return true;
    }
    
    public boolean add(K key, V value){
        if(contains(key)) return false;
        if(root==null)
            root = new Node<K,V>(key,value);
        else
            insert(key,value,root,null,false);
        currentSize++;
        modCounter++;
        return true;
    }
    
    public boolean delete(K key){
        if(isEmpty()) return false;
        if(!contains(key)) return false;
        if(currentSize==1 && ((Comparable<K>)key).compareTo(root.key)==0) 
                root=null;
        else{
            Node<K,V> node = findNode(key,root);
            delete(node,parent);
        }
        currentSize--;
        modCounter++;
        return true;
    }
    
    public V getValue(K key){
        return findValue(key,root);
    }
    
    public K getKey(V value){
        setKey=null;
        findKey(value,root);
        return setKey;
    }
    
    public int size(){
        return currentSize;
    }
    
    public boolean isFull(){
        return false;
    }
    
    public boolean isEmpty(){
        if(currentSize==0) return true;
        return false;                
    }
    
    public void clear(){
        root=null;
        currentSize=0;
        modCounter=0;
    }
    
    public Iterator<K> keys(){
        return new KeyIteratorHelper();
    }
    
    public Iterator<V> values(){
        return new ValueIteratorHelper();
    }
    
    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    private V findValue(K key, Node<K,V> n){
        if(n==null) return null;
        if(((Comparable<K>)key).compareTo(n.key)<0)
            return findValue(key,n.leftChild);  //go left
        if(((Comparable<K>)key).compareTo(n.key)>0)
                return findValue(key,n.rightChild); //go right
        return (V) n.value;
    }
    
    private void findKey(V value, Node<K,V> n){
        if(n==null) return;
        if(((Comparable<V>)value).compareTo(n.value)==0){
            setKey=n.key;
            return;
        }   
        findKey(value,n.leftChild);
        findKey(value,n.rightChild);      
    }
    
    private void insert(K k, V v, Node<K,V> n, Node<K,V> parent, boolean wasLeft){
        if(n==null){
            if(wasLeft) parent.leftChild = new Node<K,V>(k,v);
            else parent.rightChild = new Node<K,V>(k,v);
        }
        else if(((Comparable<K>)k).compareTo((K)n.key)<0)   
            insert(k,v,n.leftChild,n,true);     //go left
        else                                                
            insert(k,v,n.rightChild,n,false);   //go right
    }
    
    private Node<K,V> findNode(K key, Node<K,V> n){
        if(n==null) return null;
        if(((Comparable<K>)key).compareTo(n.key)<0){
            wasLeft=true;
            parent=n;
            return findNode(key,n.leftChild);
        }
        if(((Comparable<K>)key).compareTo(n.key)>0){
            wasLeft=false;
            parent=n;
            return findNode(key,n.rightChild);
        }
        else
            return n;
    }
    
    private void delete(Node<K,V> n, Node<K,V> parent){ 
        //case 1: no child                              
        if(n.leftChild==null && n.rightChild==null){
            if(wasLeft) parent.leftChild=null; 
            else parent.rightChild=null;
        }     
        //case 2: one child
        else if(n.leftChild==null || n.rightChild==null){
            if(n==root){
                if(n.leftChild==null){
                    root=n.rightChild;
                    n.rightChild=null;
                }
                else{
                    root=n.leftChild;
                    n.leftChild=null;
                }
            }
            else if(n.leftChild==null && wasLeft){
                parent.leftChild=n.rightChild;
                n.rightChild=null;
            }
            else if(n.rightChild==null && wasLeft){
                parent.leftChild=n.leftChild;
                n.leftChild=null;
            }
            else if(n.leftChild==null && !wasLeft){
                parent.rightChild=n.rightChild;
                n.rightChild=null;
            }
            else{
                parent.rightChild=n.leftChild;
                n.leftChild=null;
            }            
        }
        //case 3: two children
        else{
            Node<K,V> rightNode,successor;
            rightNode=successor=n.rightChild;
            if(rightNode.leftChild==null){
                n.key=rightNode.key;
                n.value=rightNode.value;
                n.rightChild=rightNode.rightChild;
            }
            else{
                while(successor.leftChild!=null){
                    rightNode=successor;
                    successor=successor.leftChild;
                }
                n.key=successor.key;
                n.value=successor.value;
                rightNode.leftChild = 
                        (successor.rightChild!=null) ? successor.rightChild : null;
            }
        }
    }
        
    ////////////////////////////////////////////////////////////////////////////
    abstract class IteratorHelper<E> implements Iterator<E>{
        protected int index,builder,modCheck;
        Node<K,V>[] array;
        
        public IteratorHelper(){
            index=builder=0;
            modCheck=modCounter;
            array = new Node[currentSize];
            build(root);            
        }
        
        public boolean hasNext(){
            if(modCheck!=modCounter) 
                throw new ConcurrentModificationException();
            return index<currentSize;                
        }
        
        public void remove(){
            throw new UnsupportedOperationException();
        }
        
        private void build(Node<K,V> n){
            if(n!=null){
                build(n.leftChild);
                array[builder++]=n;
                build(n.rightChild);
            }
        }
    }
    
    class KeyIteratorHelper<K> extends IteratorHelper<K>{
        public KeyIteratorHelper(){
            super();
        }
        public K next(){
            if(!hasNext()) throw new NoSuchElementException();
            return (K) array[index++].key;
        }        
    }
    
    class ValueIteratorHelper<V> extends IteratorHelper<V>{
        public ValueIteratorHelper(){
            super();
        }
        public V next(){
            if(!hasNext()) throw new NoSuchElementException();
            return (V) array[index++].value;
        }
    }
}
