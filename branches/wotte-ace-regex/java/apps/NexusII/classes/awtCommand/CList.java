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

public class CList extends List {
    protected Command selectCommand 	= null;
    protected Command deselectCommand 	= null;
    protected Command actionCommand 	= null;

    /** 
     * Handles the event
     */
    public boolean handleEvent(Event evt) {
	switch (evt.id) {
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
     * Called if the Enter key is pressed. 
     * This results in a call to the actionCommand object 
     * with <code>what</code> set to the selected item.
     */
    public boolean action(Event evt, Object what) {
        if (actionCommand != null)
            actionCommand.execute(this, evt, what);
	return false;
    }

    /** 
     * Called if the mouse selects an item. 
     * This results in a call to the selectCommand object 
     * with <code>what</code> set to the selected index.
     */
    public boolean select(Event evt, Object what) {
        if (selectCommand != null)
            selectCommand.execute(this, evt, what);
	return false;
    }


    /** 
     * Called if the mouse deselects an item. 
     * This results in a call to the deselectCommand object 
     * with <code>what</code> set to the deselected index.
     */
    public boolean deselect(Event evt, Object what) {
        if (deselectCommand != null)
            deselectCommand.execute(this, evt, what);
	return false;
    }


    /**
     * Sets the actionCommand.
     */
    public void setActionCommand(Command action) {
	actionCommand = action;
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
