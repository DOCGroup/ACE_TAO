/*************************************************
 *
 * = PACKAGE
 *    JACE.ServiceConfigurator
 *
 * = FILENAME
 *    ServiceLoader.java
 * 
 * Implementation of a ClassLoader  
 *
 *@author Prashant Jain, Everett Anderson
 *
 *************************************************/
package JACE.ServiceConfigurator;

import java.io.*;
import java.util.*;
import java.net.*;
import JACE.OS.*;
import JACE.Misc.*;

public class ServiceLoader extends ClassLoader
{
  /**
   * Constructor
   */
  public ServiceLoader ()
  {
    super ();
    this.getClassPath ();
  }

  /**
   * Searches the class path for a given file
   *
   *@param filename     File name to look for
   *@return             Returns the absolute path to the file
   *                    (useful for package name)
   */
  public String findFileInClasspath (String filename) 
  {
    // Checks for the given name across the classpath
    StringTokenizer tokens = new StringTokenizer (this.classPath_,
						  this.pathSeparator_);

    while (tokens.hasMoreTokens()) 
      {
	String fn = tokens.nextToken() + this.fileSeparator_ + filename;

	File f = new File (fn);

	if (f.exists() && f.isFile() && f.canRead())
	  return new String(f.getAbsolutePath());
      }

    return null;
  }

  /**
   * Load a class from across the network 
   *@exception ClassNotFoundException Couldn't find the class
   */
  public Class loadClass (URL url, boolean resolve) throws ClassNotFoundException
  {
    Class newClass = null;

    // Extract the name of the class from the URL

    String className = url.getFile();

    // Remove any directory information
    int idx = className.lastIndexOf("/");
    if (idx != -1)
      className = className.substring(idx + 1);

    // Get rid of the class suffix
    idx = className.lastIndexOf(".class");
    if (idx != -1)
      className = className.substring(0, idx);

    ACE.DEBUG("The name of the class about to load is " + className);

    // Try to load it the class by reading in the bytes.
    // Note that we are not catching ClassNotFoundException here
    // since our caller will catch it.
    try
      {
	URLConnection urlConnection = url.openConnection ();

	// Get the input stream associated with the URL connection and
	// pipe it to a newly created DataInputStream
	DataInputStream i = new DataInputStream (urlConnection.getInputStream ());

	// Allocate a buffer big enough to hold the contents of the
	// data we are about to read
	byte [] buf = new byte [urlConnection.getContentLength ()];

	// Now read all the data into the buffer
	i.readFully (buf);

	newClass = defineClass (className, buf, 0, buf.length);
	//	    ACE.DEBUG ("Loaded class: "+ name);
	    
	// Check if we need to load other classes referred to by this class.
	if (resolve)
	  resolveClass (newClass);
      }
    catch (IOException e)
      {
	throw new ClassNotFoundException (e.toString ());
      }
    return newClass;
  }


  /**
   * Load a class file:
   *
   * @param fn       A file name WITHOUT the .class extension
   * @param resolve  Standard resolve flag -- user should set to true
   *
   * @return   A Class file capable of creating quasi-useful instances
   *           of the loaded class.  They can't exist in the normal
   *           Java space, though, so it's impossible to cast them
   *           to something useful.  Use a wrapper and reflection
   *           as in ServiceRecords.
   *@exception ClassNotFoundException Couldn't find the class
   */
  public Class loadClass (String fn, boolean resolve) throws ClassNotFoundException
  {
    Class newClass;

    // Load built-in java classes with the system loader
    if (fn.startsWith("java")) {
      newClass = findSystemClass(fn);
      if (newClass == null)
	throw (new ClassNotFoundException());
      else
	return newClass;
    }

    // If given a dot separated qualified name, put it in OS path format.
    // This assumes the file separator is one char
    String str = new String(fn);
    if (str.indexOf('.') >= 0)
      str = str.replace('.', this.fileSeparator_.charAt(0));
    str = str + ".class";
    
    // Search the class path for the given file name
    String fullname = this.findFileInClasspath(str);
    if (fullname == null) {
      
      // If we can't find the class file, see if the
      // system can
      if ((newClass = findSystemClass(fn)) != null) {
	return newClass;
      } else 
	throw (new ClassNotFoundException());
    }

    try
      {
	// Try to load it the class by reading in the bytes.
	// Note that we are not catching ClassNotFoundException here
	// since our caller will catch it.
	try
	  {
	    byte[] buf = bytesForClass (fullname);

	    // ***** Note *****
	    // This looks inside the class file and digs up the true
	    // fully qualified class name.  You need this to
	    // load the class!
	    String className = ClassReader.getClassName(fullname);

	    if (className != null) {
	      ACE.DEBUG("Defining class with name: " + className);
	      newClass = defineClass (className, buf, 0, buf.length);
	    } else {
	      // Try it anyway
	      newClass = defineClass (null, buf, 0, buf.length);
	     
	      ACE.ERROR("Unknown class name");
	    }  

	    // Check if we need to load other classes referred to by this class.
	    if (resolve)
	      resolveClass (newClass);
	  	  
	  } catch (ClassNotFoundException e) {
	  
	    ACE.DEBUG ("Using default loader for class: " + fn);

	    if ((newClass = findSystemClass (fn)) != null)
	      return newClass;
	    else 
	      throw (e);   // Rethrow the exception
	  }
      }
    catch (IOException e)
      {
	throw new ClassNotFoundException (e.toString ());
      }

    return newClass;
  }

  /**
   * Get system properties for later use
   */
  private void getClassPath ()
  {
    // Cache system properties that are needed when trying to find a
    // class file
    this.classPath_     = System.getProperty ("java.class.path", ".");
    this.pathSeparator_ = System.getProperty ("path.separator", ":");
    this.fileSeparator_ = System.getProperty ("file.separator", "/");
  }

  /**
   * Read file into a byte array
   */
  private byte[] bytesForClass (String name) throws IOException, ClassNotFoundException
  {
    // Set up the stream
    FileInputStream in = new FileInputStream (name);

    // Get byte count
    int length = in.available ();
	    
    if (length == 0)
      throw new ClassNotFoundException (name);

    // Create an array of bytes to read the file in
    byte[] buf = new byte[length];

    // Read the file
    in.read (buf);

    // Return byte array
    return buf;
  }

  private String classPath_;
  // Class path that is loaded at construction

  private String pathSeparator_;
  // Platform-dependent path separator (e.g., : or ;)

  private String fileSeparator_;
  // Platform-dependent file separator (e.g., / or \)

  private String context_ = null;
}






