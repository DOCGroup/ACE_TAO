/*************************************************
 *
 * = PACKAGE
 *    JACE.ServiceConfigurator
 *
 * = FILENAME
 *    ServiceRepository.java
 *
 * The service repository stores the network services, allowing them to be 
 * removed, suspended, resumed, etc.  
 *
 *@see JACE.ServiceConfigurator.ServiceConfig;
 *@see JACE.ServiceConfigurator.Service;
 *
 *@author Everett Anderson
 *
 *************************************************/
package JACE.ServiceConfigurator;

import java.io.*;
import java.util.*;
 
/**
 * Stores Services, providing operations such as remove, suspend, resume, etc.
 */
public class ServiceRepository
{
  /**
   * Constructor
   */
  public ServiceRepository ()
  {
    this.serviceMap_ = new Hashtable ();
    this.serviceNames_ = new Vector ();
  }

  /**
   * Constructor
   *
   *@param initialSize    Initial vector size for the repository
   */
  public ServiceRepository (int initialSize)
  {
    this.serviceMap_ = new Hashtable (initialSize);
    this.serviceNames_ = new Vector (initialSize);
  }

  /**
   *  Shut down all the services, closing them in reverse order of insertion.
   *  This calls fini on each service.
   *@return -1 on failure, 0 on sucess
   */
  public int close() 
  { 
    int result = 0;

    for (int i = this.size() - 1; i >= 0; i--) {

      String name = (String)this.serviceNames_.elementAt (i);

      Service s = (Service)this.serviceMap_.get (name);

      result = (s.fini () == -1 ? -1 : result);
    }

    this.serviceMap_.clear ();
    this.serviceNames_.removeAllElements ();

    return result;
  }

  /**
   * Insert a Service into the repository.
   * (If already in, calls fini() and replaces)
   *
   *@param service      Service to add
   */
  public void insert (Service service)
  {
    String name = service.name ();
    Service alreadyIn = this.find (name);

    if (alreadyIn != null) {

      alreadyIn.fini ();
      this.remove (alreadyIn);

    } else {

      this.serviceMap_.put (name, service);
      this.serviceNames_.addElement (name);

    }
  }

  /**
   * Returns an enumeration of all the Service objects.
   *
   */
  public Enumeration services ()
  {
    return this.serviceMap_.elements ();
  }

  /**
   * Returns an enumeration of all the Service names
   */
  public Enumeration serviceNames ()
  {
    return this.serviceMap_.keys ();
  }

  /**
   * Convenience method that returns null when the service isn't
   * found.
   */
  public Service find (String name) 
  {
    Object serviceObj = this.serviceMap_.get (name);

    if (serviceObj == null)
      return null;

    return (Service)serviceObj;
  }

  /**
   * Finds the Service associated with a given
   * name.
   *
   *@param name    Name of the service to find
   *@exception NoSuchElementException if the given service is not found
   */
  protected Service findService (String name) throws NoSuchElementException
  {
    Object serviceObj = this.serviceMap_.get (name);

    if (serviceObj == null)
      throw new NoSuchElementException ("Service " + name + " not found.");

    return (Service)serviceObj;
  }

  /**
   * Removes the given Service and calls its fini () method.
   *@param service Service to remove
   *@return -1 on failure, 0 on success
   */
  protected int remove (Service service)
  {
    String name = service.name ();

    this.serviceMap_.remove (name);

    int index = this.serviceNames_.indexOf (name);

    this.serviceNames_.removeElementAt (index);

    return service.fini ();
  }

  /**
   * Shut down the specified Service.
   *
   *@param name name of the Service to shut down
   *@return -1 on failure, 0 on success
   */
  public int fini (String name)
  {
    Service service = this.findService (name);

    return service.fini ();
  }

  /**
   * Remove the specified Service, calling its fini () method.
   *
   *@param name name of the Service to remove
   *@return -1 on failure, 0 on success
   */
  public int remove (String name)
  {
    Service service = this.findService (name);

    return this.remove (service);
  }

  /**
   * Resume a suspended service
   *@param name    Name of the service to resume
   *@return -1 on failure, 0 on success
   */
  public int resume (String name)
  {
    Service service = this.findService (name);

    return service.resume();
  }

  /**
   * Suspend a service
   *@param name    Name of the service to suspend
   *@return -1 on failure, 0 on success
   */
  public int suspend (String name)
  {
    Service service = this.findService (name);
    
    if (service.suspended ())
      return 0;

    return service.suspend ();
  }

  /**
   * Returns status information about the specified Service.
   *
   *@param name name of the Service to query
   *@return String of information about the Service's status
   */
  public String info (String name)
  {
    Service service = this.findService (name);

    return service.info ();
  }

  /** 
   * Check to see if the specified Service is suspended or not
   */
  public boolean suspended (String name)
  {
    Service service = this.findService (name);

    return service.suspended ();
  }

  /**
   * Initialize the specified Service with the given command
   * line arguments.
   */
  public int init (String name, String [] args)
  {
    Service service = this.findService (name);

    return service.init (args);
  }

  /**
   * Returns the number of items in the repository
   */
  public int size ()
  {
    return this.serviceNames_.size();
  }

  /**
   * Stores the Service names in order of insertion
   */
  Vector serviceNames_;

  /**
   * A mapping between Service names and Service objects
   */
  Hashtable serviceMap_;
};
  


  

  

  
