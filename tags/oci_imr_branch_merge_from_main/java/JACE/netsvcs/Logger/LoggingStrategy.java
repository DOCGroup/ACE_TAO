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
package JACE.netsvcs.Logger;

import java.io.*;

/**
 * Encapsulates the handling of a LogRecord from a given host, allowing
 * easy swapping of behavior in the logging service.  Strategies could
 * be developed to save to a file, print on a certain stream, forward
 * to another service, etc.
 *
 *@see StderrStrategy
 *@see LogRecord
 */
public interface LoggingStrategy
{
  /**
   * Process the given LogRecord.
   *
   *@param hostname host from which this LogRecord originated
   *@param record LogRecord instance to process
   */
  public void logRecord (String hostname,
			 LogRecord record);
}
