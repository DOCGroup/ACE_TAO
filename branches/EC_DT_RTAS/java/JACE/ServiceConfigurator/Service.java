package JACE.ServiceConfigurator;

/**
 * Interface common to all services loaded with the
 * Service Configurator.
 * <P>
 * In order to create a completely new type of service, all that
 * is necessary is to implement this interface, and ServiceConfig
 * will be able to load it.  A concrete example is ServiceObject,
 * the base class for the network services.
 * <P>
 * Implementing classes must also:
 * <UL>
 * <LI> Provide a default constructor
 * <LI> Begin in their own thread (probably in init (String[]))
 * </UL>
 * <P>
 * Implementing classes should also:
 * <UL>
 * <LI> Shut down completely when close () has been called.  That
 * means ending thread activity.  
 * </UL>
 *
 *@see JACE.ServiceConfigurator.ServiceObject
 *@see JACE.ServiceConfigurator.ServiceConfig
 *@author Everett Anderson
 */
public interface Service
{
  /**
   * Temporarily disable this service.  This will only be called for a 
   * service which returns false from its suspended() method.
   * <P>
   *@return -1 on failure, 0 on success
   */
  int suspend ();

  /**
   * Re-enable this service.  This will only be called for a service
   * which returns true from its suspended() method.
   * <P>
   *@return -1 on failure, 0 on success
   */
  int resume ();

  /**
   * Initialize this service.  The arguments will be given as if they
   * were from the command line, separated into Strings using spaces
   * as the delimiters.
   * <P>
   *@param args set of command line arguments
   *@return -1 on failure, 0 on success
   */
  int init (String [] args);

  /**
   * Close this service and free any internal resources.
   * <P>
   *@return -1 on failure, 0 on success
   */
  int fini ();

  /**
   * Provide a status message for this service.
   * <P>
   *@return status message
   */
  String info ();

  /**
   * Return the name of this service.  The name is typically set
   * in ServiceConfig to a name supplied in a configuration file.
   * <P>
   *@return name of this service
   */
  String name ();

  /**
   * Set the name of this service.  This is typically caled in
   * ServiceConfig, setting the name to one supplied in a configuration
   * file.
   * <P>
   *@param name new name for this service
   */
  void name (String name);

  /**
   * Returns whether or not this service is suspended.  The result 
   * determines whether or not this service's resume() and suspend()
   * methods will be called by ServiceConfig.  (For instance, while
   * a service returns true, its suspend() method will not be called.)
   * <P>
   *@return true if suspended, else false
   */
  boolean suspended ();
}
