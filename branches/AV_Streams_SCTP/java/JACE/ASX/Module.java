/*************************************************
 *
 * = PACKAGE
 *    JACE.ASX
 *
 * = FILENAME
 *    Module.java
 *
 *@author Prashant Jain
 *
 *************************************************/
package JACE.ASX;

import JACE.OS.*;

/**
 *   Provides an abstraction for managing a bi-directional flow of
 *   messages. <P>
 *
 *   This is based on the Module concept in System V Streams,
 *   which contains a pair of Tasks, one for handling upstream
 *   processing, one for handling downstream processing.
 */
public class Module
{
  // = Initialization and termination methods.

  /**
   * Create an empty Module.
   */
  public Module ()
  {
    // Do nothing...
    this.name ("<unknown>");
  }

  /*
   * Create an initialized module.
   *@param modName identity of the module.
   *@param writerQ writer task of the module.
   *@param readerQ reader task of the module.
   *@param flags Module flags
   */
  public Module (String modName,
		 Task writerQ,
		 Task readerQ,
		 Object flags)
  {
    this.open (modName, writerQ, readerQ, flags);
  }
		
  /*
   * Create an initialized module.
   *@param modName identity of the module.
   *@param writerQ writer task of the module.
   *@param readerQ reader task of the module.
   *@param flags Module flags
   */
  public void open (String modName,
		   Task writerQ,
		   Task readerQ,
		   Object arg)
  {
    this.name (modName);
    this.arg_ = arg;

    if (writerQ == null)
      writerQ = new ThruTask ();
    if (readerQ == null)
      readerQ = new ThruTask ();

    this.reader (readerQ);
    this.writer (writerQ);

    // Setup back pointers.
    readerQ.module (this);
    writerQ.module (this);
  }


  /*
   * Set the writer task.
   *@param q the writer task
   */
  public void writer (Task q)
  { 
    this.qPair_[1] = q;
    if (q != null)
      q.flags (ACE.CLR_BITS (q.flags (), TaskFlags.ACE_READER));
  }

  /*
   * Set the reader task.
   *@param q the reader task
   */
  public void reader (Task q) 
  {
    this.qPair_[0] = q;
    if (q != null)
      q.flags (ACE.SET_BITS (q.flags (), TaskFlags.ACE_READER));
  }

  /*
   * Link this Module on top of Module.
   *@param m the module to link this on top of.
   */
  public void link (Module m)
  {
    this.next (m);
    this.writer ().next (m.writer ());
    m.reader ().next (this.reader ());
  }

  /*
   * Set and get pointer to sibling Task in Module.
   *@param orig the task to get the sibling for
   *@return the sibling of the task
   */
  public Task sibling (Task orig)
  {
  if (this.qPair_[0] == orig)
    return this.qPair_[1];
  else if (this.qPair_[1] == orig)
    return this.qPair_[0];
  else
    return null;
  }

  /*
   * Close down the module and its tasks.
   *@param flags Module flags
   *@return 0 on success, -1 on failure
   */
  public int close (long flags)
  {
    Task readerQ = this.reader ();
    Task writerQ = this.writer ();
    int result = 0;

    if (readerQ != null)
      {
	if (readerQ.close (flags) == -1)
	  result = -1;
	readerQ.flush (flags);
	readerQ.next (null);
      }

    if (writerQ != null)
      {
	if (writerQ.close (flags) == -1)
	  result = -1;
	writerQ.flush (flags);
	writerQ.next (null);
      }

    return result;
  }

  /* 
   * Get the argument passed to tasks.
   *@return the argument passed to tasks.
   */
  public Object arg ()
  {
    return this.arg_;
  }

  /* 
   * Set the argument to be passed to tasks.
   *@param a the argument to be passed to tasks.
   */
  public void arg (Object a)
  {
    this.arg_ = a;
  }

  /* 
   * Get the name of the module.
   *@return the name of the module.
   */
  public String name ()
  {
    return this.name_;
  }

  /* 
   * Set the name of the module.
   *@param n the name of the module.
   */
  public void name (String n)
  {
    this.name_ = n;
  }

  /* 
   * Get the writer task of the module.
   *@return the writer task of the module.
   */
  public Task writer ()
  { 
    return this.qPair_[1];
  }

  /* 
   * Get the reader task of the module.
   *@return the reader task of the module.
   */
  public Task reader ()
  {
    return this.qPair_[0];
  }

  /*
   * Get the next pointer to the module above in the stream.
   *@return the next pointer to the module above in the stream.
   */
  public Module next ()
  {
    return this.next_;
  }

  /*
   * Set the next pointer to the module above in the stream.
   *@param m the next pointer to the module above in the stream.
   */
  public void next (Module m) 
  {
    this.next_ = m;
  }

  private Task qPair_[] = new Task[2];
  // Pair of Tasks that form the "read-side" and "write-side" of the
  // ACE_Module partitioning.

  private String name_ = null;
  // Name of the ACE_Module.

  private Module next_;
  // Next ACE_Module in the stack.

  private Object arg_;
  // Argument passed through to the reader and writer task when they
  // are opened.

}

