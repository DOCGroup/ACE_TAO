package gjt;

import java.awt.Event;

/**
 * An interface for handling mouse events.<p>
 *
 * Components delegate handling of mouse events to a 
 * MouseController derivation.<p>
 *
 * For instance:<p>
 *<pre> 
 *      mouseDown(Event event, int x, int y) {
 *          return controller.mouseDown(event,x,y);
 *      }
 *</pre>
 * @version 1.0, Apr 1 1996
 * @author  David Geary
 * @see     ImageButton
 * @see     ImageButtonController
 * @see     SpringyImageButtonController
 * @see     StickyImageButtonController
 */
public interface MouseController {
    public boolean mouseEnter(Event event, int x, int y);
    public boolean mouseExit (Event event, int x, int y);

    public boolean mouseMove (Event event, int x, int y);
    public boolean mouseDown (Event event, int x, int y);
    public boolean mouseUp   (Event event, int x, int y);
    public boolean mouseDrag (Event event, int x, int y);
}
