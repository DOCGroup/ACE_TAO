/**
 *  Title:        TooManyPrimitivesException
 *  Description:  An exception class for when an attempt is made to add a
 *                primitives when the maximum number have already been added
 */
package EAC;

public class TooManyPrimitivesException extends Exception {

  public TooManyPrimitivesException(String msg) {
    super(msg);
  }
}