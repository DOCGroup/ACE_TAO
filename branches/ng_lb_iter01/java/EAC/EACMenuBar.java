/**
 * Title:        EACMenuBar
 * Description:  A subclass of MenuBar for the Event Analysis Configurator
 */
package EAC;
import java.awt.*;
import java.awt.event.*;

public class EACMenuBar extends MenuBar {

  protected EACPanel canvas;
  protected Configuration config;
  protected TextField inputArea;
  protected Label reportArea;

  public EACMenuBar(EACPanel p, Configuration c, TextField i, Label r) {
    canvas = p;
    config = c;
    inputArea = i;
    reportArea = r;

    // Configuration menu
    Menu menu = new Menu("Configuration");

    // Configuration menu options
    MenuItem mi = new MenuItem("Clear");
    mi.addActionListener(new ActionListener() {
      public void actionPerformed(ActionEvent e) {
        if (config.mode == config.RUNNING) {
          config.suspend();
          config.mode = config.STOPPED;
        } /* if */
        config.clear();
        canvas.repaint();
        inputArea.setText("");
        reportArea.setText("Configuration cleared");
      } /* actionPerformed */
    }); /* addActionListener */
    menu.add(mi);

    mi = new MenuItem("Load");
    mi.addActionListener(new ActionListener() {
      public void actionPerformed(ActionEvent e) {
        if (config.mode == config.RUNNING) {
          config.suspend();
          config.mode = config.STOPPED;
        } /* if */
        config.clear();
        canvas.repaint();
        inputArea.setText("");
        reportArea.setText("Please enter a filename");
        canvas.mode = canvas.LOAD;
      } /* actionPerformed */
    }); /* addActionListener */
    menu.add(mi);

    mi = new MenuItem("Save");
    mi.addActionListener(new ActionListener() {
      public void actionPerformed(ActionEvent e) {
        if (config.mode == config.RUNNING) {
          config.suspend();
          config.mode = config.STOPPED;
        } /* if */
        canvas.repaint();
        canvas.mode = canvas.SAVE;
        inputArea.setText("");
        reportArea.setText("Please enter a filename");
      } /* actionPerformed */
    }); /* addActionListener */
    menu.add(mi);

    mi = new MenuItem("Run/Restart");
    mi.addActionListener(new ActionListener() {
      public void actionPerformed(ActionEvent e) {
        if (config.mode == config.INITIAL) {
          config.start();
        } else if (config.mode == config.STOPPED) {
          config.restart();
          canvas.repaint();
          config.resume();
        }
        System.out.println();
        System.out.println("Configuration running...");
        config.mode = config.RUNNING;
      } /* actionPerformed */
    }); /* addActionListener */
    menu.add(mi);

    mi = new MenuItem("Pause/Unpause");
    mi.addActionListener(new ActionListener() {
      public void actionPerformed(ActionEvent e) {
        if (config.mode == config.RUNNING) {
          config.suspend();
          config.mode = config.STOPPED;
        } else if (config.mode == config.STOPPED) {
          config.resume();
          config.mode = config.RUNNING;
        }
      } /* actionPerformed */
    }); /* addActionListener */
    menu.add(mi);

    add(menu);

    // New menu
    menu = new Menu("New");

    // New menu options
    mi = new MenuItem("Component");
    mi.addActionListener(new ActionListener() {
      public void actionPerformed(ActionEvent e) {
        if (config.mode != config.RUNNING) {
          canvas.current_component = new EACComponent(canvas,config);
          canvas.repaint();
          canvas.mode = canvas.COMPONENT;
        }
      } /* actionPerformed */
    }); /* addActionListener */
    menu.add(mi);

    mi = new MenuItem("Union");
    mi.addActionListener(new ActionListener() {
      public void actionPerformed(ActionEvent e) {
        if (config.mode != config.RUNNING) {
          canvas.current_shape = new Union(canvas);
          canvas.repaint();
          canvas.mode = canvas.EDIT;
        }
      } /* actionPerformed */
    }); /* addActionListener */
    menu.add(mi);

    mi = new MenuItem("Intersection");
    mi.addActionListener(new ActionListener() {
      public void actionPerformed(ActionEvent e) {
        if (config.mode != config.RUNNING) {
          canvas.current_shape = new Intersection(canvas);
          canvas.repaint();
          canvas.mode = canvas.EDIT;
        }
      } /* actionPerformed */
    }); /* addActionListener */
    menu.add(mi);

    mi = new MenuItem("Source");
    mi.addActionListener(new ActionListener() {
      public void actionPerformed(ActionEvent e) {
        if (config.mode != config.RUNNING) {
          canvas.current_source = new Source(canvas,config);
          canvas.repaint();
          canvas.mode = canvas.SOURCE;
        }
      } /* actionPerformed */
    }); /* addActionListener */
    menu.add(mi);

    mi = new MenuItem("Sink");
    mi.addActionListener(new ActionListener() {
      public void actionPerformed(ActionEvent e) {
        if (config.mode != config.RUNNING) {
          canvas.current_shape = new Sink(canvas);
          canvas.repaint();
          canvas.mode = canvas.EDIT;
        }
      } /* actionPerformed */
    }); /* addActionListener */
    menu.add(mi);

    mi = new MenuItem("Connector");
    mi.addActionListener(new ActionListener() {
      public void actionPerformed(ActionEvent e) {
        if (config.mode != config.RUNNING) {
          canvas.repaint();
          canvas.mode = canvas.CONNECTING;
        }
      } /* actionPerformed */
    }); /* addActionListener */
    menu.add(mi);

    mi = new MenuItem("Label");
    mi.addActionListener(new ActionListener() {
      public void actionPerformed(ActionEvent e) {
        if (config.mode != config.RUNNING) {
          reportArea.setText("Enter the text for your label in the input area");
          inputArea.setText("");
          canvas.repaint();
          canvas.mode = canvas.LABEL_TYPING;
        }
      } /* actionPerformed */
    }); /* addActionListener */
    menu.add(mi);

    add(menu);

  } /* constructor */

} /* EACMenuBar */

