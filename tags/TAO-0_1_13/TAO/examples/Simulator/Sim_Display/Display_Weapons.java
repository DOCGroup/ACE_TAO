// $Id$ 

import java.io.*;
import java.net.*;
import java.util.*;
import java.awt.*;

public class Display_Weapons
  extends Panel
  implements Display_Object
{
  private final static String ONLINE = "Online",
    OFFLINE = "Offline";
  private final static Font FONT_BIG = new Font ("Dialog", Font.BOLD, 14),
    FONT_SMALL = new Font ("Dialog", Font.BOLD, 10);
  private final static Color BLUE = new Color (30, 144, 255);

  private int count_ = 0;
  private Hashtable weapons_ = new Hashtable ();
  private GridBagLayout gbl_= new GridBagLayout ();
  private GridBagConstraints gbc_ = new GridBagConstraints ();
  
  public Display_Weapons ()
    {
      Label default_label =
	new Label ("No weapons available", Label.CENTER);

      default_label.setFont (FONT_BIG);
      default_label.setForeground (BLUE);

      setLayout (gbl_);
      gbc_.gridx = 0;
      gbc_.gridy = 0;
      gbc_.gridheight = 1;
      gbc_.gridwidth  = 1;
      gbc_.anchor = GridBagConstraints.NORTH;
      gbc_.fill = GridBagConstraints.NONE;
      setBackground (Color.black);

      gbl_.setConstraints (default_label, gbc_);
      add (default_label);
    }

  public Dimension getPreferredSize ()
    {
      return new Dimension (250, 200);
    }

  public Dimension getMinimumSize ()
    {
      return new Dimension (250, 100);
    }
  
  public int update_display (Display_Push_Consumer display_push_consumer)
    {      
      Weapons weapons = display_push_consumer.get_weapons ();

      for (int i = 0; i < weapons.number_of_weapons && i < 5; i++)
	{
	  String weapon;
	  int status;
	  switch (i)
	    {
	    default:
	    case 0: weapon = weapons.weapon1_identifier;						      
	      status = weapons.weapon1_status;
	      break;
	    case 1: weapon = weapons.weapon2_identifier;						      
	      status = weapons.weapon2_status;
	      break;
	    case 2: weapon = weapons.weapon3_identifier;						      
	      status = weapons.weapon3_status;
	      break;
	    case 3: weapon = weapons.weapon4_identifier;						      
	      status = weapons.weapon4_status;
	      break;
	    case 4: weapon = weapons.weapon5_identifier;						      
	      status = weapons.weapon5_status;
	      break;
	    }


	  Label status_label = (Label)weapons_.get (weapon);

	  if (status_label != null)
	    status_label.setText ((status == 1) ? ONLINE : OFFLINE);
	  else
	    {
	      if (count_ == 0)
		removeAll ();

	      count_++;
	      Label weapon_label =
		new Label (count_ + ". " + weapon, Label.LEFT);
	      status_label =
		new Label ((status == 1) ? ONLINE : OFFLINE, Label.RIGHT);

	      status_label.setFont (FONT_SMALL);
	      weapon_label.setFont (FONT_SMALL);
	      weapon_label.setForeground (BLUE);
	      
	      gbc_.gridx = 0;
	      gbc_.anchor = GridBagConstraints.WEST;
	      gbl_.setConstraints (weapon_label, gbc_);
	      add (weapon_label);
	      gbc_.gridx = 1;
	      gbc_.anchor = GridBagConstraints.EAST;
	      gbl_.setConstraints (status_label, gbc_);
	      add (status_label);
	      
	      gbc_.gridy++;
	      weapons_.put (weapon, status_label);
	    }

	  status_label.setForeground ((status == 1) ?
				      Color.lightGray :
				      Color.darkGray);
	}

      validate ();
      return 0;
    }
}
