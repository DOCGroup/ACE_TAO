package gjt.test;

import java.applet.Applet;
import java.net.URL;
import java.awt.*;
import gjt.*;

/**
 * Lays out 3 image buttons, and provides controls for setting
 * orientations and gaps on the fly.<p>
 *
 * @version 1.0, April 25, 1996
 * @author  David Geary
 * @see     gjt.test.UnitTest
 * @see     gjt.ImageButton
 * @see     gjt.Box
 */
public class RowLayoutTest extends UnitTest {
    public String title() { 
        return "RowLayout Test";
    }
    public Panel centerPanel() {
        RowButtonPanel buttonPanel = new RowButtonPanel(this);
        Panel panel = new Panel();

        panel.setLayout(new BorderLayout());
        panel.add("Center", buttonPanel);
        panel.add("North",  new Box(new RowPicker(buttonPanel),
                            "Row Layout Settings"));
        return panel;
    }
}

class RowButtonPanel extends Panel implements DialogClient {
    private ImageButton    one, two, three;
    private Panel          panel  = new Panel();
    private TenPixelBorder border = new TenPixelBorder(panel);

    public RowButtonPanel(Applet applet) {
        URL cb = applet.getCodeBase();

        one   = new ImageButton(applet.getImage(cb, 
                                            "gifs/one.gif"));
        two   = new ImageButton(applet.getImage(cb, 
                                            "gifs/two.gif"));
        three = new ImageButton(applet.getImage(cb, 
                                            "gifs/three.gif"));

        panel.setLayout(new RowLayout(0));
        panel.add(one);   
        panel.add(two);   
        panel.add(three); 

        setLayout(new BorderLayout());
        add      ("Center", border);
    }
    public void updateOrientations(Orientation horient, 
                                   Orientation vorient,
                                   int gap) {
        panel.setLayout(new RowLayout(horient, vorient, gap));
        border.validate();
    }
    public void dialogDismissed(Dialog d) { }
}

class RowPicker extends Panel {
    private Label  horientLabel = new Label("Horizontal:");
    private Label  vorientLabel = new Label("Vertical:");
    private Label  gapLabel     = new Label("Gap:");

    private Choice hchoice   = new Choice();
    private Choice vchoice   = new Choice();
    private Choice gapChoice = new Choice();

    private RowButtonPanel buttonPanel;

    public RowPicker(RowButtonPanel buttonPanel) {
        Panel orientations = new Panel();
        Panel gap          = new Panel();

        this.buttonPanel = buttonPanel;
        hchoice.addItem("left");
        hchoice.addItem("center");
        hchoice.addItem("right");
        hchoice.select(1);

        vchoice.addItem("top");
        vchoice.addItem("center");
        vchoice.addItem("bottom");
        vchoice.select(1);

        gapChoice.addItem("0");
        gapChoice.addItem("5");
        gapChoice.addItem("10");
        gapChoice.addItem("15");
        gapChoice.addItem("20");

        orientations.add(horientLabel);  
        orientations.add(hchoice);
        orientations.add(vorientLabel);  
        orientations.add(vchoice);

        gap.add(gapLabel);               
        gap.add(gapChoice);

        add(new Box(orientations, "Orientations"));
        add(new Box(gap,          "Gap"));
    }
    public boolean action(Event event, Object what) {
        String horient, vorient;
        int    gap;

        horient = hchoice.getSelectedItem();
        vorient = vchoice.getSelectedItem();
        gap     = 
        (new Integer(gapChoice.getSelectedItem())).intValue();

        buttonPanel.updateOrientations(
                    Orientation.fromString(horient), 
                    Orientation.fromString(vorient), gap);

        return true;
    }
}
