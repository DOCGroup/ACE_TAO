// $Id$
//
// = FILENAME
//    DemoCore.java
//
// = AUTHOR
//    Michael Kircher (mk1@cs.wustl.edu)
//
// = DESCRIPTION
//   This class servers as the core class of the simulation demo.
//   It connects the push consumer of the event service with
//   one or several Java Beans.
//
// ============================================================================


import java.awt.*;
import java.awt.event.*;

public class DemoCore extends Frame {

  private static final int MAX_VIS_COMPS = 10;

  private PushConsumerFactory pushConsumerFactory_;
  private VisCompFactory visCompFactory_;
  private DataHandler dataHandler_;
  private java.util.Vector vis_comp_list_;
  private int countVisComp_ = 0;
  private GridBagLayout gridbag_;
  private GridBagConstraints constraints_;
  private boolean use_queueing_ = false;
  private boolean connections_established_ = false;

  DemoCore (String nameServiceIOR,
	    String nameServicePort,
            String[] args,
            boolean use_queueing,
	    java.applet.Applet applet) {
    super ();

    use_queueing = use_queueing;

    setSize (600,400);
    setBounds (new Rectangle (50,50,800,500));
    setVisible (true);

    gridbag_ = new GridBagLayout();
    constraints_ = new GridBagConstraints();
    constraints_.fill = GridBagConstraints.BOTH;
    constraints_.weightx = 1.0;
    constraints_.weighty = 1.0;
    this.setLayout (gridbag_);

    // Instantiate the DataHandler and the PushConsumer
    dataHandler_ = new NavWeapDataHandler ();
    pushConsumerFactory_ = new PushConsumerFactory (dataHandler_,
                                                    nameServiceIOR,
                                                    nameServicePort,
                                                    args,
                                                    use_queueing_,
                                                    applet);

    // List of Visualization Components
    vis_comp_list_ = new java.util.Vector();

    MenuBar menubar_ = new MenuBar ();
    Menu menu_ = new Menu ("File");
    MenuItem menu_quit_item_ = new MenuItem ("Quit");
    menu_quit_item_.setEnabled (true);
    MenuItem menu_add_item_ = new MenuItem ("Add");
    menu_add_item_.setEnabled (true);
    menu_.add (menu_add_item_);
    menu_.add (menu_quit_item_);
    menubar_.add (menu_);

    setMenuBar (menubar_);

    menu_quit_item_.addActionListener (new ActionListener ()
				       {
					 public void actionPerformed (ActionEvent e) {
					   System.exit (0);
					 }
				       });

    menu_add_item_.addActionListener (new ActionListener ()
				       {
					 public void actionPerformed (ActionEvent e) {
					   ObservablesDialog obsDialog_ =
					     new ObservablesDialog (DemoCore.this,
								    dataHandler_.getObservablesList());
					   obsDialog_.addAnswerListener (new AnswerListener ()
									{
									  public void ok (AnswerEvent e)
									    {
									      DemoCore.this.addConnection (e.selected_);
									    }
									});
					   obsDialog_.setVisible (true);
					 }
				       });

    // instantiate the Factory for Visualization Components
    visCompFactory_ = new VisCompFactory ();

    // Traverse the args looking for switches that determine connections.
    int arg_index = 0;
    while (args.length > arg_index)
      {
        if (args[arg_index].equals ("-dualECdemo") ||
            args[arg_index].equals ("-dualECdemo1") ||
            args[arg_index].equals ("-dualECdemo2"))
          {
            // Use monotonic scales in the double precision data windows
            DoubleVisComp.monotonic_scale (true);

            // Establish connections
            if (! connections_established_)
              {
                connections_established_ = true;
                addConnection ("Weapons");
                addConnection ("Weapons Latency (100 ns)");
                addConnection ("Weapons Latency Jitter (100 ns)");
                addConnection ("Navigation");
                addConnection ("Navigation Latency (100 ns)");
                addConnection ("Navigation Latency Jitter (100 ns)");
                break;
              }
          }
        if (args[arg_index].equals ("-dualECdemo") ||
            args[arg_index].equals ("-dualECdemo1") ||
            args[arg_index].equals ("-dualECdemo2"))
          {
            // Use monotonic scales in the double precision data windows
            DoubleVisComp.monotonic_scale (true);

            // Establish connections
            if (! connections_established_)
              {
                connections_established_ = true;
                addConnection ("Weapons");
                addConnection ("Weapons Latency (100 ns)");
                addConnection ("Weapons Latency Jitter (100 ns)");
                addConnection ("Navigation");
                addConnection ("Navigation Latency (100 ns)");
                addConnection ("Navigation Latency Jitter (100 ns)");
                break;
              }
          }

        else if (args[arg_index].equals ("-PRdemo"))
          {
            // Establish connections
            if (! connections_established_)
              {
                connections_established_ = true;
                addConnection ("High Consumer Persian Recursion", 2);
                addConnection ("Low Consumer Persian Recursion", 2);
		// addConnection ("High Consumer Execution Time (100 ns)", 2);
		// addConnection ("Low Consumer Execution Time (100 ns)", 2);
                break;
              }
          }

        // Skip over anything else.
        else
          {
            arg_index ++;
          }
      }


    // If connections have not been established, set up defaults
    if (! connections_established_)
      {
        connections_established_ = true;
        addConnection ("Weapons");
        addConnection ("Weapons Latency (100 ns)");
        addConnection ("Navigation");
        addConnection ("Navigation Latency (100 ns)");
      }
  }


  public boolean addConnection (String selected, int max_in_a_row) {
		// to not fill too many into it
    if (countVisComp_ < MAX_VIS_COMPS) {

      // get a reference to the Observable
      DemoObservable observable_ = dataHandler_.getObservable (selected);

      if (observable_ != null) {

        VisComp visComp_ = visCompFactory_.getNewVisComp (observable_.getProperty (), selected);

	if (visComp_ != null) {
	  vis_comp_list_.addElement (visComp_);

	  // connect the Observer with the Observable
	  observable_.addObserver (visComp_);

	  countVisComp_++;

          // not more than three in a row
	  if (countVisComp_ == max_in_a_row){
	    constraints_.gridwidth = GridBagConstraints.REMAINDER;
          }
          if (countVisComp_ > max_in_a_row) {
	    constraints_.gridwidth = 1;
	  }

	  gridbag_.setConstraints ((java.awt.Component) visComp_, constraints_);

	  // add the Visualization Component to the Frame
	  DemoCore.this.add ((java.awt.Component) visComp_);
	  DemoCore.this.show ();

          return true;
        }
      }
    }
    return false;
  }

  public boolean addConnection (String selected) {
    return addConnection (selected, 3);
  }

  public void init ()
    {
    }

  public void paint (Graphics g)
    {
    }

  public void run ()
    {
      // Wait passive until events come in
      pushConsumerFactory_.run ();
    }
}


