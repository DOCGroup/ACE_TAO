package gjt.animation;

import java.util.Vector;
import java.awt.*;
import java.awt.image.ImageObserver;
import gjt.Util;
import gjt.Stopwatch;

/**
 * A sequence of images used in an animation.  Each sequence
 * keeps track of the number of cycles the sequence is to run,
 * and reports whether or not the cycles have been completed
 * via the boolean animationOver() method.
 *
 * @version 1.0, Apr 1 1996
 * @author  David Geary
 * @see     Sprite
 * @see     Playfield
 * @see     gjt.test.SimpleAnimationTest
 * @see     gjt.test.BumpAnimationTest
 * @see     gjt.test.TwoDrinkersAnimationTest
 */
public class Sequence {
    private static long infiniteCycle = -1;

    private Vector    cells               = new Vector();
    private Point     lastPaintLocation   = new Point(0,0);
    private Stopwatch cellAdvanceTimer    = new Stopwatch();
    private Image     currentImage, lastImagePainted;
    private long      cellAdvanceInterval = 0,
                      currentCycle        = 0, 
                      cyclesPerAnimation  = 0;

    public Sequence() { }

    public Sequence(Component component, Image[] images) {
        for(int i=0; i < images.length; ++i) {
            addImage(component, images[i]);
        }
        cyclesPerAnimation = infiniteCycle;
    }
    public void  start          () { cellAdvanceTimer.start(); }
    public Image getLastImage   () { return lastImagePainted;  }
    public Point getLastLocation() { return lastPaintLocation; }
    public int   getNumImages   () { return cells.size();      } 

    public long getCurrentCycle()       { return currentCycle; }
    public void setCurrentCycle(long c) { currentCycle = c;    }

    public long getCyclesPerAnimation() { 
        return currentCycle;            
    }
    public void setCyclesPerAnimation(long cyclesPerAnimation) { 
        this.cyclesPerAnimation = cyclesPerAnimation;
    }
    public Image getFirstImage() {
        return (Image)cells.firstElement();
    }
    public Image getCurrentImage() { 
        return currentImage;      
    }
    public int getCurrentImagePosition() { 
        return cells.indexOf(currentImage); 
    }
    public Image getNextImage() {
        int   index = cells.indexOf(currentImage);
        Image image;

        if(index == cells.size() - 1)   
            image = (Image)cells.elementAt(0);
        else                                
            image = (Image)cells.elementAt(index + 1); 

        return image;
    }
    public void setAdvanceInterval(long interval) {
        cellAdvanceInterval = interval;
    }
    public void addImage(Component component, Image image) {
        if(currentImage == null)
            currentImage = image;

        Util.waitForImage(component, image);
        cells.addElement(image);
    }
    public void removeImage(Image image) {
        cells.removeElement(image);
    } 
    public boolean needsRepainting(Point point) {
        return (lastPaintLocation.x != point.x ||
                lastPaintLocation.y != point.y ||
                lastImagePainted    != currentImage);
    }
    public void paint(Graphics g, int x, int y, 
                      ImageObserver observer) {
        g.drawImage(currentImage, x, y,  observer);
        lastPaintLocation.x = x;
        lastPaintLocation.y = y;
        lastImagePainted    = currentImage;
    }
    public boolean isAtLastImage() {
        return getCurrentImagePosition() == (cells.size() - 1);
    }
    public boolean timeToAdvanceCell() {
        return 
          cellAdvanceTimer.elapsedTime() > cellAdvanceInterval;
    }
    public boolean animationOver() {
        return (cyclesPerAnimation != infiniteCycle) &&
               (currentCycle >= cyclesPerAnimation);
    }
    public void advance() {
        if(isAtLastImage())
            ++currentCycle;

        currentImage = getNextImage();
        cellAdvanceTimer.reset();
    }
}
