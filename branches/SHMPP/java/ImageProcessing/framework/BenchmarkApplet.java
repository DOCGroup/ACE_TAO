package imaging.framework;

import JACE.Timers.ProfileTimer;
import imaging.filters.*;
import java.awt.*;
import java.awt.image.*;
import java.applet.*;
import java.net.*;
import java.util.*;
import java.io.*;
import gjt.*;

public class BenchmarkApplet extends Applet implements Runnable
{
  public static final Font GLOBAL_FONT = new Font("Dialog", Font.BOLD, 10);
  private static final String NEW_LINE = System.getProperty("line.separator");
  private static final String WELCOME = "Welcome to MedJava Image Processing Benchmarking Tool";
  private static final String COPYRIGHT = "(c) 1997 Distributed Object Computing Group, Washington Univesity";
  private static final String ABORT = "Image Processing Tests Aborted";
  private static final String STOP = "Stop";
  private static final String START= "Start";
  private static final String CLEAR= "Clear";
  private static final String RANDOM= "Create Random Image";
  private static final int INTERNAL_ITERATIONS = 10;
  private static final int TRIALS = 1;
  
  private List images_, filters_;
  private TextArea console_;
  private TextField new_image_;
  private Button start_button_;
  private Button stop_button_;
  private Button clear_button_;
  private Button random_button_;
  private ButtonPanel button_panel_ = new ButtonPanel();
  private Hashtable filter_table_ = new Hashtable();
  private Hashtable image_table_ = new Hashtable();
  private Thread benchmarker_;
  private long elapsed_time_;
  private SpatialFilter filter_;
  private Object monitor_ = new Object();
  
  public void init()
    {
      Panel control_panel = new Panel();
      Panel sub_panel1 = new Panel();
      Panel filter_panel = new Panel();
      Panel image_panel = new Panel();
      Label image_panel_label = new Label("Images");
      Label filter_panel_label = new Label("Filters");

      new_image_ = new TextField();
      console_ = new TextArea();
      images_ = new List();
      filters_ = new List();

      console_.setEditable(false);
      images_.setMultipleSelections(true);
      filters_.setMultipleSelections(true);
      console_.setFont(GLOBAL_FONT);
      images_.setFont(GLOBAL_FONT);
      filters_.setFont(GLOBAL_FONT);
      image_panel_label.setFont(GLOBAL_FONT);
      filter_panel_label.setFont(GLOBAL_FONT);
      
      console_.appendText(WELCOME + NEW_LINE);
      console_.appendText(COPYRIGHT + NEW_LINE);
      
      image_panel.setLayout(new BorderLayout());
      image_panel.add("North", image_panel_label);
      image_panel.add("Center", images_);
      image_panel.add("South", new_image_);

      filter_panel.setLayout(new BorderLayout());
      filter_panel.add("North", filter_panel_label);
      filter_panel.add("Center", filters_);

      sub_panel1.setLayout(new GridLayout(1, 2, 5, 5));
      sub_panel1.add(filter_panel);
      sub_panel1.add(image_panel);

      control_panel.setLayout(new GridLayout(2, 1, 5, 5));
      control_panel.add(sub_panel1);
      control_panel.add(console_);
      
      setLayout(new BorderLayout());

      stop_button_ = button_panel_.add(STOP);
      start_button_ = button_panel_.add(START);
      clear_button_ = button_panel_.add(CLEAR);
      random_button_ = button_panel_.add(RANDOM);

      stop_button_.disable();
      add("Center", control_panel);
      add("South", button_panel_);

      getFilters();
      getImages();      
    }
  
  synchronized public void run()
    {
      Image image;
      String image_name, filter_name;
      int image_height, image_width;
      String[] image_list = images_.getSelectedItems();
      String[] filter_list = filters_.getSelectedItems();
      ImageProducer image_source;
      FilteredImageSource filtered_image;
      //      BenchmarkFrame bframe = BenchmarkFrame.instance();
      
      double cumulative_time;
      double best_time;

      SpatialFilter.setTimer(INTERNAL_ITERATIONS);
      console_.appendText(NEW_LINE);
      console_.appendText("Benchmark tests:  [mem=" +
			  Runtime.getRuntime().freeMemory() +"/" +
			  Runtime.getRuntime().totalMemory() +"]"
			  + NEW_LINE);
      
      for (int i = 0; i < image_list.length; i++)
	{

	  image_name = image_list[i];
	  image = (Image)image_table_.get(image_name);
	  
	  image_width = image.getWidth(this);
	  image_height = image.getHeight(this);
	  
	  console_.appendText("Begining tests for image " + image_name);
	  console_.appendText(" (size " + image_width + "x" + image_height +
			      " = " +  image_height*image_width*4 + " bytes )");
	  console_.appendText(NEW_LINE);

	  console_.appendText(NEW_LINE + " ********** " + NEW_LINE);
	  //	  bframe.init("Begining tests for image " + image_name, image, image_width, image_height);

	  //	  if (! bframe.isShowing())
	  //	    bframe.show();
	  
	  for (int j = 0; j < filter_list.length; j++)
	    {
	      filter_name = filter_list[j];
	      filter_ = (SpatialFilter)filter_table_.get(filter_name);

	      if (filter_ == null)
		System.err.println(filter_name + " is null");
	      
	      best_time = Double.MAX_VALUE;
	      cumulative_time = 0;
	      image_source = image.getSource();
	      filtered_image = new FilteredImageSource(image.getSource(), filter_);

	      for (int l = 0; l < TRIALS; l++)
		{
		  elapsed_time_ = 0;
		  prepareImage(createImage(filtered_image), this);

		  try
		    {
		      //		      synchronized(monitor_)
		      //			{
			  while (elapsed_time_ == 0)
			    {
			      System.out.println("Waiting for image loading to complete.");
			      //			      monitor_.wait();
			      wait();
			    }

			  System.out.println("Image loading has completed.");
			  //			}
		    }
		  catch(Exception excp)
		    {
		      System.out.println(excp);
		      stop();
		    }
		  
		  cumulative_time = (double)elapsed_time_ / (double)INTERNAL_ITERATIONS;		      
		  System.out.println(image_name + " " + filter_name + " " +  elapsed_time_);

		  if (best_time > cumulative_time)
		    best_time = cumulative_time;
		}
	      
	      console_.appendText(filter_name + ": best time from " +
				  TRIALS + " trials of " + INTERNAL_ITERATIONS +
				  " iterations equals: " + best_time + " ms");
	      console_.appendText(NEW_LINE);
	      Thread.yield();
	    }
	  
	  console_.appendText(NEW_LINE);
	}

      console_.appendText("Tests concluded." + NEW_LINE);

      //      if (bframe.isShowing())
      //	bframe.hide();
      
      stop_button_.disable();
      start_button_.enable();
    }

  synchronized public boolean imageUpdate(Image img, int infoflags, int x, int y,
			     int width, int height)
    {
      System.out.println("Image Update called");
      
      boolean return_value = false;
      try
	{
	  if ((infoflags & ALLBITS) != 0 ||
	      (infoflags & SOMEBITS) != 0)
	    {
	      System.out.println("notify called" + width + " " + height);
	      elapsed_time_ = filter_.filterTime();
	      return_value = true;
	      //	      monitor_.notify();
	      notifyAll();
	    }
	  else if ((infoflags & ImageObserver.ABORT) != 0 ||
		   (infoflags & ERROR) != 0)
	    {
	      System.out.println("image error!");
	      //	      stop();
	      return_value = true;
	    }
	}
      catch(SecurityException excp)
	{
	  System.err.println(excp);
	}

      return return_value;
    }
  
  public void stop()
    {
      try
	{
	  if (benchmarker_.isAlive())
	    {
	      BenchmarkFrame bframe = BenchmarkFrame.instance();

	      if (bframe.isShowing())
		bframe.hide();
	      
	      System.out.println("Stopping tests...");
	      benchmarker_.stop();
	      console_.appendText(NEW_LINE + ABORT + NEW_LINE);	      
	    }
	  
	  start_button_.enable();
	  stop_button_.disable();
	}
      catch(SecurityException exception)
	{
	  console_.appendText(NEW_LINE + "Stop Failed." + NEW_LINE);
	}      
    }

  public boolean action(Event evt, Object what)
    {
      if (what.equals(START))
	{
	  boolean proceed = ! (benchmarker_ != null && benchmarker_.isAlive());
	  
	  stop_button_.enable();
	  
	  if (proceed &&
	      images_.getSelectedIndexes() != null &&
	      filters_.getSelectedIndexes() != null)
	    {
	      System.out.println("Starting tests...");
	      start_button_.disable();
	      benchmarker_ = new Thread(this);
	      benchmarker_.setPriority(Math.min(Thread.currentThread().getPriority() + 2,
						Thread.MAX_PRIORITY - 1));
	      benchmarker_.start();
	    }
	}
      else if (what.equals(STOP))
	{
	  stop();
	}
      else if (what.equals(CLEAR))
	{
	  console_.setText("");
	  console_.appendText(WELCOME + NEW_LINE);
	  console_.appendText(COPYRIGHT + NEW_LINE);	  
	}
      else if (what.equals(RANDOM))
	{
	  RandomImageDialog.instance(this).show();
	}	
      else if (evt.target == new_image_)
	{
	  getSingleImage(new_image_.getText());
	}
	

      return true;
    }

  private void getSingleImage(String image_url)
    {
      Image image;
      MediaTracker tracker = new MediaTracker(this);

      try
	{
	  image = getImage(new URL(image_url));
	  tracker.addImage(image, 0);
	  tracker.waitForID(0);
	      
	  if (image != null)
	    {
	      int index = image_url.lastIndexOf('/');
	      
	      if (index != -1)
		image_url = image_url.substring(index + 1);
	      
	      image_table_.put(image_url, image);
	      images_.addItem(image_url);
	    }
	}
      catch(MalformedURLException e)
	{
	  System.err.println(e);
	}
      catch(InterruptedException excp)
	{
	  System.err.println(excp);
	}

    }
  
  private void getImages()
    {
      Image image;
      String image_list = getParameter("images"), image_url;
      StringTokenizer str_tok = new StringTokenizer(image_list, ",");

      while (str_tok.hasMoreTokens())
	{
	  image_url = str_tok.nextToken();
	  getSingleImage(image_url);
	}
    }
  
  private void getFilters()
    {
      URL url;
      Object download;
      String config_file;
      String configInfo= null;
      Choice choice = new Choice ();
      ImageFilter filter;
      
      config_file = getParameter ("configFile");
      
      if (config_file == null)
	config_file = "http://www.cs.wustl.edu/~pjain/java/ACE_wrappers/java/ImageProcessing/framework/filter.conf";
      
      try
	{
	  System.out.println ("Configuration File: " + config_file);
	  // Create a new URL
	  url = new URL (config_file);
	  
	  // Get the input stream and pipe it to a DataInputStream
	  DataInputStream iStream = new DataInputStream (url.openStream ());
	  
	  // Create a buffer to hold all the data we get
	  StringBuffer tempBuf = new StringBuffer ();
	  // Keep reading the data until we are done
	  String tempString = iStream.readLine ();
	  while (tempString != null)
	    {
	      tempBuf.append (tempString);
	      tempBuf.append (" ");
	      tempString = iStream.readLine ();
	    }
	  
	  configInfo = tempBuf.toString ();	  
	  System.out.println(configInfo);
	  
	  if (configInfo != null)
	    {
	      StringTokenizer tokens = new StringTokenizer (configInfo);
	      String fullFilterName = null;
	      String filterName = null;
	      // Now parse the string, picking up filter names. Use these
	      // names to load the actual filters as well add new choices to
	      // the filter choices.
	      while (tokens.hasMoreTokens ())
		{
		  // Get the next token
		  fullFilterName = tokens.nextToken ();
		  filterName = this.extractFilterName (fullFilterName);
		  
		  System.out.println ("Loading: " + fullFilterName);
		  // Load the filter class
		  Class c = Class.forName (fullFilterName);
		  //		Class c = this.filterRepository_.load (filter);

		  filter = (ImageFilter)c.newInstance();

		  if (filter instanceof SpatialFilter)
		    {
		      // Add the filter to the Filter Repository
		      filter_table_.put(filterName, filter);
		      
		      // Add filter name to the list of filter choices
		      filters_.addItem (filterName);
		    }
		}
	    }	  
	}
      catch (Exception e)
	{
	  System.err.println ("Filter not found: " + e);
	}      
  }

  private String extractFilterName (String s)
  {
    String filterName = null;
    StringTokenizer tokens = new StringTokenizer (s, ".");
    while (tokens.hasMoreTokens ())
      filterName = tokens.nextToken ();
    return filterName;
  }

  public void generateRandomImage(String name, int width, int height)
    {
      Image image = createImage(width, height);
      Graphics g = image.getGraphics();
      FilteredImageSource filtered_image;

      if (g != null)
	{
	  ImageFilter filter = new RandomizeFilter();
	  g.fillRect(0, 0, width, height);
	  filtered_image = new FilteredImageSource(image.getSource(), filter);
	  image_table_.put(name, createImage(filtered_image));
	  images_.addItem(name);
	}
    }
}

class RandomImageDialog extends Frame
{
  private static String GENERATE = "Generate";
  private static String CANCEL = "Cancel";
  
  private TextField name_ = new TextField(30),
  width_ = new TextField(5),
    height_= new TextField(5);
  private BenchmarkApplet applet_;
  private Button generate_button_;
  private Button cancel_button_;
  private ButtonPanel button_panel_ = new ButtonPanel();
  
  private static RandomImageDialog instance_;  

  public static RandomImageDialog instance(BenchmarkApplet applet)
    {
      if (instance_ == null)
	instance_ = new RandomImageDialog(applet);
      
      return instance_;	  
    }

  protected RandomImageDialog(BenchmarkApplet applet)
    {
      super("Generate Random Image");
      applet_ = applet;

      Panel controls = new Panel();
      GridBagLayout gb = new GridBagLayout();
      GridBagConstraints gc = new GridBagConstraints();
      Label width = new Label("Width: "),
	height = new Label("Height: "),
	name = new Label("Name: ");

      reshape (0, 0, 300, 200);
      
      controls.setLayout(gb);
      gc.gridx = 0;
      gc.gridy = 0;
      gc.gridwidth = 1;
      gc.insets = new Insets(5,5,5,5);
      gc.anchor = GridBagConstraints.WEST;
      width.setFont(BenchmarkApplet.GLOBAL_FONT);
      gb.setConstraints(width, gc);
      controls.add(width);

      gc.gridx = 1;
      gc.anchor = GridBagConstraints.CENTER;
      gc.fill = GridBagConstraints.HORIZONTAL;
      gb.setConstraints(width_, gc);
      controls.add(width_);
      
      gc.gridx = 2;
      gc.anchor = GridBagConstraints.WEST;
      gc.fill = GridBagConstraints.NONE;
      height.setFont(BenchmarkApplet.GLOBAL_FONT);
      gb.setConstraints(height, gc);
      controls.add(height);

      gc.gridx = 3;
      gc.anchor = GridBagConstraints.CENTER;
      gc.fill = GridBagConstraints.HORIZONTAL;
      gb.setConstraints(height_, gc);
      controls.add(height_);

      gc.gridx = 0;
      gc.gridy = 1;
      gc.gridwidth = 1;
      gc.fill = GridBagConstraints.NONE;
      gc.anchor = GridBagConstraints.WEST;
      name.setFont(BenchmarkApplet.GLOBAL_FONT);
      gb.setConstraints(name, gc);
      controls.add(name);

      gc.gridx = 1;
      gc.gridwidth = GridBagConstraints.REMAINDER;
      gc.anchor = GridBagConstraints.CENTER;
      gc.fill = GridBagConstraints.HORIZONTAL;
      gb.setConstraints(name_, gc);
      controls.add(name_);
      
      setLayout(new BorderLayout());
      generate_button_ = button_panel_.add(GENERATE);
      cancel_button_ = button_panel_.add(CANCEL);
      add("Center", controls);
      add("South", button_panel_);

    }

  public boolean action(Event evt, Object what)
    {
      if (what.equals(GENERATE))
	{
	  String width = width_.getText(),
	    height = height_.getText(),
	    name = name_.getText();
	  
	  try
	    {
	      int width_num = Integer.parseInt(width),
		height_num = Integer.parseInt(height);

	      applet_.generateRandomImage(name, width_num, height_num);
	      dispose();		      	      
	    }
	  catch(NumberFormatException nfe)
	    {
	      width_.setText("");
	      height_.setText("");
	    }
	}
      else if (what.equals(CANCEL))
	dispose();

      return true;
    }  
}

class BenchmarkFrame extends Frame
{
  private String desc_;
  private Image image_;
  private int height_, width_;
  private static BenchmarkFrame instance_;
  private static final Font FONT = new Font("Dialog", Font.BOLD, 12);
  
  public static BenchmarkFrame instance()
    {
      if (instance_ == null)
	instance_ = new BenchmarkFrame();
      
      return instance_;	  
    }
  
  public void init(String desc, Image image, int width, int height)
    {
      reshape(0, 0, width, height + 50);
      setImage(desc, image);
    }
  
  public void setImage(String desc, Image image)
    {/*
      try
	{
	  MediaTracker tracker = new MediaTracker(this);
	  tracker.addImage(image, 0);
	  tracker.waitForID(0);
	}
      catch(InterruptedException excp)
	{
	}
	*/     
      desc_ = desc;
      image_ = image;
    }

  public void paint(Graphics g)
    {
      g.setColor(Color.black);
      g.setFont(FONT);
      g.drawImage(image_, 0, 50, this);      
      g.drawString(desc_, 5, 40);
    }
  
  protected BenchmarkFrame()
    {
      super("Benchmark Frame");
    }
}
