package gjt.test;

import java.awt.*;
import java.util.Vector;
import java.applet.Applet;
import java.net.URL;

import gjt.Border;
import gjt.ButtonPanel;
import gjt.ColumnLayout;
import gjt.ComponentScroller;
import gjt.EtchedBorder;
import gjt.ImageButton;
import gjt.RowLayout;
import gjt.Separator;
import gjt.StickyImageButtonController;

/**
 * A phony image store, where you can purchase images.<p>
 *
 * @version 1.0, April 25, 1996
 * @author  David Geary
 * @see     gjt.test.UnitTest
 * @see     Border
 * @see     ButtonPanel
 * @see     ImageButton
 * @see     Separator
 * @see     StickyImageButtonController
 * @see     ComponentScroller
 */
public class ComponentScrollerTest extends UnitTest {
    public String title() { 
        return "ComponentScroller Test"; 
    }
    public Panel  centerPanel() { 
        return new ComponentScrollerTestPanel(this);
    }
}

class ComponentScrollerTestPanel extends Panel {
    private ComponentScroller scroller;
    private Panel             purchasePanel;
    private ImageButtonRow    nextRow;
    private String[][]        imageNames = {
        { "gifs/ballot_box.gif",  "gifs/filmstrip.gif",
          "gifs/fly.gif",         "gifs/eagle.gif",
          "gifs/bullet_hole.gif" },
        { "gifs/mad_hacker.gif",  "gifs/tricycle.gif",
          "gifs/light_bulb1.gif", "gifs/scissors.gif",
          "gifs/palette.gif" },
        { "gifs/frog.gif",        "gifs/gear.gif",
          "gifs/wrench.gif",      "gifs/www.gif",
          "gifs/Dining.gif" },
        { "gifs/ant.gif",         "gifs/abomb.gif",
          "gifs/basketball.gif",  "gifs/soccer.gif",
          "gifs/skelly.gif" },
    };
    public ComponentScrollerTestPanel(Applet applet) {
        URL    base = applet.getCodeBase();
        Image  nextImage;
        Border border, blackBorder;

        purchasePanel = new Panel();
        purchasePanel.setLayout(new ColumnLayout());

        for(int r=0; r < imageNames.length; ++r) {
            nextRow = new ImageButtonRow();
            nextRow.setLayout(new RowLayout());

            for(int c=0; c < imageNames[r].length; ++c) {
                nextImage = applet.getImage(base, 
                                            imageNames[r][c]);
                nextRow.add(nextImage);
            }
            purchasePanel.add(nextRow);
        }
        purchasePanel.add(new ButtonPurchaseForm());

        scroller    = new ComponentScroller();
        border      = new Border(purchasePanel, 3, 2);
        blackBorder = new Border(border, 1, 0);

        border.setLineColor(Color.gray);
        blackBorder.setLineColor(Color.black);
        scroller.setComponent(blackBorder);

        setLayout(new BorderLayout());
        add("Center", scroller);
    }
}

class ButtonPurchaseForm extends Panel {
    TextField nameField    = new TextField(25);
    TextField addressField = new TextField(25);
    TextField cityField    = new TextField(15);
    TextField stateField   = new TextField(2);

    Choice    paymentChoice = new Choice();

    Button    paymentButton = new Button("Purchase");
    Button    cancelButton   = new Button("Cancel");

    public ButtonPurchaseForm() {
        GridBagLayout      gbl = new GridBagLayout();
        GridBagConstraints gbc = new GridBagConstraints();

        Separator sep = new Separator();
        Label title   = 
            new Label("Purchase A Fine Image Today");
        Label name    = new Label("Name:");
        Label address = new Label("Address:");
        Label payment = new Label("Purchase Method:");
        Label phone   = new Label("Phone:");
        Label city    = new Label("City:");
        Label state   = new Label("State:");

        setLayout(gbl);

        paymentChoice.addItem("Visa");
        paymentChoice.addItem("MasterCard");
        paymentChoice.addItem("COD");

        title.setFont(new Font("Times-Roman", 
                               Font.BOLD + Font.ITALIC,
                               16));
        gbc.anchor    = GridBagConstraints.NORTH;
        gbc.gridwidth = GridBagConstraints.REMAINDER;
        gbl.setConstraints(title, gbc);
        add(title);

        gbc.anchor    = GridBagConstraints.NORTH;
        gbc.gridwidth = GridBagConstraints.REMAINDER;
        gbc.fill      = GridBagConstraints.HORIZONTAL;
        gbc.insets    = new Insets(0,0,10,0);
        gbl.setConstraints(sep, gbc);
        add(sep);

        gbc.anchor    = GridBagConstraints.WEST;
        gbc.gridwidth = 1;
        gbc.insets    = new Insets(0,0,0,10);
        gbl.setConstraints(name, gbc);
        add(name);

        gbc.gridwidth = GridBagConstraints.REMAINDER;
        gbl.setConstraints(nameField, gbc);
        add(nameField);

        gbc.gridwidth = 1;
        gbl.setConstraints(address, gbc);
        add(address);

        gbc.gridwidth = GridBagConstraints.REMAINDER;
        gbc.fill = GridBagConstraints.HORIZONTAL;
        gbl.setConstraints(addressField, gbc);
        add(addressField);

        gbc.gridwidth = 1;
        gbl.setConstraints(city, gbc);
        add(city);

        gbl.setConstraints(cityField, gbc);
        add(cityField);

        gbl.setConstraints(state, gbc);
        add(state);

        gbl.setConstraints(stateField, gbc);
        gbc.gridwidth = GridBagConstraints.REMAINDER;
        gbl.setConstraints(stateField, gbc);
        add(stateField);

        gbc.gridwidth = 1;
        gbl.setConstraints(payment, gbc);
        gbc.insets = new Insets(5,0,5,0);
        add(payment);

        gbc.gridwidth = GridBagConstraints.REMAINDER;
        gbc.fill      = GridBagConstraints.NONE;
        gbl.setConstraints(paymentChoice, gbc);
        add(paymentChoice);

        ButtonPanel buttonPanel = new ButtonPanel();

        buttonPanel.add(paymentButton);
        buttonPanel.add(cancelButton);

        gbc.anchor    = GridBagConstraints.SOUTH;
        gbc.insets    = new Insets(5,0,0,0);
        gbc.fill      = GridBagConstraints.HORIZONTAL;
        gbc.gridwidth = 4;
        gbl.setConstraints(buttonPanel, gbc);
        add(buttonPanel);
    }
}
class ImageButtonRow extends Panel {
    public ImageButtonRow() {
        setLayout(new RowLayout());
    }
    public void add(Image image) {
        ImageButton button = new ImageButton(image);
        add(button);
        button.setController(
                new StickyImageButtonController(button));
    }
}
