package gjt;

import java.awt.*;

/**
 * A dialog used for selecting a font.  FontDialog is 
 * constructed with a Frame, DialogClient, initial font to 
 * display, and boolean that indicates modality.<p>
 *
 * FontDialog contains a preview panel which previews the 
 * currently selected font.  Updating of the preview panel is 
 * triggered by a preview button at the bottom of the dialog.<p>
 *
 * FontDialog contains 3 methods which define the labels for 
 * the buttons it contains:
 * <dl>
 * <dd> String getPreviewButtonLabel()
 * <dd> String getOkButtonLabel()
 * <dd> String getCancelButtonLabel()
 * </dl><p>
 * 
 * By default the 3 methods return "Preview", "Ok" and "Cancel" 
 * respectively.  FontDialog may be extended and the 3 methods 
 * overridden to customize the labels displayed in the 
 * buttons.<p>
 * 
 * FontDialog uses Toolkit to get a list of fonts by invoking
 * Toolkit.getFontList().  This is done in the getFontNames() 
 * method, which may be overridden by extensions of FontDialog 
 * in case the standard set of font names are inadequate.<p>
 *
 * Finally, font sizes are obtained by the getFontSizes() 
 * method.  FontDialog defines 8 sizes by default:  8, 12, 14, 
 * 16, 18, 24, 48 and 64. Extensions of FontDialog may override 
 * getFontSizes() to provide a different list of sizes.<p>
 *
 * See gjt.test.FontDialogTest for an example of an extension 
 * of FontDialog which overrides the methods discussed above.<p>
 *
 * @version 1.0, Apr 1 1996
 * @author  David Geary
 * @see     java.awt.Dialog
 * @see     java.awt.Toolkit
 * @see     DialogClient
 * @see     gjt.test.FontDialogTest
 */
public class FontDialog extends Dialog {
    private static String _defaultSizes[] = 
        { "8",  "12", "14", "16", "18", "24", "48", "64" };

    private FontPanel    fontPanel;
    private Font         fontSelected;
    private DialogClient client;

    public FontDialog(Frame        frame, 
                      DialogClient client, 
                      Font         font,    // initial font
                      boolean      modal) {
        super(frame, "Select A Font", modal);
        this.client = client;
        
        setLayout(new BorderLayout());
        add("Center", fontPanel = new FontPanel(this, font));
    }
    public boolean handleEvent(Event event) {
        if(event.id == Event.WINDOW_DESTROY)
            done(null);

        return super.handleEvent(event);
    }
    public String[] getFontNames() { 
        return getToolkit().getFontList(); 
    }
    public String[] getFontSizes() { 
        return _defaultSizes;   
    }

    public String getPreviewButtonLabel() { return "Preview"; }
    public String getOkButtonLabel     () { return "Ok";      }
    public String getCancelButtonLabel () { return "Cancel";  }

    public void show() {
        Point frameLoc = getParent().location();
        reshape(frameLoc.x + 50, frameLoc.x + 50, 550, 450);
        super.show();
    }
    public void done(Font font) {
        fontSelected = font;
        client.dialogDismissed(this);
        hide   ();
        dispose();
    }
    public Font getFontSelected() {
        return fontSelected;
    }
    public void listSelectedInPicker() {
        fontPanel.getPreviewButton().requestFocus();
    }
}

class FontPanel extends Panel {
    private static Font defaultFont = 
        new Font("TimesRoman", Font.PLAIN, 12);

    private FontPreviewPanel   preview;
    private FontSelectionPanel fsp;

    public FontPanel(FontDialog dialog, Font f) {
        Font font = f == null ? defaultFont : f;

        setLayout(new BorderLayout());
        add("North",  preview = new FontPreviewPanel  ());
        add("Center", fsp     = 
            new FontSelectionPanel(dialog, preview, font));
    }
    public Button getPreviewButton() {
        return fsp.getPreviewButton();
    }
}

class FontPreviewPanel extends Panel {
    TextField textField = new TextField();
    Box       box       = new Box(textField, "Preview");

    public FontPreviewPanel() {
        textField.setEditable(false);

        setLayout(new BorderLayout());
        add("Center", box);
    }
    public void setPreviewFont(Font font) {
        String name  = font.getName();
        String size  = String.valueOf(font.getSize());
        String style = new String();

        if(font.isPlain () == true) style = "Plain";
        else {
            if(font.isBold  () == true) style += "Bold";
            if(font.isItalic() == true) style += "Italic";
        }
        textField.setFont(font);
        textField.setText(name + " " + style + " " + size);
        retrofitPreviewPanel();
    }
    private void retrofitPreviewPanel() {
        Dimension tfps, tfs;
        FontPanel fontPanel = (FontPanel)getParent();

        tfps = textField.preferredSize();
        tfs  = textField.size();

        if(tfps.width != tfs.width || 
           tfps.height != tfs.height) {
            fontPanel.invalidate();
            fontPanel.getParent().validate();
            box.repaint();           // Only necessary on Win95
        }
    }
}

class FontSelectionPanel extends Panel {
    private FontPickerPanel  picker;
    private FontButtonsPanel buttons;
    private FontPreviewPanel preview;
    private Font             initialFont;

    public FontSelectionPanel(FontDialog       dialog,
                              FontPreviewPanel preview, 
                              Font initialFont) {
        this.preview     = preview;
        this.initialFont = initialFont;

        picker  = new FontPickerPanel (dialog, initialFont);
        buttons = new FontButtonsPanel(dialog, picker, preview);

        setLayout(new BorderLayout());
        add("Center", picker);
        add("South",  buttons);
    }
    public void addNotify() {
        super.addNotify();
        preview.setPreviewFont(initialFont);
    }
    public Button getPreviewButton() {
        return buttons.getPreviewButton();
    }
}

class FontPickerPanel extends Panel {
    private FontDialog dialog;
    private Button     previewButton;
    private List       fonts  = new List();
    private List       styles = new List();
    private List       sizes  = new List();
    private Font       initialFont;

    public FontPickerPanel(FontDialog dialog, 
                           Font       initialFont) {
        GridBagLayout      gbl    = new GridBagLayout();
        GridBagConstraints gbc    = new GridBagConstraints();
        Label              family = new Label("Family");
        Label              style  = new Label("Style");
        Label              size   = new Label("Size");

        this.initialFont = initialFont;
        this.dialog      = dialog;
        
        populateFonts ();
        populateStyles();
        populateSizes ();

        setLayout(gbl);

        gbc.anchor    = GridBagConstraints.NORTH;
        gbc.gridwidth = 1;
        gbl.setConstraints(family, gbc); add(family);
        gbl.setConstraints(style,  gbc); add(style);
        gbc.gridwidth = GridBagConstraints.REMAINDER;
        gbl.setConstraints(size,   gbc); add(size);

        gbc.gridwidth = 1;
        gbc.weighty   = 1.0;
        gbc.weightx   = 1.0;
        gbc.fill      = GridBagConstraints.BOTH;
        gbl.setConstraints(fonts,  gbc); add(fonts);
        gbl.setConstraints(styles, gbc); add(styles);
        gbl.setConstraints(sizes,  gbc); add(sizes);
    }
    public boolean handleEvent(Event event) {
        if(event.id  == Event.LIST_SELECT) {
            dialog.listSelectedInPicker();
            return true;
        }
        return false;
    }
    public void addNotify() {
        super.addNotify();
        String initialFamily = initialFont.getName();
        int    initialSize   = initialFont.getSize();
        int    initialStyle  = initialFont.getStyle();

        styles.select(initialStyle);

        for(int i=0; i < fonts.countItems(); ++i) {
            String nextFamily = fonts.getItem(i);
            if(nextFamily.equals(initialFamily))
                fonts.select(i);
        }
        for(int i=0; i < sizes.countItems(); ++i) {
            String nextSize = sizes.getItem(i);
            if(nextSize.equals(String.valueOf(initialSize)))
                sizes.select(i);
        }
    }
    public String fontSelected() { 
        return fonts.getSelectedItem (); 
    }
    public String styleSelected() { 
        return styles.getSelectedItem(); 
    }
    public int sizeSelected() {
        String szstring = sizes.getSelectedItem();

        if(szstring != null) {
            Integer integer = new Integer(szstring);
            return integer.intValue();
        }
        else
            return 0;
    }
    private void populateFonts() {
        String names[] = dialog.getFontNames();

        for(int i=0; i < names.length; ++i) {
            fonts.addItem(names[i]);
        }
    }
    private void populateSizes() {
        String sizeArray[] = dialog.getFontSizes();

        for(int i=0; i < sizeArray.length; ++i) {
            sizes.addItem(sizeArray[i]);
        }
    }
    private void populateStyles() {
        styles.addItem("Plain");
        styles.addItem("Bold");
        styles.addItem("Italic");
        styles.addItem("BoldItalic");
    }
}

class FontButtonsPanel extends Panel {
    private FontDialog       dialog;
    private FontPickerPanel  picker;
    private FontPreviewPanel preview;
    private Button           previewButton, 
                             okButton, 
                             cancelButton;

    public FontButtonsPanel(FontDialog       dialog,
                            FontPickerPanel  picker, 
                            FontPreviewPanel preview) {
        this.picker  = picker;
        this.preview = preview;
        this.dialog  = dialog;

        add(previewButton = 
            new Button(dialog.getPreviewButtonLabel()));
        add(cancelButton  = 
            new Button(dialog.getCancelButtonLabel()));
        add(okButton      = 
            new Button(dialog.getOkButtonLabel()));
    }
    public void addNotify() {
        super.addNotify();
        cancelButton.requestFocus();
    }
    public boolean action(Event event, Object object) {
        Button  button = (Button)event.target;
        boolean handledEvent = true;

        if(event.target == previewButton) {
            Font selectedFont = fontSelected();

            if(selectedFont != null) {
                preview.setPreviewFont(selectedFont);
                okButton.requestFocus();
            }
        }
        else if(event.target == okButton)     
            dialog.done(fontSelected());
        else if(event.target == cancelButton) 
            dialog.done(null);
        else                                  
            handledEvent = false;

        return handledEvent;
    }
    public Button getPreviewButton() {
        return previewButton;
    }
    private Font fontSelected() {
        String font   = picker.fontSelected ();
        String style  = picker.styleSelected();
        int    size   = picker.sizeSelected ();
        int    istyle = Font.PLAIN;

        if(font != null && style != null && size > 0) {
            if(style.equals("Bold"))   istyle = Font.BOLD;
            if(style.equals("Plain"))  istyle = Font.PLAIN;
            if(style.equals("Italic")) istyle = Font.ITALIC;

            if(style.equals("BoldItalic")) 
                istyle = Font.BOLD + Font.ITALIC;

            return new Font(font, istyle, size);
        }
        else
            return null;
    }
}
