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
import java.beans.*;

public class DemoCore extends Frame {

  private static final int MAX_VIS_COMPS = 10;

  private PushConsumerFactory pushConsumerFactory_;
  private DataHandler dataHandler_;
  private java.util.Vector vis_comp_list_;
  private int countVisComp_ = 0;
  private GridBagLayout gridbag_;
  private GridBagConstraints constraints_;


  DemoCore () {
    super ();

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
    pushConsumerFactory_ = new PushConsumerFactory (dataHandler_);

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
    
    addConnection ("Cpu_UsageObservable");
  }
  
  public boolean addConnection (String selected) {	
    // to not fill too many into it
    if (countVisComp_ < MAX_VIS_COMPS) {
      
      // get a reference to the Observable
      DemoObservable observable_ = dataHandler_.getObservable (selected);

      if (observable_ != null) {

	ClassLoader classLoader_ = this.getClass().getClassLoader();	

	VisComp visComp_ = null;
	String visCompName_ = "VisComp";
	switch (observable_.getProperty ()) {
	case Properties.DOUBLE:
	  visCompName_ = "Double"+visCompName_;
	  break;
	case Properties.NAVIGATION:
	  visCompName_ = "Navigation"+visCompName_;
	  break;
	case Properties.WEAPONS:
	  visCompName_ = "Weapons"+visCompName_;
	  break;
	default: return false;
	}
	try {
	  System.out.println ("Trying to connect: " + visCompName_+ " .. ");
	  visComp_ = (VisComp) Beans.instantiate (classLoader_, visCompName_);
	  visComp_.setName (selected);
	  System.out.println ("Connected: " + visCompName_);
	}
	catch (Exception e) {
	  System.out.println ("Unable to load JavaBean: " + e);
	  return false;
	}
	if (visComp_ != null) {
	  vis_comp_list_.addElement (visComp_);	 
	  
	  // connect the Observer with the Observable
	  observable_.addObserver (visComp_);
	  
	  countVisComp_++;
	  
	  if (countVisComp_ == 3){
	    constraints_.gridwidth = GridBagConstraints.REMAINDER;
	  }
	  if (countVisComp_ > 3) {
	    constraints_.gridwidth = 1;
	  }
	  
	  gridbag_.setConstraints ((java.awt.Component) visComp_, constraints_);
	  
	  // add the Visualization Component to the Frame
	  DemoCore.this.add ((java.awt.Component) visComp_);
	
	  System.out.println ("Connected the consumer with the visualization component!");
	  DemoCore.this.show ();
	  
	  return true;
	}
      }
    }
    return false;
  }
  
  public void init () {

  }

  public void paint (Graphics g) 
    {
    }

  public void run () {

    // Wait passive until events come in
    pushConsumerFactory_.run ();

  } 
}
  
  
