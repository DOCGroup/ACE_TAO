// ========================================================================
// $Id$
// 
// = FILENAME
//    Server_Discovery_Util.java
//
// = AUTHOR
//    Seth Widoff <sbw1@cs.wustl.edu>
//
// ========================================================================

import java.io.*;
import TAO_VR.Movie;

public class Server_Discovery_Util
{
  //  public static native int init (String[] args);
  
  public static native void query_trader ();

  public static native String[] get_servers ();

  public static native TAO_VR.Movie[] get_movie_info (String host_name);
    
  public static native
  void load_movie (String server_name, Movie movie_name);
  
  public static native
  float evaluate_performance_property (String server_name, String prop_name);  
}
