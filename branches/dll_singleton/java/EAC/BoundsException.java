/**
 *  Title:        BoundsException
 *  Description:  An exception class for when graphical objects are positioned
 *                partially out of their container's boundaries
 */
package EAC;

public class BoundsException extends Exception {

  public BoundsException(String msg) {
    super(msg);
  }
}