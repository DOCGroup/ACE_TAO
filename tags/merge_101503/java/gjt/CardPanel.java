package gjt;

import java.awt.*;

/**
 * CardPanel employs a BorderLayout to lay out North and Center 
 * panels; extensions of CardPanel must implement 
 * Component viewSelector().  The component returned from 
 * Component viewSelector() is centered in the North panel, and 
 * should contain UI controls that allow selection of the 
 * component to be displayed in the Center panel.<p>
 *
 * @version 1.0, Apr 1 1996
 * @author  David Geary
 * @see     IconCardPanel
 * @see     ChoiceCardPanel
 * @see     gjt.test.ChoiceCardPanelTest
 * @see     gjt.test.IconCardPanelTest
 */
public abstract class CardPanel extends Panel {
    private Panel       north, center;
    private CardLayout  cards;

    abstract public Component viewSelector();

    public CardPanel() {
        center = new Panel();
        north  = new Panel();
        
        setLayout(new BorderLayout());
        center.setLayout(cards = new CardLayout());
        north.setLayout (new BorderLayout());

        add("North",  north);
        add("Center", center);
    }
    public void addNotify() {
        super.addNotify();
        north.add("Center", viewSelector());
        north.add("South",  new Separator());
    }
    protected void addView(String name, Component component) {
        center.add(name, component);
    }
    protected void showView(String name) {
        cards.show(center, name); 
    }
}
