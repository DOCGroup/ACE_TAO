package gjt.animation;

import java.awt.*;
import java.util.Enumeration;
import java.util.Vector;
import gjt.Orientation;

/**
 * A CollisionDetector that detects collisions between Sprites
 * residing in a CollisionArena.<p>
 *
 * @version 1.0, Apr 1 1996
 * @author  David Geary
 * @see     CollisionArena
 * @see     CollisionDetector
 * @see     Sprite
 * @see     gjt.test.SimpleAnimationTest
 * @see     gjt.test.BumpAnimationTest
 * @see     gjt.test.TwoDrinkersAnimationTest
 */
public class SpriteCollisionDetector extends CollisionDetector {
    public SpriteCollisionDetector(CollisionArena arena) {
        super(arena);
    }
    public void detectCollisions() {
        Enumeration sprites = arena.getSprites().elements();
        Sprite      sprite;

        while(sprites.hasMoreElements()) {
            sprite = (Sprite)sprites.nextElement();

            Enumeration otherSprites = 
                            arena.getSprites().elements();
            Sprite otherSprite;

            while(otherSprites.hasMoreElements()) {
                otherSprite=(Sprite)otherSprites.nextElement();

                if(otherSprite != sprite) 
                    if(sprite.willIntersect(otherSprite))
                      arena.spriteCollision(sprite,otherSprite);
            }
        }
    }
}
