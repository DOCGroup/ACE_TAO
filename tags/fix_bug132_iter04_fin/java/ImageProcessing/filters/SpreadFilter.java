package imaging.filters;

import java.awt.image.*;
import java.util.Random;

public class SpreadFilter extends SpatialFilter
{
  private int pwidth_ = 5;
  private int pheight_ = 5;

  public SpreadFilter()
    {
    }
  
  public SpreadFilter(int pwidth, int pheight)
    {
      pwidth_ = pwidth;
      pheight_ = pheight;
    }

  public String info ()
  {
    return "Spreads an image (an effect of being under water).";
  }

  public void imageComplete(int status)
  {
    if (status == IMAGEERROR || status == IMAGEABORTED)
      {
	consumer.imageComplete(status);
	System.out.println("Spread Image Error: " + status);
	return;
      }

    System.gc();
    
    Random rand = new Random();
    int[] pixels = new int[columns_*rows_];
    int d, dx, dy, x1, y1, xrng, xoff, yrng, yoff;
    int minx, maxx, miny, maxy, rdist, tmp;

    profile_timer_.start();

    for (int z = 0; z < SpatialFilter.iterations_; z++)
      {    
	for (int y = 0, i=0; y < rows_; y++)
	  {
	    for (int x = 0; x < columns_; x++, i++)
	      {
		if (pwidth_ < 0)
		  {
		    d = (pwidth_ < 0 ? -pwidth_ : pwidth_);
		    
		    minx = x - d;
		    if (minx < 0)
		      minx = 0;
		    
		    maxx = x + d;
		    if (maxx >= columns_)
		      maxx = columns_ - 1;
		    
		    tmp = rand.nextInt();
		    tmp = (tmp < 0 ? -tmp : tmp);
		    x1 = minx + tmp % ((maxx - minx) + 1);
		    
		    miny = y - d;
		    if (miny < 0)
		      miny = 0;
		    
		    maxy = y + d;
		    if (maxy >= rows_)
		      maxy = rows_ - 1;
		    
		    rdist = d - (x1 < x ? -(x1 - x) : x1 - x);
		    if (y - miny > rdist)
		      miny = (y - rdist);
		    if (maxy - y > rdist)
		      maxy = (y + rdist);
		    
		    tmp = rand.nextInt();
		    tmp = (tmp < 0 ? -tmp : tmp);
		    y1 = miny + tmp % ((maxy - miny) + 1);
		  }
		else
		  {
		    minx = x - pwidth_;
		    if (minx < 0)
		      minx = 0;
		    
		    maxx = x + pwidth_;
		    if (maxx >= columns_)
		      maxx = columns_ - 1;
		    
		    tmp = rand.nextInt();
		    tmp = (tmp < 0 ? -tmp : tmp);
		    x1 = minx + tmp % ((maxx - minx) + 1);
		    
		    miny = y - pheight_;
		    if (miny < 0)
		      miny = 0;
		    
		    maxy = y + pheight_;
		    if (maxx >= columns_)
		      maxx = columns_ - 1;
		    
		    tmp = rand.nextInt();
		    tmp = (tmp < 0 ? -tmp : tmp);
		    y1 = miny + tmp % ((maxy - miny) + 1);
		  }
		
		if (x1 >= 0 && y1 >= 0 && x1 < columns_ && y1 < rows_)
		  {
		    int pixel = raster_[i];
		    int alpha = (pixel >> 24) & 0xff;
		    int red = (pixel >> 16) & 0xff;
		    int green = (pixel >> 8) & 0xff;
		    int blue = pixel & 0xff;
		    
		    pixels[i] = (alpha << 24) | (red << 16) | (green << 8) | blue;
		  }
	      }
	  }
      }

    profile_timer_.stop();
    
    consumer.setPixels(0, 0, columns_, rows_, defaultRGB_, pixels, 0, columns_);
    consumer.imageComplete(status);
  }
}
