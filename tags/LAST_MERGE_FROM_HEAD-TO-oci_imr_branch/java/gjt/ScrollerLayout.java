package gjt;

import java.awt.*;

/**
 * Layout manager for a Scroller.<p>
 *
 * Lays out 3 Components:  a horizontal scrollbar, a vertical 
 * scrollbar and a viewport (Panel).<p>
 *
 * Valid names/Component pairs that can be added via 
 * addLayoutComponent(String, Component):<p>
 * <dl>
 * <dd> "East"   Scrollbar (vertical)
 * <dd> "West"   Scrollbar (vertical)
 * <dd> "North"  Scrollbar (horizontal)
 * <dd> "South"  Scrollbar (horizontal)
 * <dd> "Scroll" Panel (viewport)
 * </dl>
 *
 * @version 1.0, Apr 1 1996
 * @author  David Geary
 * @see     Scroller
 */
public class ScrollerLayout implements LayoutManager {
    private Scroller  scroller;
    private Scrollbar hbar, vbar;
    private String    hbarPosition, vbarPosition;
    private Component viewport;
    private int       top, bottom, right, left;

    public ScrollerLayout(Scroller scroller) {
        this.scroller = scroller;
    }

    public void addLayoutComponent(String name, 
                                   Component comp) {
        Assert.notFalse(comp != null);

        if(comp instanceof Scrollbar) {
            Scrollbar sbar = (Scrollbar)comp;

            if(sbar.getOrientation() == Scrollbar.VERTICAL) {
                Assert.notFalse("East".equals(name) == true ||
                                "West".equals(name) == true);
                vbar         = sbar;
                vbarPosition = name;
            }
            else {
                Assert.notFalse("North".equals(name) == true ||
                                "South".equals(name) == true);
                hbar         = sbar;
                hbarPosition = name;
            }
        }
        else {
            Assert.notFalse("Scroll".equals(name) == true);
            viewport = comp;
        }
    }
    public void removeLayoutComponent(Component comp) {
        if(comp == vbar)     vbar     = null;
        if(comp == hbar)     hbar     = null;
        if(comp == viewport) viewport = null;
    }
    public Dimension preferredLayoutSize(Container parent) {
        Dimension dim = new Dimension(0,0);

        if(vbar != null && vbar.isVisible()) {
            Dimension d = vbar.preferredSize();
            dim.width += d.width;
            dim.height = d.height;
        }
        if(hbar != null && hbar.isVisible()) {
            Dimension d = hbar.preferredSize();
            dim.width += d.width;
            dim.height = Math.max(d.height, dim.height);
        }
        if(viewport != null && viewport.isVisible()) {
            Dimension d = viewport.preferredSize();
            dim.width += d.width;
            dim.height = Math.max(d.height, dim.height);
        }
        return dim;
    }
    public Dimension minimumLayoutSize(Container parent) {
        Dimension dim = new Dimension(0,0);

        if(vbar != null && vbar.isVisible()) {
            Dimension d = vbar.minimumSize();
            dim.width += d.width;
            dim.height = d.height;
        }
        if(hbar != null && hbar.isVisible()) {
            Dimension d = hbar.minimumSize();
            dim.width += d.width;
            dim.height = Math.max(d.height, dim.height);
        }
        if(viewport != null && viewport.isVisible()) {
            Dimension d = viewport.minimumSize();
            dim.width += d.width;
            dim.height = Math.max(d.height, dim.height);
        }
        return dim;
    }
    public void layoutContainer(Container target) {
        Insets insets        = target.insets();
        Dimension targetSize = target.size();

        top    = insets.top;
        bottom = targetSize.height - insets.bottom;
        left   = insets.left;
        right  = targetSize.width - insets.right;

        scroller.manageScrollbars();

        reshapeHorizontalScrollbar();
        reshapeVerticalScrollbar  ();
        reshapeViewport           ();

        scroller.setScrollbarValues();
    }
    private void reshapeHorizontalScrollbar() {
        if(hbar != null && hbar.isVisible()) {
            if("North".equals(hbarPosition)) {
                Dimension d = hbar.preferredSize();
                hbar.reshape(left, top, right - left, d.height);
                top += d.height;
            }
            else {  // South
                Dimension d = hbar.preferredSize();
                hbar.reshape(left, bottom - d.height,
                            right - left,d.height);
                bottom -= d.height;
            }
        }
    }
    private void reshapeVerticalScrollbar() {
        if(hbar != null && vbar.isVisible()) {
            if("East".equals(vbarPosition)) {
                Dimension d = vbar.preferredSize();
                vbar.reshape(right - d.width, top, 
                             d.width, bottom - top);
                right -= d.width;
            }
            else { // West
                Dimension d = vbar.preferredSize();
                vbar.reshape(left, top, 
                             d.width, bottom - top);
                left += d.width;
            }
        }
    }
    private void reshapeViewport() {
        if(viewport != null && viewport.isVisible()) {
            viewport.reshape(left, top, 
                             right - left, bottom - top);
        }
    }
}
