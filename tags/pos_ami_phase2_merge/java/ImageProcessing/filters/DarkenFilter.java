package imaging.filters;

import java.awt.image.*;

public class DarkenFilter extends RGBImageFilter implements MedFilter
{
  private int percent_;

  public DarkenFilter ()
  {  
    percent_ = 50;
    canFilterIndexColorModel = true;
  }

  public DarkenFilter(int percent)
  {  
    Assert.notFalse(percent >= 0 && percent <= 100);
    percent_ = percent;
    canFilterIndexColorModel = true;
  }

  public String info ()
  {
    return "Darkens an image.";
  }

  public int filterRGB(int x, int y, int rgb)
  {
    DirectColorModel cm = 
      (DirectColorModel)ColorModel.getRGBdefault();
    
    int    alpha = cm.getAlpha(rgb);
    int    red   = cm.getRed  (rgb);
    int    green = cm.getGreen(rgb);
    int    blue  = cm.getBlue (rgb);
    double percentMultiplier = (double)((double)1.0 - (double)percent_/100);

    red   *=  percentMultiplier;
    blue   *= percentMultiplier;
    green  *= percentMultiplier;
    
    alpha = alpha << 24;
    red   = red   << 16;
    green = green << 8;
    
    return alpha | red | green | blue;
  }
}
