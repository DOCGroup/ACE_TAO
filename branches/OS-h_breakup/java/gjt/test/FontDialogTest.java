package gjt.test;

import java.awt.*;

import gjt.FontDialog;
import gjt.DialogClient;
import gjt.Util;

/**
 * Activating the button causes the FontDialog to be displayed.  
 * Selecting a font from the FontDialog causes the button to 
 * use the selected font.<p>
 *
 * This unit test overrides FontDialog to reset the labels 
 * displayed in the buttons, and to reset the list of font 
 * sizes displayed.  See FontDialog for a discussion of the 
 * overridden methods.<p>
 *
 *<em>Note:  The FontDialog takes forever to come up in 
 * Netscape.</em>
 *
 * @version 1.0, Apr 25, 1996
 * @author  David Geary
 * @see     gjt.test.UnitTest
 * @see     gjt.Util
 * @see     gjt.FontDialog
 * @see     gjt.DialogClient
 */
class LotsOfSizesFontDialog extends FontDialog {
    private static String _defaultSizes[] = 
        { "8", "10", "12", "14", "16", 
          "18", "20", "22", "24",
          "26", "28", "30", "32", "34",
          "36", "38", "40", "42", "44",
          "46", "48", "50", "52", "54",
          "56", "58", "60", "62", "64",
          "66", "68", "70", "72", "74",
          "76", "78", "80", "82", "84",
          "86", "88", "90", "92", "94",
          "96", "98", "100" };

    public LotsOfSizesFontDialog(Frame        frame, 
                                 DialogClient client, 
                                 Font         font) {
        super(frame, client, font, true);
    }
    public String getPreviewButtonLabel() { 
        return "Preview Selected Font";
    }
    public String getOkButtonLabel     () { 
        return "I'll Take It";         
    }
    public String getCancelButtonLabel () { 
        return "Nevermind";            
    }
    public String[] getFontSizes         () { 
        return _defaultSizes;          
    }
}

public class FontDialogTest extends UnitTest {
    public String title() { return "Font Dialog Test"; }
    public Panel centerPanel() { 
        return new FontDialogTestPanel(); 
    }
}

class FontDialogTestPanel extends    Panel 
                          implements DialogClient {
    private Button fontButton;

    public FontDialogTestPanel() {
        setLayout(new BorderLayout());
        add("Center", fontButton = new Button("Fonts ..."));
    }
    public boolean handleEvent(Event event) {
        if(event.id == Event.ACTION_EVENT) {
            LotsOfSizesFontDialog d;
            d = new LotsOfSizesFontDialog(Util.getFrame(this), 
                                          this,
                                          fontButton.getFont());
            d.show();
        }
        return true;
    }
    public void dialogDismissed(Dialog d) {
        FontDialog fontDialog   = (FontDialog)d;
        Font       fontSelected = fontDialog.getFontSelected();

        if(fontSelected != null)
            fontButton.setFont(fontSelected);

        fontButton.requestFocus();
    }
}
