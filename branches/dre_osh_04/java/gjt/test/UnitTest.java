package gjt.test;

import java.awt.BorderLayout;
import java.awt.Frame;
import java.awt.Panel;
import java.applet.Applet;
import gjt.*;

/**
 * An (abstract) Applet fitted with a BorderLayout that 
 * contains a TitledPanel in the North, and a Panel created by 
 * derived classes in the Center.<p>
 *
 * Since some Applets take awhile to load, UnitTest changes the 
 * cursor to a wait cursor in init(), changing it back to the 
 * default cursor in start().  Derived classes must be sure to 
 * call super.init() if they override init(); likewise for 
 * start().<p>
 *
 * Subclasses must implement: 
 * <dl>
 * <dd>String title()
 * <dd>Panel centerPanel()
 * </dl>
 * Subclasses should populate the Panel returned from 
 * centerPanel() with whatever makes sense for their unit test.
 *
 * @version 1.0, April 25, 1996
 * @author  David Geary
 * @see     TitledPanel
 */
abstract public class UnitTest extends Applet {
    abstract public String title();
    abstract public Panel  centerPanel();

    public void init() {
        Util.getFrame(this).setCursor(Frame.WAIT_CURSOR);
        Panel titledPanel = new TitledPanel(title());
        setLayout(new BorderLayout());
        add("North",  titledPanel);
        add("Center", centerPanel());
    }
    public void start() {
        Util.getFrame(this).setCursor(Frame.DEFAULT_CURSOR);
    }
}
