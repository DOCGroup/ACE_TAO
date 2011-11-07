// $Id$
//
// = FILENAME
//    WeaponsVisComp.java
//
// = AUTHOR
//    Seth Widoff (core functionality)
//    Michael Kircher (mk1@cs.wustl.edu)
//
// = DESCRIPTION
//   This is a Visualization Component for displaying weapons.
//
// ============================================================================


import java.io.*;
import java.util.*;
import java.awt.*;


public class WeaponsVisComp extends Panel implements VisComp
{
  private final static String ONLINE = "Online";
  private final static String OFFLINE = "Offline";
  private final static Font FONT_BIG = new Font ("Dialog", Font.BOLD, 14);
  private final static Font  FONT_SMALL = new Font ("Dialog", Font.BOLD, 10);
  private final static Color BLUE = new Color (30, 144, 255);

  private int count_ = 0;
  private Hashtable weapons_table_ = new Hashtable ();
  private GridBagLayout gbl_= new GridBagLayout ();
  private GridBagConstraints gbc_ = new GridBagConstraints ();

  Label default_label_ = new Label ("No weapons available", Label.CENTER);

  public WeaponsVisComp ()
    {
      default_label_.setFont (FONT_BIG);
      default_label_.setForeground (BLUE);

      setLayout (gbl_);
      gbc_.gridx = 0;
      gbc_.gridy = 0;
      gbc_.gridheight = 1;
      gbc_.gridwidth  = 1;
      gbc_.anchor = GridBagConstraints.NORTH;
      gbc_.fill = GridBagConstraints.NONE;
      setBackground (Color.black);

      gbl_.setConstraints (default_label_, gbc_);
      add (default_label_);
    }

  public void setName (String title) {
  }

  public int getProperty () {
    return Properties.WEAPONS;
  }

  public Dimension getPreferredSize () {
    return new Dimension (250, 200);
  }

  public Dimension getMinimumSize () {
    return new Dimension (80, 80);
  }

  public void update (java.util.Observable observable, java.lang.Object obj) {
    Weapons weapons_ = null;
    try {
      weapons_ = (Weapons) obj;
    }
    catch (Exception excp) {
      System.out.println (excp);
      System.out.println ("Visualization Component received wrong data type!");
    }
    if (weapons_ != null)
    {
      for (int i = 0; i < weapons_.number_of_weapons && i < 5; i++)
	{
	  String weapon = "";
	  int status = 0;
	  switch (i)
	    {
	    default:
	      break;
	    case 0: weapon = weapons_.weapon1_identifier;
	      status = weapons_.weapon1_status;
	      break;
	    case 1: weapon = weapons_.weapon2_identifier;
	      status = weapons_.weapon2_status;
	      break;
	    case 2: weapon = weapons_.weapon3_identifier;
	      status = weapons_.weapon3_status;
	      break;
	    case 3: weapon = weapons_.weapon4_identifier;
	      status = weapons_.weapon4_status;
	      break;
	    case 4: weapon = weapons_.weapon5_identifier;
	      status = weapons_.weapon5_status;
	      break;
	    }


	  Label status_label_ = (Label)weapons_table_.get (weapon);

	  if (status_label_ != null)
	    status_label_.setText ((status == 1) ? ONLINE : OFFLINE);
	  else
	    {
	      if (count_ == 0)
		this.removeAll ();

	      count_++;
	      Label weapon_label_ = new Label (count_ + ". " + weapon, Label.LEFT);
	      status_label_ = new Label ((status == 1) ? ONLINE : OFFLINE, Label.RIGHT);

	      status_label_.setFont (FONT_SMALL);
	      weapon_label_.setFont (FONT_SMALL);
	      weapon_label_.setForeground (BLUE);

	      gbc_.gridx = 0;
	      gbc_.anchor = GridBagConstraints.WEST;
	      gbl_.setConstraints (weapon_label_, gbc_);
	      add (weapon_label_);
	      gbc_.gridx = 1;
	      gbc_.anchor = GridBagConstraints.EAST;
	      gbl_.setConstraints (status_label_, gbc_);
	      add (status_label_);

	      gbc_.gridy++;
	      weapons_table_.put (weapon, status_label_);
	    }

	  status_label_.setForeground ((status == 1) ?
				      Color.lightGray :
				      Color.darkGray);
	}

      validate ();
    }
  }
}
