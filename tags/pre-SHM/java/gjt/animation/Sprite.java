package gjt.animation;

import java.awt.*;
import java.util.Vector;
import gjt.Assert;
import gjt.Stopwatch;
import gjt.Util;

/**
 * A sequence of images which are animated and moved about on
 * a Playfield.<p>
 *
 * Each Sprite is constructed with a reference to it's 
 * Playfield, a Sequence, and a beginning position for it's 
 * upper left hand corner.<p>
 *
 * A Sprite's animation is controlled by invoking the following
 * methods:
 *<dl>
 *<dd> setMoveVector(Point)
 *<dd> setMoveInterval(long)
 *<dd> setImageChangeInterval(long)
 *<dd> setMainSequence(Sequence)
 *<dd> setSequence(Sequence)
 *</dl>
 *
 * @version 1.0, Apr 1 1996
 * @author  David Geary
 * @see     Sequence
 * @see     Playfield
 * @see     SpriteCollisionDetector
 * @see     gjt.test.SimpleAnimationTest
 * @see     gjt.test.BumpAnimationTest
 * @see     gjt.test.TwoDrinkersAnimationTest
 */
public class Sprite {
    private Playfield field;
    private Sequence currentSequence, mainSequence;
    private Stopwatch moveTimer = new Stopwatch();

    private Point ulhc       = new Point(0,0); 
    private Point start      = new Point(0,0);
    private Point moveVector = new Point(1,1);

    private Rectangle clip = new Rectangle(0,0);
    private Rectangle curBounds, lastBounds; 

    private int  width, height;
    private long moveInterval = 0;

    public Sprite(Playfield field, 
                  Sequence  sequence, 
                  Point     ulhc) {
        Assert.notNull(field);
        Assert.notNull(sequence);
        Assert.notNull(ulhc);

        this.field = field;
        this.ulhc  = ulhc;
        start.x    = ulhc.x;
        start.y    = ulhc.y;

        setSequence(sequence);
        setMainSequence(sequence);

        initializeBounds();
        moveTimer.start();
        currentSequence.start();
    }
    public Playfield getPlayfield() { return field;            }
    public Rectangle clipRect    () { return clip;             }
    public Rectangle curBounds   () { return curBounds;        }

    public int   width    () { return width;                   }
    public int   height   () { return height;                  }
    public void  reverseX () { moveVector.x = 0-moveVector.x;  }
    public void  reverseY () { moveVector.y = 0-moveVector.y;  }
    public void  reverse  () { reverseX(); reverseY();         }
    public Point start    () { return start;                   }

    public void  setMoveVector (Point p) { moveVector = p;     }
    public Point getMoveVector()         { return moveVector;  }

    public void play(Sequence sequence, long cycles) {
        setSequence(sequence);
        sequence.setCyclesPerAnimation(cycles);
        sequence.setCurrentCycle(0);
    }
    public void animate() {
        if(currentSequence.animationOver())
            currentSequence = mainSequence;

        if(timeToChangeImage()) currentSequence.advance();
        if(timeToMove())        move();
        if(needsRepainting())   field.paintSprite(this); 
    }
    public void setMainSequence(Sequence sequence) {
        mainSequence = sequence;
    }
    public Sequence getMainSequence() { 
        return mainSequence; 
    }
    public void setSequence(Sequence sequence) {
        currentSequence = sequence;

        if(curBounds != null)
            updateBounds();
    }
    public Sequence getSequence() { 
        return currentSequence; 
    }
    public boolean intersects(Sprite otherSprite) {
        return curBounds().intersects(otherSprite.curBounds());
    }
    public boolean willIntersect(Sprite otherSprite) {
        return 
          nextBounds().intersects(otherSprite.nextBounds());
    }
    public boolean timeToMove() {
        return moveTimer.elapsedTime() > moveInterval;
    }
    public boolean timeToChangeImage() {
        return currentSequence.timeToAdvanceCell();
    }
    public void moveTo(Point p) {
        ulhc = p;
        moveTimer.reset();
    }
    public boolean needsRepainting() {
        return currentSequence.needsRepainting(ulhc);
    }
    public void setMoveInterval(long interval) {
        moveInterval = interval;
    }
    public void setImageChangeInterval(long interval) {
        currentSequence.setAdvanceInterval(interval);
    }
    public void move() {
        ulhc.x += moveVector.x;
        ulhc.y += moveVector.y;
        updateBounds();
        moveTimer.reset();
    }
    public Point location() {
        return ulhc;
    }
    public Point nextLocation() {
        return new Point(ulhc.x + moveVector.x, 
                         ulhc.y + moveVector.y);
    }
    public Rectangle nextBounds() {
        Image nextImage = currentSequence.getNextImage();
        Point nextLoc   = nextLocation();

        return new Rectangle(
                        nextLoc.x, nextLoc.y, width, height);
    }
    public void paint(Graphics g) {
        currentSequence.paint(g, ulhc.x, ulhc.y, field);
    }
    private void initializeBounds() {
        Image curImage = currentSequence.getCurrentImage();

        width  = curImage.getWidth (field); 
        height = curImage.getHeight(field);

        curBounds = 
          new Rectangle(ulhc.x, ulhc.y, width, height);

        lastBounds = new Rectangle(curBounds.x, 
                                   curBounds.y,
                                   curBounds.width, 
                                   curBounds.height);

        clip = lastBounds.union(curBounds);
    }
    private void updateBounds() {
        Image curImage = currentSequence.getCurrentImage();

        lastBounds.width  = curBounds.width;
        lastBounds.height = curBounds.height;

        curBounds.width  = width  = curImage.getWidth(field);
        curBounds.height = height = curImage.getHeight(field);

        lastBounds.move(curBounds.x, curBounds.y);
        curBounds.move (ulhc.x, ulhc.y);

        clip = lastBounds.union(curBounds);
    }
}
