package gjt;

import java.awt.*;

/**
 * An extension of Scroller that smoothly scrolls an Image.<p>
 *
 * An Image must be supplied at construction time.  The image 
 * may be reset any time after construction.<p>
 *
 * @version 1.0, Apr 1 1996
 * @author  David Geary
 * @see     Scroller
 * @see     ImageCanvas
 * @see     gjt.test.ScrollerTest
 */
public class ImageScroller extends Scroller {
    private Image  image;
    private ScrollerImageCanvas canvas;

    public ImageScroller(Image image) {
        viewport.setLayout(new BorderLayout());
        setImage(image);
    }
    public void resetImage(Image image) {
        viewport.remove(canvas);
        setImage(image);
        invalidate();
        validate();
    }
    public void scrollTo(int x, int y) {
        Graphics g = canvas.getGraphics();
        if(g != null) {
            g.translate(-x,-y);
            g.drawImage(image, 0, 0, this);
        }
    }
    public Dimension getScrollAreaSize() {
        return new Dimension(image.getWidth(this), 
                             image.getHeight(this));
    }
    private void setImage(Image image) {
        this.image = image;
        hbar.setValue(0);
        vbar.setValue(0);
        viewport.add("Center", 
            canvas = new ScrollerImageCanvas(this, image));
    }
}

class ScrollerImageCanvas extends ImageCanvas {
    private ImageScroller scroller;

    public ScrollerImageCanvas(ImageScroller scroller, 
                               Image         image) {
        super(image);
        this.scroller = scroller;
    }
    public void paint(Graphics g) {
        scroller.repaint(); 
    }
}
