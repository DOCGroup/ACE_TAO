/*************************************************
 *
 * = PACKAGE
 *    JACE.Connection
 *
 * = FILENAME
 *    CreationStrategy.java
 *
 *@author Prashant Jain
 *
 *************************************************/
package JACE.Connection;

/**
 * <hr>
 * <h2>SYNOPSIS</h2>
 *<blockquote>
 * Defines the interface for specifying a creation strategy for a
 * <a href="ACE.Connection.SvcHandler.html#_top_"><tt>SvcHandler</tt></a> to the
 * <a href="ACE.Connection.StrategyAcceptor.html#_top_"><tt>StrategyAcceptor</tt></a>.
 *</blockquote>
 *
 * <p><b>DESCRIPTION</b><br>
 *<blockquote>
 *     The default behavior is to make a new SvcHandler.  However,
 *     subclasses can override this strategy to perform SvcHandler
 *     creation in any way that they like (such as creating subclass
 *     instances of SvcHandler, using a singleton, dynamically
 *     linking the handler, etc.).
 *</blockquote>
 *
 *@see SvcHandler
 *@see StrategyAcceptor
 *@see AcceptStrategy
 *@see ActivateStrategy
 */
public class CreationStrategy
{
  /**
   * Create an instance of Creation Strategy.
   *@param handlerFactory Svc Handler factory that is used to create
   * an instance of a Svc Handler
   */
  public CreationStrategy (Class handlerFactory)
  {
    this.handlerFactory_ = handlerFactory;
  }

  /**
   * Create a new SvcHandler. Note that subclasses should override
   * this method to provide a new creation strategy.
   *@return reference to a new instance of the SvcHandler (or subclass)
   *@exception InstantiationException Unable to instantiate.
   *@exception IllegalAccessException No handler factory available.
   */
  public SvcHandler makeSvcHandler () throws InstantiationException, 
    IllegalAccessException
    {
      // Create a new Svc_Handler
      return (SvcHandler) handlerFactory_.newInstance ();
    }

  private Class handlerFactory_;
}
