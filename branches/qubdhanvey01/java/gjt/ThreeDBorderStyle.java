package gjt;

/**
 * Constants for 3D border styles.
 *
 * This class may not be instantiated.
 *
 * @version 1.0, Apr 11 1996
 * @author  David Geary
 */
public class ThreeDBorderStyle {
    public static final ThreeDBorderStyle RAISED = 
                            new ThreeDBorderStyle();
    public static final ThreeDBorderStyle INSET  = 
                            new ThreeDBorderStyle();

    public String toString() {
        if(this == ThreeDBorderStyle.RAISED) 
            return getClass().getName() + "=RAISED";
        else                      
            return getClass().getName() + "=INSET";
    }
    private ThreeDBorderStyle() { }  // defeat instantiation
}
