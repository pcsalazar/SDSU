/*  Carlo Alejandro Salazar
    masc0381
*/

public class GenericSorter<E> {    
    public static <E> E[] insertionSort(E[] array){
        E[] n = array;
        int in, out;
        E temp;
        for(out=1; out<n.length; out++){
            temp=n[out];
            in=out;
            while(in>0 && ((Comparable<E>)n[in-1]).compareTo(temp)>0){
                n[in] = n[in-1];
                in--;
            }
            n[in]=temp;
        }
        return n;
    }
    
    public static <E> E[] shellSort(E[] array){
        E[] n = array;
        int in,out,h=1;
        int size=array.length;
        E temp;
        
        while(h<=size/3)
            h=h*3+1;
        while(h>0){
            for(out=h; out<size; out++){
                temp=n[out];
                in=out;
                while(in>h -1 && ((Comparable<E>) n[in-h]).compareTo(temp)>=0){
                    n[in] = n[in-h];
                    in -= h;
                }
                n[in]=temp;
            }
            h=(h-1)/3;
        }        
        return n;
    }
    
    public static <E> E[] quickSort(E[] array){
        E[] n = array;
        quickSort(n, 0, n.length-1);
        return n;
    }

    private static <E> void quickSort(E[] array, int left, int right){
        E[] n = array;
        int lPtr = left;
        int rPtr = right;
        E pivot = n[left + (right-left)/2];
        
        while(lPtr <= rPtr){
            while(((Comparable<E>)n[lPtr]).compareTo(pivot)<0)
                lPtr++;
            
            while(((Comparable<E>)n[rPtr]).compareTo(pivot)>0)
                rPtr--;
            
            if(lPtr <= rPtr){
                swap(n, lPtr, rPtr);
                lPtr++;
                rPtr--;
            }
        }
        
        if(left < rPtr)
            quickSort(n, left, rPtr);
        if(lPtr < right)
            quickSort(n,lPtr, right);   
    }

    private static <E> void swap(E[] array, int one, int two){
        E[] n = array;
        E temp = n[one];
        n[one] = n[two];
        n[two] = temp;
    }

    
    
        
    
    
        
    
}
