/**
 *  Title:        Component
 *  Description:  A real-time component emulator, for use in the Event Analysis
 *                Configurator.
 */
package EAC;
import java.awt.*;

public class EACComponent extends Primitive {

  class QueueEntry {

    Source source;
    long request_time;
    boolean running;

  } /* QueueEntry */

  public class ServiceQueue {

    public QueueEntry queue[];

    public int size;

    void enqueue(QueueEntry e) {
      int i;

      for (i = 0; i < size; i++)
        if (e.source == queue[i].source) {
          System.out.println("Internal error: attempt to enqueue duplicate source");
          System.exit(-1);
        }

      if (size >= MAX_INPUTS) {
        System.out.println("Internal error: queue size at max");
        System.exit(-1);
      }

      queue[size++] = e;
    } /* enqueue */

    QueueEntry dequeue() {
      QueueEntry front = queue[0];
      int i;

      for (i = 0; i < size; i++)
        queue[i] = queue[i+1];

      --size;

      return front;
    } /* dequeue */

    boolean containsSource (Primitive p) {
      int i;

      for (i = 0; i < size; i++)
        if (queue[i].source == p)
          return true;

      return false;
    } /* containsSource */

  } /* ServiceQueue */

  // queue of running/waiting event sources
  protected ServiceQueue sq;

  // Font for text
  protected Font text_font = new EACFont().getFont();

  // dimensions of the rectangle to be drawn on-screen
  protected final int HEIGHT = 40;
  protected final int WIDTH = 30;

  // Configuration to which the component belongs
  protected Configuration config;

  // percentage of Resource Pool consumed during execution
  protected int resource_consumption;

  // duration of execution
  protected int duration;

  // input
  protected Connector input;

  // outputs
  protected Connector output[] = new Connector[MAX_OUTPUTS];
  protected int output_count = 0;

  // helps with dragging the component
  protected boolean selected = false;
  protected Point old_anchor;

  // Black-on-white (normal) or white-on-black (reversed)?
  protected boolean reverse_color = false;

  public EACComponent(EACPanel p, Configuration c) {
    super(p);
    config = c;
    sq = new ServiceQueue();
    sq.queue = new QueueEntry[MAX_INPUTS];
    sq.size = 0;
  } /* constructor */

  public void restart() {
    sq.size = 0;
    reverse_color = false;
    selected = false;
  } /* restart */

  public void setTop(Point p) {
    anchor = new Point(p.x - (int) (WIDTH / 2), p.y);
  } /* setTop */

  public Point getTop() {
    return new Point(anchor.x + (int) (WIDTH / 2), anchor.y);
  } /* getTop */

  public void setResourceConsumption(int rc) throws NumberFormatException {
    if ((rc > 0) && (rc <= 100))
      resource_consumption = rc;
    else
      throw new NumberFormatException();
  } /* setResourceConsumption */

  public int getResourceConsumption() {
    return resource_consumption;
  } /* getResourceConsumption */

  public void setDuration(int d) throws NumberFormatException {
    if (d > 0)
      duration = d;
    else
      throw new NumberFormatException();
  } /* setDuration */

  public int getDuration() {
    return duration;
  } /* getDuration */

  public void event(Source s) throws ConnectionException {
    long time = config.getTime();

    s.reverseColor();
    try {
      s.draw();
    } catch (BoundsException be) {}

    //System.out.print(label.getText());
    //System.out.print(" event at: ");
    //System.out.println(time);

    if (sq.containsSource(s)) { // missed a deadline
      config.missed_deadlines++;
      System.out.print(label.getText());
      System.out.print(" missed a deadline for ");
      System.out.print(s.getLabel().getText());
      System.out.print(" at: ");
      System.out.println(java.lang.Long.toString(time));
    } else { // there's no existing service call for this source
      // prepare to add new entry to Service Queue
      QueueEntry e = new QueueEntry();
      e.source = s;
      e.request_time = time;
      if (config.rp.reserve(resource_consumption)) { // resource reservation succeeded
        reverseColor(); // indicate we're working it
        try {
          draw();
        } catch (BoundsException be) {}
        config.eventEnqueue(this,time + duration); // set wakeup call to release resources

        // new entry will be running
        e.running = true;
      } else { // resource reservation failed
        config.eventEnqueue(this,time + 1); // set wakeup call to try for resources again

        // new entry will be waiting
        e.running = false;
      } /* else */

      sq.enqueue(e); // add new entry to Service Queue
    } /* else */
  } /* event */

  public void wakeup(long t) throws ConnectionException {
    int i;

    //System.out.print(label.getText());
    //System.out.println(" wakeup at: " + java.lang.Long.toString(t));

    while (sq.size > 0) {
      if ((sq.queue[0].running) && (sq.queue[0].request_time + duration <= t)) { // time to release
        config.rp.release(resource_consumption);
        try {
          Source s = sq.dequeue().source; // dequeue and get the source
          s.normalColor();  // indicates we're done
          normalColor();
          s.draw();
          draw();
        } catch (BoundsException be) {}
      } else
        break;
    } /* while */

    // satisfy as many waiting service calls as possible
    for (i = 0; i < sq.size; i++) {
      if (!sq.queue[i].running) {
        if (config.rp.reserve(resource_consumption)) { // resource reservation succeeded
          config.eventEnqueue(this,t + duration); // set wakeup call to release resources
          sq.queue[i].request_time = t;
          sq.queue[i].running = true;
          sq.queue[i].source.reverseColor(); // indicates we're working it
          reverseColor();
          try {
            draw();
            sq.queue[i].source.draw();
          } catch (BoundsException be) {}
        } else { // resource reservation failed
          config.eventEnqueue(this,t + 1); // set wakeup call to try for resources again
          break; // no reason to keep trying right now
        } /* else */
      } /* if */
    } /* while */

  } /* wakeup */

  public int addInput(Connector c) throws ConnectionException {
    if (input == null) {
      input = c;
      return 1;
    }
    else
      throw new ConnectionException("ERROR: Input already established for this component");
  } /* setInput */

  public int addOutput(Connector c) throws ConnectionException {
    if (output_count < MAX_OUTPUTS)
      output[output_count++] = c;
    else
      throw new ConnectionException("ERROR: Maximum outputs established for this component");

    return output_count;
  } /* addOutput */

  public Connector getInput(int i) throws ConnectionException {
    if (i != 0)
      throw new ConnectionException("ERROR: Bad input index for component");
    else if (input == null)
      throw new ConnectionException("ERROR: No input for this component");
    else
      return input;
  } /* getInput */

  public Connector getOutput(int i) throws ConnectionException {
    if ((i >= 0) && (i < output_count))
      return output[i];
    else
      throw new ConnectionException("ERROR: Bad output index for component");
  } /* getOutput */

  public void removeInput(int i) throws ConnectionException {
    if (i != 0)
      throw new ConnectionException("ERROR: Bad input index for component");
    else
      input = null;
  } /* removeInput */

  public void removeOutput(int i) throws ConnectionException {
    if ((i >= 0) && (i < output_count)) {
      output[i] = output[output_count-1];
      --output_count;
    } else
      throw new ConnectionException("ERROR: Bad output index for component");
  } /* removeOutput */

  public int getOutputCount() throws ConnectionException {
    return output_count;
  } /* getOutputCount */

  public int getInputCount() throws ConnectionException {
    return (input == null) ? 0 : 1;
  } /* getInputCount */

  public boolean contains(Point p) {
    if ((p.x >= anchor.x) &&
        (p.x <= anchor.x + WIDTH) &&
        (p.y >= anchor.y) &&
        (p.y <= anchor.y + HEIGHT))
      return true;
    else
      return false;
  } /* contains */

  public Point upperLeft() {
    return anchor;
  } /* upperLeft */

  public Point lowerRight() {
    return new Point(anchor.x + WIDTH, anchor.y + HEIGHT);
  } /* lowerRight */

  public Point upperRight() {
    return new Point(anchor.x + WIDTH, anchor.y);
  } /* upperRight */

  public Point lowerLeft() {
    return new Point(anchor.x, anchor.y + HEIGHT);
  } /* lowerLeft */

  public void draw() throws BoundsException {
    if (selected)
      selected = false;
    else if (inBounds()) {
      Graphics g = canvas.getGraphics();
      char stringData[];
      Color bg,
            fg;

      if (reverse_color) {
        bg = Color.black;
        fg = Color.white;
      } else {
        bg = Color.white;
        fg = Color.black;
      }

      g.setColor(Color.black); // rectangle is always black
      g.drawRect(anchor.x,anchor.y,WIDTH,HEIGHT);

      g.setColor(bg); // interior
      g.fillRect(anchor.x+1,anchor.y+1,WIDTH-1,HEIGHT-1);

      g.setColor(fg); // middle line and text
      g.drawLine(anchor.x,anchor.y + (int) (HEIGHT / 2),
                 anchor.x + WIDTH, anchor.y + (int) (HEIGHT / 2));

      // print resource consumption percentage
      g.setFont(text_font);
      g.drawString(java.lang.String.valueOf(resource_consumption),
                   anchor.x + 16 - (3 * java.lang.String.valueOf(resource_consumption).length()),
                   anchor.y + (int) (HEIGHT / 3));

      // print duration
      g.drawString(java.lang.String.valueOf(duration),
                   anchor.x + 16 - (3 * java.lang.String.valueOf(duration).length()),
                   anchor.y + (int) (HEIGHT * 3 / 4) + 3);

    } else
      throw new BoundsException("ERROR: Attempted to place Component partially out of bounds");
  } /* draw */

  public void reverseColor() {
    reverse_color = true;
  } /* reverseColor */

  public void normalColor() {
    reverse_color = false;
  } /* normalColor */

  public void specialDraw() {
    Graphics g = canvas.getGraphics();

    g.setXORMode(canvas.getBackground());
    g.drawRect(anchor.x,anchor.y,WIDTH,HEIGHT);
  } /* specialDraw */

  public void specialUndraw() {
    Graphics g = canvas.getGraphics();

    g.setColor(canvas.getBackground());
    g.setXORMode(canvas.getForeground());
    g.drawRect(anchor.x,anchor.y,WIDTH,HEIGHT);
  } /* specialUndraw */

  public void selectedDraw() {
    // this will preclude drawing this component for one repaint() call
    selected = true;

    old_anchor = anchor;

    super.selectedDraw();
  } /* selectedDraw */

  public void reconnect() {
    int i;

    // move output anchors
    for (i = 0; i < output_count; i++)
      output[i].setAnchor(new Point(output[i].getAnchor().x + anchor.x - old_anchor.x,
                                    output[i].getAnchor().y + anchor.y - old_anchor.y));

    // move input endpoint
    input.setEndpoint(new Point(input.upperRight().x + anchor.x - old_anchor.x,
                                input.upperRight().y + anchor.y - old_anchor.y));

    // move label anchor
    label.setAnchor(new Point(label.getAnchor().x + anchor.x - old_anchor.x,
                              label.getAnchor().y + anchor.y - old_anchor.y));
  } /* reconnect */

  public boolean inBounds() {
    if ((canvas.contains(upperLeft())) &&
        (canvas.contains(lowerRight())))
      return true;
    else
      return false;
  } /* inBounds */

  public void write(File f) throws java.io.IOException {
    char temp[]; // for writing label
    int i, n;

    f.writeInt(f.COMPONENT);
    f.writeInt(anchor.x);
    f.writeInt(anchor.y);
    f.writeInt(resource_consumption);
    f.writeInt(duration);

    // Write out the associated label
    n = label.getText().length();
    f.writeInt(n); // text length
    temp = new char[n];
    temp = label.getText().toCharArray();
    for (i = 0; i < n; i++)
      f.writeChar(temp[i]);
    f.writeInt(label.getAnchor().x); // label position
    f.writeInt(label.getAnchor().y);
  } /* write */

  public void read(File f) throws java.io.IOException {
    char temp[]; // for reading label
    int i, n;
    int label_x, label_y;

    anchor = new Point();
    anchor.x = f.readInt();
    anchor.y = f.readInt();
    resource_consumption = f.readInt();
    duration = f.readInt();

    // get label info
    n = f.readInt(); // text length
    label = new EACLabel(canvas,n);
    temp = new char[n];
    for (i = 0; i < n; i++)
      temp[i] = f.readChar();
    label.setText(java.lang.String.valueOf(temp));
    label_x = f.readInt();
    label_y = f.readInt();
    label.setAnchor(new Point(label_x,label_y));
    label.setLabelee(this);
    try {
      config.addPrimitive(label);
    } catch (TooManyPrimitivesException tmpe) {
      // have faith :-)
    }
  } /* read */
}