/*************************************************
 *
 * = PACKAGE
 *    netsvcs.Logger
 *
 * = FILENAME
 *    DefaultLMR.java
 *
 *
 *@author Everett Anderson
 *
 *************************************************/
package netsvcs.Logger;

import java.io.*;

/**
 *
 * <p><h2>DESCRIPTION</h2>
 *
 * <blockquote>
 * The LogMessageReceiver removes the code that handles a log message from
 * the logging service acceptor.  The DefaultLMR simply calls the LogRecord's
 * print method.  
 *
 * @see netsvcs.Logger.ServerLoggingAcceptor
 * @see netsvcs.Logger.LogRecord
 */
class DefaultLMR implements LogMessageReceiver
{
  public void logRecord (String hostname,
			 LogRecord record)
    {
      record.print(hostname, true, System.err);
    }
}
