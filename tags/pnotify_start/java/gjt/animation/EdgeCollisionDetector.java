package gjt.animation;

import java.awt.*;
import java.util.Enumeration;
import java.util.Vector;
import gjt.Orientation;

/**
 * A CollisionDetector that detects collisions between Sprites
 * and the edges of the CollisionArena in which they reside.<p>
 *
 * @version 1.0, Apr 1 1996
 * @author  David Geary
 * @see     CollisionDetector
 * @see     Sprite
 * @see     gjt.test.SimpleAnimationTest
 * @see     gjt.test.BumpAnimationTest
 * @see     gjt.test.TwoDrinkersAnimationTest
 */
public class EdgeCollisionDetector extends CollisionDetector {
    public EdgeCollisionDetector(CollisionArena arena) {
        super(arena);
    }
    public void detectCollisions() {
        Enumeration sprites     = arena.getSprites().elements();
        Dimension   arenaSize   = arena.getSize();
        Insets      arenaInsets = arena.getInsets();
        Sprite      sprite;

        while(sprites.hasMoreElements()) {
            sprite = (Sprite)sprites.nextElement();
            
            Point nl = sprite.nextLocation ();
            Point mv = sprite.getMoveVector();
            int   nextRightEdge   = nl.x + sprite.width();
            int   nextBottomEdge  = nl.y + sprite.height();
            int   arenaBottomEdge = arenaSize.height - 
                                    arenaInsets.bottom;
            int   arenaRightEdge  = arenaSize.width - 
                                    arenaInsets.right;

            if(nextRightEdge > arenaRightEdge) 
                arena.edgeCollision(sprite, Orientation.LEFT);
            else if(nl.x < arenaInsets.left)
                arena.edgeCollision(sprite, Orientation.RIGHT);

            if(nextBottomEdge > arenaBottomEdge) 
                arena.edgeCollision(sprite, Orientation.BOTTOM);
            else if(nl.y < arenaInsets.top)
                arena.edgeCollision(sprite, Orientation.TOP);
        }
    }
}
