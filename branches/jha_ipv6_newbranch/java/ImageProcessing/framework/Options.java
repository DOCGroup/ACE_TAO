package imaging.framework;

import java.awt.*;

class Options
{
  // Set GridBagConstraints
  public static void constrain (Container container, Component component,
				int gridx, int gridy, 
				int gridwidth, int gridheight,
				int fill, int anchor)
  {
    GridBagConstraints c = new GridBagConstraints ();
    GridBagLayout gbl = (GridBagLayout) container.getLayout ();

    c.gridx = gridx;
    c.gridy = gridy;
    c.gridwidth = gridwidth;
    c.gridheight = gridheight;
    c.fill = fill;
    c.anchor = anchor;
    gbl.setConstraints (component, c);
    container.add (component);
  }
}
