package gjt;

/**
 * Constants for Etching.
 *
 * This class may not be instantiated.
 *
 * @version 1.0, Apr 11 1996
 * @author  David Geary
 */
public class Etching {
    public static final Etching OUT = new Etching();
    public static final Etching IN  = new Etching();

    public String toString() {
        if(this == Etching.OUT) 
            return getClass().getName() + "=OUT";
        else                    
            return getClass().getName() + "=IN";
    }
    private Etching() { }
}
