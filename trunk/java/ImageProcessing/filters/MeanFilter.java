package imaging.filters;

import java.awt.image.*;

public class MeanFilter extends FivebyfiveFilter
{
  public MeanFilter()
  {
  }

  public String info ()
  {
    return "Blurs an image.";
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
    int new_red = 0, new_green = 0, new_blue = 0;
    int alpha;

    for (int x = 2; x < rows_ + 2; x++)
      {
	for (int y = 2; y < columns_ + 2; y++)
	  {
	    alpha = defaultRGB_.getAlpha(raster_[pixel++]);

	    row1 = columns_*(x - 2) + y;
	    row2 = row1 + columns_;
	    row3 = row2 + columns_;
	    row4 = row3 + columns_;
	    row5 = row4 + columns_;
	    
	    new_red =
	      (red_raster_[row1 - 2]
	      + red_raster_[row1 - 1]
	      + red_raster_[row1] 
	      + red_raster_[row1 + 1]
	      + red_raster_[row1 + 2]
	      + red_raster_[row2 - 2]
	      + red_raster_[row2 - 1]
	      + red_raster_[row2] 
	      + red_raster_[row2 + 1]
	      + red_raster_[row2 + 2]
	      + red_raster_[row3 - 2]
	      + red_raster_[row3 - 1]
	      + red_raster_[row3] 
	      + red_raster_[row3 + 1]
	      + red_raster_[row3 + 2]
	      + red_raster_[row4 - 2]
	      + red_raster_[row4 - 1]
	      + red_raster_[row4] 
	      + red_raster_[row4 + 1]
	      + red_raster_[row4 + 2]
	      + red_raster_[row5 - 2]
	      + red_raster_[row5 - 1]
	      + red_raster_[row5] 
	      + red_raster_[row5 + 1]
	      + red_raster_[row5 + 2])/25;

	    new_green =
	      (green_raster_[row1 - 2]
	      + green_raster_[row1 - 1]
	      + green_raster_[row1] 
	      + green_raster_[row1 + 1]
	      + green_raster_[row1 + 2]
	      + green_raster_[row2 - 2]
	      + green_raster_[row2 - 1]
	      + green_raster_[row2] 
	      + green_raster_[row2 + 1]
	      + green_raster_[row2 + 2]
	      + green_raster_[row3 - 2]
	      + green_raster_[row3 - 1]
	      + green_raster_[row3] 
	      + green_raster_[row3 + 1]
	      + green_raster_[row3 + 2]
	      + green_raster_[row4 - 2]
	      + green_raster_[row4 - 1]
	      + green_raster_[row4] 
	      + green_raster_[row4 + 1]
	      + green_raster_[row4 + 2]
	      + green_raster_[row5 - 2]
	      + green_raster_[row5 - 1]
	      + green_raster_[row5] 
	      + green_raster_[row5 + 1]
	      + green_raster_[row5 + 2])/25;

	    new_blue =
	      (blue_raster_[row1 - 2]
	      + blue_raster_[row1 - 1]
	      + blue_raster_[row1] 
	      + blue_raster_[row1 + 1]
	      + blue_raster_[row1 + 2]
	      + blue_raster_[row2 - 2]
	      + blue_raster_[row2 - 1]
	      + blue_raster_[row2] 
	      + blue_raster_[row2 + 1]
	      + blue_raster_[row2 + 2]
	      + blue_raster_[row3 - 2]
	      + blue_raster_[row3 - 1]
	      + blue_raster_[row3] 
	      + blue_raster_[row3 + 1]
	      + blue_raster_[row3 + 2]
	      + blue_raster_[row4 - 2]
	      + blue_raster_[row4 - 1]
	      + blue_raster_[row4] 
	      + blue_raster_[row4 + 1]
	      + blue_raster_[row4 + 2]
	      + blue_raster_[row5 - 2]
	      + blue_raster_[row5 - 1]
	      + blue_raster_[row5] 
	      + blue_raster_[row5 + 1]
	      + blue_raster_[row5 + 2])/25;


	    if (new_red > 255)
	      new_red = 255;

	    if (new_green > 255)
	      new_green = 255;
	    
	    if (new_blue > 255)
	      new_blue = 255;

	    pixels[y - 2] = (alpha << 24) | (new_red << 16) | (new_green << 8) | new_blue;

	  }
	consumer.setPixels(0, x-2, columns_, 1, defaultRGB_, pixels, 0, columns_);
      }
    //    System.out.println(timer);
    consumer.imageComplete(status);
  }

}
