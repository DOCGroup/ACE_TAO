package imaging.filters;

import java.awt.image.*;
import java.util.Random;

public class RandomizeFilter extends RGBImageFilter implements MedFilter
{
  private static Random rand = new Random(System.currentTimeMillis());
  
  public RandomizeFilter ()
  {  
    canFilterIndexColorModel = true;
  }

  public String info ()
  {
    return "Randomizes an image.";
  }

  public int filterRGB(int x, int y, int rgb)
  {
    DirectColorModel cm = 
      (DirectColorModel)ColorModel.getRGBdefault();
    
    int    alpha = cm.getAlpha(rgb);
    int    red   = rand.nextInt() % 255;
    int    green = rand.nextInt() % 255;
    int    blue  = rand.nextInt() % 255;
    
    alpha = alpha << 24;
    red   = red   << 16;
    green = green << 8;
    
    return alpha | red | green | blue;
  }
}
