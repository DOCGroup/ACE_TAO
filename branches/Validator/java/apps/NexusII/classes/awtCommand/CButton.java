
package awtCommand;

import java.lang.*;
import java.awt.*;

public class CButton extends java.awt.Button {
    protected Command actionCommand = null;

    /**
     * Constructs a CButton.
     */
    public CButton() {
	super();
    }

    /**
     * Constructs a CButton with the given name.
     */
    public CButton(String name) {
	super(name);
    }

   /**
    * Sets the actionCommand object.
    */
    public void setActionCommand(Command action) {
	actionCommand = action;
    }

    /**
     * Called when the button is selected..
     * This results in a call to the actionCommand object 
     * with <code>what</code> set to the button's label.
     */
    public boolean action(Event evt, Object what) {
	if (actionCommand != null)
	    actionCommand.execute(this, evt, what);
	return false;
    }
}
