package gjt;

import java.applet.Applet;
import java.awt.*;

/**
 * A handy collection of methods for getting a component's 
 * frame, getting a component's applet, waiting for a 
 * component's image, and wallpapering a components background.
 * <p>
 *
 * @version 1.0, Apr 1 1996
 * @author  David Geary
 */
public class Util {
    public static Frame getFrame(Component component) {
        Component c = component;

        if(c instanceof Frame)
            return (Frame)c;

        while((c = c.getParent()) != null) {
            if(c instanceof Frame)
                return (Frame)c;
        }
        return null;
    }
    public static Applet getApplet(Component component) {
        Component c = component;

        if(c instanceof Applet)
            return (Applet)c;

        while((c = c.getParent()) != null) {
            if(c instanceof Applet)
                return (Applet)c;
        }
        return null;
    }
    public static void waitForImage(Component component, 
                                    Image image) {
        MediaTracker tracker = new MediaTracker(component);
        try {
            tracker.addImage(image, 0);
            tracker.waitForID(0);
        }
        catch(InterruptedException e) { Assert.notNull(null); }
    }
    public static void wallPaper(Component component, 
                                 Graphics  g, 
                                 Image     image) {
        Dimension compsize = component.size();
        Util.waitForImage(component, image);

        int patchW = image.getWidth(component);
        int patchH = image.getHeight(component);

        Assert.notFalse(patchW != -1 && patchH != -1);

        for(int r=0; r < compsize.width; r += patchW) {
            for(int c=0; c < compsize.height; c += patchH)
            g.drawImage(image, r, c, component);
        }
    }
    public static void setCursor(int cursor, 
                                 Component component) {
        getFrame(component).setCursor(cursor);      
    }
}
