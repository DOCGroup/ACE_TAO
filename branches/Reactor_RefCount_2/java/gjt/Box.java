package gjt;

import java.awt.*;

/**
 * A Panel containing a single component; an etched rectangle is 
 * drawn around the component, and a Label is centered at the top
 * of the rectangle. Of course, the single component may be
 * a container, and therefore a Box may surround many components.
 * <p>
 * 
 * Both the Component around which the box is drawn, and the 
 * String drawn at the top of the box are specified at 
 * construction time.<p>
 *
 * Etching of the box is controlled by etchedIn() and 
 * etchedOut().  Default etching is etched in.<p>
 *
 * <em>Note: AWT 1.0.2 contains a bug which causes the 
 * Label.CENTER argument of the Label created for the title
 * to be ignored, under Win95.  Therefore, under Win95, the
 * title will be off-center.</em><p>
 *
 * @version 1.0, Apr 1 1996
 * @author  David Geary
 * @see     EtchedRectangle
 * @see     gjt.test.BoxTest
 */
public class Box extends Panel {
    private EtchedRectangle box = new EtchedRectangle(this);
    private Label           titleLabel;

    public Box(Component surrounded, String title) {
        this(surrounded, new Label(title, Label.CENTER));
    }
    public Box(Component surrounded, Label label) {
        Assert.notNull(surrounded);
        Assert.notNull(label);

        titleLabel = label;

        GridBagLayout      gbl = new GridBagLayout();
        GridBagConstraints gbc = new GridBagConstraints();

        setLayout(gbl);
        gbc.gridwidth = GridBagConstraints.REMAINDER;
        gbc.anchor    = GridBagConstraints.NORTH;
        gbl.setConstraints(titleLabel, gbc);
        add(titleLabel);

        gbc.insets  = new Insets(0,10,10,10);
        gbc.anchor  = GridBagConstraints.CENTER;
        gbc.weighty = 1.0;
        gbc.weightx = 1.0;
        gbc.fill    = GridBagConstraints.BOTH;
        gbl.setConstraints(surrounded,gbc); 
        add(surrounded);
    }
    public void etchedIn ()           { box.etchedIn (); }
    public void etchedOut()           { box.etchedOut(); }
    public void paint    (Graphics g) { box.paint();     }

    public void resize(int w, int h) {
        reshape(location().x, location().y, w, h);
    }
    public void reshape(int x, int y, int w, int h) {
        super.reshape(x,y,w,h);

        FontMetrics fm   = titleLabel.getFontMetrics(
                                titleLabel.getFont());
        int         top  = insets().top + fm.getAscent();
        Dimension   size = size();

        box.reshape(0, top, size.width-1, size.height-top-1);
    }
    protected String paramString() {
        return super.paramString() + ",etching=" +
        (box.isEtchedIn() ? Etching.IN : Etching.OUT) +
        ",title=" + titleLabel;
    }
}
