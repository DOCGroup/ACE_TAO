/*************************************************
 *
 * = PACKAGE
 *    JACE.ServiceConfigurator
 *
 * = FILENAME
 *    ServiceLoader.java
 * 
 * Implementation of a network-capable class loader
 *
 *@author Prashant Jain
 *@author Everett Anderson
 *
 *************************************************/
package JACE.ServiceConfigurator;

import java.io.*;
import java.util.*;
import java.net.*;
import JACE.OS.*;
import JACE.Misc.*;

/**
 * ClassLoader that can load classes from across the network
 * via provided URL search paths, or from the local machine. 
 * <P>
 * This operates in the same way as the JDK 1.2 ClassLoader by
 * first checking for preloaded classes, then trying to use the system
 * loader, and finally trying to load it via the network.  The user can also
 * provide URL search paths, or explicitly call the loadClass method which
 * takes a URL. 
 *
 *@see java.lang.ClassLoader
 *@see java.lang.Class
 */
public class ServiceLoader extends ClassLoader
{
  /**
   * Creates a loader that will only search the local machine for classes
   * until URL paths are registered via addURLPath.
   */
  public ServiceLoader ()
  {
    this.URLSearchPaths_ = new Vector();
  }

  /**
   * Creates a loader that will search the local machine followed by
   * this array of URL paths when loading classes.  Note that additional
   * paths can be added later with the addURLPath method.
   *@param remotePaths array of URLs to search when loading classes, after
   *attempts to load from the local machine have failed
   */
  public ServiceLoader (URL remotePaths[]) 
  {
    this();

    if (remotePaths == null)
      return;

    for (int i = 0; i < remotePaths.length; i++) 
      this.addURLPath(remotePaths[i]);
  }

  /**
   * Add the given URL to the list of URL paths to search when loading
   * classes.  
   *@param url URL ending in '/' where classes can be found
   */
  public void addURLPath (URL url) 
  {
    if (!URLSearchPaths_.contains(url))
      URLSearchPaths_.addElement(url);
  }

  /**
   * Add the given URL to the list of URL paths to search when loading
   * classes.
   *@param String representation of a URL ending in '/'
   *@exception MalformedURLException the given String wasn't a valid URL
   */
  public void addURLPath (String url) throws MalformedURLException
  {
    URL resource = new URL (url);
    this.addURLPath (resource);
  }

  /**
   * Tries to load the given class by following the example of JDK 1.2
   * ClassLoader.  First check loaded classes, then try to use the system
   * loader, and only then perform the user defined subclass behavior
   * from findClass (in this case, it's searching over the network).
   *@param name name of the class to load
   *@param resolve whether or not to resolve the class 
   *@exception ClassNotFoundException if the class could not be found
   *@return loaded Class instance
   */
  protected Class loadClass(String name,
			    boolean resolve)
    throws ClassNotFoundException 
  {
    
    Class c = null;
      
    // Has this class already been loaded once?
    c = findLoadedClass(name);
    if (c != null)
      return c;
    
    try {

      // Can the system loader find it?
      c = findSystemClass(name);
      return c;

    } catch (ClassNotFoundException e2) {
      // The system loader couldn't find it
    }
    
    // If none of the above, try searching our way
    return findClass(name);
  }

  /**
   * This should duplicate the behavior of this class in JDK 1.2.  It is
   * called by the protected loadClass method after trying to load the
   * class in other ways.  It simply calls loadClassData and then defines
   * the class.
   *@param name name of the class to load
   *@exception ClassNotFoundException couldn't find the class
   *@return loaded Class
   */
  protected Class findClass(String name) throws ClassNotFoundException 
  {
    byte[] b = loadClassData(name);
    return defineClass(name, b, 0, b.length);
  }

  /**
   * Try loading a class with the given name, searching the remote
   * URL paths that have been registered.  Note that this will only
   * be called after first checking to see if the class has already
   * been loaded, then checking to see whether or not the system
   * loader can find it.
   * <P>
   * This could be overriden by subclasses to define different
   * loading behavior.
   *
   *@param className name of the class (not file name) of the class to load
   *@exception ClassNotFoundException couldn't find the class
   *@return bytes of a .class file
   */
  private byte[] loadClassData(String className) throws ClassNotFoundException
  {
    byte data[] = null;

    // Try to load it by reading in the bytes from the net
    Enumeration e = URLSearchPaths_.elements();

    while (e.hasMoreElements()) 
      {
	try
	  {
	    URL path = (URL)e.nextElement();
	    URL classFileURL;

	    try 
	      {
		classFileURL = new URL(path.toExternalForm() +
				       className +
				       ".class");
	      } catch (MalformedURLException ex) {
		ACE.ERROR("Bad URL: " + ex.getMessage());
		continue;
	      }

	    ACE.DEBUG("Looking for " + classFileURL.toExternalForm());
	    URLConnection urlConnection = classFileURL.openConnection ();

	    // Get the input stream associated with the URL connection and
	    // pipe it to a newly created DataInputStream
	    DataInputStream i = new DataInputStream 
	      (urlConnection.getInputStream ());

	    // Allocate a buffer big enough to hold the contents of the
	    // data we are about to read
	    data = new byte [urlConnection.getContentLength ()];

	    // Now read all the data into the buffer
	    i.readFully (data);

	  } catch (IOException ex) {
	    // Either the URL wasn't there or we couldn't get the data
	    continue;
	  }
      }

    // Couldn't find it or it loaded improperly
    if (data == null)
      throw new ClassNotFoundException("Can't obtain " + className);

    return data;
  }

  /**
   * Load a class from across the network
   *
   * The newer way to do this is similar to JDK 1.2 URLClassLoader:
   *   Add URLs that end in '/' via the addURLPath method
   *   Call loadClass (class name without .class extension) method
   *
   *@param url URL of the class to load
   *@exception ClassNotFoundException Couldn't find the class
   *@return loaded Class
   */
  public Class loadClass (URL url) throws ClassNotFoundException
  {
    // Extract the name of the class from the URL
    String URLPath = url.getFile();

    // Get the class name by removing any directory information
    int idx = URLPath.lastIndexOf("/");
    if (idx == -1)
      throw new ClassNotFoundException("Can't find " + URLPath);
    String className = URLPath.substring(idx + 1);
    URLPath = URLPath.substring(0, idx + 1);

    // Get rid of the class suffix
    idx = className.lastIndexOf(".class");
    if (idx != -1)
      className = className.substring(0, idx);

    ACE.DEBUG("The name of the class about to load is " + className);

    try {
      // Add the URL to the list of URL search paths
      URL path = new URL(URLPath);

      this.addURLPath(path);

    } catch (MalformedURLException e) {
      throw new ClassNotFoundException("Can't find " + URLPath);
    }
    
    // Try to load the class
    return loadClass(className);
  }

  /**
   * Collection of URLs which end in the HTTP path separator.  Used
   * for searching for classes across the network.
   */
  protected Vector URLSearchPaths_;
}
