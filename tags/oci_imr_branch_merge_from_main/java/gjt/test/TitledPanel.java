package gjt.test;

import java.awt.BorderLayout;
import java.awt.Label;
import java.awt.Panel;
import gjt.Separator;

/**
 * A Panel fitted with a BorderLayout that contains a Label 
 * (title) in the North, and a Separator in the South.
 *
 * @version 1.0, Apr 2 1996
 * @author  David Geary
 */

public class TitledPanel extends Panel {
    public TitledPanel(String title) {
        setLayout(new BorderLayout());
        add("North", new Label(title, Label.CENTER));
        add("South", new Separator());
    }   
}
