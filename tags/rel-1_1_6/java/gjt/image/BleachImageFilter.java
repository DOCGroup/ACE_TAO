package gjt.image;

import java.awt.image.*;
import gjt.Assert;

/**
 * A derivation of RGBImageFilter that bleaches an image.<p>
 *
 * Extent of the bleaching effect is controlled by the only 
 * constructor argument: an integer representing the percentage 
 * of bleaching.  The percentage of bleaching may also be 
 * controlled after instantiation by invoking the 
 * void percent(int) method.<p>
 *
 * @version 1.0, Apr 1 1996
 * @author  David Geary
 * @see     RGBImageFilter
 */
public class BleachImageFilter extends RGBImageFilter {
    private int percent;

    public BleachImageFilter(int percent) {
        Assert.notFalse(percent >= 0 && percent <= 100);
        this.percent = percent;
        canFilterIndexColorModel = true;
    }
    public int  percent()            { return percent;    }
    public void percent(int percent) { percent = percent; }

    public int filterRGB(int x, int y, int rgb) {
        DirectColorModel cm = 
            (DirectColorModel)ColorModel.getRGBdefault();

        int    alpha = cm.getAlpha(rgb);
        int    red   = cm.getRed  (rgb);
        int    green = cm.getGreen(rgb);
        int    blue  = cm.getBlue (rgb);
        double percentMultiplier = (double)percent/100;

        red   = Math.min((int)
                (red + (red * percentMultiplier)), 255);
        green = Math.min((int)
                (green + (green * percentMultiplier)), 255);
        blue  = Math.min((int)
                (blue  + (blue  * percentMultiplier)), 255);

        alpha = alpha << 24;
        red   = red   << 16;
        green = green << 8;

        return alpha | red | green | blue;
    }
}
