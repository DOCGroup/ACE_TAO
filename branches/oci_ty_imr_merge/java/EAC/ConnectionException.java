/**
 *  Title:        ConnectionException
 *  Description:  An exception class for when an attempt is made to connect
 *                an input where one is already connected, or to add an
 *                output when the maximum number of outputs are already set
 */
package EAC;

public class ConnectionException extends Exception {

  public ConnectionException(String msg) {
    super(msg);
  }
}