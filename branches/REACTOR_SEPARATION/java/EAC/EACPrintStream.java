/**
 *  Title:        EACFileStream
 *  Description:  A stream class for conducting file I/O in the Event Analysis Configurator.
 */
package EAC;

import java.io.RandomAccessF;

public class EACPrintStream {

  PrintStream ps;

  public EACPrintStream(String filename) {
    ps = new PrintStream(new FileOutputStream(filename), true);
  }
}