package JACE.netsvcs.Token;

import java.io.*;
import java.net.*;
import java.util.*;
import JACE.OS.*;
import JACE.Misc.*;
import JACE.Connection.*;
import JACE.netsvcs.Server;

/**
 * Server for the token service.  Launches TokenRequestHandlers as
 * connections are made.  Currently, the activation strategy must be
 * thread per connection since the operations are allowed to block
 * during acquires, etc.
 * <P>
 * Two types of locks are supported by default -- Mutex and RWMutex.
 * New lock types can be added from the command line without changing
 * any code in the service.  To do this, just create a class which
 * implements the LockHandler interface.
 * <P>
 * When a request for a new lock comes in, a LockHandler of the corresponding
 * type is created and a mapping is created between the lock name and the
 * handler.  Later requests reuse that mapping.
 * <P>
 * <B>Valid command line arguments:</B>
 * <PRE>
 *    -f (class name):(type)  Specify a LockHandler for a type of lock");
 *    -p (port number)        Port to listen on for clients");
 *    -d                      Enable debugging messages");
 * </PRE>
 *
 *@see JACE.netsvcs.Server
 *@see TokenRequestHandler
 *@see LockHandler
 *@see LockTypes
 */
public class TokenAcceptor extends Server
{ 
  protected void addDefaultFactories() throws ClassNotFoundException {
    addHandlerFactory(LockTypes.MUTEX,
		      Class.forName("JACE.netsvcs.Token.MutexHandler"));
    addHandlerFactory(LockTypes.RWLOCK,
		      Class.forName("JACE.netsvcs.Token.RWMutexHandler"));
  }
  
  /**
   * Default constructor.
   */
  public TokenAcceptor() {

    // Set the name in case we aren't using the service configurator.
    name ("Token Service");

    lockHandlerMap_ = new Hashtable();
    handlerFactoryMap_ = new Hashtable();
    clientHandlerMap_ = new Hashtable ();
  }
  
  /**
   * Add a map between a type of lock and the factory which
   * creates LockHandler instances that handle it.
   *
   *@see LockTypes
   *@param type number representing the type of lock
   *@param factory Class object for a LockHandler class
   */
  public void addHandlerFactory(Integer type, Class factory) {
    handlerFactoryMap_.put(type, factory);
  }
  
  /**
   * Add a map between a type of lock and the factory which
   * creates LockHandler instances that handle it.
   *
   *@see LockTypes
   *@param type number representing the type of lock
   *@param factory Class object for a LockHandler class
   */
  public void addHandlerFactory(int type, Class factory) {
    addHandlerFactory(new Integer(type), factory);
  }
  
  /**
   * Remove the LockHandler factory which handles locks
   * of the specified type.
   *
   *@param type type of LockHandler to cease supporting
   *@return the LockHandler instance (or null if not found)
   */
  public Object removeHandlerFactory(Integer type) {
    return handlerFactoryMap_.remove(type);
  }
  
  /**
   * Remove the LockHandler factory which handles locks
   * of the specified type.
   *
   *@param type type of LockHandler to cease supporting
   *@return the LockHandler instance (or null if not found)
   */
  public Object removeHandlerFactory(int type) {
    return handlerFactoryMap_.remove(new Integer(type));
  }

  /**
   * Retrieve the LockHandler corresponding to the given name
   * or create a new one if it doesn't exist.  This is called by
   * TokenRequestHandlers.
   *
   *@param lockName name of the lock to retrieve or create a LockHandler for
   *@param lockType type of the lock
   *@return LockHandler which handles the lock with that name
   */
  public LockHandler getLockHandler (String lockName, 
				     int lockType) {
    synchronized (lockHandlerMap_) {
      
      Object obj = lockHandlerMap_.get(lockName);
      
      if (obj != null)
	return (LockHandler)obj;
      else {
	LockHandler handler = newHandler (lockType);
	lockHandlerMap_.put (lockName, handler);
	return handler;
      }
    }
  }
  
  /**
   * Create a new LockHandler of the specified type.
   *
   *@param type type of LockHandler to create
   *@return a new LockHandler instance
   */
  protected LockHandler newHandler (int type) {
    ACE.DEBUG ("Creating new handler of type " + type);
    Object factoryObj = handlerFactoryMap_.get(new Integer(type));
    if (factoryObj == null) 
      return null;
    
    Class factory = (Class)factoryObj;
    LockHandler handler = null;
    
    try {
      handler = (LockHandler)factory.newInstance();
    } catch (InstantiationException e) {
      ACE.ERROR("Can't create a handler of type " + type);
    } catch (IllegalAccessException e) {
      ACE.ERROR("Handler of type " + type +
		" must have a default constructor");
    }
    return handler;
  }
  
  /**
   * Simple main program.  See the class description for more
   * information about command line arguments.
   */
  public static void main(String args[]) {
    TokenAcceptor ta = new TokenAcceptor();
    
    ta.init(args);
  }
  
  /**
   * Create a new TokenRequestHandler instance.
   */
  protected SvcHandler makeSvcHandler() 
  {
    return new TokenRequestHandler();
  }
  
  /**
   * Sets up the default factories so the user can override them on
   * the command line, then delegates back to Server.init (String[]).
   *
   *@see JACE.netsvcs.Server#init
   *@param args command line arguments
   *@return -1 on failure, 0 on success
   */
  public int init(String [] args) {
    try {
      addDefaultFactories ();
    } catch (ClassNotFoundException e) {
      ACE.ERROR ("Can't find default factory " + e.getMessage ());
      return -1;
    }

    return super.init (args);
  }
  
  /**
   * Prints out the valid command line arguments.  See the class
   * description for more information.  Called by Server.init when
   * parseArgs returns -1.
   */
  protected void printUsage ()
  {
    ACE.ERROR ("Valid options:\n");
    ACE.ERROR ("-f <class name>:<type>  Specify a handler for a type of lock");
    ACE.ERROR ("-p <port number>        Port to listen on for clients");
    ACE.ERROR ("-d                      Enable debugging messages");
  }

  /**
   * Parses the command line arguments.  See the class description
   * for more information.
   *
   *@param args command line arguments
   *@return -1 on failure, 0 on success
   */
  protected int parseArgs(String [] args) 
  {
    int c = 0;
    GetOpt opt = new GetOpt(args, "p:f:d", true);
    
    try {

      while ((c = opt.next ()) != -1) {
	switch (c) 
	  {
	  case 'd':
	    ACE.enableDebugging ();
	    ACE.DEBUG ("Debugging is enabled");
	    break;
	  case 'p':
	    if (!port (opt.optarg ()))
	      return -1;
	    break;
	  case 'f':
	    if (newHandlerFactory (opt.optarg ()) < 0)
	      return -1;
	    break;
	  default:
	    ACE.ERROR("Unknown argument: " + (char)c);
	    return -1;
	  }
      }
    } catch (ArrayIndexOutOfBoundsException e) {
      ACE.ERROR ("Option -" + (char)c + " requires an argument");
      return -1;
    }
    
    return 0;
  }

  /**
   * Load the Class for the specified LockHandler and create a mapping
   * from its type to the Class instance.  Used to parse the command
   * line pair of (class name):(type).
   *
   *@param nameAndType (class name):(type) pair from the command line
   *@return -1 on failure, 0 on success
   */
  protected int newHandlerFactory (String nameAndType)
  {
    int colon = nameAndType.lastIndexOf (':');

    if (colon < 1) {
      ACE.ERROR ("Invalid -f <class name>:<type num> for handler: " + 
		 nameAndType);
      return -1;
    }

    int type = 0;
    try {
      type = Integer.parseInt (nameAndType.substring (colon + 1));
    } catch (NumberFormatException e) {
      ACE.ERROR ("Invalid token type: " + e.getMessage ());
      return -1;
    }

    String name = nameAndType.substring (0, colon);

    Class factory;
    try {
      factory = Class.forName (name);
    } catch (ClassNotFoundException e) {
      ACE.ERROR (e.getMessage ());
      return -1;
    }

    addHandlerFactory (type, factory);
    ACE.DEBUG ("New handler " + name + " with type " + type);

    return 0;
  }

  /**
   * Create a mapping between a client ID and a LockHandler.  This is
   * only used by TokenRequestHandlers in order to keep track of which
   * locks a client touches.  That way, if/when a client disconnects,
   * all its locks can be abandoned successfully.
   *
   *@param clientID client identification (key in the mapping)
   *@param handler LockHandler to map to (value in the mapping)
   *
   */
  void addClientLockHandler (String clientID, 
			     LockHandler handler)
  {
    Object obj = clientHandlerMap_.get (clientID);
    if (obj == null) {
      // Probably won't have more than 10 locks per client ID, and the Vector
      // should resize automatically even if someone does.
      Vector handlerList = new Vector (10);
      handlerList.addElement (handler);
      clientHandlerMap_.put (clientID, handlerList);
    } else {
      Vector handlerList = (Vector)obj;
      int alreadyThereIndex = handlerList.indexOf (handler);
      if (alreadyThereIndex == -1)
	handlerList.addElement (handler);
    }
  }

  /**
   * Called by TokenRequestHandlers to remove a specified client ID
   * from the client ID to LockHandler mapping.
   */
  void removeClient (String clientID)
  {
    clientHandlerMap_.remove (clientID);
  }

  /**
   * Called by TokenRequestHandlers to obtain a list of all LockHandlers
   * accessed by a particular client.  Useful for abandoning the locks.
   */
  Enumeration getClientLockHandlers (String clientID)
  {
    Object obj = clientHandlerMap_.get (clientID);
    if (obj == null)
      return null;
    else
      return ((Vector)obj).elements ();
  }

  // These should be replaced by weak hash maps when available
  
  // Map consisting of (token name) to (LockHandler instance) pairs
  private Hashtable lockHandlerMap_;
  
  // Map consisting of (Integer token type) to (Class instance for 
  // corresponding LockHandler class)
  private Hashtable handlerFactoryMap_;

  // Map consisting of (client ID) to (Vector of LockHandler) pairs
  private Hashtable clientHandlerMap_;
}

