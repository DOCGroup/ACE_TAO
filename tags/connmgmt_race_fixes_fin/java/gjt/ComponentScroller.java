package gjt;

import java.awt.*;

/**
 * Scrolls any component.  Component to be scrolled may be a
 * container, so ultimately many components may be scrolled
 * at once.<p>
 *
 * Component to be scrolled may be specified at construction
 * time, or may be set after construction via 
 * void setComponent(Component).<p>
 *
 * @version 1.0, Apr 1 1996
 * @author  David Geary
 * @see     BulletinLayout
 * @see     Scroller
 * @see     ScrollerLayout
 * @see     ImageScroller
 * @see     gjt.test.ComponentScrollerTest
 */
public class ComponentScroller extends Scroller {
    private Component scrollMe;

    public ComponentScroller() {
    }
    public ComponentScroller(Component component) {
        setComponent(component);
    }
    public void setComponent(Component component) {
        scrollMe = component;
        viewport.setLayout(new BulletinLayout());
        viewport.add      (scrollMe);
        viewport.move     (0,0);
    }
    public void scrollTo(int x, int y) {
        scrollMe.move(-x,-y);
    }
    public Dimension getScrollAreaSize() {
        return scrollMe.preferredSize();
    }
}
