
package gjt.test;

import java.awt.*;
import java.applet.Applet;

import gjt.MessageDialog;
import gjt.DialogClient;
import gjt.Util;

/**
 * Simple unit test that exercises gjt.MessageDialog.  This 
 * unit test serves to illustrate the use of gjt.DialogClient.  
 * For a unit test which covers all of the gjt dialogs, 
 * see gjt.test.DialogTest.
 *
 * @version 1.0, April 25, 1996
 * @author  David Geary
 * @see     gjt.test.UnitTest
 * @see     gjt.DialogClient
 * @see     gjt.MessageDialog
 */
public class MessageDialogTest extends UnitTest {
    public String title() { 
        return "Message Dialog Test"; 
    }
    public Panel centerPanel() { 
        return new MessageDialogLauncher();  
    }
}

class MessageDialogLauncher extends Panel 
                            implements DialogClient {
    private MessageDialog  messageDialog;

    public MessageDialogLauncher() {
        add(new Button("Show Message Dialog"));
    }
    public boolean action(Event event, Object what) {
        messageDialog = MessageDialog.getMessageDialog(
                     Util.getFrame(this), this, 
                     "Example Message Dialog", 
                     "This is an example of a message dialog.");
        messageDialog.show();
        return true;
    }
    public void dialogDismissed(Dialog d) {
        System.out.println("MessageDialog Down");
    }
}
