package gjt;

import java.awt.*;

/**
 * A CardPanel whose Component viewSelector() returns
 * a panel with image buttons to control the selection of the
 * panel to be displayed beneath the view selector panel.<p>
 *
 * @version 1.0, Apr 1 1996
 * @author  David Geary
 * @see     CardPanel
 * @see     ChoiceCardpanel
 * @see     gjt.test.IconCardPanelTest
 */
public class IconCardPanel extends CardPanel {
    private ImageButtonPanel viewSelector;

    public IconCardPanel() {
        viewSelector = new IconViewSelector(this);
    }
    public Component viewSelector() {
        return viewSelector;
    }
    public void addImageButton(Image     image, 
                               String    name, 
                               Component component) {
        ImageButton newButton;

        viewSelector.add(
            newButton = new ImageButton(image), name);
        newButton.setController(
            new StickyImageButtonController(newButton));
        super.addView(name, component);
    }
}

class IconViewSelector extends ImageButtonPanel {
    private IconCardPanel mvp;

    public IconViewSelector(IconCardPanel panel) {
        super(Orientation.HORIZONTAL);
        setLayout(new FlowLayout());
        mvp = panel;
    }
    public boolean handleEvent(Event event) {
        if(event.id == Event.MOUSE_DOWN) {
            if(event.target instanceof ImageButton) {
                ImageButton ib = (ImageButton)event.target;
                    mvp.showView(getButtonName(ib)); 
            }
        }
        return super.handleEvent(event);
    }
}
