// $Id$ 

import java.awt.*;
import java.util.*;
import java.io.*;

class Sim_Panel extends Panel
{
  private GridBagLayout gbl_ = new GridBagLayout ();
  private GridBagConstraints gbc_ = new GridBagConstraints ();

  private Hashtable sims_ = new Hashtable();
  
  public Sim_Panel()
    {
      setLayout (gbl_);
      gbc_.gridy = 0;
      gbc_.weightx= 1;
      gbc_.gridwidth = 1;
      gbc_.gridheight = 1;
      gbc_.gridx = GridBagConstraints.RELATIVE;
      gbc_.anchor = GridBagConstraints.CENTER;
    }  

  public void add_simulator (String name, Display_Object disp_obj)
    {
      String short_name;      

      if (! sims_.contains (name))
	{      
	  Label label = new Label(name, Label.CENTER);
	  Panel panel = new Panel();
	  
	  label.setFont(new Font("Dialog", Font.BOLD, 12));
	  
	  panel.setLayout(new BorderLayout(0, 2));
	  panel.add("Center", (Component)disp_obj);
	  panel.add("North", label);
	  sims_.put(name, panel);

	  gbl_.setConstraints (panel, gbc_);
	  add (panel);
	}
    }

  public void update_simulator (String name, Display_Push_Consumer display_push_consumer)
   
    {
      Container panel = (Container) sims_.get (name);
      Display_Object sim = (panel != null) ? (Display_Object) panel.getComponent(0) : null;

      if(sim != null)
	sim.update_display (display_push_consumer);
    }

  public Display_Object get_simulator(String name)
    {
      return (Display_Object)((Container) sims_.get (name)).getComponent (0);
    }
}




