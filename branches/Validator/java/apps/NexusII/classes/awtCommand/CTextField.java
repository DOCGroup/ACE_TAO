/*
 * Copyright 1996 Jan Newmarch, University of Canberra.
 * Permission to use, copy, modify, and distribute this
 * software and its documentation for any purpose and without
 * fee is hereby granted, provided that the above copyright
 * notice appear in all copies.  The author
 * makes no representations about the suitability of this
 * software for any purpose.  It is provided "as is" without
 * express or implied warranty.
 */

package awtCommand;

import java.awt.*;

public class CTextField extends TextField {

    protected Command actionCommand = null,
		      gotFocusCommand = null,
		      lostFocusCommand = null;

    /**
     * Constructs a new CTextField.
     */
    public CTextField() {
	super();
    }

    /**
     * Constructs a new CTextField initialized with the specified columns.
     * @param cols the number of columns
     */
    public CTextField(int cols) {
	super(cols);
    }

    /**
     * Constructs a new CTextField initialized with the specified text.
     * @param text the text to be displayed
     */
    public CTextField(String text) {
	super(text);
    }


    /**
     * Constructs a new CTextField initialized with the specified text and columns.
     * @param text the text to be displayed
     * @param cols the number of columns
     */
    public CTextField(String text, int cols) {
	super(text, cols);
    }


    /**
     * Called when Enter is pressed in the text field.
     * This results in a call to the actionCommand object 
     * with <code>what</code> set to the text contents.
     */
    public boolean action(Event evt, Object what) {
	if (actionCommand != null)
            actionCommand.execute(this, evt, what);
	return false;
    }

    /**
     * Called when the text field gains the focus.
     * This results in a call to the gotFocusCommand object 
     * with <code>what</code> set to null.
     */
    public boolean gotFocus(Event evt, Object what) {
	if (gotFocusCommand != null)
            gotFocusCommand.execute(this, evt, what);
	return false;
    }

    /**
     * Called when the text field loses the focus.
     * This results in a call to the lostFocusCommand object 
     * with <code>what</code> set to null.
     */
    public boolean lostFocus(Event evt, Object what) {
	if (lostFocusCommand != null)
            lostFocusCommand.execute(this, evt, what);
	return false;
    }

   /**
    * Sets the actionCommand object.
    */
    public void setActionCommand(Command c) {
	actionCommand = c;
    }

 
   /**
    * Sets the gotFocusCommand object.
    */
    public void setGotFocusCommand(Command c) {
	gotFocusCommand = c;
    }

    /**
     * Sets the lostFocusCommand object.
     */
    public void setLostFocusCommand(Command c) {
	lostFocusCommand = c;
    }
}
