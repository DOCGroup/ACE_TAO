package imaging.filters;

import java.awt.image.*;

public class EmbossFilter extends SpatialFilter
{
  int[] grey_raster_;
  
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
    createColorRasters();
    int pixel = 0;
    int red, green, blue;
    int row1, row2, row3, row4, row5;
    int new_grey = 0;
    int alpha;

    for (int x = 1; x < rows_; x++)
      {
	for (int y = 1; y < columns_; y++)
	  {
	    alpha = defaultRGB_.getAlpha(raster_[pixel++]);

	    row1 = columns_*(x - 1) + y;
	    row2 = row1 + columns_;
	    row3 = row2 + columns_;
	    row4 = row3 + columns_;
	    row5 = row4 + columns_;
	    
	    new_grey =
	      (- grey_raster_[row1 - 1]
	      - grey_raster_[row1]
	      - grey_raster_[row2 - 1]
	      + grey_raster_[row2 + 1]
	      + grey_raster_[row3]
	      + grey_raster_[row3 + 1])/6 + 128;

	    if (new_grey > 255)
	      new_grey = 255;

	    pixels[y - 1] = (alpha << 24) | (new_grey << 16) | (new_grey << 8) | new_grey;

	  }
	consumer.setPixels(0, x-1, columns_, 1, defaultRGB_, pixels, 0, columns_);
      }
    //    System.out.println(timer);
    consumer.imageComplete(status);
  }

  protected void createColorRasters()
  {
    int off = 0;
    int index = columns_;
    int pixel;
    grey_raster_ = new int[(rows_ + 1)*(columns_ + 1)];
    
    for (int x = 1; x < rows_ + 1; x++)
      {
	for (int y = 1; y < columns_ + 1; y ++)
	  {
	    index++;
	    pixel = raster_[off];
	    grey_raster_[index] = (int)(((double).212671*(double)defaultRGB_.getRed(pixel))
	      + (double)((double).715160*(double)defaultRGB_.getGreen(pixel))
	      + (double)((double).072169*(double)defaultRGB_.getBlue(pixel)));
										
	    off++;
	  }
      }
  }
}
