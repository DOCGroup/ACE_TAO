package gjt.image;

import java.awt.*;
import java.awt.image.*;
import gjt.Util;

/**
 * Given an image, an ImageDissolver produces an array of
 * images of varying opacity that are used in the fadeIn()
 * and fadeOut() methods for fading the image in and out
 * respectively.<p>
 *
 * As a convenience, ImageDissolver has a static method:
 * Image[] createImages() that creates the array of images
 * mentioned above, in case clients would like to create their
 * own array of images instead of using an ImageDissolver
 * directly.<p>
 *
 * @version 1.0, Apr 1 1996
 * @author  David Geary
 * @see     ThreeDBorder
 * @see     ImageButtonController
 * @see     SpringyImageButtonController
 * @see     StickyImageButtonController
 * @see     BleachImageFilter
 * @see     gjt.test.ImageButtonTest
 */
public class ImageDissolver {
    private static int _defaultNumImages = 10, 
                       _defaultPause = 50;
    Component comp;
    int       numImages, pauseInterval;
    Image     image, offscreen;
    Image[]   dissolvedImages;

    static public Image[] createImages(Image image, 
                                       int numImages,
                                       Component component) {
        Image        images[] = new Image[numImages];
        MediaTracker tracker  = new MediaTracker(component);

        DissolveFilter      filter;
        FilteredImageSource fis;

        for(int i=0; i < numImages; ++i) {
            filter = new DissolveFilter((255/(numImages-1))*i);
            fis    = new FilteredImageSource(image.getSource(), 
                                             filter);

            images[i] = component.createImage(fis);
            tracker.addImage(images[i], i);
        }
        try { tracker.waitForAll(); }
        catch(InterruptedException e) { }

        return images;
    }

    public ImageDissolver(Component comp, Image image) {
        this(comp, image, _defaultNumImages, _defaultPause);
    }
    public ImageDissolver(Component comp, Image im, 
                          int numImages, int pause) {
        this.image      = im;
        this.comp       = comp;
        this.numImages  = numImages;
        dissolvedImages = new Image[numImages];
        pauseInterval   = pause;

        Util.waitForImage(comp, im);
        dissolvedImages = createImages(image, numImages, comp);
    }
    public void fadeIn(int x, int y) {
        if(offscreen == null) 
            offscreen = comp.createImage(image.getWidth(comp), 
                                         image.getHeight(comp));

        Graphics offg  = offscreen.getGraphics();
        Graphics compg = comp.getGraphics();

        if(offg != null && compg != null) {
            clearComponent(compg, x, y);
            for(int i=0; i < numImages; ++i) {
                blitImage(compg, offg, x, y, i);
                pause    ();
            }
            blitOpaqueImage(compg, offg, x, y);
        }
    }
    public void fadeOut(int x, int y) {
        if(offscreen == null) 
            offscreen = comp.createImage(image.getWidth(comp), 
                                         image.getHeight(comp));

        Graphics offg  = offscreen.getGraphics();
        Graphics compg = comp.getGraphics();

        if(offg != null && compg != null) {
            blitOpaqueImage(compg, offg, x, y);
            for(int i=numImages-1; i >= 0; --i) {
                clearOffscreen();
                blitImage     (compg, offg, x, y, i);
                pause         ();
            }
        }
    }
    private void blitImage(Graphics compg, Graphics offg, 
                           int x, int y, int index) {
        offg.drawImage (dissolvedImages[index], 0, 0, comp);
        compg.drawImage(offscreen,              x, y, comp);
    }
    private void blitOpaqueImage(Graphics compg, Graphics offg, 
                                 int x, int y) {
        offg.drawImage(image, 0, 0, comp);
        compg.drawImage(offscreen, x, y, comp);
    }
    private void clearComponent(Graphics compg, int x, int y) {
        clearOffscreen();
        compg.drawImage(offscreen, x, y, comp);
    }
    private void clearOffscreen() {
        Graphics offg = offscreen.getGraphics();

        offg.setColor(comp.getBackground());
        offg.fillRect(0, 0, 
                image.getWidth(comp), image.getHeight(comp));
    }
    private void pause() {
        try { Thread.currentThread().sleep(pauseInterval); }
        catch(InterruptedException e) { }       
    }
}
