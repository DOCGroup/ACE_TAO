package gjt;

import java.awt.*;

/**
 * A dialog that uses a bargauge to indicate progress made on a
 * task that presumably takes some time to complete.
 *
 * ProgressDialog implements the singleton pattern:  clients
 * may only access the one and only ProgressDialog through the
 * static getProgressDialog() method.<p>
 *
 * <em>Note:  The 1.0.2 version of the AWT has introduced a
 * bug that breaks the ProgressDialog under Motif - the 
 * bargauge does not function.  This worked fine in 1.0.1.<em>
 *
 * @version 1.0, Apr 1 1996
 * @author  David Geary
 * @see     GJTDialog
 * @see     Bargauge
 * @see     gjt.test.DialogTest
 */
public class ProgressDialog extends GJTDialog {
    static private ProgressDialog _theProgressDialog;
    static private int     _preferredWidth  = 400;
    static private int     _preferredHeight = 75;
    static private Color   _color;
    static private boolean _dialogUp;

    private Bargauge bargauge;

    static public ProgressDialog getProgressDialog(
                                   Frame  frame,
                                   String title,
                                   Color  color){
        if(_theProgressDialog == null) 
            _theProgressDialog = new ProgressDialog(frame, 
                                                    title, 
                                                    color);
        else {
            _theProgressDialog.setTitle (title);
            _theProgressDialog.reset    ();
        }
        return _theProgressDialog;
    }
    private ProgressDialog(Frame frame, 
                           String title, 
                           Color color) {
        super(frame, title, null, true);
        setLayout(new BorderLayout());
        add("Center", bargauge = new Bargauge(color));
        pack();
    }
    public void setPercentComplete(double percent) {
        bargauge.setFillPercent(percent);
        bargauge.fill();

        if(percent == 100)
            hide();
    }
    public void reset() {
        bargauge.setFillPercent(0);
    }
    public Dimension preferredSize() {
        return new Dimension(_preferredWidth, _preferredHeight);
    }
}
