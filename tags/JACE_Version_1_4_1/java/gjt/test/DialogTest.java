package gjt.test;

import java.awt.*;
import java.applet.Applet;

import gjt.Util;
import gjt.DialogClient;
import gjt.MessageDialog;
import gjt.ProgressDialog;
import gjt.QuestionDialog;
import gjt.YesNoDialog;

/**
 * Tests 4 gjt custom dialogs:
 * <dl>
 * <dd> MessageDialog  (a dialog which displays a message)
 * <dd> QuestionDialog (a dialog which asks a question)
 * <dd> YesNoDialog    (a dialog with yes/no buttons)
 * <dd> ProgressDialog (a dialog which records progress of task)
 * </dl>
 * @version 1.0, April 25, 1996
 * @author  David Geary
 * @see     gjt.test.UnitTest
 * @see     gjt.DialogClient
 * @see     gjt.MessageDialog
 * @see     gjt.ProgressDialog;
 * @see     gjt.QuestionDialog;
 * @see     gjt.YesNoDialog;
 */
public class DialogTest extends UnitTest {
    public String title() { 
        return "Graphic Java Toolkit Dialog Test"; 
    }
    public Panel centerPanel() { 
        return new DialogLauncher();
    }
}

class DialogLauncher extends Panel implements DialogClient {
    private MessageDialog  messageDialog;
    private QuestionDialog questionDialog;
    private YesNoDialog    yesNoDialog;
    private ProgressDialog progressDialog;

    private Button messageDialogButton, questionDialogButton,
                   yesNoDialogButton,   progressDialogButton;

    public DialogLauncher() {
        setLayout(new GridLayout(0,1));

        add(messageDialogButton  = 
            new Button("Message Dialog"));

        add(questionDialogButton = 
            new Button("Question Dialog"));

        add(yesNoDialogButton    = 
            new Button("YesNo Dialog"));

        add(progressDialogButton = 
                new Button("Progress Dialog"));
    }
    public boolean action(Event event, Object what) {
        if(event.target == messageDialogButton) {
            messageDialog = MessageDialog.getMessageDialog(
                Util.getFrame(this), this, 
                "Example Message Dialog", 
                "This is an example of a message dialog.");

            messageDialog.show();
        }
        else if(event.target == questionDialogButton) {
            questionDialog = 
                new QuestionDialog(Util.getFrame(this), this, 
                                   "Example Question Dialog", 
                                   "Name:  ", "Gumby", 45);
            questionDialog.show();
        }
        else if(event.target == yesNoDialogButton) {
            yesNoDialog = 
                YesNoDialog.getYesNoDialog(Util.getFrame(this), 
                                       this, 
                                       "Example YesNo Dialog",
                                       "Another cup of Java?");
            yesNoDialog.show();
        }
        else if(event.target == progressDialogButton) {
            progressDialog = 
                ProgressDialog.getProgressDialog(
                    Util.getFrame(this), 
                    "Example Progress Dialog",
                    Color.blue);

            progressDialog.show();

            ProgressThread thread = 
                new ProgressThread(progressDialog);
            thread.start();
        }

        return true;
    }
    public void dialogDismissed(Dialog d) {
        if(d == messageDialog)  {
            System.out.println("MessageDialog Down");
        }
        if(d == questionDialog) {
            if(questionDialog.wasCancelled())
                System.out.println("CANCELLED"); 
            else 
            System.out.println(
                "Name:  " + 
                questionDialog.getTextField().getText());
        }
        if(d == yesNoDialog) {
            if(yesNoDialog.answeredYes())
                System.out.println("YES");
            else
                System.out.println("NO");
        }
    }
}

class ProgressThread extends Thread {
    private ProgressDialog dialog;
    private double         percentComplete = 0;

    public ProgressThread(ProgressDialog dialog) {
        this.dialog = dialog;
    }
    public void run() {
        while(percentComplete <= 100) {
            try { Thread.currentThread().sleep(500); }
            catch(InterruptedException e) { }

            dialog.setPercentComplete(percentComplete);
            percentComplete += 10;
        }
    }
}
