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
  public void logRecord (String hostname,
			 LogRecord record)
  {
    record.print(hostname, true, System.err);
  }
}
