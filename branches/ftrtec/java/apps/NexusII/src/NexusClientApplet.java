/* 
   $RCSfile$
   $Author$

   Last Update: $Date$
   $Revision$

   REVISION HISTORY:
   $Log$
   Revision 1.1  1997/01/31 01:10:59  sumedh
   Added the Nexus II source code files.

# Revision 1.3  1996/12/06  07:25:29  rajeev
# Got SetRoomName to work -- needed for LROOMS
#
# Revision 1.2  1996/12/06  04:48:21  rajeev
# added handling for l_users and L_room packets from  server
#
# Revision 1.1  1996/12/06  03:37:22  rajeev
# Initial revision
#

   This is the client applet 
   Part of Nexus-II project !!
   Copyright : Aravind Gopalan, Rajeev Bector, Sumedh Mungee
*/

// package NexusII; 

// I hate packages

import java.applet.Applet;
import java.awt.* ; 
import java.io.*;
import java.net.*;
import java.util.*;

//import NexusII.networking.* ; 
//import NexusII.client.* ;
//import NexusII.util.* ; 

// get the awtCommand package now 
import awtCommand.* ; 

// The applet runs the root thread -- easy access to graphics now ... 

public class NexusClientApplet extends CApplet implements consts,Runnable {

  // Who am I  -- will be modified if the /nick command is given 
  public static String myName = new String("anonymous"); 

  /* All Event Handlers */ 

  // To check if applet is connected
  boolean connected = false;

  // The connector handler  
  connectionHandler nexusConnector ;  

  // join handler 
  joinHandler  nexusJoiner ; 

  // command handler 
  commandParser nexusParser ; 

  commandHandler commandLineHandler ; 
  // Streams for writing and reading from Socket 
  DataOutputStream outStream;
  DataInputStream  inStream;

  // Thread which does everything for the applet 
  Thread mainThread ; 

  // this is just a visit 
  // host where I live actually ;-)
  String host;
  
  // Which server to connect to 
  Integer serverPort ; 

  // Where are the queues for reading and writing
  // reader and writers will access these and also the interpretor and dep.

  MT_Bounded_Queue read_q ; 
  MT_Bounded_Queue write_q ; 
  MT_Bounded_Queue root_q  ; 
  // Where Aravind writes his commands 
  MT_Bounded_Queue command_q ; 




  // ***********************************************************
  // Graphics objects
  // ***********************************************************

  // Total applet size
  int minWidth_ = 500;
  int minHeight_ = 450;

  // State variables needed to be maintained
  boolean connected_ = false;
  int numRooms_ = 0;

  // Buttons
  private CButton bConn; 
  private CButton bJoin; 
  private CButton bLeave; 
  private CButton bRooms;
  private CButton bUsers;

  private CTextField tfCommandLine = new CTextField(80);
  private CTextField tfRoomName = new CTextField(16);
  private List lRooms = new List(10,false);
  private Label roomLabel = new Label("Rooms:");
  private List lUsers = new List(10,false);
  private Label userLabel = new Label("Users:");

//  private RoomSpace roomspace = new RoomSpace();
  private Panel roomspace = new Panel();
    
  public Dimension preferredSize() {
      return minimumSize();
  }

  public Dimension minimumSize() {
      return new Dimension(minWidth_, minHeight_);
  }

  // -----------------------------------------------------------------
  // Basically draws up all the components (not sub-components) of the
  // Applet window ...
  // Pretty much straightforward barring quirks of AWT -- ArGo
  void SetUpButtons(GridBagLayout gbl, GridBagConstraints gbc) { 
    // The Connect Button
    gbc.weightx = 1.0;
    gbc.weighty = 0.5;
    gbc.gridx = 2;
    gbc.gridy = 1;
    gbc.fill = GridBagConstraints.HORIZONTAL;
    gbc.anchor = GridBagConstraints.CENTER;
    gbc.ipady = 5;
    bConn = new CButton("Connect");
    bConn.setActionCommand(nexusConnector); 
    gbl.setConstraints(bConn, gbc);
    add(bConn);
    gbc.ipady = 0;

    // The remaining buttons
    gbc.fill = GridBagConstraints.BOTH;
    gbc.anchor = GridBagConstraints.CENTER;
    gbc.weightx = 1.0;
    gbc.weighty = 0.5;
    gbc.insets = new Insets(2,2,2,2);
    gbc.ipadx = 2;
    gbc.ipady = 2;

    // Join Button
    gbc.gridx = 0;
    gbc.gridy = 3;
    bJoin = new CButton("Join");
    gbl.setConstraints(bJoin,gbc);
    add(bJoin);

    // Leave Button
    gbc.gridx = 1;
    gbc.gridy = 3;
    bLeave = new CButton("Leave");
    gbl.setConstraints(bLeave,gbc);
    add(bLeave);

    // List Rooms Button
    gbc.gridx = 0;
    gbc.gridy = 4;
    bRooms = new CButton("List Rooms");
    gbl.setConstraints(bRooms,gbc);
    add(bRooms);

    // List Users Button
    gbc.gridx = 1;
    gbc.gridy = 4;
    bUsers = new CButton("List Users");
    gbl.setConstraints(bUsers,gbc);
    add(bUsers);

  }   // of SetUpButtons 

  void SetUpGraphics() { 
    
    resize(preferredSize());

    GridBagLayout gbl = new GridBagLayout();
    GridBagConstraints gbc = new GridBagConstraints();
    setFont(new Font("Helvetica", Font.PLAIN, 14));
    setLayout (gbl);

    gbc.weightx = 1.0;
    gbc.weighty = 1.0;

    // For the list of rooms
    // First the Room list label 
    gbc.weightx = 0;
    gbc.weighty = 0;
    gbc.gridx = 0;
    gbc.gridy = 0;
    gbc.anchor = GridBagConstraints.SOUTHWEST;
    gbc.fill = GridBagConstraints.NONE;
    gbl.setConstraints(roomLabel,gbc);
    add(roomLabel);
    // Now the Room combo -- list + text field
    // the actual list
    gbc.weightx = 1.0;
    gbc.weighty = 1.0;
    gbc.gridy = 1;
    gbc.fill = GridBagConstraints.VERTICAL;
    gbc.anchor = GridBagConstraints.NORTH;
    // gbc.insets = new Insets(0,2,0,2);
    gbl.setConstraints(lRooms,gbc);
    add(lRooms);
    if (DEBUG)
      lRooms.addItem("Room_1");
    // The text field
    gbc.weightx = 0;
    gbc.weighty = 0;
    gbc.gridy = 2;
    gbc.anchor = GridBagConstraints.NORTH;
    gbc.fill = GridBagConstraints.HORIZONTAL;
    gbc.insets = new Insets(0,0,0,0);
    gbl.setConstraints(tfRoomName,gbc);
    add(tfRoomName);

    // Similarly for the list of users
    // label
    gbc.weightx = 0;
    gbc.weighty = 0;
    gbc.gridx = 1;
    gbc.gridy = 0;
    gbc.anchor = GridBagConstraints.SOUTHWEST;
    gbc.fill = GridBagConstraints.NONE;
    gbc.insets = new Insets(0,0,0,0);
    gbl.setConstraints(userLabel,gbc);
    add(userLabel);
    // The list
    gbc.weightx = 1;
    gbc.weighty = 1;
    gbc.gridy = 1;
    gbc.fill = GridBagConstraints.VERTICAL;
    gbc.anchor = GridBagConstraints.NORTH;
    gbl.setConstraints(lUsers,gbc);
    add(lUsers);
    if (DEBUG)
      lUsers.addItem("USer #1");

    // Setup all the buttons
    SetUpButtons(gbl,gbc) ; 

    // The command line
    gbc.gridx = 0;
    gbc.gridy = 5;
    gbc.ipadx = 0;
    gbc.ipady = 0;
    gbc.insets = new Insets(0,0,0,0);
    gbc.gridwidth = GridBagConstraints.REMAINDER;
    gbc.fill = GridBagConstraints.NONE;
    gbc.anchor = GridBagConstraints.CENTER;
    gbl.setConstraints(tfCommandLine,gbc);
    add(tfCommandLine);


    // We are done -- startup with all buttons except connect disabled
    OnDisconnect(); 
    return;
  }

  public void init() {
    //Get the address of the host the applet was downloaded from 
    host = getCodeBase().getHost();
    // Get servers portnumber from the home page 
    serverPort = Integer.valueOf(getParameter("serverport")); 
    

    // Create the nexusconnector object whose execute will be called later
    // on pressing the connect button 
    nexusConnector = new connectionHandler(host,serverPort,this); 

    if(DEBUG){
      System.out.println("Server Portnumber is " + serverPort + "\n");
    }

    // initialize the graphics 
    SetUpGraphics(); 
  } // of method init 

  public synchronized void start() { 
    // Aravind will create the "connect" button here and register an instance

    if (DEBUG) {
      System.out.println("In start() method.");
    }
  }


  //  Here is the stop for the applet
  //  Called when the user goes away from the page 
  public synchronized void stop() {
    if (!DEBUG) { 
      showStatus("Stop has been called ");
    }

    if (connected) {
      nexusConnector.execute(null,null,null);
    }
  }



    
  // -----------------------------------------------------------------------
  // Public Access functions
  public Hashtable Mapper() {
    return nexusJoiner.Mapper();
  }
  
  public String GetRoomName() { 
    StringTokenizer t = new StringTokenizer(tfRoomName.getText()); 
    if(t.hasMoreTokens()) { 
      return  t.nextToken(); 
    }
    else
      return "" ; 
  }
  
  public void AddRoomName(String name) {
    int count = lRooms.countItems(); 
    for (int i=0;i<count;i++) 
      if (lRooms.getItem(i).equalsIgnoreCase(name))
	return;
    lRooms.addItem(name);
  }
 
  public void SetRoomName(String name) { 
    int count = lRooms.countItems(); 
    for(int i=0;i<count;i++) { 
      if(lRooms.getItem(i).equalsIgnoreCase(name)) { 
	// Have to simulate a select event by doing the selection ourselves
	lRooms.select(i);
	tfRoomName.setText(lRooms.getSelectedItem());
	tfRoomName.selectAll();
	break ; 
      } 
    } 
  } 
  
  public void  sendNickToServer(String name) { 
    nexusJoiner.sendTestPacket("ROOT","NICK",name,nexusConnector.writer_q());
  }


  // will run the mainthread 
  public void run() { 
    if(DEBUG) { 
      System.out.println("Root thread is starting -- ");
    }

    root_q = new MT_Bounded_Queue(QUEUE_SIZE);
    nexusJoiner.Mapper().put("ROOT",root_q);
    // continously wait for something to read and then update the graphics
    // objects ----
    if(DEBUG) { 
      nexusJoiner.sendTestPacket("ROOT","LROOMS","a b c d",root_q);
    }
 
    for(;;) { 
      dataPacket rootPacket = (dataPacket)root_q.dq(); 
      if(DEBUG) { 
	System.out.println("Root got a packet --- ");
      }
      if(rootPacket.contentType().equalsIgnoreCase("LROOMS")) { 
	update_lrooms(rootPacket.content()); 
      } 
      else if(rootPacket.contentType().equalsIgnoreCase("LUSERS")) { 
	update_lusers(rootPacket.destination(),rootPacket.content()); 
      } 
    } 
    
  }
  

  
  // This method will update the graphics of lrooms 
  void update_lrooms(String data) { 
    lRooms.disable(); 
    tfRoomName.disable(); 
    lRooms.clear(); 
    
    StringTokenizer t = new StringTokenizer(data);
    while(t.hasMoreTokens()) { 
      lRooms.addItem(t.nextToken()); 
    }
    if (lRooms.countItems() != 0) {
      lRooms.select(0);
      tfRoomName.setText(lRooms.getItem(0));
    }
    tfRoomName.enable(); 
    lRooms.enable(); 
    repaint(); 
  } 

  void update_lusers(String name,String data) { 
    lUsers.disable(); 
    userLabel.setText("Users: "+ name);
    lUsers.clear();
    StringTokenizer t = new StringTokenizer(data);
    while(t.hasMoreTokens()) { 
      lUsers.addItem(t.nextToken()); 
    }
    lUsers.enable(); 
    repaint(); 
  } 
    
  // -----------------------------------------------------------------------
  // User Input handling -- ArGo

  // Function to handle Connect button being pressed
  public void OnConnect(){
    // Create the event handlers -- 
    nexusJoiner    = new joinHandler(nexusConnector.writer_q(),this); 
    nexusParser    = new commandParser(nexusConnector.writer_q(),this); 
    commandLineHandler = new commandHandler(this); 
    // start the main thread in the applets context 
    mainThread  =  new Thread(this); 
    mainThread.start(); 
    // test 
      // Graphics here -- 
    bJoin.enable();
    bJoin.setActionCommand(nexusJoiner); 
    bLeave.enable(); 
    bLeave.setActionCommand(nexusJoiner); 
    bRooms.enable();
    bRooms.setActionCommand(nexusParser); 
    bUsers.enable();
    bUsers.setActionCommand(nexusParser); 
    tfCommandLine.enable();
    tfCommandLine.setActionCommand(commandLineHandler); 
    tfRoomName.enable();
    lRooms.enable();
    lUsers.enable();
    // Change the name to "Disconnect" and ensure that update happens
    bConn.setLabel("Disconnect");
    validate();	// Don't know if this is necessary
    connected = true; // Applet is now connected
  }

  // Function to handle Connect button being pressed
  public void OnDisconnect(){
    bJoin.disable();
    bLeave.disable();
    bRooms.disable();
    bUsers.disable();
    tfCommandLine.disable();
    tfRoomName.disable();
    lRooms.disable();
    lUsers.disable();
    // Change the name back to "Connect" and ensure update happens
    bConn.setLabel("Connect");
    validate();
    // Stop the ROOT room thread
    if (connected) {
      mainThread.stop();
      connected = false;  // Applet is disconnected
    }
  }

  // Basically take care of the inputs
  public boolean handleEvent(Event event) {
    if (event.target == lRooms) {
      if (event.id == Event.LIST_SELECT) {	// selected the item
	tfRoomName.setText(lRooms.getSelectedItem());
	tfRoomName.selectAll();
	if (DEBUG)
	  System.out.println("Actually handling the select event");
	return true;
      }
    }
    return super.handleEvent(event);
  }

  public void update(Graphics g) {
    paint(g);
  }
} // of class NexusClientApplet 


  
