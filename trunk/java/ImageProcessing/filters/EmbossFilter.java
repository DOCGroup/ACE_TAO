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
    Timer timer = new Timer();
    
    if (status == IMAGEERROR || status == IMAGEABORTED)
      {
	consumer.imageComplete(status);
	System.out.println("Image Error: " + status);
	return;
      }
    
    int[] pixels = new int[columns_];
    int pixel = 0;
    int red, green, blue;
    int row1, row2, row3, ul, um, ml, mr, lm, lr;
    int new_grey = 0;
    int alpha;

    for (int x = 1; x < rows_ - 1; x++)
      {
	for (int y = 1; y < columns_ - 1; y++)
	  {

	    row1 = columns_*(x - 1) + y;
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
	    
	    pixels[y] = (alpha << 24) | (new_grey << 16) | (new_grey << 8) | new_grey;

	  }
	consumer.setPixels(0, x, columns_, 1, defaultRGB_, pixels, 0, columns_);
      }
    //    System.out.println(timer);
    consumer.imageComplete(status);
  }
}
