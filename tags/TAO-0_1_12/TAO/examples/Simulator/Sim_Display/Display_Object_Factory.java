// $Id$ 

import java.io.*;
import java.util.*;

public class Display_Object_Factory
{
  public final static int LONG_SIZE = 4;
  public final static int DOUBLE_SIZE = 8;

  public final static int ART_HORIZON_ENUM = 0,
    WEAPONS_ENUM = 1;

  private final static String[] DISPLAY_NAMES =
  {
    "Artificial Horizon",
    "Weapons Display",
  };

  private static Hashtable objs_ = new Hashtable ();

  static
    {
      objs_.put (DISPLAY_NAMES[ART_HORIZON_ENUM],
		 new Display_Art_Horizon ());
      objs_.put (DISPLAY_NAMES[WEAPONS_ENUM],
		 new Display_Weapons ());
    }

  public static String resolve_name (long enum)
    {
      return (enum < DISPLAY_NAMES.length) ?
	DISPLAY_NAMES[(int)enum] : null;
    }
  
  public static Display_Object create_display_object (String name)
    {
      Object obj = objs_.get (name);
      return (obj == null) ? null : (Display_Object) obj;
    }
}

