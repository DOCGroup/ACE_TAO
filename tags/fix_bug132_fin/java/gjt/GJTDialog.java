package gjt;

import java.awt.*;

/**
 * A base class for gjt dialog classes, this concrete class 
 * establishes the relationship between a dialog and its 
 * client (DialogClient).<p>
 *
 * Note that show() is overridden to ensure that the dialog is 
 * centered in the frame which is specified as its parent.  This
 * is necessary due to a bug in the Win95 implementation of the 
 * AWT (version 1.0.2) that causes dialogs to be displayed at 
 * a screen coordinate of 0,0.  While the overridden show() is 
 * not necessary under non-Win95 Java implementations, it 
 * alleviates the Win95 bug and results in no dire consequences 
 * on other platforms.<p>
 *
 * @version 1.0, Apr 1 1996
 * @author  David Geary
 * @see     MessageDialog
 * @see     QuestionDialog
 * @see     YesNoDialog
 * @see     ProgressDialog
 * @see     gjt.test.DialogTest
 */
public class GJTDialog extends Dialog {
    protected DialogClient client;

    public GJTDialog(Frame        frame, 
                     String       title, 
                     DialogClient client, 
                     boolean      modal) {
        super(frame, title, modal); 
        setClient(client);
    }
    public void setClient(DialogClient client) {
        this.client = client;
    }
    public void show() {  // Fixes bug under Win95
        Dimension frameSize = getParent().size();
        Point     frameLoc  = getParent().location();
        Dimension mySize    = size();
        int       x,y;

        x = frameLoc.x + (frameSize.width/2) -(mySize.width/2); 
        y = frameLoc.y + (frameSize.height/2)-(mySize.height/2); 
        reshape(x,y,size().width,size().height);
        super.show();
    }
}
