/*************************************************
 *
 * = PACKAGE
 *    JACE.ServiceConfigurator
 *
 * = FILENAME
 *    ParseNode.java
 *
 * Base class for the data types used in the parse tree for adjusting 
 * services.  Things like SuspendNode extend this. 
 *
 *@author Everett Anderson
 *
 *************************************************/
package JACE.ServiceConfigurator;

import java.io.*;
import JACE.OS.*;

public class ParseNode 
{
  /** 
   * Constructor
   *
   */
  public ParseNode ()
  {
    this.name_ = new String("Uninitialized");
  }

  /**
   * Initialize the service (subclasses
   * may do more than set the name)
   */
  public void init (String name)
  {
    this.name_ = name;
  }

  /**
   * Subclasses override to do real work, usually
   * initiating a service or modifying one
   */
  public void apply () 
  {
    // Empty
  }

  /**
    * Retrive the service name
    */
  public String name() 
  {
    return this.name_;
  }

  /**
   * Set the name of the service
   */
  public void name(String newName)
  {
    this.name_ = newName;
  }
  
  String name_;
};

 
  
