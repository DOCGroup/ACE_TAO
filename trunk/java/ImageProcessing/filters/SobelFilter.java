package imaging.filters;

import java.awt.image.*;

public class SobelFilter extends FivebyfiveFilter
{
  public SobelFilter()
  {
  }
  
  public String info ()
  {
    return "Edge detection filter.";
  }

  public void imageComplete(int status)
  {
    Timer timer = new Timer();
    
    if (status == IMAGEERROR || status == IMAGEABORTED)
      {
	consumer.imageComplete(status);
	System.out.println("Image Error");
	return;
      }
    
    int[] pixels = new int[columns_];
    createColorRasters();
    int pixel = 0;
    int red, green, blue;
    int row1, row2, row3, row4, row5;
    int new_red1 = 0, new_green1 = 0, new_blue1 = 0,
      new_red2 = 0, new_green2 = 0, new_blue2 = 0;
    int alpha;

    for (int x = 1; x < rows_ + 1; x++)
      {
	for (int y = 1; y < columns_ + 1; y++)
	  {
	    alpha = defaultRGB_.getAlpha(raster_[pixel++]);

	    row1 = columns_*(x - 1) + y;
	    row2 = row1 + columns_;
	    row3 = row2 + columns_;
	    row4 = row3 + columns_;
	    row5 = row4 + columns_;
	    
	    new_red1 =
	      (red_raster_[row1 - 1]
	       + (red_raster_[row1] << 1)
	       + red_raster_[row1 + 1]
	       - red_raster_[row3 - 1]
	       - (red_raster_[row3] << 1)
	       - red_raster_[row3 + 1]);
	    
	    new_green1 =
	      (green_raster_[row1 - 1]
	       + (green_raster_[row1] << 1)
	       + green_raster_[row1 + 1] 
	       - green_raster_[row3 - 1]
	       - (green_raster_[row3] << 1)
	       - green_raster_[row3 + 1]);

	    new_blue1 =
	      (blue_raster_[row1 - 1]
	       + (blue_raster_[row1] << 1)
	       + blue_raster_[row1 + 1] 
	       - blue_raster_[row3 - 1] 
	       - (blue_raster_[row3] << 1)
	       - blue_raster_[row3 + 1]);
	    
	    new_red2 =
	      (- red_raster_[row1 - 1]
	       + red_raster_[row1 + 1] 
	       - (red_raster_[row2 - 1] << 1)
	       + (red_raster_[row2 + 1] << 1)
	       - red_raster_[row3 - 1] 
	       + red_raster_[row3 + 1]);
	    
	    new_green2 =
	      (- green_raster_[row1 - 1]
	       + green_raster_[row1 + 1] 
	       - (green_raster_[row2 - 1] << 1)
	       + (green_raster_[row2 + 1] << 1)
	       - green_raster_[row3 - 1] 
	       + green_raster_[row3 + 1]);

	    new_blue2 =
	      (- blue_raster_[row1 - 1] 
	       + blue_raster_[row1 + 1] 
	       - (blue_raster_[row2 - 1] << 1)
	       + (blue_raster_[row2 + 1] << 1)
	       - blue_raster_[row3 - 1] 
	       + blue_raster_[row3 + 1]);

	    new_red1 = (int)Math.sqrt(new_red1*new_red1 + new_red2*new_red2);
	    new_green1 = (int)Math.sqrt(new_green1*new_green1 + new_green2*new_green2);
	    new_blue1 = (int)Math.sqrt(new_blue1*new_blue1 + new_blue2*new_blue2);
	    
	    if (new_red1 > 255)
	      new_red1 = 255;

	    if (new_green1 > 255)
	      new_green1 = 255;
	    
	    if (new_blue1 > 255)
	      new_blue1 = 255;

	    pixels[y - 1] = (alpha << 24) | (new_red1 << 16) | (new_green1 << 8) | new_blue1;

	  }
	consumer.setPixels(0, x-1, columns_, 1, defaultRGB_, pixels, 0, columns_);
      }
    //    System.out.println(timer);
    consumer.imageComplete(status);
  }
}
