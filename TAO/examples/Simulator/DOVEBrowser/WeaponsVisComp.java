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
  private GridLayout gl_= new GridLayout (1,1);   
  Label default_label_ = new Label ("No weapons available", Label.CENTER);

  public WeaponsVisComp (String title)
    {
      default_label_.setFont (FONT_BIG);
      default_label_.setForeground (BLUE);

      setLayout (gl_);

      setBackground (Color.black);
      add (default_label_);
    }

  public int getProperty ()
    {
      return Properties.WEAPONS;
    }

  public Dimension getPreferredSize ()
    {
      return new Dimension (250, 200);
    }

  public Dimension getMinimumSize ()
    {
      return new Dimension (80, 80);
    }
  
  public void update (java.util.Observable observable, java.lang.Object obj)
  {
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
	  String weapon;
	  int status;
	  switch (i)
	    {
	    default:
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


	  Label status_label = (Label)weapons_table_.get (weapon);

	  if (status_label != null)
	    status_label.setText ((status == 1) ? ONLINE : OFFLINE);
	  else
	    {	 
	      if (count_ == 0)
		this.removeAll ();
	      count_++;
	      Label weapon_label = new Label (count_ + ". " + weapon, Label.LEFT);
	      status_label = new Label ((status == 1) ? ONLINE : OFFLINE, Label.LEFT);

	      status_label.setFont (FONT_SMALL);
	      weapon_label.setFont (FONT_SMALL);
	      weapon_label.setForeground (BLUE);
	      
	      gl_.setRows (count_);
	      gl_.setColumns (2);
  
	      this.add (weapon_label);
	      this.add (status_label);	
	      weapons_table_.put (weapon, status_label);
	    }

	  status_label.setForeground ((status == 1) ?
				      Color.lightGray :
				      Color.darkGray);
	}

      validate ();
    }
  }
}
