package gjt;

import java.awt.*;

/**
 * A Canvas that displays an image.<p>
 *
 * update() is overridden to call paint() directly, thus 
 * bypassing the default implementation of update() which 
 * erases the background of the canvas before calling paint().
 * This eliminates nasty flashing.<p>
 *
 * @version 1.0, Apr 1 1996
 * @author  David Geary
 * @see     Util
 */
class ImageCanvas extends Canvas {
    private Image image;

    public ImageCanvas(Image image) {
        this.image = image;
        Util.waitForImage(this, image);
        resize(image.getWidth(this), image.getHeight(this));
    }
    public void paint(Graphics g) {
        g.drawImage(image, 0, 0, this);
    }
    public void update(Graphics g) {
        paint(g);
    }
}
