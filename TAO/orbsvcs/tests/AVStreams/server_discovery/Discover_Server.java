// $Id$

import java.io.*;
import java.net.*;
import java.awt.*;
import java.awt.event.*;
import java.util.*;
import org.omg.CORBA.*;
import CosTrading.*;
import CosTradingDynamic.*;
import CosTrading.LookupPackage.*;
import Property_Evaluator;
import TS_Resolve;

public class Discover_Server
  extends Frame
  implements ItemListener, ActionListener
{
  private static final String CURRENT_DIR = System.getProperty ("user.dir");
  private static String FILE_SEPARATOR = System.getProperty ("file.separator");
  private static final String SERVICE_TYPE = "MMDevice";
  private static final String REFERENCE = "Reference";
  private static final String MOVIE_INFO = "Movie_Info";
  private static final String MOVIE_NAMES = "Movie_Names";
  private static final String SERVER_NAME = "Server_Name";
  private static final String HAS_CONNECTIONS =
  " Number_Of_Connections < Max_Connections";
  private static final String HTML_FILE =
  CURRENT_DIR + FILE_SEPARATOR  + "movie-list.html";
  private static final String STATS_FILE =
  CURRENT_DIR + FILE_SEPARATOR  + "movie-stats.html";
  private static final String COMPILE = " Compile ";
  private static final String SELECT = " Select ";
  private static final String CLOSE = " Close ";
  private static final byte JAVA_INIT = 35; 
  private static final Font FONT = new Font ("Dialog", Font.BOLD, 14);

  private ORB orb_;
  private int port_;
  private Lookup lookup_;
  private boolean first_click_ = true;
  private String currently_shown_ = null;
  private List movies_selector_ = new List (14, false);
  private Hashtable movies_ = new Hashtable ();
  private Button compile_ = new Button (COMPILE);
  private Button select_ = new Button (SELECT);
  private Button close_ = new Button (CLOSE); 
  
  public Discover_Server (ORB orb, Lookup lookup, int port)    
    {
      super ("Server Discovery");
      
      System.out.println ("yadda");
      this.orb_ = orb;
      this.port_ = port;
      this.lookup_ = lookup;
      
      Panel button_panel = new Panel ();
      button_panel.setLayout (new FlowLayout (FlowLayout.CENTER, 25, 10));
      button_panel.add (this.compile_);
      button_panel.add (this.select_);
      button_panel.add (this.close_);

      Panel list_panel = new Panel ();
      list_panel.setLayout (new CardLayout (10, 10));
      list_panel.add ("List", this.movies_selector_);
      
      this.compile_.setFont (FONT);
      this.select_.setFont (FONT);
      this.close_.setFont (FONT);
      this.movies_selector_.setFont (FONT);

      this.compile_.requestFocus ();
      this.compile_.addActionListener (this);
      this.select_.addActionListener (this);
      this.close_.addActionListener (this);
      this.movies_selector_.addItemListener (this);
      this.movies_selector_.addActionListener (this);
      
      this.setLayout (new BorderLayout (0, 0));
      this.add ("North", list_panel);
      this.add ("South", button_panel);
    }
  
  public void compile_movies ()
    {
      String configuration = "";
      String[] properties = new String[3];
      SpecifiedProps desired_props = new SpecifiedProps ();
      OfferSeqHolder offers = new OfferSeqHolder ();
      OfferIteratorHolder offer_iter = new OfferIteratorHolder ();
      
      properties[0] = MOVIE_INFO;
      properties[1] = MOVIE_NAMES;
      properties[2] = SERVER_NAME;
      desired_props.prop_names (properties);
      
      String constraint = configuration + HAS_CONNECTIONS;
      this.query_trader (constraint, "", desired_props, offers, offer_iter);

      this.movies_selector_.removeAll ();
      for (int i = 0; i < offers.value.length; i++)
	this.extract_movies (offers.value[i]);

      if (offer_iter.value != null)
	{
	  boolean more_in_iter = true;
	  OfferSeqHolder iter_offers = new OfferSeqHolder ();
	  while (more_in_iter)
	    {
	      more_in_iter = offer_iter.value.next_n (10, iter_offers);

	      for (int i = 0; i < iter_offers.value.length; i++)
		this.extract_movies (iter_offers.value[i]);
	    }
	}

      // Add the movies to the list.
      Enumeration movie_names = this.movies_.keys ();

      // Future: alphabetize this!
      while (movie_names.hasMoreElements ())
	{
	  String movie_name = (String) movie_names.nextElement ();
	  System.out.println (movie_name);
	  this.movies_selector_.add (movie_name);
	}
    }
  
  public void view_info (String movie_name)
    {      
      Runtime runtime = Runtime.getRuntime ();

      try
	{
	  if (! movie_name.equals (this.currently_shown_))
	    {
	      this.generate_html (movie_name);
	      
	      if (! this.first_click_)
		runtime.exec ("netscape-remote " + HTML_FILE);
	      else
		{
		  //		  runtime.exec ("netscape-remote-new " + HTML_FILE);
		  this.first_click_ = false;
		}

	      this.currently_shown_ = movie_name;
	    }	    
	}
      catch (Exception exception)
	{
	  System.out.println (exception);
	}
    }
  
  public void select_movie (String movie_name)
    {
      // This will be replaced by the server performance visualization
      // stage.

      if (movie_name != null)
	{
	  // Perform lookup to retrieve the vector of offers associated
	  // with this movie name.
	  Vector vector = (Vector) this.movies_.get (movie_name);
	  Hashtable props = (Hashtable) vector.elementAt (0);
	  
	  org.omg.CORBA.Object object =
	    (org.omg.CORBA.Object) props.get (REFERENCE);
	  String ior = this.orb_.object_to_string (object);

	  TAO_VR.Movie movie_info = (TAO_VR.Movie) props.get (movie_name);
	  	  
	  System.out.println (this.port_ + " " + ior + " " + movie_info.filename_);
	  this.load_movie (ior, movie_info.filename_);
	  
	  // this.shutdown ();
	}
    }

  public void itemStateChanged (ItemEvent e)
    {
      Integer index = (Integer) e.getItem ();
      String movie_name = this.movies_selector_.getItem	(index.intValue ());      
      this.view_info (movie_name);
    }

  public void actionPerformed (ActionEvent e)
    {
      String action_command = e.getActionCommand ();

      if (action_command.equals (COMPILE))
	this.compile_movies ();
      else if (action_command.equals (SELECT))
	{
	  String selected_movie = this.movies_selector_.getSelectedItem ();
	  this.select_movie (selected_movie);
	}
      else if (action_command.equals (CLOSE))
	{
	  this.shutdown ();
	}
      else
	this.select_movie (action_command);
    }
  
  private void query_trader (String constraint,
			     String preference,
			     SpecifiedProps desired_props,
			     OfferSeqHolder offers,
			     OfferIteratorHolder offer_itr)
    {
      String ior = null;
      Policy[] policies = new Policy[5];
      Any boolean_true = this.orb_.create_any ();
      Any cardinality = this.orb_.create_any ();
      Any follow_option_any = this.orb_.create_any ();
      FollowOption follow_option_rule = FollowOption.always;
      PolicyNameSeqHolder limits_applied = new PolicyNameSeqHolder ();

      desired_props.all_(true);
      cardinality.insert_ulong (60);
      boolean_true.insert_boolean (false);
      FollowOptionHelper.insert (follow_option_any, follow_option_rule);
      policies[0] = new Policy ("exact_type_match", boolean_true);
      policies[1] = new Policy ("link_follow_rule", follow_option_any);
      policies[2] = new Policy ("return_card", cardinality);
      policies[3] = new Policy ("search_card", cardinality);
      policies[4] = new Policy ("match_card", cardinality);
      
      try
	{
	  // Compile list of names
	  this.lookup_.query (SERVICE_TYPE,
			      constraint,
			      preference,
			      policies,
			      desired_props,
			      10,
			      offers,
			      offer_itr,
			      limits_applied);

	  //	  this.dump_all_properties (offers.value, offer_itr.value);	  
	}
      catch (Exception excp)
	{
	  System.err.println (excp);
	}
    }

  private void load_movie (String ior, String movie_name)
    {
      try
	{
	  InetAddress inet_addr = InetAddress.getLocalHost ();
	  // Open a socket to the controller process of the video client.
	  //	  Socket controller_sock = new Socket (inet_addr,
	  //this.port_);
	  Socket controller_sock = new Socket ("merengue", this.port_);	  
	  OutputStream controller_stream = controller_sock.getOutputStream ();
	  //	  DataOutputStream dos = new DataOutputStream (controller_stream);
	  
	  // Write the demultiplexing key for this controller operation:
	  controller_stream.write (JAVA_INIT);
	  //	  dos.writeByte (JAVA_INIT);

	  // Write the length and contents of the ior string.
	  controller_stream.write (ior.length ());
	  System.out.println ("Writing: " + ior.length ());  
	  controller_stream.write (ior.getBytes ());
	  System.out.println ("Writing: " + ior.getBytes ());
	  //dos.writeChars (ior);
	  
	  // Write the length and contents of the ior string.
	  controller_stream.write (movie_name.length ());
	  controller_stream.write (movie_name.getBytes ());
	  //dos.writeChars (movie_name);

	  // Close the socket.
	  //controller_sock.close ();
	}
      catch (Exception excp)
	{
	  System.out.println ("Failed to load movie.");
	  System.out.println (excp);
	}
    }

  private void dump_all_properties (Offer[] offers,
				    OfferIterator offer_iterator)
    {
      try
	{
	  this.dump_properties (offers);

	  if (offer_iterator != null)
	    {
	      OfferSeqHolder iter_offers = new OfferSeqHolder ();
	      System.out.println ("*** Dumping Properties in Iterator.");
	      
	      boolean more_in_iter = true;
	      while (more_in_iter)
		{
		  System.out.println ("*** Fetching another batch of offers.");
		  more_in_iter = offer_iterator.next_n (10, iter_offers);
		  
		  System.out.println ("*** Length: " + iter_offers.value.length);
		  this.dump_properties (iter_offers.value);
		}
	    }
	}
      catch (DPEvalFailure exception)
	{
	  System.out.println (exception);
	}
    }
  
  private void dump_properties (Offer[] offer_seq)
    throws DPEvalFailure
    {
      for (int i = 0; i < offer_seq.length; i++)
	{
	  Property_Evaluator prop_eval = new Property_Evaluator (offer_seq[i]);

	  for (int j = 0; j < offer_seq[i].properties.length; j++)
	    {
	      System.out.println ("Property: " +
				  offer_seq[i].properties[j].name +
				  "  Value: " +
				  prop_eval.property_value (j, this.orb_));
				  //prop_eval.property_to_string (j, this.orb_));
	    }
	  
	  System.out.println ("----------------------------");
	}
    }

  private void extract_movies (Offer offer)
    {
      Hashtable prop_map = new Hashtable ();

      prop_map.put (REFERENCE, offer.reference);
      for (int i = 0; i < offer.properties.length; i++)
	{
	  Property property = offer.properties[i];
	  String property_name = property.name;

	  // Use a hashtable to map property names to their values.
	  prop_map.put (property_name, property);
	  System.out.println ("Found property " + property_name);
	  if (property_name.equals (MOVIE_INFO))
	    {
	      try
		{
		  Property_Evaluator prop_eval = new Property_Evaluator (offer);
		  Any tmp = prop_eval.property_value (i, this.orb_);
		  TAO_VR.Movie[] movie_info = TAO_VR.Movie_InfoHelper.extract (tmp);
		  
		  // In the top level hashtable, a movie name is mapped to
		  // a vector where each element is reserved for an offer
		  // that has that movie. In that element is the
		  // aforementioned interior hashtable. Pretty damn
		  // complicated. How did I think of this? 
		  for (int j = 0; j < movie_info.length; j++)
		    {
		      Vector movie_vector = null;
		      movie_vector = (Vector) this.movies_.get (movie_info[j].name_);
		      
		      if (movie_vector == null)
			{
			  System.out.println ("Adding movie to map " + movie_info[j]);
			  movie_vector = new Vector ();
			  this.movies_.put (movie_info[j].name_, movie_vector);
			}

		      movie_vector.addElement (prop_map);
		      prop_map.put (movie_info[j].name_, movie_info[j]);
		    }		  
		}
	      catch (DPEvalFailure dpeval)
		{
		  System.out.println (dpeval);
		}
	    }
	}
    }

  private void generate_html (String movie_name)
    throws IOException
    {
      String first_url = null;
      Vector movie_offers = (Vector) this.movies_.get (movie_name);      
      FileOutputStream fos = new FileOutputStream (STATS_FILE);
      PrintWriter pwr = new PrintWriter (fos);
      
      pwr.println ("<html>");
      pwr.println ("<BODY bgcolor=\"FFFFFF\" BACKGROUND=\"000000\"");
      pwr.println ("TEXT= \"000055\" VLINK=\"0000BF\" LINK=\"BF0000\">");
      pwr.println ("<b> " + movie_name + " Availability: </b><p>");

      for (int i = 0; i < movie_offers.size (); i++)
	{
	  Hashtable offer_props = (Hashtable) movie_offers.elementAt (i);
	  Property name_prop = (Property) offer_props.get (SERVER_NAME);
	  TAO_VR.Movie movie_info = (TAO_VR.Movie) offer_props.get (movie_name);		  
	  String server_name = name_prop.value.extract_string  ();
	  
	  pwr.println ("<b>Server: " + server_name + "</b><br>");	  
	  if (i == 0)
	    first_url = movie_info.description_;

	  pwr.println ("<font size =-1>");
	  pwr.println ("<tt> <a href=\"" +
		       movie_info.description_ +
		       "\" Target=\"main\"\"> " +  
		       movie_info.description_ +
		       "</a></tt><p>");
	  pwr.println ("<li> Filename: <tt>" +
		       movie_info.filename_ + "</tt><br>"); 
	  pwr.println ("<li> Format: <tt></tt> <br>");
	  pwr.println ("<li> File size: <tt></tt> <br>");
	  pwr.println ("<li> Duration: <tt></tt> <br>");
	  pwr.println ("<li> Frame Rate: <tt></tt> <br>");
	  pwr.println ("<li> Frame Size: <tt></tt> <br>");
	  pwr.println ("</font>");
	  pwr.println ("<p>");
	}
      
      pwr.println ("</html>");
      pwr.close ();

      fos = new FileOutputStream (HTML_FILE);
      pwr = new PrintWriter (fos);
      
      pwr.println ("<html>");
      pwr.println ("<head>");
      pwr.println ("<title> " +  movie_name + " </title>");
      pwr.println ("</head>");

      pwr.println ("<FRAMESET COLS=\"250,*\" FRAMEBORDER=\"yes\" BORDER=\"1\" BORDERCOLOR=\"#000000\">");
      pwr.println ("<FRAME SRC=\"file:" + STATS_FILE + "\"");
      pwr.println ("NAME=\"menu\" NORESIZE SCROLLING=\"auto\" MARGINWIDTH=0>");
      pwr.println ("<FRAME SRC=\"" + first_url + "\"");
      pwr.println ("NAME=\"main\" NORESIZE SCROLLING=\"auto\" MARGINWIDTH=0>");
      pwr.println ("</FRAMESET>");
      pwr.println ("</html>");
      pwr.close ();
    }

  private void shutdown ()
    {
      Runtime runtime = Runtime.getRuntime ();
      this.dispose ();
      runtime.exit (0);
    }
  
  public static void main (String[] args)
    {
      int port = 6676;
      
      if (args.length > 1)
	port = Integer.parseInt (args[0]);
      
      // Initialize the ORB
      java.util.Properties props = null;
      ORB orb = ORB.init (args, props);
      
      // Resolve the Trading Service
      Lookup lookup = TS_Resolve.resolve_trading_service (orb);
      
      if (lookup != null)	
	{
	  Discover_Server ds = new Discover_Server (orb, lookup, port);	  

	  ds.setBounds (20, 20, 320, 350);	  
	  ds.show ();
	}
    }
}
