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

public class CPanel extends Panel {

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
    protected Command actionCommand = null;
    protected Command scrollAbsoluteCommand = null,
	      	      lineDownCommand = null,
	      	      lineUpCommand = null,
	      	      pageDownCommand = null,
	      	      pageUpCommand = null;
    protected Command selectCommand 	= null,
		      deselectCommand 	= null;

    /**
     * Constructs a new CPanel.
     */
    public CPanel() {
	super();
    }


    /**
     * Handles the event.
     * Calls methods for variousL events and passes
     * others to its superclass method.
     */
    public boolean handleEvent(Event evt) {
	switch (evt.id) {
	    case Event.SCROLL_ABSOLUTE:
		return scrollAbsolute(evt, evt.arg);
	    case Event.SCROLL_LINE_DOWN:
		return lineDown(evt, evt.arg);
	    case Event.SCROLL_LINE_UP:
		return lineUp(evt, evt.arg);
	    case Event.SCROLL_PAGE_DOWN:
		return pageDown(evt, evt.arg);
	    case Event.SCROLL_PAGE_UP:
		return pageUp(evt, evt.arg);
	    case Event.LIST_SELECT:
		return select(evt, evt.arg);
	    case Event.LIST_DESELECT:
		return deselect(evt, evt.arg);
	    default:
		return super.handleEvent(evt);
	}
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


    /**
     * Called when an ACTION_EVENT is generated.
     * This results in a call to the actionCommand object 
     * with <code>what</code> set to the event's arg.
     */
    public boolean action(Event evt, Object what) {
	if (actionCommand != null)
	    actionCommand.execute(this, evt, what);
	return false;
    }

    /**
     * Called when a scrollbar is dragged.
     * This results in a call to the scrollAbsoluteCommand object
     * with <code>what</code> set to the slider location value.
     */
    public boolean scrollAbsolute(Event evt, Object what) {
        if (scrollAbsoluteCommand != null)
            scrollAbsoluteCommand.execute(this, evt, what);
	return false;
    }

    /**
     * Called when a scrollbar is incremented down.
     * This results in a call to the lineDownCommand object
     * with <code>what</code> set to the slider location value.
     */
    public boolean lineDown(Event evt, Object what) {
        if (lineDownCommand != null)
            lineDownCommand.execute(this, evt, what);
	return false;
    }

    /**
     * Called when a scrollbar is incremented up.
     * This results in a call to the lineUpCommand object
     * with <code>what</code> set to the slider location value.
     */
    public boolean lineUp(Event evt, Object what) {
        if (lineUpCommand != null)
            lineUpCommand.execute(this, evt, what);
	return false;
    }

    /**
     * Called when a scrollbar pages up.
     * This results in a call to the pageUpCommand object
     * with <code>what</code> set to the slider location value.
     */
    public boolean pageUp(Event evt, Object what) {
        if (pageUpCommand != null)
            pageUpCommand.execute(this, evt, what);
	return false;
    }

    /**
     * Called when a scrollbar pages down.
     * This results in a call to the pageDownCommand object
     * with <code>what</code> set to the slider location value.
     */
    public boolean pageDown(Event evt, Object what) {
        if (pageDownCommand != null)
            pageDownCommand.execute(this, evt, what);
	return false;
    }

    /** 
     * Called if the mouse selects an item in a List. 
     * This results in a call to the selectCommand object 
     * with <code>what</code> set to the selected index.
     */
    public boolean select(Event evt, Object what) {
        if (selectCommand != null)
            selectCommand.execute(this, evt, what);
	return false;
    }


    /** 
     * Called if the mouse deselects an item in a List. 
     * This results in a call to the deselectCommand object 
     * with <code>what</code> set to the deselected index.
     */
    public boolean deselect(Event evt, Object what) {
        if (deselectCommand != null)
            deselectCommand.execute(this, evt, what);
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


   /**
    * Sets the actionCommand object.
    */
    public void setActionCommand(Command action) {
	actionCommand = action;
    }

    /**
     * Sets the scrollAbsoluteCommand.
     */
    public void setScrollAbsoluteCommand(Command c) {
	scrollAbsoluteCommand = c;
    }

    /**
     * Sets the lineUpCommand.
     */
    public void setLineUpCommand(Command c) {
	lineUpCommand = c;
    }

    /**
     * Sets the lineDownCommand.
     */
    public void setLineDownCommand(Command c) {
	lineDownCommand = c;
    }

    /**
     * Sets the pageUpCommand.
     */
    public void setPageUpCommand(Command c) {
	pageUpCommand = c;
    }

    /**
     * Sets the pageDownCommand.
     */
    public void setPageDownCommand(Command c) {
	pageDownCommand = c;
    }

    /**
     * Sets the selectCommand.
     */
    public void setSelectCommand(Command select) {
	selectCommand = select;
    }

    /**
     * Sets the deselectCommand.
     */
    public void setDeselectCommand(Command deselect) {
	deselectCommand = deselect;
    }

}
