import java.awt.*;
import com.sun.java.swing.*;
import com.sun.java.swing.event.*;
import java.util.*;
import Strip_Chart;

public class Server_Discovery_Perf
  extends JInternalFrame
  implements Runnable
{
  private static final int HSPACE = 10;
  private static final int VSPACE = 10;
  private static final int MAX_WIDTH = 5;
  private static final int MAX_HEIGHT = 5;
  private static final int GRAPH_DIMENSION = 175;

  public boolean continue_ = true;
  private JPanel[] graph_panels_ = new JPanel[MAX_HEIGHT];
  private Hashtable props_ = new Hashtable ();
  private String server_name_ = null;
  // Vector containing each of the dynamic properties we will
  // periodically poll for their status.

  class Prop_Struct
  {
    public String dp_;
    public Strip_Chart graph_;

    public Prop_Struct (String dp, Strip_Chart graph)
      {
        this.dp_ = dp;
        this.graph_ = graph;
      }
  }  
  
  public Server_Discovery_Perf (String server_name)
    {
      super (server_name, true, true, true, true);
      this.setSize (MAX_WIDTH*(GRAPH_DIMENSION + HSPACE), GRAPH_DIMENSION);
      this.setLocation (375, 15);
      this.server_name_ = server_name;
      
      // Create each of the five graph rows.
      this.getContentPane().setLayout (new GridLayout (5, 1, HSPACE, 0));
      FlowLayout panel_layout = new FlowLayout (FlowLayout.CENTER, 0, VSPACE);
      for (int i = 0; i < MAX_HEIGHT; i++)
        {
          this.graph_panels_[i] = new JPanel ();
          this.graph_panels_[i].setLayout (panel_layout);
          this.getContentPane ().add (this.graph_panels_[i]);
        }

      // When the frame closes, end the graph updating.
      this.addInternalFrameListener (new InternalFrameAdapter ()
                                     {
                                       public void internalFrameClosing (InternalFrameEvent event)
                                         {
                                          
                                           cease_updates ();
                                          
                                         }
                                     }
                                      );
    }

  public void add_dynamic_property (String prop_name)
    {
      Prop_Struct ps = new Prop_Struct (prop_name, new Strip_Chart (prop_name));
      this.props_.put (prop_name, ps);

      int num_graphs = this.props_.size ();
      int row_panel_index = num_graphs % MAX_HEIGHT;
      this.graph_panels_[row_panel_index].add (ps.graph_);
    }
  
  public Dimension getPreferredSize ()
    {
      // The preferred size is: height = the height of the number of
      // rows; width = static --- always the width of 5 graphs.
      int num_graphs = this.props_.size ();
      int num_rows = (num_graphs % MAX_HEIGHT) + 1;

      return new Dimension (MAX_WIDTH * (GRAPH_DIMENSION + HSPACE),
                            num_rows * (GRAPH_DIMENSION + VSPACE));
    }
  
  public void run ()
    {
      // Periodically awaken and poll the dynamic properties, updating 
      // the graphs with the new values.
      //float new_point = 0;
      for (;;)
      //for (;;)
        {
          //System.out.println("Within loop");
          synchronized (this)
            {
              // An atomic operation.
             
              if (! this.continue_)
                {
             
                  break;
                }
            }

          // Update each dynamic property's graph.
          Enumeration dprops = this.props_.elements ();
          while (dprops.hasMoreElements ())
            {
             
              Prop_Struct ps = (Prop_Struct) dprops.nextElement ();

              // Pull the new value of the dynamic property and insert 
              // it into the corresponding strip chart.
              try
                {
                  float new_point =
                    Server_Discovery_Util.evaluate_performance_property (this.server_name_, ps.dp_);
                  
                  ps.graph_.update (new_point);
                  
                }
              catch (Exception excp) {}
            }

          // Go to sleep for 2 seconds.
          try
            {
              Thread.sleep (2000);
            }
          catch (Exception excp) {}
        }
    }

  private void cease_updates ()
    {      
      synchronized (this)
        {
          // An atomic operation.
          //System.out.println("Just after cease updates");
          this.continue_ = false;
        }
    }
}
