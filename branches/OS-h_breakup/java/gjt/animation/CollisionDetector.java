package gjt.animation;

/**
 * Collision detectors detect collisions that take place within
 * a CollisionArena.
 *
 * @version 1.0, Apr 1 1996
 * @author  David Geary
 * @see     CollisionArena
 * @see     EdgeCollisionDetector
 * @see     SpriteCollisionDetector
 * @see     gjt.test.SimpleAnimationTest
 * @see     gjt.test.BumpAnimationTest
 * @see     gjt.test.TwoDrinkersAnimationTest
 */
abstract public class CollisionDetector {
    protected CollisionArena arena;

    abstract public void detectCollisions();

    public CollisionDetector(CollisionArena arena) {
        this.arena = arena;
    }
}
