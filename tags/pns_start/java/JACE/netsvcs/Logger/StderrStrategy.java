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
package JACE.netsvcs.Logger;

import java.io.*;

/**
 * Default LoggingStrategy for the logging service.  This prints out the
 * LogRecord on the System.err stream.
 *
 * @see JACE.netsvcs.Logger.LogRecord
 */
public class StderrStrategy implements LoggingStrategy
{
  /**
   * Process the given LogRecord by printing it on the System.err stream.
   *
   *@param hostname host from which this LogRecord originated
   *@param record LogRecord instance to process
   */
  public void logRecord (String hostname,
			 LogRecord record)
  {
    record.print(hostname, true, System.err);
  }
}
