package gjt.image;

import java.awt.image.*;
import gjt.Assert;

/**
 * A derivation of RGBImageFilter that partially or wholly
 * dissolves an image.<p>
 *
 * Extent of dissolving is set by the setOpacity(int) method,
 * which is passed an integer between 0 and 255 (inclusive).
 * The integer represents the alpha value to be applied to
 * every color in the image.<p>
 *
 * An alpha value of 255 signifies an opaque color, while an
 * alpha value of 0 signifies a translucent color.<p>
 *
 * @version 1.0, Apr 1 1996
 * @author  David Geary
 * @see     RGBImageFilter
 */
public class DissolveFilter extends RGBImageFilter {
    private int opacity;

    public DissolveFilter() {
        this(0);
    }
    public DissolveFilter(int opacity) {
        canFilterIndexColorModel = true;
        setOpacity(opacity);
    }
    public void setOpacity(int opacity) {
        Assert.notFalse(opacity >= 0 && opacity <= 255);
        this.opacity = opacity;
    }
    public int filterRGB(int x, int y, int rgb) {
        DirectColorModel cm = 
            (DirectColorModel)ColorModel.getRGBdefault();
        int alpha = cm.getAlpha(rgb);
        int red   = cm.getRed  (rgb);
        int green = cm.getGreen(rgb);
        int blue  = cm.getBlue (rgb);

        alpha = opacity;

        return alpha << 24 | red << 16 | green << 8 | blue;
    }
}
