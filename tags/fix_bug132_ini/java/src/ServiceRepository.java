/*************************************************
 *
 * = PACKAGE
 *    JACE.ServiceConfigurator
 *
 * = FILENAME
 *    ServiceRepository.java
 *
 * The service repository stores the network services, allowing them to be removed, suspended,
 * resumed, etc.  To reload a service, ServiceConfig.prepareForReload() must be called.  This is
 * already done in the ServiceConfig.remove method.
 *
 *@see JACE.ServiceConfigurator.ServiceRecord;
 *@see JACE.ServiceConfigurator.ServiceConfig;
 *
 *@author Everett Anderson
 *
 *************************************************/
package JACE.ServiceConfigurator;

import java.io.*;
import java.util.*;
 
public class ServiceRepository
{
  /**
   * Constructor
   */
  public ServiceRepository ()
  {
    this.serviceVector_ = new Vector();
  }

  /**
   * Constructor
   *
   *@param initialSize    Initial vector size for the repository
   */
  public ServiceRepository (int initialSize)
  {
    this.serviceVector_ = new Vector (initialSize);
  }

  /**
   *  Shut down all the services, closing them in reverse order of insertion
   *  
   *  Maybe should be called by finalize?
   */
  public int close() 
  { 
    for (int i = this.size() - 1; i >= 0; i--) {
      ServiceRecord  rec = (ServiceRecord)this.serviceVector_.elementAt(i);

      rec.fini();

      this.serviceVector_.removeElementAt(i);
    }

    return 0;
  }

  /**
   * Insert a ServiceRecord into the repository.
   * (If already in, calls fini() and replaces)
   *
   *@param srvRec      ServiceRecord to add
   */
  public void insert (ServiceRecord srvRec)
  {
    ServiceRecord alreadyIn = find(srvRec.name());

    // Replace the service
    if (alreadyIn != null) {
      alreadyIn.fini();
      this.serviceVector_.removeElement(alreadyIn);
    }

    this.serviceVector_.addElement(srvRec);
  }

  /**
   * Finds the ServiceRecord associated with a given
   * service name.  Note -- the user should not try to
   * get a ServiceObject out of the ServiceRecord.
   * Same as saying ignoreSuspended is false on the
   * next method.
   *
   *@param name    Name of the service to find
   */
  public ServiceRecord find (String name)
  {
    return this.find(name, false);
  }

  /** Return the service record for the given service.  The caller
    * should NOT try to access a ServiceObject (or Module or Stream)
    * by taking it out of the ServiceRecord -- just make the calls
    * through the record!
    *
    *@param   name             Name of the service to find
    *@param   ignoreSuspended  Allow finding suspended services?
    */
  public ServiceRecord find (String name, boolean ignoreSuspended)
  {
    ServiceRecord rec;

    for (int i = 0; i < this.size(); i++) {
      rec = (ServiceRecord)this.serviceVector_.elementAt(i);

      if ((rec.name().equals(name)) && ((!ignoreSuspended) || (!rec.suspended())))
	return rec;
    }

    return null;
  }

  /** Take the given service out of the repository.  This also sets the
   * reference in the repository to null to ensure there are no
   * hidden references to the old ServiceObject.  To reload, the
   * ServiceConfig.prepareToReload method must be called.  This is already
   * done in the ServiceConfig.remove method.
   */
  public int remove (String name)
  {
    ServiceRecord rec = this.find(name, false);

    if (rec == null)
      return -1;

    int index = this.serviceVector_.indexOf(rec);

    // Shut down the service
    rec.fini();

    // Make sure there are no hidden references left
    this.serviceVector_.setElementAt(null, index);

    this.serviceVector_.removeElementAt(index);

    return 0;
  }

  /**
   * Resume a suspended service
   *@param name    Name of the service to resume
   */
  public int resume (String name)
  {
    ServiceRecord rec = this.find(name, false);

    if (rec == null)
      return -1;

    return rec.resume();
  }

  /**
   * Suspend a service
   *@param name    Name of the service to suspend
   */
  public int suspend (String name)
  {
    ServiceRecord rec = this.find(name, true);

    if (rec == null)
      return -1;

    return rec.suspend();
  }

  /**
   * Returns the number of items in the repository
   */
  public int size ()
  {
    return this.serviceVector_.size();
  }

  // Vector representation
  Vector serviceVector_;
};
  


  

  

  
