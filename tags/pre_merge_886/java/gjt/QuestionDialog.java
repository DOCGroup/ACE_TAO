package gjt;

import java.awt.*;

/**
 * A dialog that presents a prompt and a TextField into which 
 * a reply may be entered.  Comes complete with an Ok button 
 * and a Cancel button, whose uses will be left to the 
 * imagination.<p>
 *
 * @version 1.0, Apr 1 1996
 * @author  David Geary
 * @see     GJTDialog
 * @see     gjt.test.DialogTest
 */
public class QuestionDialog extends GJTDialog {
    static private int  _defaultTextFieldSize = 20;
    private Button      okButton;
    private Button      cancelButton;
    private String      question;
    private TextField   textField;
    private boolean     wasCancelled;
    private ButtonPanel buttonPanel = new ButtonPanel();

    public QuestionDialog(Frame  frame, DialogClient client,
                          String title, String       question,
                          String initialResponse) {
        this(frame, client, title, question, initialResponse, 
             _defaultTextFieldSize);
    }
    public QuestionDialog(Frame  frame, DialogClient client,
                          String title, String       question) {
        this(frame, client, title,
             question, null, _defaultTextFieldSize);
    }
    public QuestionDialog(Frame  frame, DialogClient client,
                          String title, String       question, 
                          int    textFieldSize) {
        this(frame, client, title, 
             question, null, textFieldSize);
    }
    public QuestionDialog(Frame  frame, DialogClient client, 
                          String title, String       question,
                          String initialResponse, 
                          int textFieldSize) {
        super(frame, title, client, true);

        QuestionPanel questionPanel;

        okButton     = buttonPanel.add("Ok");
        cancelButton = buttonPanel.add("Cancel");

        setLayout(new BorderLayout());
        add("North", questionPanel = 
            new QuestionPanel(this, question, 
                              initialResponse, textFieldSize));
        add("South", buttonPanel);
        textField = questionPanel.getTextField();
        pack();
    }
    public boolean action(Event event, Object what) {
        if(event.target == cancelButton) wasCancelled = true;
        else                             wasCancelled = false;

        hide();
        dispose();
        client.dialogDismissed(this);
        return true;
    }
    public void show() {
        textField.requestFocus();
        super.show();
    }
    public void returnInTextField() {
        okButton.requestFocus();
    }
    public TextField getTextField() {
        return textField;
    }
    public String getAnswer() {
        return textField.getText();
    }
    public boolean wasCancelled() {
        return wasCancelled;
    }
    private void setQuestion(String question) {
        this.question = question;
    }
}

class QuestionPanel extends Panel {
    private TextField      field;
    private QuestionDialog dialog;

    public QuestionPanel(QuestionDialog dialog, 
                         String question) {
        this(dialog, question, null, 0);
    }
    public QuestionPanel(QuestionDialog dialog, String question,
                         int columns) {
        this(dialog, question, null, columns);
    }
    public QuestionPanel(QuestionDialog dialog, String question,
                         String initialResponse, int cols) {
        this.dialog = dialog;
        setLayout(new RowLayout());
        add(new Label(question));

        if(initialResponse != null) {
            if(cols != 0) 
                add(field=new TextField(initialResponse, cols));
            else          
                add(field=new TextField(initialResponse));
        }
        else {
            if(cols != 0) add(field = new TextField(cols));
            else          add(field = new TextField());
        }
    }
    public TextField getTextField() {
        return field;
    }
    public boolean action(Event event, Object what) {
        dialog.returnInTextField();
        return false;
    }
    public Insets insets() {
        return new Insets(10,10,10,10);
    }
}
