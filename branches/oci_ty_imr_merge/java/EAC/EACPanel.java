/**
 * Title:        EACPanel
 * Description:  A subclass of Panel for the Event Analysis Configurator
 */
package EAC;
import java.awt.*;
import java.awt.event.*;

public class EACPanel extends Panel {

  protected TextField inputArea;
  protected Label reportArea;

  // input modes
  public final int COMMAND = 0;
  public final int SELECT = 1;
  public final int CUT = 2;
  public final int COPY = 3;
  public final int PASTE = 4;
  public final int EDIT = 5;
  public final int CONNECTING = 6;
  public final int COMPONENT = 7;
  public final int LABEL_TYPING = 8;
  public final int LABEL_ADDING = 9;
  public final int RC_TYPING = 10;
  public final int DURATION_TYPING = 11;
  public final int PERIOD_TYPING = 12;
  public final int SOURCE = 13;
  public final int CONNECTING_END = 14;
  public final int LOAD = 15;
  public final int SAVE = 16;

  // current input mode
  protected int mode;

  // current primitive for drawing
  protected Primitive current_shape;

  // current text label
  protected EACLabel current_label;

  // current component
  protected EACComponent current_component;

  // current source
  protected Source current_source;

  // current connector, and associated primitives (indices into config's collection)
  protected Connector current_connector;
  protected int connector_start_index;
  protected int connector_end_index;

  // flag for associating the required label with a component or source
  protected boolean component_needs_label = false;
  protected boolean source_needs_label = false;

  // Component Configuration associated with this panel
  protected Configuration config;

  // for dragging primitives around the panel
  int moving_dx,
      moving_dy;
  boolean repainted;


  // Constructor
  public EACPanel(Configuration c, TextField i, Label r) {
    config = c;
    inputArea = i;
    inputArea.addKeyListener(eac_key_adapter);
    reportArea = r;
    mode = COMMAND;
    addMouseListener(eac_mouse_adapter);
    addMouseMotionListener(eac_mm_adapter);
  } /* constructor */

  // Mouse Adapter
  private class EACMouseAdapter extends MouseAdapter
    implements MouseListener {

      EACPanel canvas;

      public EACMouseAdapter(EACPanel p) {
        canvas = p;
      } /* constructor */

      public void mousePressed(MouseEvent e) {
      switch (mode) {
      case EDIT:
        current_shape.setTop(new Point(e.getX(),e.getY()));
        try {
          if ((config.leftSideOverlaps(current_shape) == -1) &&
              (config.rightSideOverlaps(current_shape) == -1)) {
            current_shape.draw();
            reportArea.setText("");
            config.addPrimitive(current_shape);
            mode = COMMAND;
          } else {
            reportArea.setText("ERROR: Primitives too close together");
          } /* if */
        } catch (BoundsException be) {
          reportArea.setText(be.getMessage());
        } catch (TooManyPrimitivesException tmpe) {
          reportArea.setText(tmpe.getMessage());
        } /* try */
        break;
      case SOURCE:
        current_source.setTop(new Point(e.getX(),e.getY()));
        try {
          if ((config.leftSideOverlaps(current_source) == -1) &&
              (config.rightSideOverlaps(current_source) == -1)) {
            current_source.draw();
            config.addPrimitive(current_source);
            reportArea.setText("Please enter the period for this source in the input area");
            inputArea.setText("");
            mode = PERIOD_TYPING;
          } else {
            reportArea.setText("ERROR: Primitives too close together");
          } /* if */
        } catch (BoundsException be) {
          reportArea.setText(be.getMessage());
        } catch (TooManyPrimitivesException tmpe) {
          reportArea.setText(tmpe.getMessage());
        } /* try */
        break;
      case COMPONENT:
        current_component.setTop(new Point(e.getX(),e.getY()));
        try {
          if ((config.leftSideOverlaps(current_component) == -1) &&
              (config.rightSideOverlaps(current_component) == -1)) {
            current_component.draw();
            config.addPrimitive(current_component);
            reportArea.setText("Please enter the resource consumption for this component in the input area");
            inputArea.setText("");
            mode = RC_TYPING;
          } else {
            reportArea.setText("ERROR: Primitives too close together");
          } /* if */
        } catch (BoundsException be) {
          reportArea.setText(be.getMessage());
        } catch (TooManyPrimitivesException tmpe) {
          reportArea.setText(tmpe.getMessage());
        } /* try */
        break;
      case CONNECTING:
        current_connector = new Connector(canvas);
        current_connector.anchor(new Point(e.getX(),e.getY()));
        connector_start_index = config.leftSideOverlaps(current_connector);
        if (connector_start_index == -1)
          reportArea.setText("ERROR: Connector must begin inside a primitive");
        else
          mode = CONNECTING_END;
        break;
      case LABEL_ADDING:
        current_label.setTop(new Point(e.getX(),e.getY()));
        try {
          current_label.specialUndraw();
          current_label.draw();
          reportArea.setText("");
          config.addPrimitive(current_label);
          mode = COMMAND;
        } catch (BoundsException be) {
          reportArea.setText(be.getMessage());
        } catch (TooManyPrimitivesException tmpe) {
          reportArea.setText(tmpe.getMessage());
        } /* try */
        break;
      default: // SELECT mode
        mode = SELECT;
        current_shape = config.primitiveContaining(new Point(e.getX(),e.getY()));
        repaint();
        if (current_shape != null) {
          repaint(); // if there was a previously selected shape, it ignored
                     // the last repaint()  (kludge--looks better for dragging)
          moving_dx = e.getX() - current_shape.getTop().x;
          moving_dy = e.getY() - current_shape.getTop().y;
          current_shape.selectedDraw();
          repainted = false;
        } else
          repaint(); // need to repaint a second time
        break;
      } /* switch */
      } /* mousePressed */

      public void mouseReleased(MouseEvent e) {
      switch (mode) {
      case CONNECTING_END:
        current_connector.end(new Point(e.getX(),e.getY()));
        try {
          connector_end_index = config.rightSideOverlaps(current_connector);
          if (connector_end_index != -1) {
            if (connector_end_index == connector_start_index)
              reportArea.setText("ERROR: Connector cannot start and end at same primitive");
            else {
              Primitive startP = config.getPrimitive(connector_start_index);
              Primitive endP = config.getPrimitive(connector_end_index);

              // connect primitives
              try {
                startP.addOutput(current_connector);
                current_connector.setInput(startP);
                current_connector.setOutput(endP);
                endP.addInput(current_connector);

                // if everything okay, draw arrow
                current_connector.draw();
                reportArea.setText("");
                config.addPrimitive(current_connector);

                mode = COMMAND;

              } catch (ConnectionException ce) {
                reportArea.setText(ce.getMessage());
              }
            } /* else */
          } else {
            reportArea.setText("ERROR: Connector must terminate inside a primitive");
          } /* if */
        } catch (BoundsException be) {
          reportArea.setText(be.getMessage());
        } catch (TooManyPrimitivesException tmpe) {
          reportArea.setText(tmpe.getMessage());
        } /* try */
        mode = CONNECTING;
        break;
      case SELECT:
        if (current_shape != null)
          current_shape.reconnect();
        if (repainted) // dragging occurred
          repaint();
        break;
      default:
        //no-op
      } /* switch */
      } /* mouseReleased */

      public void mouseClicked(MouseEvent e) {}
      public void mouseEntered(MouseEvent e) {}
      public void mouseExited(MouseEvent e) {}

  } /* EACMouseAdapter */

  // Mouse Motion Adapter
  private class EACMouseMotionAdapter extends MouseMotionAdapter
    implements MouseMotionListener {

    public void mouseDragged(MouseEvent e) {
    switch (mode) {
    case CONNECTING_END:
      current_connector.stretch(new Point(e.getX(),e.getY()));
      break;
    case SELECT:
      if (current_shape != null) {
        Point old_top = current_shape.getTop();
        if (!repainted) {
          repaint();
          repainted = true;
        } /* if */
        current_shape.specialUndraw();
        current_shape.setTop(new Point(e.getX() - moving_dx,e.getY() - moving_dy));
        if (!current_shape.inBounds())
          current_shape.setTop(old_top);
        current_shape.specialDraw();
      } /* if */
    default:
      //no-op
    } /* switch */
    } /* mouseDragged */

    public void mouseMoved(MouseEvent e) {
    switch (mode) {
    case COMPONENT:
      if (current_component.getAnchor() != null)
        current_component.specialUndraw();
      current_component.setTop(new Point(e.getX(),e.getY()));
      current_component.specialDraw();
      break;
    case SOURCE:
      if (current_source.getAnchor() != null)
        current_source.specialUndraw();
      current_source.setTop(new Point(e.getX(),e.getY()));
      current_source.specialDraw();
      break;
    case EDIT:
      if (current_shape.getAnchor() != null)
        current_shape.specialUndraw();
      current_shape.setTop(new Point(e.getX(),e.getY()));
      current_shape.specialDraw();
      break;
    case LABEL_ADDING:
      if (current_label.getAnchor() != null)
        current_label.specialUndraw();
      current_label.setTop(new Point(e.getX(),e.getY()));
      current_label.specialDraw();
      break;
    default:
      //no-op
    } /* switch */
    } /* mouseMoved */

  } /* EACMouseMotionAdapter */

  // Key Adapter
  private class EACKeyAdapter extends KeyAdapter implements KeyListener {

    EACPanel canvas;

    public EACKeyAdapter(EACPanel p) {
      canvas = p;
    } /* constructor */

    public void keyPressed(KeyEvent e) {
      Graphics g = getGraphics();
      String input;

      if ((mode == RC_TYPING) && (e.getKeyChar() == '\n')) {
        input = new String(inputArea.getText());
        try {
          current_component.setResourceConsumption(Integer.parseInt(input));
          current_component.draw();
          reportArea.setText("Please enter the duration for this component in the input area");
          inputArea.setText("");
          mode = DURATION_TYPING;
        } catch (NumberFormatException nfe) {
          reportArea.setText("Please enter an integer value between 1 and 100, inclusive");
          inputArea.setText("");
          return;
        } catch (BoundsException be) {
          reportArea.setText(be.getMessage());
        } /* try */
      } else if ((mode == DURATION_TYPING) && (e.getKeyChar() == '\n')) {
        input = new String(inputArea.getText());
        try {
          current_component.setDuration(Integer.parseInt(input));
          current_component.draw();
          reportArea.setText("Please enter a label for this component in the input area");
          inputArea.setText("");
          component_needs_label = true;
          mode = LABEL_TYPING;
        } catch (NumberFormatException nfe) {
          reportArea.setText("Please enter an integer value");
          inputArea.setText("");
          return;
        } catch (BoundsException be) {
          reportArea.setText(be.getMessage());
        } /* try */
      } else if ((mode == LABEL_TYPING) && (e.getKeyChar() == '\n')) {
        current_label = new EACLabel(canvas,inputArea.getText().length());
        current_label.setText(inputArea.getText());
        if (component_needs_label) {
          component_needs_label = false;
          current_component.setLabel(current_label);
          current_label.setLabelee(current_component);
        } else if (source_needs_label) {
          source_needs_label = false;
          current_source.setLabel(current_label);
          current_label.setLabelee(current_source);
        }
        reportArea.setText("Click at desired label location");
        inputArea.setText("");
        mode = LABEL_ADDING;
      } else if ((mode == PERIOD_TYPING) && (e.getKeyChar() == '\n')) {
        input = new String(inputArea.getText());
        try {
          current_source.setPeriod(Integer.parseInt(input));
          current_source.draw();
          inputArea.setText("");
          reportArea.setText("Please enter a label for this source in the input area");
          mode = LABEL_TYPING;
          source_needs_label = true;
        } catch (NumberFormatException nfe) {
          reportArea.setText("Please enter a positive integer value");
          inputArea.setText("");
          return;
        } catch (BoundsException be) {
          reportArea.setText(be.getMessage());
        } /* try */
      } else if ((mode == LOAD) && (e.getKeyChar() == '\n')) {
        File input_file;
        input = new String(inputArea.getText());
        try {
          input_file = new File(input,"r",config,canvas);
          input_file.readConfiguration();
          input_file.close();
          reportArea.setText("Configuration loaded successfully");
          inputArea.setText("");
        } catch (java.io.FileNotFoundException fnfe) {
        } catch (java.io.IOException ioe) {
        } catch (TooManyPrimitivesException tmpe) {
          reportArea.setText(tmpe.getMessage());
        }
      } else if ((mode == SAVE) && (e.getKeyChar() == '\n')) {
        File output_file;
        input = new String(inputArea.getText());
        try {
          output_file = new File(input,"rw",config,canvas);
          output_file.writeConfiguration();
          output_file.close();
          reportArea.setText("Configuration saved successfully");
          inputArea.setText("");
        } catch (java.io.FileNotFoundException fnfe) {
        } catch (java.io.IOException ioe) {}
      } /* if */
    } /* keyPressed */
  } /* EACKeyAdapter */

  // Mouse Adapter Instances
  private EACMouseAdapter eac_mouse_adapter = new EACMouseAdapter(this);
  private EACMouseMotionAdapter eac_mm_adapter = new EACMouseMotionAdapter();

  // Key Adapter Instance
  private EACKeyAdapter eac_key_adapter = new EACKeyAdapter(this);

  public void paint(Graphics g) {
    g.setColor(getBackground());
    g.fillRect(0,0,getSize().width,getSize().height);
    g.setColor(getForeground());
    g.drawRect(0,0,getSize().width - 1,getSize().height - 1);
    config.draw();
  } /* paint */

  public void repaint() {
    paint(getGraphics());
  } /* repaint */
}