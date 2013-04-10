// $Id$
//
// = FILENAME
//    ObservablesDialog.java
//
// = AUTHOR
//    Michael Kircher (mk1@cs.wustl.edu)
//
// = DESCRIPTION
//   Dialog for selecting Observables.
//
// ============================================================================

import java.awt.*;
import java.awt.event.*;

public class ObservablesDialog extends Dialog {

  List list_;
  DemoCore parent_;
  AnswerListener listener_;

  ObservablesDialog (DemoCore parent,java.util.Enumeration enumeration) {

    super ((Frame)parent, "Select Observables", true);
    parent_ = parent;
    this.setSize(400, 300);
    setBounds (new Rectangle (50,50,400,300));

    list_ = new List ();
    list_.setFont ( new Font ("Helvetica", Font.PLAIN, 10));
    list_.setSize (200,200);

    for (; enumeration.hasMoreElements();) {
      list_.add ((String)enumeration.nextElement());
    }


    Button ok_button_ = new Button ("OK");
    ok_button_.setFont ( new Font ("DialogHelvetica", Font.PLAIN, 10));
    Button cancel_button_ = new Button ("Cancel");
    cancel_button_.setFont ( new Font ("DialogHelvetica", Font.PLAIN, 10));

    GridBagLayout gridbag_ = new GridBagLayout ();
    GridBagConstraints constraints_ = new GridBagConstraints ();

    constraints_.fill = GridBagConstraints.BOTH;
    this.setLayout (gridbag_);

    constraints_.weightx = 1.0;
    constraints_.weighty = 1.0;
    constraints_.gridwidth = GridBagConstraints.REMAINDER;
    gridbag_.setConstraints (list_, constraints_);
    this.add (list_);

    constraints_.weightx = 1.0;
    constraints_.weighty = 1.0;
    constraints_.gridwidth = GridBagConstraints.RELATIVE;
    constraints_.fill = GridBagConstraints.NONE;
    gridbag_.setConstraints (ok_button_, constraints_);
    gridbag_.setConstraints (cancel_button_, constraints_);
    this.add (ok_button_);
    this.add (cancel_button_);

    ok_button_.addActionListener (new ActionListener () {
      public void actionPerformed (ActionEvent e) {

	String selected_ = ObservablesDialog.this.list_.getSelectedItem ();
	  if (selected_ != null) {
	    System.out.println (">>>>> " + selected_);
	    if (listener_ != null) {
	      AnswerEvent ev = new AnswerEvent (ObservablesDialog.this, selected_);
	      listener_.ok (ev);
	    }
	    ObservablesDialog.this.setVisible (false);
	  }
      }
    });

     cancel_button_.addActionListener (new ActionListener () {
      public void actionPerformed (ActionEvent e) {
	ObservablesDialog.this.setVisible (false);
      }
     });
  }

  public void addAnswerListener (AnswerListener al) {
    listener_ = al;
  }
}
