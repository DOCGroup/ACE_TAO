/**
 *  Title:        Configurator
 *  Description:  The application class for the Event Analysis Configurator
 */
package EAC;
import java.awt.*;
import java.awt.event.*;

public class Configurator extends Frame {

  // Initial dimensions of the frame
  public final int INITIAL_WIDTH = 800;
  public final int INITIAL_HEIGHT = 600;

  protected TextField inputArea = new TextField("Input Area", 80);
  protected Label reportArea = new Label();
  protected Configuration config = new Configuration(inputArea,reportArea);
  protected EACPanel canvas = new EACPanel(config,inputArea,reportArea);
  protected EACMenuBar menuBar = new EACMenuBar(canvas,config,inputArea,reportArea);

  Configurator(int tick) {
    config.setTick(tick);

    setTitle("The Event Analysis Configurator");
    setLayout(new BorderLayout());
    reportArea.setText("Report Area");

    // Input Area at top of frame
    add(inputArea,"North");

    // Report Area at bottom of frame
    add(reportArea,"South");

    // Canvas in the middle of frame
    add(canvas,"Center");

    // Menu Bar
    setMenuBar(menuBar);

    // Move and resize
    setLocation(0,0);
    setSize(INITIAL_WIDTH,INITIAL_HEIGHT);

    // For exiting
    addWindowListener(new Closer());
  }

  public static void main (String[] argv) {
    new Configurator(java.lang.Integer.valueOf(argv[0]).intValue()).show();
  }

  class Closer extends WindowAdapter {
    public void windowClosing (WindowEvent e) {
	System.exit (0);
    }
  }

} /* Configurator */
