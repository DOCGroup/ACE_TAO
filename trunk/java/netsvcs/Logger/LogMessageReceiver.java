/*************************************************
 *
 * = PACKAGE
 *    netsvcs.Logger
 *
 * = FILENAME
 *    LogMessageReceiver.java
 *
 *@author Everett Anderson
 *
 *************************************************/
package netsvcs.Logger;

import java.lang.*;
import java.io.*;
import netsvcs.Logger.LogRecord;

/**
 *
 * <p><h2>DESCRIPTION</h2>
 *
 * <blockquote>
 * The LogMessageReceiver removes the code that handles a log message from
 * the logging service acceptor.  The DefaultLMR simply calls the LogRecord's
 * print method.  Other implementations of this interface can be built and
 * given to the ServerLoggingAcceptor to change the result.
 *
 * @see netsvcs.Logger.ServerLoggingAcceptor, netsvcs.Logger.LogRecord
 */
public interface LogMessageReceiver
{
  public void logRecord (String hostname,
			 LogRecord record);
};

class DefaultLMR implements LogMessageReceiver
{
  public void logRecord (String hostname,
			 LogRecord record)
    {
      record.print(hostname, true, System.err);
    }
}
