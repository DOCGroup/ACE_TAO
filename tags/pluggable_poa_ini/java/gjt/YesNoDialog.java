package gjt;

import java.awt.*;

/**
 * Similar in fuction to the MessageDialog, YesNoDialog poses 
 * a question, that is answered by selection of either a Yes 
 * button or a No button.<p>
 *
 * Note that the YesNoDialog is a singleton - meaning there is
 * only one YesNoDialog in existence per applet.  Clients
 * may obtain the YesNoDialog by invoking getYesNoDialog().<p>
 *
 * @version 1.0, Apr 1 1996
 * @author  David Geary
 * @see     GJTDialog
 * @see     MessageDialog
 * @see     gjt.test.DialogTest
 */
public class YesNoDialog extends GJTDialog {
    static private YesNoDialog _theYesNoDialog;
    private Button      yesButton;
    private Button      noButton;
    private String      message;
    private boolean     answer      = false;
    private ButtonPanel buttonPanel = new ButtonPanel();

    static public YesNoDialog getYesNoDialog( Frame frame,
                                  DialogClient client,
                                  String       title,
                                  String       message) {
        if(_theYesNoDialog == null) 
            _theYesNoDialog = new YesNoDialog(frame,client,
                                              title,message);
        else {
            _theYesNoDialog.setClient (client);
            _theYesNoDialog.setTitle  (title);
            _theYesNoDialog.setMessage(message);
        }
        return _theYesNoDialog;
    }
    private YesNoDialog(Frame  frame, DialogClient client, 
                        String title, String       message) {
        super(frame, title, client, true);
        yesButton = buttonPanel.add("Yes");
        noButton  = buttonPanel.add("No");

        setLayout(new BorderLayout());
        add("Center", new YesNoPanel(message));
        add("South", buttonPanel);
        pack();
    }
    public void show() {
        yesButton.requestFocus();
        super.show();
    }
    public boolean answeredYes() {
        return answer;
    }
    public boolean action(Event event, Object what) {
        if(event.target == yesButton) answer = true;
        else                          answer = false;

        hide();
        client.dialogDismissed(this);
        return true;
    }
    private void setMessage(String message) {
        this.message = message;
    }
}

class YesNoPanel extends Panel {
    public YesNoPanel(String question) {
        add("Center", new Label(question, Label.CENTER));
    }
    public Insets insets() {
        return new Insets(10,10,10,10);
    }
}
