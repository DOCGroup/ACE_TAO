package gjt;

import java.awt.Dialog;

/**
 * DialogClients are notified when the Dialog with which they 
 * are associated is dismissed.  A reference to the dismissed 
 * Dialog is passed as a parameter of dialogDismissed() in case 
 * a DialogClient is a client of more than one Dialog.<p>
 *
 * @version 1.0, Apr 1 1996
 * @author  David Geary
 * @see     MessageDialog
 * @see     QuestionDialog
 * @see     YesNoDialog
 * @see     gjt.test.DialogTest
 */
public interface DialogClient {
    abstract public void dialogDismissed(Dialog d); 
}
