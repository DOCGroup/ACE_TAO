package gjt.test;

import java.net.URL;
import java.applet.Applet;
import java.awt.*;
import java.awt.Panel;

import gjt.Util;
import gjt.Orientation;
import gjt.animation.*;

/**
 * An animation playfield containing two "java drinkers", that
 * both bounce off the sides of the playfield.<p>
 *
 * One of the java drinkers moves slow and spins fast, while
 * the other java drinker moves fast and spins slow.  When
 * the two java drinkers collide, they both play a bump
 * sequence - at different speeds.<p>
 *
 * @version 1.0, Apr 1 1996
 * @author  David Geary
 * @see     gjt.test.AnimationTest
 * @see     gjt.animation.Playfield
 * @see     gjt.animation.Sprite
 */
public class TwoDrinkersAnimationTest extends UnitTest {
    public String title() { 
        return 
            "TwoDrinkers Animation - Mouse Down Starts/Stops"; 
    } 
    public Panel centerPanel() { 
        return new TwoDrinkersAnimationTestPanel(this); 
    }
}

class TwoDrinkersAnimationTestPanel extends Panel {
    public TwoDrinkersAnimationTestPanel(Applet applet) {
        setLayout(new BorderLayout());
        add("Center", new TwoDrinkersPlayfield(applet)); 
    }
}

class TwoDrinkersPlayfield extends Playfield {
    private Applet   applet;
    private URL      cb;
    private Sprite   moveFastSpinSlow, moveSlowSpinFast;
    private Sequence fastSpinSequence, 
                     slowSpinSequence,
                     fastBumpSequence,
                     slowBumpSequence;

    public TwoDrinkersPlayfield(Applet applet) {
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
        if(moveSlowSpinFast.getSequence() != fastBumpSequence) {
            sprite.reverse();
            sprite2.reverse();

            moveSlowSpinFast.play(fastBumpSequence, 3);
            moveFastSpinSlow.play(slowBumpSequence, 3);
        }   
    }
    public void edgeCollision(Sprite      sprite, 
                              Orientation orientation) {
        if(orientation == Orientation.RIGHT || 
           orientation == Orientation.LEFT) 
            sprite.reverseX();
        else 
            sprite.reverseY();
    }
    private void makeSequencesAndSprites() {
        String  file;
        Image[] spinImages = new Image[19];
        Image[] bumpImages = new Image[6];
        Image[] volleyball = new Image[4];

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
        fastSpinSequence = new Sequence(this, spinImages);
        slowSpinSequence = new Sequence(this, spinImages);

        fastBumpSequence = new Sequence(this, bumpImages);
        slowBumpSequence = new Sequence(this, bumpImages);

        moveFastSpinSlow = 
            new Sprite(this, 
                slowSpinSequence, new Point(25, 75));

        moveSlowSpinFast = 
            new Sprite(this, 
                fastSpinSequence, new Point(250,250));

        fastSpinSequence.setAdvanceInterval(50);
        slowSpinSequence.setAdvanceInterval(300);

        fastBumpSequence.setAdvanceInterval(25);
        slowBumpSequence.setAdvanceInterval(200);

        moveFastSpinSlow.setMoveVector(new Point(2,3));
        moveSlowSpinFast.setMoveVector(new Point(-1,-1));

        moveSlowSpinFast.setMoveInterval(100);

        addSprite(moveFastSpinSlow);
        addSprite(moveSlowSpinFast);
    }
}
