package gjt;

import java.awt.*;

/**
 * A dialog that displays a message and comes equipped with an 
 * Ok button with which the dialog is dismissed.<p>
 *
 * Note that there is only one MessageDialog, that gets
 * reused.  Clients must call getMessageDialog() in order to
 * access the one and only MessageDialog.<p>
 *
 * <em>Note:  The 1.0.2 version of the AWT seems to have
 * introduced a bug that causes pack() to work incorrectly
 * under Win95.</em>
 *
 * @version 1.0, Apr 1 1996
 * @author  David Geary
 * @see     GJTDialog
 * @see     gjt.test.MessageDialogTest
 * @see     gjt.test.DialogTest
 */
public class MessageDialog extends GJTDialog {
    static private MessageDialog _theMessageDialog;

    private Button      okButton;
    private String      message;
    private ButtonPanel buttonPanel = new ButtonPanel();

    static public MessageDialog getMessageDialog(Frame frame,
                                  DialogClient client,
                                  String       title,
                                  String       message) {
        if(_theMessageDialog == null) 
            _theMessageDialog = new MessageDialog(frame,
                                                  client,
                                                  title,
                                                  message);
        else {
            _theMessageDialog.setClient (client);
            _theMessageDialog.setTitle  (title);
            _theMessageDialog.setMessage(message);
        }
        return _theMessageDialog;
    }
    private MessageDialog(Frame  frame, DialogClient client, 
                          String title, String       message) {
        super(frame, title, client, true);
        okButton = buttonPanel.add("Ok");

        setLayout(new BorderLayout());
        add("Center", new MessagePanel(message));
        add("South", buttonPanel);
        pack();
    }
    public void show() {
        okButton.requestFocus();
        super.show();
    }
    public boolean action(Event event, Object what) {
        hide();
        client.dialogDismissed(this);
        return true;
    }
    private void setMessage(String message) {
        this.message = message;
    }
}

class MessagePanel extends Panel {
    public MessagePanel(String message) {
        add("Center", new Label(message, Label.CENTER));
    }
    public Insets insets() {
        return new Insets(10,10,10,10);
    }
}
