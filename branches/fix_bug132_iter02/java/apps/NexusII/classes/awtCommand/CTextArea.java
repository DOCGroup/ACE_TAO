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

public class CTextArea extends TextArea {
    
    protected Command gotFocusCommand = null,
		      lostFocusCommand = null;

    /**
     * Constructs a new TextArea.
     */
    public CTextArea() {
	super();
    }

    /**
     * Constructs a new TextArea with the specified number of rows and columns.
     * @param rows the number of rows
     * @param cols the number of columns
     */
    public CTextArea(int rows, int cols) {
	super(rows, cols);
    }

    /**
     * Constructs a new TextArea with the specified text displayed.
     * @param text the text to be displayed 
     */
    public CTextArea(String text) {
        super(text);
    }

    /**
     * Constructs a new TextArea with the specified text and the 
     * specified number of rows 
     * and columns.
     * @param text the text to be displayed
     * @param rows the number of rows
     * @param cols the number of cols
     */
    public CTextArea(String text, int rows, int cols) {
	super(text, rows, cols);
    }


    /**
     * Called when the text area gains the focus.
     * This results in a call to the gotFocusCommand object 
     * with <code>what</code> set to null.
     */
    public boolean gotFocus(Event evt, Object what) {
	if (gotFocusCommand != null)
            gotFocusCommand.execute(this, evt, what);
	return false;
    }

    /**
     * Called when the text area loses the focus.
     * This results in a call to the lostFocusCommand object 
     * with <code>what</code> set to null.
     */
    public boolean lostFocus(Event evt, Object what) {
	if (lostFocusCommand != null)
            lostFocusCommand.execute(this, evt, what);
	return false;
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
