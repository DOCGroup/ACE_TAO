/*
   $RCSfile$
   $Author$

   Last Update: $Date$
   $Revision$

   REVISION HISTORY:
   $Log$
   Revision 1.1  1997/01/31 01:11:02  sumedh
   Added the Nexus II source code files.

# Revision 1.2  1996/12/07  06:25:18  rajeev
# backup
#
# Revision 1.1  1996/12/07  06:15:12  rajeev
# Initial revision
#


*/
import java.awt.*;
import awtCommand.*;
import java.util.* ;

//import NexusII.client.* ;
//import NexusII.util.* ;
//import NexusII.networking.*;

class RoomFrame extends CFrame implements consts {
  private static String rcsId = new String("$Id$");

  // Graphics Objects
  private CTextField tfInput ; 
  private CTextArea taOutput ;
  private CButton bLeave ;
  private ImageCanvas icOutput;
  private Font normalFont = new Font("Helvetica", Font.PLAIN, 14);
  private Font boldFont = new Font("Helvetica", Font.BOLD, 14);
  private Font italicFont = new Font("Helvetica", Font.ITALIC, 14);
  private static final int LINE_LENGTH = 70;

  // Other required objects
  private MT_Bounded_Queue write_q_ ; 
  private NexusClientApplet applet_ ; 
  private String myName_ ; 

  public RoomFrame(MT_Bounded_Queue write_q,
		   NexusClientApplet applet,
		   String name) {
    super(name);
    write_q_ = write_q ; 
    applet_  = applet ; 
    myName_  = name ; 
    SetUpGraphics();
    this.pack();
    this.show();
  }

  void SetUpGraphics() {
    // Initialize the graphics objects
    // The input text line
    tfInput = new CTextField(LINE_LENGTH);
    textHandler handlerT = new textHandler(write_q_,applet_,myName_);
    tfInput.setActionCommand(handlerT);
    // The leave button
    bLeave = new CButton(LEAVE_STR);
    bLeave.setActionCommand(applet_.nexusJoiner);
	
    icOutput = new ImageCanvas(myName_);
    taOutput = new CTextArea(10,LINE_LENGTH);

    GridBagLayout gbl = new GridBagLayout();
    GridBagConstraints gbc = new GridBagConstraints();
    setFont(normalFont);
    setLayout (gbl);

    gbc.insets = new Insets(5,5,5,5);
    gbc.ipadx  = 5;
    gbc.ipady  = 5;
    gbc.weightx = 1.0;
    gbc.weighty = 1.0;

    // First the Image so that sizes are fixed
    gbc.gridx = 1;
    gbc.gridy = 0;
    gbc.anchor = GridBagConstraints.CENTER;
    gbc.fill = GridBagConstraints.BOTH;
    gbl.setConstraints(icOutput, gbc);
    add(icOutput);

    // The Text Output Area
    gbc.gridx = 0;
    gbc.gridy = 0;
    gbc.weighty = 0.0;
    gbc.anchor = GridBagConstraints.SOUTH;
    gbc.fill = GridBagConstraints.BOTH;
    gbl.setConstraints(taOutput,gbc);
    taOutput.setEditable(false);
    add(taOutput);

    // The Text Input Field
    gbc.gridx = 0;
    gbc.gridy = 1;
    gbc.weighty = 0.0;
    gbc.anchor = GridBagConstraints.NORTH;
    gbc.fill = GridBagConstraints.HORIZONTAL;
    gbl.setConstraints(tfInput,gbc);
    add(tfInput);

    // The Leave Button
    gbc.gridx = 1;
    gbc.gridy = 1;
    gbc.anchor = GridBagConstraints.NORTH;
    gbc.fill = GridBagConstraints.HORIZONTAL;
    gbl.setConstraints(bLeave,gbc);
    add(bLeave);

    resize(800,280);
    setResizable(true);	// Allow the user to resize
    validate();
  }

  public boolean handleEvent(Event event) {
    if (event.id == Event.WINDOW_DESTROY)
      dispose();
    return super.handleEvent(event);
  }

  // This function will be called by the RoomHandler when it receives
  // anything for this room and will add the data 
  public  void addText(String tobeadded) {
    if (false) {
      String speaker = null;
      String msg = null;
      int pos = tobeadded.indexOf(':');
      if (pos >= 0)
        speaker = tobeadded.substring(0,pos+1);
      else
        speaker = new String("");
      msg = tobeadded.substring(pos+1);
      Font oldf = taOutput.getFont();
      taOutput.setFont(boldFont);
      taOutput.appendText(speaker);
      taOutput.setFont(italicFont);
      taOutput.appendText(msg+"\n");
      taOutput.setFont(oldf);
    }
    else {
      taOutput.appendText(tobeadded+"\n");
    }
    return;
  }

  // This function will also be called by the RoomHandler whenit receives
  // Image data for this room. 
  public void 	updateImage(Image im) { 
    if(DEBUG) { 
      System.out.println("Room : I got an image");
    }
    icOutput.setImage(im);
  } 
  
  
}



/**
 * The ImageCanvas Class -- for the image in the class
 */
class ImageCanvas extends Canvas implements consts {

  Image image_ = null;
  int defWidth_ = 128;
  int defHeight_ = 128;
  int margin = 5;

  String name_ = new String("Nexus Room");
  Font nameFont = new Font("Helvetica",0,14);

  public ImageCanvas(String s) 
  {
    name_ = s;
  }

  public Dimension preferredSize() {
    return minimumSize();
  }
  public Dimension minimumSize() {
    return new Dimension(defWidth_, defHeight_);
  }

  public void setImage(Image newIm) {
    image_ = newIm;
    repaint();
  }
  public Image getImage() {
    return image_;
  }

  public void name(String n) {
    name_ = n;
  }
  public String name() {
    return name_;
  }

  public void paint(Graphics g) {
    Dimension d = size();
    int width = d.width;
    int height = d.height;

    if (DEBUG)
      System.out.println("ImageCanvas:: width = "+width+ " height="+height);

    g.setColor(Color.black);
    g.fillRect(0,0,width-1,height-1);
    g.setColor(Color.white);
    g.fillRect(margin,margin,width-2*margin,height-2*margin);
    // Create image if reqd
    if (image_ == null)
      image_ = createImage(width-2*margin,height-2*margin);

    g.drawImage(image_, margin, margin,
		width-2*margin, height-2*margin, Color.white, this);
    g.setColor(Color.blue);
    g.setFont(nameFont);
    FontMetrics fm = g.getFontMetrics();
    g.drawString(name_,(width - fm.stringWidth(name_))/2,
		 (int)(height*0.9 - fm.getMaxDescent()));
    validate();
    return;
  }

  public void update(Graphics g) {
    paint(g);
  }

} // End of the Image Canvas Class



// this is the event handler for the textfield -- whenever anything is typed 
class textHandler implements Command,consts,Runnable { 
  private MT_Bounded_Queue write_q_ ; 
  private NexusClientApplet applet_ ; 
  private String name_ ; 
  Object what_ ; 

  public textHandler(MT_Bounded_Queue write_q,
		     NexusClientApplet applet,
		     String name) { 
    write_q_ = write_q ;
    applet_   = applet ; 
    name_ = name ; 
  }

  public void execute(Object target, Event evt, Object what) { 
    // get the string and send it across in a different thread 
    what_ = what ; 
    // clear the field in the gui 
    ((TextField)target).setText("");
    // send it off 
    new Thread(this).start(); 
  }

  // send off the string in a different thread 
  public void run() {
    String data = (String)what_; 
    // check if data begins with / and is followed by url 
    if(!data.startsWith("/url")) { 
      
      String user = NexusClientApplet.myName ;
      String command = user + ":" + data ; 
      String len = Integer.toString(command.length());
      dataPacket packet  = new dataPacket(NexusClientApplet.myName,name_,
					  "TEXT",len,command);
      write_q_.nq(packet);
    }
    else { 
      // it is /url 
      StringTokenizer t  = new StringTokenizer(data); 
      // take out the /url from here 
      String ur = t.nextToken(); 
      // data now 
      String command = t.nextToken(); 
      String len = Integer.toString(command.length());
      dataPacket packet  = new dataPacket(NexusClientApplet.myName,name_,
					  "URL",len,command);
      write_q_.nq(packet);
    }
  }
} // end of class   
	
	

