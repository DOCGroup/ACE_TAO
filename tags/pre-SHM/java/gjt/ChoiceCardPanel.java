package gjt;

import java.awt.*;

/**
 * ChoiceCardPanel is an extension of CardPanel which presents
 * an awt.Choice for selecting the panel to be displayed
 * in the center panel.<p>
 *
 * @version 1.0, Apr 1 1996
 * @author  David Geary
 * @see     CardPanel
 * @see     IconCardPanel
 * @see     gjt.test.IconCardPanelTest
 * @see     gjt.test.ChoiceCardPanelTest
 */
public class ChoiceCardPanel extends CardPanel {
    private ChoiceViewSelector viewSelector;

    public ChoiceCardPanel() {
        viewSelector = new ChoiceViewSelector(this);
    }
    public Component viewSelector() {
        return viewSelector;
    }
    public void addChoice(String    name, 
                          Component component) {
        viewSelector.addItem(name);
        super.addView(name, component);
    }
}

class ChoiceViewSelector extends Panel {
    private ChoiceCardPanel mvp;
    private Choice          choice;

    public ChoiceViewSelector(ChoiceCardPanel panel) {
        setLayout(new FlowLayout());
        add(choice = new Choice());
        mvp = panel;
    }
    public void addItem(String name) {
        choice.addItem(name);
    }
    public boolean handleEvent(Event event) {
        if(event.id == Event.ACTION_EVENT) {
            if(event.target instanceof Choice) {
                mvp.showView(choice.getSelectedItem());
            }
        }
        return super.handleEvent(event);
    }
}
