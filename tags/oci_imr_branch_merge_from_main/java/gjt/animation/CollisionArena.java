package gjt.animation;

import java.awt.Dimension;
import java.awt.Insets;
import java.util.Vector;
import gjt.Orientation;

/**
 * A CollisionArena is defined as an arena in which collisions
 * may take place.<p>
 *
 * CollisionArenas must be able to report their size and 
 * insets, and return a Vector of the Sprites contained in the 
 * arena.<p>
 *
 * CollisionArenas must also implement two methods for handling
 * sprite and edge collisions, respectively.
 *
 * @version 1.0, Apr 1 1996
 * @author  David Geary
 * @see     Playfield
 * @see     CollisionDetector
 * @see     EdgeCollisionDetector
 * @see     SpriteCollisionDetector
 * @see     gjt.test.SimpleAnimationTest
 * @see     gjt.test.BumpAnimationTest
 * @see     gjt.test.TwoDrinkersAnimationTest
 */
public interface CollisionArena {
    abstract public Vector    getSprites();
    abstract public Dimension getSize   ();
    abstract public Insets    getInsets ();

    abstract public void spriteCollision(Sprite sprite, 
                                         Sprite other);

    abstract public void edgeCollision(Sprite      sprite, 
                                       Orientation orient); 
}
