package gjt.test;

import java.applet.Applet;
import java.awt.*;
import java.net.URL;
import gjt.IconCardPanel; 

/**
 * A gjt.IconCardPanel that controls 3 Panels.<p>
 *
 * @version 1.0, April 25, 1996
 * @author  David Geary
 * @see     gjt.test.UnitTest
 * @see     gjt.IconCardPanel
 */
public class IconCardPanelTest extends UnitTest {
    public String title() { return "IconCardPanel Test"; }
    public Panel centerPanel() { 
        return new CardPanelTestPanel(this); 
    }
}
class CardPanelTestPanel extends Panel {
    IconCardPanel mvp = new IconCardPanel();

    public CardPanelTestPanel(Applet applet) {
        URL cb = applet.getCodeBase();

        setLayout(new BorderLayout());

        Image folks = applet.getImage(cb,"gifs/cell_phone.gif");
        Image pencil = applet.getImage(cb,"gifs/clipboard.gif");
        Image library =
                applet.getImage(cb, "gifs/mad_hacker.gif");

        mvp.addImageButton(folks, 
                           "Attributes", 
                           new AttributesPanel(applet));
        mvp.addImageButton(pencil, 
                           "Connections", 
                           new ConnectionsPanel());
        mvp.addImageButton(library, 
                           "Oracle", 
                           new OccupationOracle());

        add("Center", mvp);
    }
}
