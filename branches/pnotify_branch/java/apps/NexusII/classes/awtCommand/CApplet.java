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
import java.applet.*;

public class CApplet extends Applet {

    protected Command gotFocusCommand = null,
		      lostFocusCommand = null;
    protected Command mouseDownCommand = null,
		      mouseDragCommand = null,
		      mouseEnterCommand = null,
		      mouseExitCommand = null,
		      mouseMoveCommand = null,
		      mouseUpCommand = null;
    protected Command keyUpCommand = null,
		      keyDownCommand = null;

    /**
     * Constructs a new CApplet.
     */
    public CApplet() {
	super();
    }

    /*
     * event handling methods
     */

    /**
     * Called if the window gains focus. This results in a call to
     * the gotFocusCommand object with <code>what</code> set to null.
     */
    public boolean gotFocus(Event evt, Object what) {
	if (gotFocusCommand != null)
	    gotFocusCommand.execute(this, evt, what);
	return false;
    }

    /**
     * Called if the window loses focus. This results in a call to
     * the lostFocusCommand object with <code>what</code> set to null.
     */
     public boolean lostFocus(Event evt, Object what) {
	if (lostFocusCommand != null)
	    lostFocusCommand.execute(this, evt, what);
	return false;
    }

    /**
     * Called if the mouse is down.
     * This results in a call to the mouseDownCommand object with 
     * <code>what</code> set to Point(x, y)
     */
    public boolean mouseDown(Event evt, int x, int y) {
	if (mouseDownCommand != null)
	    mouseDownCommand.execute(this, evt, new Point(x, y));
	return false;
    }

    /**
     * Called if the mouse is dragged.
     * This results in a call to the mouseDragCommand object with 
     * <code>what</code> set to Point(x, y)
     */
    public boolean mouseDrag(Event evt, int x, int y) {
	if (mouseDragCommand != null)
	    mouseDragCommand.execute(this, evt, new Point(x, y));
	return false;
    }

    /**
     * Called if the mouse enters the window.
     * This results in a call to the mouseEnterCommand object with 
     * <code>what</code> set to Point(x, y)
     */
     public boolean mouseEnter(Event evt, int x, int y) {
	if (mouseEnterCommand != null)
	    mouseEnterCommand.execute(this, evt, new Point(x, y));
	return false;
    }

    /**
     * Called if the mouse moves inside the window.
     * This results in a call to the mouseMoveCommand object with 
     * <code>what</code> set to Point(x, y)
     */
    public boolean mouseMove(Event evt, int x, int y) {
	if (mouseExitCommand != null)
	    mouseExitCommand.execute(this, evt, new Point(x, y));
	return false;
    }

    /**
     * Called if the mouse is up.
     * This results in a call to the mouseUpCommand object with 
     * <code>what</code> set to Point(x, y)
     */
    public boolean mouseUp(Event evt, int x, int y) {
	if (mouseUpCommand != null)
	    mouseUpCommand.execute(this, evt, new Point(x, y));
	return false;
    }

    /**
     * Called if a character is pressed.
     * This results in a call to the keyDownCommand object with 
     * <code>what</code> set to Integer(key).
     */
    public boolean keyDown(Event evt, int key) {
	if (keyDownCommand != null)
	    keyDownCommand.execute(this, evt, new Integer(key));
	return false;
    }

    /**
     * Called if a character is released.
     * This results in a call to the keyUpCommand object with 
     * <code>what</code> set to Integer(key).
     */
    public boolean keyUp(Event evt, int key) {
	if (keyUpCommand != null)
	    keyUpCommand.execute(this, evt, new Integer(key));
	return false;
    }


    /*
     * set...Command methods
     */ 


    /**
     * Sets the mouseDownCommand object.
     */
    public void setMouseDownCommand(Command c) {
	mouseDownCommand = c;
    }

    /**
     * Sets the moueDragCommand object.
     */
    public void setMouseDragCommand(Command c) {
	mouseDragCommand = c;
    }

    /**
     * Sets the mouseEnterCommand object.
     */
    public void setMouseEnterCommand(Command c) {
	mouseEnterCommand = c;
    }

    /**
     * Sets the mouseExitCommand object.
     */
    public void setMouseExitCommand(Command c) {
	mouseExitCommand = c;
    }

    /**
     * Sets the mouseMoveCommand object.
     */
    public void setMouseMoveCommand(Command c) {
	mouseMoveCommand = c;
    }

    /**
     * Sets the mouseUpCommand object.
     */
    public void setMouseUpCommand(Command c) {
	mouseUpCommand = c;
    }

    /**
     * Sets the keyDownCommand object.
     */
    public void setKeyDownCommand(Command c) {
	keyDownCommand = c;
    }

    /**
     * Sets the keyUpCommand object.
     */
    public void setKeyUpCommand(Command c) {
	keyUpCommand = c;
    }
}
