/**
 * Title:        Queue
 * Description:  The primary event queue for the Event Analysis Configurator
 */
package EAC;

public class Queue {

  private class Node {

    public Primitive primitive;
    public long time;
    public Node next;

    public Node(Primitive p, long t, Node n) {
      primitive = p;
      time = t;
      next = n;
    } /* constructor */

  } /* Node */

  private Node head;

  private int count;

  public Queue() {
    count = 0;
  } /* constructor */

  public boolean empty() {
    return (count == 0);
  } /* empty */

  public void clear() {
    while (!empty())
    try {
      dequeue();
    } catch (EmptyQueueException eqe) {
    // can't happen
    }
  } /* clear */

  public void enqueue(Primitive p, long t) {
    if (head == null) {
      head = new Node(p,t,null);
      count = 1;
    } else { // non-empty
      if ((head.time == t) && (head.primitive == p))
        return; // no duplicates
      else if (head.time >= t) { // need new head
        Node temp = new Node(p,t,head);
        head = temp;
      } else { // find insertion point
        Node ptr = head;

        while (ptr.next != null)
          if (ptr.next.time < t)
            ptr = ptr.next;
          else
            break;

        if (ptr.next == null)
          ptr.next = new Node(p,t,null);
        else if ((ptr.next.time == t) && (ptr.next.primitive == p))
          return; // no duplicates
        else {
          Node temp = new Node(p,t,ptr.next);
          ptr.next = temp;
        }
      }

      count++;
    } /* else */
  } /* enqueue */

  public Primitive dequeue () throws EmptyQueueException {
    if (count == 0)
      throw new EmptyQueueException("ERROR: dequeue called when queue empty");

    Primitive temp = head.primitive;
    head = head.next;
    --count;
    return temp;
  } /* dequeue */

  public long frontTime () throws EmptyQueueException {
    if (count == 0)
      throw new EmptyQueueException("ERROR: frontTime called when queue empty");

    return head.time;
  } /* frontTime */

}