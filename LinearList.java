/*  Carlo Alejandro Salazar
    masc0381
*/

package data_structures;

import java.util.Iterator;
import java.util.NoSuchElementException;

public class LinearList<E> implements LinearListADT<E> {   
    private int currentSize = 0;
    private Node<E> head,tail;
    
    public class Node<E>{
        E data;
        Node<E> previous;
        Node<E> next;
        
        public Node(E data){
            this.data = data;
            previous = next = null;
        }
    }
    
    public boolean addFirst(E obj){ 
        Node<E> newNode = new Node<E>(obj);
        if(head==null){head=tail=newNode;}  //If list is empty
        else{
            newNode.next = head;
            head.previous = newNode;
            head = newNode;
        }
        currentSize++;
        return true;
    }
     
    public boolean addLast(E obj){  
        Node<E> newNode = new Node<E>(obj);
        if(head==null){head=tail=newNode;}  //If list is empty
        else{
            tail.next = newNode;
            newNode.previous = tail;
            tail = newNode;
        }
        currentSize++;
        return true;
    }   
    
    public E removeFirst(){ 
        if(head==null){return null;}    //If list is empty
        E tmp = head.data;
        head = head.next;
        if(head==null){tail=null;}  //If only 1 element in list is removed
        else{
            head.previous=null;
        }
        currentSize--;
        return tmp;
    } 
    
    public E removeLast(){ 
        if(tail==null){return null;}
        E tmp = tail.data;
        tail = tail.previous;
        if(tail==null){head=null;}
        else{
            tail.next=null;
        }
        currentSize--;
        return tmp;
    }  
    
    public E remove(E obj){ 
        Node<E> prev=null,cur=head;
        while(cur!=null && ((Comparable<E>)obj).compareTo(cur.data)!=0){    //Finds node to remove
            prev=cur;
            cur=cur.next;
            //If cur!=null, then node was found
        }
        if(cur==null){return null;} //If empty
        if(cur==head){return removeFirst();}    //First node will be removed
        else if(cur==tail){return removeLast();}    //Last node will be removed
        else{   //Node to remove is in middle of list
            prev.next=cur.next; 
        }
        currentSize--;
        return cur.data;
    }
    
    public E peekFirst(){
        if(head==null){return null;}
        return head.data;
    }
    
    public E peekLast(){
        if(head==null){return null;}
        return tail.data;
    }                       

    public boolean contains(E obj){
        Node<E> tmp = head;
        while(tmp!=null){
            if(((Comparable<E>)tmp.data).compareTo(obj)==0){return true;}   //Searches for node
            tmp=tmp.next;
        }
        return false;
    }
    
    public E find(E obj){
        Node<E> tmp = head;
        while(tmp!=null){
            if(((Comparable<E>)tmp.data).compareTo(obj)==0){return tmp.data;}   //Searches for node
            tmp=tmp.next;
        }
        return null;
    }       

    public void clear(){
        head = tail = null;
        currentSize=0;
    }
   
    public boolean isEmpty(){
        return head==null;
    }
    
    public boolean isFull(){
        return false;
    }  

    public int size(){
        return currentSize;
    }
    
    public Iterator<E> iterator(){  
        return new IteratorHelper();    
    }
    class IteratorHelper implements Iterator<E>{
        Node<E> pointer;
        
        public IteratorHelper(){
            pointer = (Node<E>)head;
        }
        
        public boolean hasNext() {
            return pointer != null;
        }
        
        public E next() {   
            if(!hasNext()){throw new NoSuchElementException();}
            
            E tmp = pointer.data;
            pointer = pointer.next;
            return tmp;
        }
        public void remove(){
            throw new UnsupportedOperationException();
        }
    }      
}
