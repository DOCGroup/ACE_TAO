/**
 * @file VisComp.java
 * @author Michael Kircher (mk1@cs.wustl.edu)
 * @brief This is the interface for Java Beans.
 */

public interface VisComp extends java.util.Observer {

  public void setName (String title);
  public int getProperty ();
}
