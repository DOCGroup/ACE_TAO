/*************************************************
 *
 * = PACKAGE
 *    JACE.ServiceConfigurator
 *
 * = FILENAME
 *    ParseNode.java
 *
 * Base class for the data types used in the parse tree for adjusting 
 * services.  Things like SuspendNode extend this.  They help connect
 * CUP/JLex with the service configurator system.
 *
 *@author Everett Anderson
 *
 *************************************************/
package JACE.ServiceConfigurator;

import java.io.*;
import JACE.OS.*;
import java_cup.runtime.*;

public class ParseNode extends java_cup.runtime.symbol 
{
  /** 
   * Constructor
   *
   */
  public ParseNode (int number)
  {
    super(number);

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

 
  
