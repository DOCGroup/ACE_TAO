package imaging.filters;

import java.awt.image.*;

public class EmbossFilter extends SpatialFilter
{
  
  public EmbossFilter()
  {
  }
    
  public String info ()
  {
    return "Embosses an image.";
  }

  public void imageComplete(int status)
  {
    if (status == IMAGEERROR || status == IMAGEABORTED)
      {
	consumer.imageComplete(status);
	System.out.println("Emboss: Image Error: " + status);
	return;
      }

    System.out.println("Beginning to emboss an image.");
    System.gc();
    
    int[] pixels = new int[columns_*rows_];
    int pixel = 0;
    int red, green, blue;
    int row1, row2, row3, ul, um, ml, mr, lm, lr;
    int new_grey = 0, lcv, lcv2;
    int alpha;

    profile_timer_.start();

    for (int z = SpatialFilter.iterations_; z-- > 0;)
      {    
	lcv = rows_ -1;
	for (int x = 1, index = 0; x < lcv; x++, index += columns_)
	  {
	    lcv2 = columns_ - 1;
	    for (int y = 1; y < lcv2; y++)
	      {
		
		row1 = index + y;
		row2 = row1 + columns_;
		row3 = row2 + columns_;
		
		ul = raster_[row1 - 1];
		um = raster_[row1];
		ml = raster_[row2 - 1];
		mr = raster_[row2 + 1];
		lm = raster_[row3];
		lr = raster_[row3 + 1];
		
		alpha = (raster_[row2] >> 24) & 0xff;
		
		red = ((- (((ul >> 16) & 0xff) << 1) - ((um >> 16) & 0xff) -
			((ml >> 16) & 0xff) + ((mr >> 16) & 0xff) +
			((lm >> 16) & 0xff) + (((lr >> 16) & 0xff) << 1)) >> 3) + 128;
		
		green = ((- (((ul >> 8) & 0xff) << 1) - ((um >> 8) & 0xff) -
			  ((ml >> 8) & 0xff) + ((mr >> 8) & 0xff) +
			  ((lm >> 8) & 0xff) + (((lr >> 8) & 0xff) << 1)) >> 3) + 128;
		
		blue = ((- ((ul & 0xff) << 1) - (um & 0xff) -
			 (ml & 0xff) + (mr & 0xff) +
			 (lm & 0xff) + ((lr & 0xff) << 1)) >> 3) + 128;
		
		new_grey = (((int)(red)*11 + ((int)(green) << 4) + (int)(blue)*5) >> 5);
		
		pixels[row2] = (alpha << 24) | (new_grey << 16) | (new_grey << 8) | new_grey;
		
	      }
	  }
      }

    profile_timer_.stop();
	
    consumer.setPixels(0, 0, columns_, rows_, defaultRGB_, pixels, 0, columns_);
    //    System.out.println(timer);
    consumer.imageComplete(status);
  }
}
