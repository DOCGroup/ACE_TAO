package gjt.test;

import java.applet.Applet;
import java.awt.*;
import java.net.URL;
import gjt.StateButton;
import gjt.ImageButtonEvent;

/**
 * A StateButton which cycles through a fascinating series of 
 * Images.<p>
 *
 * @version 1.0, April 25, 1996
 * @author  David Geary
 * @see     gjt.test.UnitTest
 * @see     gjt.StateButton
 */
public class StateButtonTest extends UnitTest {
    public String title      () { return "StateButton Test";  }
    public Panel  centerPanel() { 
        return new StateButtonTestPanel(this); 
    }
}

class StateButtonTestPanel extends Panel {
    private URL         codeBase;
    private Image[]     images;
    private StateButton button;

    public StateButtonTestPanel(Applet applet) {
        codeBase  = applet.getCodeBase();
        images    = new Image[3];
        images[0] = applet.getImage(codeBase, "gifs/fly.gif");
        images[1] = applet.getImage(codeBase, "gifs/frog.gif");
        images[2] = applet.getImage(codeBase, "gifs/eagle.gif");
        button    = new StateButton(images);

        setLayout(new FlowLayout(FlowLayout.CENTER, 20, 20));
        add      (button);
    }
}
