package gjt.test;

import java.applet.Applet;
import java.awt.*;
import gjt.ChoiceCardPanel;

/**
 * A ChoiceCardPanel that controls three Panels.<p>
 *
 * @version 1.0, April 25, 1996
 * @author  David Geary
 * @see     gjt.test.UnitTest
 * @see     gjt.ChoiceCardPanel
 */
public class ChoiceCardPanelTest extends UnitTest {
    public String title() { return "Choice CardPanel Test"; }
    public Panel centerPanel() {
        return new ChoiceCardPanelTestPanel(this);
    }
}

class ChoiceCardPanelTestPanel extends Panel {
    private ChoiceCardPanel mvp = new ChoiceCardPanel();

    public ChoiceCardPanelTestPanel(Applet applet) {
        setLayout(new BorderLayout());

        mvp.addChoice("Attributes",  
                        new AttributesPanel(applet));
        mvp.addChoice("Connections", new ConnectionsPanel());
        mvp.addChoice("Oracle",      new OccupationOracle());
        add("Center", mvp);
    }
}
