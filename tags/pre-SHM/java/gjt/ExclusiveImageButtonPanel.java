package gjt;

import java.awt.Image;

/**
 * An ImageButtonPanel which fits all of its ImageButtons with 
 * a StickyImageButtonController.  ExclusiveImageButtonPanel 
 * relies upon its superclass' controller:  a 
 * RadioImageButtonPanelController, which ensures that only one 
 * of the ImageButtons is selected at a time.<p>
 *
 * @version 1.0, Apr 1 1996
 * @author  David Geary
 * @see     ImageButton
 * @see     ImageButtonPanel
 * @see     gjt.test.ToolbarTest
 */
public class ExclusiveImageButtonPanel extends 
                                       ImageButtonPanel {
    public ExclusiveImageButtonPanel(Orientation orient) {
        this(orient, 5);
    }
    public ExclusiveImageButtonPanel(Orientation orient, 
                                     int gap) {
        super(orient, gap);
    }
    public ExclusiveImageButtonPanel(Orientation orient, 
                                     Orientation horient,
                                     Orientation vorient,
                                     int gap) {
        super(orient, horient, vorient, gap);
    }
    public void add(ImageButton button) {
        super.add(button);
        new StickyImageButtonController(button);
    }
    public ImageButton add(Image image) {
        ImageButton button = super.add(image);
        new StickyImageButtonController(button);
        return button;
    }
    public ImageButton add(Image image, String name) {
        ImageButton button = super.add(image, name);
        new StickyImageButtonController(button);
        return button;
    }
}
