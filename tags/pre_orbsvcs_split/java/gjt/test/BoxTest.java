package gjt.test;

import java.applet.Applet;
import java.awt.*;
import gjt.Box;
import gjt.ExclusiveImageButtonPanel;
import gjt.Orientation;

/**
 * Three Boxes, each of which surrounds either:  ImageButtons, 
 * Labels or Checkboxes.  The Box surrounding the ImageButtons 
 * is etched out, while the other two Boxes are etched in.<p>
 *
 * @version 1.0, April 25, 1996
 * @author  David Geary
 * @see     gjt.test.UnitTest
 * @see     gjt.ImageButton
 * @see     gjt.Box
 */
public class BoxTest extends UnitTest {
    public String title() { 
        return "Box Test";             
    }
    public Panel centerPanel() { 
        return new BoxTestPanel(this); 
    }
}

class BoxTestPanel extends Panel {
    private Applet applet;
    private Box    iconbox, labelbox, checkboxbox;
    private Panel  panelInLabelbox    = new Panel(); 
    private Panel  panelInCheckboxbox = new Panel();
    private ExclusiveImageButtonPanel panelInIconbox;

    public BoxTestPanel(Applet applet) {
        GridBagLayout      gbl = new GridBagLayout();
        GridBagConstraints gbc = new GridBagConstraints();

        this.applet    = applet;
        panelInIconbox = new ExclusiveImageButtonPanel(
                                Orientation.HORIZONTAL);

        populateIconPanel    ();
        populateLabelPanel   ();
        populateCheckboxPanel();

        iconbox     = new Box(panelInIconbox, 
                              "Meaningless Images");
        labelbox    = new Box(panelInLabelbox,    "Labels");
        checkboxbox = new Box(panelInCheckboxbox, "Fruits");
        iconbox.etchedOut();

        setLayout(gbl);
        gbc.anchor    = GridBagConstraints.NORTH;
        gbc.gridwidth = GridBagConstraints.REMAINDER;
        gbc.weighty   = 0.50;
        gbl.setConstraints(iconbox,  gbc); 
        add(iconbox);
        gbl.setConstraints(labelbox, gbc); 
        add(labelbox);

        gbc.anchor    = GridBagConstraints.SOUTH;
        gbc.weighty   = 0;
        gbl.setConstraints(checkboxbox, gbc); 
        add(checkboxbox);
    }
    private void populateIconPanel() {
        Image ballot, film, ticket;

        ballot  = applet.getImage(applet.getCodeBase(), 
                                  "gifs/ballot_box.gif");
        ticket  = applet.getImage(applet.getCodeBase(), 
                                  "gifs/movie_ticket.gif");
        film    = applet.getImage(applet.getCodeBase(), 
                                  "gifs/filmstrip.gif");

        panelInIconbox.add(ballot);
        panelInIconbox.add(ticket);
        panelInIconbox.add(film);
    }   
    private void populateLabelPanel() {
        panelInLabelbox.add(new Label("Label One"));
        panelInLabelbox.add(new Label("Label Two"));
        panelInLabelbox.add(new Label("Label Three"));
        panelInLabelbox.add(new Label("Label Four"));
        panelInLabelbox.add(new Label("Label Five"));
    }
    private void populateCheckboxPanel() {
        CheckboxGroup group = new CheckboxGroup();

        panelInCheckboxbox.setLayout(new GridLayout(3,0));
        panelInCheckboxbox.add(new Checkbox("apples",  
                                            group, false));
        panelInCheckboxbox.add(new Checkbox("oranges", 
                                            group, false));
        panelInCheckboxbox.add(new Checkbox("pears",   
                                            group, true));
    }
}
