/**
 *  Title:        Configuration
 *  Description:  A container of Event Analysis Configurator primitives,
 *                capable of executing interactions among those primitives
 *                one "tick" at a time.
 */
package EAC;
import java.awt.*;
import java.lang.*;

public class Configuration extends Thread {

  // highest simulation clock value
  public final long MAX_SIM_CLOCK = 1000000;

  // maximum number of primitives in a configuration
  public final int MAX_PRIMITIVES = 100;

  // execution modes
  public final int INITIAL = 0;
  public final int RUNNING = 1;
  public final int STOPPED = 2;

  // current mode
  protected int mode = INITIAL;

  // simulation "clock"
  protected long simulation_time;

  // one simulation time unit (msec)
  public int tick; // will be set by runtime parameter

  // number of missed execution deadlines
  protected int missed_deadlines;

  // simulated resource pool
  protected ResourcePool rp;

  // event queue
  protected Queue eventQ;

  // input and output areas
  protected TextField inputArea;
  protected Label reportArea;

  // collection of primitives
  private Primitive primitive[];

  // number of primitives currently stored
  private int primitive_count;

  public Configuration(TextField i, Label r) {
    inputArea = i;
    reportArea = r;
    eventQ = new Queue();
    clear();
    mode = INITIAL;
  } /* constructor */

  public void clear() {
    primitive_count = 0;
    simulation_time = 0;
    missed_deadlines = 0;
    rp = new ResourcePool();
    primitive = new Primitive[MAX_PRIMITIVES];
    eventQ.clear();
  } /* clear */

  public void restart() {
    int i;

    simulation_time = 0;
    missed_deadlines = 0;
    rp.release(rp.usage());
    eventQ.clear();
    for (i = 0; i < primitive_count; i++)
      primitive[i].restart();

  } /* restart */

  public void setTick(int t) {
    tick = t;
  } /* setTick */

  public void addLabel(Primitive p) {
    reportArea.setText("Need a label");
  } /* addLabel */

  public int getPrimitiveCount() {
    return primitive_count;
  } /* getPrimitiveCount */

  public Primitive getPrimitive(int n) {
    return primitive[n];
  } /* getPrimitive */

  protected void setPrimitive(int n, Primitive p) {
    primitive[n] = p;
  } /* setPrimitive */

  public int addPrimitive(Primitive p) throws TooManyPrimitivesException {
    if (primitive_count == MAX_PRIMITIVES)
      throw new TooManyPrimitivesException("ERROR: Too many primitives");

    primitive[primitive_count++] = p;
    return primitive_count;
  } /* addPrimitive */

  public boolean deletePrimitive(int i) {
    if (primitive[i] == null)
      return false;
    else {
      primitive[i] = null;
      return true;
    }
  } /* deletePrimitive */

  public int leftSideOverlaps(Primitive p) {
    int i;

    for (i = 0; i < primitive_count; i++) {
      if (primitive[i] != null) {
        if ((primitive[i].contains(p.upperLeft())) ||
            (primitive[i].contains(p.lowerLeft())) ||
            (p.contains(primitive[i].lowerLeft())) ||
            (p.contains(primitive[i].upperLeft()))) {
          return i;
        } /* if */
      } /* if */
    } /* for */

    // no overlap detected
    return -1;
  } /* leftSideOverlaps */

  public int rightSideOverlaps(Primitive p) {
    int i;

    for (i = 0; i < primitive_count; i++) {
      if (primitive[i] != null) {
        if ((primitive[i].contains(p.lowerRight())) ||
            (primitive[i].contains(p.upperRight())) ||
            (p.contains(primitive[i].upperRight())) ||
            (p.contains(primitive[i].lowerRight()))) {
          return i;
        } /* if */
      } /* if */
    } /* for */

    // no overlap detected
    return -1;
  } /* rightSideOverlaps */

  public Primitive primitiveContaining (Point p) {
    int i;

    for (i = 0; i < primitive_count; i++)
      if (primitive[i].contains(p))
        return primitive[i];

    return null;
  } /* primitiveContaining */

  public void draw() {
    int i;

    for (i = 0; i < primitive_count; i++)
      if (primitive[i] != null)
        try {
          primitive[i].draw();
        } catch (BoundsException be) {
          // can't happen
        }
  } /* draw */

  public void eventEnqueue(Primitive p, long t) {
    eventQ.enqueue(p,t);
  } /* eventEnqueue */

  public long getTime() {
    return simulation_time;
  } /* getTime */

  public void run() {
    Primitive nextP;

    while (true) {
      simulation_time += 1;
      simulation_time %= MAX_SIM_CLOCK;
      try {
        reportArea.setText("Simulation Time: " +
                         java.lang.Long.toString(simulation_time) +
                         "    Resource Consumption: " +
                         java.lang.Integer.toString(rp.usage()) +
                         "%   Missed Deadlines: " +
                         java.lang.Integer.toString(missed_deadlines) +
                         "    Next Event: " +
                         java.lang.Long.toString(eventQ.frontTime()));

        // wake up all primitives who've set alarms for current time
        while (simulation_time == eventQ.frontTime()) {
          nextP = eventQ.dequeue();
          nextP.wakeup(simulation_time);
        } /* while */
      } catch (EmptyQueueException eqe) {
        System.out.println("Event queue empty.  We're done?");
      } catch (ConnectionException ce) {
        System.out.println("Connection exception in run");
      }

      try {
        sleep(tick);
      }
      catch (InterruptedException ie) {
        System.out.println("Interrupted");
      }
    } /* outer while */
  } /* run */

}