package gjt;

import java.awt.*;

/**
 * Lays out components as though they were pinned to 
 * a bulletin board.<p>
 *
 * Components are simply reshaped to their location and their
 * preferred size.  BulletinLayout is preferrable to setting
 * a container's layout manager to null and explicitly positioning
 * and sizing components.<p>
 *
 * @version 1.0, Apr 1 1996
 * @author  David Geary
 */
public class BulletinLayout implements LayoutManager {
    public BulletinLayout() {
    }
    public void addLayoutComponent(String name, Component comp) {
    }
    public void removeLayoutComponent(Component comp) {
    }
    public Dimension preferredLayoutSize(Container target) {
        Insets    insets      = target.insets();
        Dimension dim         = new Dimension(0,0);
        int       ncomponents = target.countComponents();
        Component comp;
        Dimension d;
        Rectangle preferredBounds = new Rectangle(0,0);
        Rectangle compPreferredBounds;

        for (int i = 0 ; i < ncomponents ; i++) {
            comp = target.getComponent(i);

            if(comp.isVisible()) {
                d = comp.preferredSize();
                compPreferredBounds = 
                    new Rectangle(comp.location());
                compPreferredBounds.width  = d.width;
                compPreferredBounds.height = d.height;

                preferredBounds = 
                    preferredBounds.union(compPreferredBounds);
            }
        }
        dim.width  += insets.left + insets.right;
        dim.height += insets.top + insets.bottom;

        return dim;
    }
    public Dimension minimumLayoutSize(Container target) {
        Insets    insets      = target.insets();
        Dimension dim         = new Dimension(0,0);
        int       ncomponents = target.countComponents();
        Component comp;
        Dimension d;
        Rectangle minimumBounds = new Rectangle(0,0);
        Rectangle compMinimumBounds;

        for (int i = 0 ; i < ncomponents ; i++) {
            comp = target.getComponent(i);

            if(comp.isVisible()) {
                d = comp.minimumSize();
                compMinimumBounds = 
                    new Rectangle(comp.location());
                compMinimumBounds.width  = d.width;
                compMinimumBounds.height = d.height;

                minimumBounds = 
                    minimumBounds.union(compMinimumBounds);
            }
        }
        dim.width  += insets.left + insets.right;
        dim.height += insets.top + insets.bottom;

        return dim;
    }
    public void layoutContainer(Container target) {
        Insets    insets      = target.insets();
        int       ncomponents = target.countComponents();
        Component comp;
        Dimension ps;
        Point loc;

        for (int i = 0 ; i < ncomponents ; i++) {
            comp = target.getComponent(i);

            if(comp.isVisible()) {
                ps  = comp.preferredSize();    
                loc = comp.location();

                comp.reshape(insets.left + loc.x, 
                             insets.top + loc.y,
                             ps.width, ps.height);
            }
        }
    }
}
