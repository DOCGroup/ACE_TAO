package imaging.filters;

import java.awt.image.*;

public class BlurFilter extends SpatialFilter
{
  private int[][] mean_filter;     
  private int degree_ = 3;
  
  public BlurFilter()
    {
    }

  public BlurFilter(int degree)
    {
      degree_ = degree;
    }

  public void imageComplete(int status)
  {
    if (status == IMAGEERROR || status == IMAGEABORTED)
      {
	consumer.imageComplete(status);
	System.out.println("Image Error");
	return;
      }

    int[] pixels = new int[columns_];
    int pixel = 0, count = 0;
    int red, green, blue;
    int alpha, n2 = degree_/2;

    for (int y = 0; y < rows_; y++)
      {
	for (int x = 0; x < columns_ - 1; x++)
	  {	    
	    red = 0; green = 0; blue = 0; count = 0;
	    alpha = (raster_[y*columns_ + x] >> 24) & 0xff;
	    
	    for (int y1 = y - n2; y1 < y + n2; y1++)
	      {
		if (y1 >= 0 && y1 < rows_)
		  {		
		    for (int x1 = x - n2; x1 < x + n2; x1++)
		      {
			if (x1 >= 0 && x1 < columns_)
			  {
			    pixel = raster_[y1*columns_ + x1];
			    
			    red += (pixel >> 16) & 0xff;
			    green += (pixel >> 8) & 0xff;
			    blue += (pixel) & 0xff;
			    count++;
			  }
		      }
		  }
	      }
	    
	    red /= count;
	    green /= count;
	    blue /= count;

	    if (red < 0) red = 0;
	    if (green < 0) green = 0;
	    if (blue < 0) blue = 0 ;

	    if (red > 255) red = 255;
	    if (green > 255) green = 255;
	    if (blue > 255) blue = 255;

	    pixels[x] = (alpha << 24) | (red << 16) | (green << 8) | blue;
	  }
	
	consumer.setPixels(0, y, columns_, 1, defaultRGB_, pixels, 0, columns_);
      }

    System.out.println("Finished altering image");
    consumer.imageComplete(status);
  }
  
  public String info ()
    {
      return "Blurs an image.";
    }
}
