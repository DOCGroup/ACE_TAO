package imaging.filters;

/**
 * A simple assertion mechanism for asserting validity of
 * arguments.<p>
 *
 * @version 1.0, Apr 1 1996
 * @author  David Geary
 */
class Assert {
    static public void notFalse(boolean b) 
                       throws IllegalArgumentException {
        if(b == false) 
            throw new IllegalArgumentException(
                            "boolean expression false");
    }
    static public void notNull(Object obj) 
                       throws IllegalArgumentException {
        if(obj == null) 
            throw new IllegalArgumentException("null argument");
    }

    static public void notFalse(boolean b, String s) 
                               throws IllegalArgumentException {
        if(b == false)
            throw new IllegalArgumentException(s);
    }
    static public void notNull(Object obj, String s) 
                               throws IllegalArgumentException {
        if(obj == null) 
            throw new IllegalArgumentException(s);
    }
}
