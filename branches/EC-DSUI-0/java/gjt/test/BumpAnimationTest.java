package gjt.test;

import java.net.URL;
import java.applet.Applet;
import java.awt.*;

import gjt.Util;
import gjt.Orientation;
import gjt.animation.*;

/**
 * A simple animation playfield with one sprite that bounces
 * off the boundaries of the playfield.<p>
 *
 * When the sprite bounces off the left wall, it plays a
 * bump sequence once; when it bounces off the right wall
 * it plays the bump sequence twice.<p>
 *
 * @version 1.0, Apr 1 1996
 * @author  David Geary
 * @see     gjt.animation.Playfield
 * @see     gjt.animation.Sprite
 */
public class BumpAnimationTest extends UnitTest {
    public String title() { 
        return "Bump Animation - Mouse Down Starts/Stops"; 
    } 
    public Panel centerPanel() { 
        return new BumpAnimationTestPanel(this); 
    }
}

class BumpAnimationTestPanel extends Panel {
    public BumpAnimationTestPanel(Applet applet) {
        setLayout(new BorderLayout());
        add("Center", new BumpPlayfield(applet)); 
    }
}

class BumpPlayfield extends Playfield {
    private Applet   applet;
    private URL      cb;
    private Sprite   javaDrinker;
    private Sequence spinSequence, bumpSequence;

    public BumpPlayfield(Applet applet) {
        this.applet = applet;
        cb          = applet.getCodeBase();
        makeSequencesAndSprites();
    }
    public void paintBackground(Graphics g) {
        Image bg = applet.getImage(cb, "gifs/background.gif");
        Util.wallPaper(this, g, bg);
    }
    public boolean mouseDown(Event event, int x, int y) {
        if(running() == true) stop ();
        else                  start();
        return true;
    }
    public void spriteCollision(Sprite sprite, Sprite sprite2) {
        // Nothing to do:  only 1 sprite!
    }
    public void edgeCollision(Sprite      sprite, 
                              Orientation orientation) {
        if(orientation == Orientation.RIGHT || 
           orientation == Orientation.LEFT) {
            if(sprite.getSequence() != bumpSequence) {
                sprite.reverseX();

                if(orientation == Orientation.RIGHT)
                    sprite.play(bumpSequence, 1);
                else
                    sprite.play(bumpSequence, 2);
            }
        }   
        else 
            sprite.reverseY();
    }
    private void makeSequencesAndSprites() {
        String  file;
        Point   startLoc   = new Point(10, 10);
        Image[] spinImages = new Image[19];
        Image[] bumpImages = new Image[6];

        for(int i=0; i < spinImages.length; ++i) {
            file = "gifs/spin";

            if(i < 10) file += "0" + i + ".gif";
            else       file += i + ".gif";

            spinImages[i] = applet.getImage(cb, file);
        }
        for(int i=0; i < bumpImages.length; ++i) {
            file = "gifs/bump0" + i + ".gif";
            bumpImages[i] = applet.getImage(cb, file);
        }
        spinSequence = new Sequence(this, spinImages);
        bumpSequence = new Sequence(this, bumpImages);
        javaDrinker  = new Sprite(this, spinSequence, startLoc);

        spinSequence.setAdvanceInterval(100);
        bumpSequence.setAdvanceInterval(200);

        javaDrinker.setMoveVector(new Point(2,2));
        addSprite(javaDrinker);
    }
}
