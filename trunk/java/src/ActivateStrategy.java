/*************************************************
 *
 * = PACKAGE
 *    ACE.Connection
 *
 * = FILENAME
 *    ActivateStrategy.java
 *
 *@author Prashant Jain
 *
 *************************************************/
package ACE.Connection;


/**
 * <hr>
 *<h2>SYNOPSIS</h2>
 * Bridge supporting activation strategy used by
 * <a href="ACE.Connection.StrategyAcceptor.html#_top_"><tt>StrategyAcceptor</tt></a>
 *
 *<h2>DESCRIPTION</h2>
 * Subclass and overload
 * <a href="#activateSvcHandler(ACE.Connection.SvcHandler)"><tt>activateSvcHandler</tt></a>
 * in order change the activation strategy.  Then, submit this subclass to 
 * <a href="ACE.Connection.StrategyAcceptor.html#_top_"><tt>StrategyAcceptor</tt></a>
 * as the activation strategy.
 *
 *@see StrategyAcceptor
 */
public class ActivateStrategy
{
  /**
   * Activate the Svc Handler. Note that subclasses should overwrite
   * this method to provide a different Activate strategy.
   *@param sh Svc Handler to activate
   *@return zero if success, non-zero for failure
   */
  public int activateSvcHandler (SvcHandler sh)
    {
      sh.open (null);
      return 0;
    }
}
