package gjt.test;

import java.awt.*;
import gjt.Etching;
import gjt.Separator;

/**
 * Two Separators, one horizontal and the other vertical, the 
 * former etched in, and the latter etched out are laid out with 
 * an adorning Label for each.<p>
 *
 * @version 1.0, April 25, 1996
 * @author  David Geary
 * @see     gjt.UnitTest
 * @see     gjt.Separator
 */
public class SeparatorTest extends UnitTest {
    public String title      () { return "Separator Test"; }
    public Panel  centerPanel() { 
        return new SeparatorTestPanel(); 
    }
}

class SeparatorTestPanel extends Panel {
    public SeparatorTestPanel() {
        setLayout(new BorderLayout());
        add("North",  new SeparatorTestNorthPanel ());
        add("Center", new SeparatorTestCenterPanel());
    }
}

class SeparatorTestNorthPanel extends Panel {
    Separator separator = new Separator();

    public SeparatorTestNorthPanel() {
        setLayout(new BorderLayout());
        add("North", new Label("North Of Etched-In Separator"));
        add("South", separator);
    }
}

class SeparatorTestCenterPanel extends Panel {
    Separator separator = new Separator(Etching.OUT);
    
    public SeparatorTestCenterPanel() {
        GridBagConstraints gbc = new GridBagConstraints();
        GridBagLayout      gbl = new GridBagLayout();
        Label label = new Label("West Of Etched-Out Separator");

        setLayout(gbl);
        gbc.anchor  = GridBagConstraints.WEST;
        gbc.insets  = new Insets(0,0,0,10);
        gbl.setConstraints(label, gbc);
        add(label);

        gbc.insets  = new Insets(0,0,0,0);
        gbc.weightx = 1.0;
        gbc.weighty = 1.0;
        gbc.fill    = GridBagConstraints.VERTICAL;
        gbl.setConstraints(separator, gbc);
        add(separator);

    }
}
