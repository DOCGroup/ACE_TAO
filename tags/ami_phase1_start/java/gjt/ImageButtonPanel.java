package gjt;

import java.awt.*;
import java.util.Enumeration;
import java.util.Hashtable;

/**
 * A panel which contains a collection of ImageButtons, 
 * arranged either horizontally or vertically.<p>
 *
 * Handling of mouse events is delegated to an image button 
 * panel controller.  By default, an image button panel is 
 * outfitted with an instance of RadioImageButtonPanelController
 * which implements mutually exclusive selection behavior.
 *
 * @version 1.0, Apr 1 1996
 * @author  David Geary
 * @see     ImageButton
 * @see     RadioImageButtonPanelController
 * @see     gjt.test.ToolbarTest
 */
public class ImageButtonPanel extends Panel {
    static private int _defaultGap = 5;

    private Hashtable nameAndButtonPairs = new Hashtable();
    private ImageButtonPanelController controller;

    public ImageButtonPanel(Orientation orient) { 
        this(orient, Orientation.CENTER, 
                     Orientation.CENTER, _defaultGap); 
    }
    public ImageButtonPanel(Orientation orient, int gap) {
        this(orient, Orientation.CENTER, 
                     Orientation.CENTER, gap);
    }
    public ImageButtonPanel(Orientation orient, 
                            Orientation horient, 
                            Orientation vorient, int gap) {
        Assert.notFalse(orient == Orientation.HORIZONTAL || 
                        orient == Orientation.VERTICAL);

        if(orient == Orientation.VERTICAL) 
            setLayout(new ColumnLayout(horient, vorient, gap));
        else                               
            setLayout(new RowLayout(horient, vorient, gap));

        setController(
            new RadioImageButtonPanelController(this));
    }
    public void setController(ImageButtonPanelController c) {
        this.controller = c;
    }
    public Insets insets() { return new Insets(10,10,10,10); }

    public ImageButton add(Image image, String name) {
        ImageButton button = new ImageButton(image);
        add(button);
        nameAndButtonPairs.put(name, button);
        return button;
    }   
    public ImageButton add(Image image) { 
        return add(image, "noname"); 
    }
    public void add(ImageButton button) { 
        add(button, "noname");       
    }
    public void add(ImageButton button, String name) {
        nameAndButtonPairs.put(name, button);
        super.add(button);
    }
    public ImageButton getButtonByName(String name) {
        return (ImageButton)nameAndButtonPairs.get(name);
    }
    public String getButtonName(ImageButton button) {
        Enumeration e = nameAndButtonPairs.keys();
        ImageButton nbutt;
        String nstr;

        while(e.hasMoreElements()) {
            nstr = (String)e.nextElement();
            nbutt = (ImageButton)nameAndButtonPairs.get(nstr);

            if(nbutt.equals(button))
              return nstr;
        }
        return null;
    }
    public void addSpacer(int sizeInPixels) {
        Assert.notFalse(sizeInPixels > 0);
        Canvas spacer = new Canvas();
        spacer.resize(sizeInPixels, sizeInPixels);
        add(spacer);
    }
    public boolean mouseDown(Event event, int x, int y) {
        return controller != null ? 
               controller.mouseDown(event,x,y) : false;
    }
    public boolean mouseDrag(Event event, int x, int y) {
        return controller != null ? 
               controller.mouseDrag(event,x,y) : false;
    }
    public boolean mouseUp(Event event, int x, int y) {
        return controller != null ? 
               controller.mouseUp(event,x,y) : false;
    }
}
