/*************************************************
 *
 * = PACKAGE
 *    JACE.ServiceConfigurator
 *
 * = FILENAME
 *    ServiceRecord.java
 *
 * This class structure is used in the ServiceRepository.  Each service
 * object, module, or stream in the repository should be wrapped by a
 * type of ServiceRecord.  The contained object does the real work.
 *
 * Modules and Streams will require records with more functionality.
 *
 * The caller should never be allowed to access the Object within the
 * record -- casting will result in a ClassCastException because of
 * the problem with loading classes with a ClassLoader.  To get
 * around this, all the method calls are made on the Object via
 * reflection.
 *
 *@see JACE.ServiceConfigurator.ServiceObject
 *@see JACE.ServiceConfigurator.ServiceRepository
 *
 *@author Everett Anderson
 *
 *************************************************/
package JACE.ServiceConfigurator;

import java.io.*;
import java.lang.reflect.*;
import JACE.OS.*;

public class ServiceRecord
{
  /** 
   * Constructor
   *
   *@param  service   A java Object, the service
   *@param  name      Name of this service
   */
  ServiceRecord (Object service, String name) 
  {
    this.service_ = service;
    this.name_ = name;
    this.suspended_ = false;
  }

  /**
   * Forward the call to suspend
   * @return -1 error
   */
  public int suspend()
  {
    this.setSuspend(true);

    Object result = this.invokeSimpleReflected("suspend");
    
    if (result == null)
      return -1;
    else
      return ((Integer)result).intValue();
  }


  /**
   * Forward the call to resume
   * @return -1 error
   */
  public int resume()
  {
    this.setSuspend(false);

    Object result = this.invokeSimpleReflected("resume");

    if (result == null)
      return -1;
    else
      return ((Integer)result).intValue();
  }


  /** 
   *  Initialize the service, provide the given command line args to it.
   *  
   */
  public int init(String [] args)
  {
    Class types[] = new Class[1];
    if (args == null)
      args = new String[0];

    types[0] = args.getClass();

    // Find the method we want to call
    Method m;
    try {
      m = this.object().getClass().getMethod("init", types);
    } catch (NoSuchMethodException e) {
      ACE.ERROR("" + e);
      return -1;
    } catch (SecurityException e) {
      ACE.ERROR("" + e);
      return -1;
    }

    Class ptypes[] = m.getParameterTypes();
    //for (int x = 0; x < ptypes.length; x++) 
    //  System.err.println(ptypes[x].getName());
    Object params[] = new Object[1];

    params[0] = args;

    int result = -1;
    try {
      result = ((Integer)m.invoke(this.object(), params)).intValue();
    } catch (IllegalAccessException e) {
      ACE.ERROR("" + e);
      return -1;
    } catch (IllegalArgumentException e) {
      ACE.ERROR("" + e);
      return -1;
    } catch (InvocationTargetException e) {
      ACE.ERROR("init(): " + e.getTargetException());
      return -1;
    }

    return result;
  }

  /**
   * Prepare to close it
   */
  public int fini()
  {
    Object result = this.invokeSimpleReflected("fini");

    if (result == null)
      return -1;
    else
      return ((Integer)result).intValue();
  }

  /** 
   * Obtain information about this service
   */
  public String info()
  {
    Object result = this.invokeSimpleReflected("info");

    if (result == null)
      return null;
    else
      return new String((String)result);  
  }

  /** Invokes the method with the given name on the ServiceObject.
   * The invoked method must take no parameters for this to work.
   * Could be adjusted to throw a generic exception.
   */
  private Object invokeSimpleReflected(String name) 
  {
    Method m;

    // find the desired method
    try {
      m = this.object().getClass().getMethod(name, null);
    } catch (NoSuchMethodException e) {
      ACE.ERROR("" + e);
      return null;
    } catch (SecurityException e) {
      ACE.ERROR("" + e);
      return null;
    }

    // Invoke it
    Object result = null;
    
    try {
      result = m.invoke(this.object(), null);
    } catch (IllegalAccessException e) {
      ACE.ERROR("" + e);
    } catch (IllegalArgumentException e) {
      ACE.ERROR("" + e);
    } catch (InvocationTargetException e) {
      ACE.ERROR(name + "(): " + e.getTargetException());
    }

    return result;
  }

  /**
   * Accessor for the name
   */
  public String name()
  {
    return new String(this.name_);
  }

  /** Set the name of the service
   */
  public void name(String name)
  {
    this.name_ = name;
  }

  /** Is this service suspended?
   */
  public boolean suspended() 
  {
    return this.suspended_;
  }

  /** Set the suspended flag
   */
  void setSuspend (boolean suspended)
  {
    this.suspended_ = suspended;
  }

  /** Accessor for the contained Object.  This should
   *  never be available to the end user since they
   *  might try casting the result.
   */
  Object object()
  {
    return this.service_;
  }

  /**
   * Set the contained object
   */
  void object(Object service)
  {
    this.service_ = service;
  }

  Object service_;
  String name_;
  boolean suspended_;
};


/*
class ModuleRecord extends ServiceRecord
{
};

class StreamRecord extends ServiceRecord
{
};
*/


