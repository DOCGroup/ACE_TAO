package imaging.framework;

import java.net.*;
import java.io.*;
import java.applet.*;

public class ListFiles
{
  public ListFiles ()
  {
  }

  public ListFiles (FileBrowser browser, Applet parent)
  {
    this.browser_ = browser;
    try
      {
	this.fileIcon_ = new Icon (parent.getCodeBase () + "doc01.gif", parent);
	this.dirIcon_ = new Icon (parent.getCodeBase () + "file03.gif", parent);
      }
    catch (MalformedURLException e)
      {
      }
  }

  public String stripProtocolHeader (String url)
  {
    if (url.startsWith ("http://"))
      {
	return url.substring (7);
      }
    else
      return url;
  }
    
  public int listFiles (String url, FileNode fileNode)
  {
    String s = this.stripProtocolHeader (url);
    String hostname = s;
    String directory = null;
    int index = -1;

    if ((index = s.indexOf ("/")) != -1)
      {
	hostname = s.substring (0, index);
	directory = s.substring (index);
      }
    return this.listFiles (hostname, directory, fileNode);
  }

  public int listFiles (String url, String directory, FileNode fileNode)
  {
    boolean validDirectory = false;
    int count = 0;
    String hostname = this.stripProtocolHeader (url);
    this.url_ = url;
    this.directory_ = directory;
    try
      {
	Socket sock = new Socket (hostname, 80);
	PrintStream out = new PrintStream (sock.getOutputStream ());
	DataInputStream in = new DataInputStream (sock.getInputStream ());
	System.out.println ("Connected to: " + hostname);

	String request = null;
	if (directory.endsWith ("/"))
	  request = "GET " + directory + "\n\n";
	else
	  request = "GET " + directory + "/\n\n";
    
	System.out.println ("Sending request: " + request);

	// Send the request
	out.println (request);
	
	String reply = null;
	// Receive the reply

	// Read all the data in a loop. Search for "Parent Directory"
	// to verify that this indeed is a directory. If we encounter
	// the string "<HTML>" then assume that this is an HTML page
	// and therefore the directory contained "index.html"
	while ((reply = in.readLine ()) != null)
	  {
	    if (validDirectory)
	      this.parse (reply, fileNode);
	    else 
	      {
		// Take a guess at the type of data we get back
		if (reply.indexOf ("Parent Directory") != -1)
		  validDirectory = true;
		else if ((reply.toUpperCase ().indexOf ("<HTML>") != -1) ||
			 (reply.toUpperCase ().indexOf ("<P>") != -1) ||
			 (reply.toUpperCase ().indexOf ("<TABLE") != -1))
		  return 0;
	      }
	  }
      }
    catch (MalformedURLException e)
      {
	System.err.println (e);
      }
    catch (IOException e)
      {
	System.err.println (e);
      }
    if (validDirectory == false)
      return -1;
    return 1;
  }

  private int parse (String s, FileNode fileNode)
  {
    int i= -1;
    int j = -1;
    int startIndex = -1;
    int endIndex = -1;
    boolean isFile = true;
    String name = null;

    if ((i = s.indexOf ("HREF=")) != -1)
      startIndex = i + 6;
    else
      return -1;

    if ((j = s.indexOf (">", i)) != -1)
      endIndex = j - 1;
    else
      return -1;

    // Check if this is a directory
    if (s.charAt (endIndex - 1) == '/')
      isFile = false;

    if (endIndex >= startIndex)
      {
	name = s.substring (startIndex, endIndex);
	if (browser_ != null)
	  {
	    //	    System.out.println (name);
	    if (isFile)	      
	      fileNode.addEntry (new FileNode (name, this.fileIcon_, null,
					       fileNode.app ()));
	    else
	      {
		FileNode f = new FileNode (name, this.dirIcon_, this,
					   fileNode.app ());
		fileNode.addEntry (f);
		f.setExpandable (true);
	      }
	  }
      }
    return 0;
  }

  private FileBrowser browser_ = null;
  private String url_ = null;
  private String directory_ = null;
  private Icon fileIcon_;
  private Icon dirIcon_;
}
