/*************************************************
 *
 * = PACKAGE
 *    ACE.ServiceConfigurator
 *
 * = FILENAME
 *    ServiceRepository.java
 *
 *@author Prashant Jain
 *
 *************************************************/
package ACE.ServiceConfigurator;

import java.io.*;
import java.util.*;
import ACE.OS.*;

public class ServiceRepository
{
  /**
   * Create a Service Repository.
   */
  public ServiceRepository ()
  {
    this.loader_ = new ServiceLoader ();
  }

  /**
   * Load a Java Class. This method first checks if the class is
   * already in the repository. If it is not, it checks if the class
   * is a system class and can be found on the local system. If both
   * these fail, the method tries to load the bytes for the class and
   * then create the Class from these bytes.
   *@param name name of the class to find in the repository
   *@return Class corresponding to name
   */
  public Class load (String name) throws ClassNotFoundException
  {
    return this.loader_.loadClass (name, true);
  }

  private ServiceLoader loader_;
}

class ServiceLoader extends ClassLoader
{
  public ServiceLoader ()
  {
    super ();
    this.getClassPath ();
  }
  
  // Load a compiled class file into runtime
  public Class loadClass (String name, boolean resolve) throws ClassNotFoundException
  {
    Class newClass;
    try
      {
	// Try to load it the class by reading in the bytes.
	// Note that we are not catching ClassNotFoundException here
	// since our caller will catch it.
	try
	  {
	    byte[] buf = bytesForClass (name);
	    newClass = defineClass (buf, 0, buf.length);
	    //	    ACE.DEBUG ("Loaded class: "+ name);
	    
	    // Check if we need to load other classes referred to by this class.
	    if (resolve)
	      resolveClass (newClass);
	  }
	catch (ClassNotFoundException e)
	  {
	    //	    ACE.DEBUG ("Using default loader for class: "+ name);
	    // Try default system loader
	    if ((newClass = findSystemClass (name)) != null)
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

  private void getClassPath ()
  {
    // Cache system properties that are needed when trying to find a
    // class file
    this.classPath_     = System.getProperty ("java.class.path", ".");
    this.pathSeparator_ = System.getProperty ("path.separator", ":");
    this.fileSeparator_ = System.getProperty ("file.separator", "/");
  }

  // Read a file into a byte array
  private byte[] bytesForClass (String name) throws IOException, ClassNotFoundException
  {
    // Break up the CLASSPATH to check for existence of file in each
    // sub-path. Note that we use the path_separator to extract every
    // sub-path and use that to check if the class file exists.
    StringTokenizer tokens = new StringTokenizer (this.classPath_, 
						  this.pathSeparator_);
    while (tokens.hasMoreTokens ())
      {
	// Create a File object which can then be used to see if the
	// class file actually exists
	File classFile = new File (tokens.nextToken () + 
				    this.fileSeparator_ + 
				    name + 
				    ".class");

	// Check if file exists, is a normal file and is readable
	if (classFile.exists () && 
	    classFile.isFile () &&
	    classFile.canRead ())
	  {    
	    // Set up the stream
	    FileInputStream in = new FileInputStream (classFile);

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
      }
    // File was not found so throw an exception.
    throw new ClassNotFoundException ("Class file " + name + " not found");
  }

  private String classPath_;
  // Class path that is loaded at construction

  private String pathSeparator_;
  // Platform-dependent path separator (e.g., : or ;)

  private String fileSeparator_;
  // Platform-dependent file separator (e.g., / or \)
}
