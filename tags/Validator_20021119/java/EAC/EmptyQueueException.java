/**
 *  Title:        EmptyQueueException
 *  Description:  An exception class for when an attempt is made to access
 *                the head of the event queue when the queue is empty
 */
package EAC;

public class EmptyQueueException extends Exception {

  public EmptyQueueException(String msg) {
    super(msg);
  }
}