package gjt.test;

import java.applet.Applet;
import java.awt.*;
import java.awt.image.FilteredImageSource;

import gjt.Util;
import gjt.image.BleachImageFilter;

/**
 * Initially displays an unbleached image.  Subsequent mouse
 * clicks in the canvas containing the image toggle between
 * a bleached version of the image and an unbleached version.<p>
 *
 * @version 1.0, Apr 1 1996
 * @author  David Geary
 * @see     gjt.test.UnitTest
 * @see     gjt.image.BleachImageFilter
 */
public class BleachImageFilterTest extends UnitTest {
    public String title() { 
        return "BleachImageFilter Test " + 
               "(Click below to Bleach/Unbleach Picture)";
    }
    public Panel centerPanel() { 
        return new BleachImageFilterTestPanel(this); 
    }
}

class BleachImageFilterTestPanel extends Panel {
    BleachImageFilterTestCanvas canvas;

    public BleachImageFilterTestPanel(Applet applet) {
        add(canvas = new BleachImageFilterTestCanvas(applet));
    }
    public boolean mouseDown(Event event, int x, int y) {
        canvas.toggleBleaching();
        canvas.repaint();
        return true;
    }
}

class BleachImageFilterTestCanvas extends Canvas {
    private Image   im;
    private Image   bleached;
    private boolean showingBleached = false;

    public BleachImageFilterTestCanvas(Applet applet) {
        int    bp;
        String bleachPercent = 
                applet.getParameter("bleachPercent");

        if(bleachPercent != null) 
            bp = new Integer(bleachPercent).intValue();
        else                      
            bp = 50;
        
        im = applet.getImage(applet.getCodeBase(), 
                             "gifs/saint.gif");
        Util.waitForImage(this, im);

        FilteredImageSource source = 
            new FilteredImageSource(im.getSource(), 
                                    new BleachImageFilter(bp));

        bleached = createImage(source);
        Util.waitForImage(this, bleached);
        
        showImageSize();
    }
    public Dimension preferredSize() {
        return new Dimension(im.getWidth(this), 
                             im.getHeight(this));
    }
    public void paint(Graphics g) {
        if(showingBleached) g.drawImage(bleached,0,0,this);
        else                g.drawImage(im,      0,0,this);
    }
    public void toggleBleaching() {
        showingBleached = showingBleached ? false : true;
    }
    private void showImageSize() {
        System.out.print  ("Image width=" + im.getWidth(this));
        System.out.println(" height=" + im.getHeight(this));
    }
}
