package gjt.animation;

import java.awt.*;
import java.util.Enumeration;
import java.util.Vector;
import gjt.Util;

/**
 * A surface upon which Sprites are animated.  Playfields are 
 * responsible for animating the sprites.<p>
 *
 * Each Playfield comes complete with two collision detectors:
 * an edge collision detector and a sprite collision detector.
 *
 * Playfield is an abstract class:  extensions must implement
 * the following methods:
 * <dl>
 * <dd> void paintBackground(Graphics)
 * <dd> void void spriteCollision(Sprite sprite, Sprite other)
 * <dd> void void edgeCollision  (Sprite sprite, Sprite other)
 * </dl>
 *
 * @version 1.0, Apr 1 1996
 * @author  David Geary
 * @see     CollisionArena
 * @see     Sprite
 * @see     SpriteCollisionDetector
 * @see     EdgeCollisionDetector
 * @see     gjt.test.SimpleAnimationTest
 * @see     gjt.test.BumpAnimationTest
 * @see     gjt.test.TwoDrinkersAnimationTest
 */
public abstract class Playfield extends    Canvas 
                                implements Runnable, 
                                           CollisionArena {
    protected Vector    sprites = new Vector();
    private   boolean   running = false;
    private   Insets    insets  = new Insets(0,0,0,0);

    private   Thread                  animationThread;
    private   Image                   bgoffscreen, 
                                      workplaceBuffer;
    private   Dimension               offscreenSize;
    private   EdgeCollisionDetector   edgeCollisionDetector;
    private   SpriteCollisionDetector spriteCollisionDetector;

    abstract public void paintBackground(Graphics g);

    public Playfield() {
        edgeCollisionDetector   = 
            new EdgeCollisionDetector(this);
        spriteCollisionDetector =
            new SpriteCollisionDetector(this);
    }
    public void      stop      ()  { running = false;  }
    public boolean   running   ()  { return running;   }
    public Dimension getSize   ()  { return size();    }
    public Insets    getInsets ()  { return insets;    }
    public Vector    getSprites()  { return sprites;   }

    public void addSprite(Sprite sprite) { 
        sprites.addElement(sprite); 
    }
    public void setInsets(Insets insets) {
        this.insets = insets;
    }
    public void start() {
        animationThread = new Thread(this);
        running = true;
        animationThread.start();
    }
    public void paint(Graphics g) {
        if(needNewOffscreenBuffer()) {
            workplaceBuffer   = createOffscreenImage(size());
            bgoffscreen = createOffscreenImage(size());
            paintBackground(bgoffscreen.getGraphics());
        }
        g.drawImage(bgoffscreen, 0, 0, this);
        paintSprites();
    }
    public void reshape(int x, int y, int w, int h) {
        super.reshape(x,y,w,h);
        repaint();
    }
    public void run() {
        while(running == true) {
            edgeCollisionDetector.detectCollisions  ();
            spriteCollisionDetector.detectCollisions();

            animateSprites();
            Thread.currentThread().yield();
        }
        animationThread = null;
    }
    private boolean needNewOffscreenBuffer() {
        return (workplaceBuffer == null              || 
                bgoffscreen     == null              ||
                size().width  != offscreenSize.width ||
                size().height != offscreenSize.height);
    }
    private Image createOffscreenImage(Dimension size) {
        Image image = createImage(size.width, size.height);
        Util.waitForImage(this, image);
        offscreenSize = size;
        return image;
    }
    protected void animateSprites() {
        Sprite      nextSprite;
        Enumeration e = sprites.elements();

        while(e.hasMoreElements()) {
            nextSprite = (Sprite)e.nextElement();
            nextSprite.animate();
        }
    }
    protected void paintSprites() {
        Sprite      nextSprite;
        Enumeration e = sprites.elements();

        while(e.hasMoreElements()) {
            nextSprite = (Sprite)e.nextElement();
            paintSprite(nextSprite);
        }
    }
    protected void paintSprite(Sprite sprite) {
        Graphics  g    = getGraphics();
        Graphics  wpg  = workplaceBuffer.getGraphics();
        Rectangle clip = sprite.clipRect();

        wpg.clipRect(clip.x, clip.y, clip.width, clip.height);
        wpg.drawImage(bgoffscreen, 0, 0, this);
        sprite.paint(wpg);

        g.clipRect (clip.x, clip.y, clip.width, clip.height);
        g.drawImage(workplaceBuffer, 0, 0, this);

        g.dispose();
        wpg.dispose();
    }
}
