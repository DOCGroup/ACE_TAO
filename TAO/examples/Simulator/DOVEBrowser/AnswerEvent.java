/**
 * @file AnswerEvent.java
 * @author Michael Kircher (mk1@cs.wustl.edu)
 * @brief Event definition for the Dialog for selecting Observables.
 */

public class AnswerEvent extends java.util.EventObject {

  protected String selected_;

  public AnswerEvent (Object source, String selected) {
    super (source);
    this.selected_ = selected;
  }
}

