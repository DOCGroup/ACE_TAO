package imaging.filters;

import java.awt.image.*;

public class ThreebythreeFilter extends SpatialFilter
{
  int[] red_raster_;
  int[] green_raster_;
  int[] blue_raster_;
  int[] new_matrix_;

  public ThreebythreeFilter()
  {
  }
  
  public ThreebythreeFilter(int[] matrix, int degree, int div_factor, int offset)
  {
    new_matrix_ = matrix;
    degree_ = degree;
    div_factor_ = div_factor;
    offset_ = offset;
  }

  public String info ()
  {
    return "Base Filter class. Doesn't do much";
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
	    
	    new_red =
	      (red_raster_[row1 - 1] * new_matrix_[0]
	      + red_raster_[row1] * new_matrix_[1]
	      + red_raster_[row1 + 1] * new_matrix_[2]
	      + red_raster_[row2 - 1] * new_matrix_[3]
	      + red_raster_[row2] * new_matrix_[4]
	      + red_raster_[row2 + 1] * new_matrix_[5]
	      + red_raster_[row3 - 1] * new_matrix_[6]
	      + red_raster_[row3] * new_matrix_[7]
	      + red_raster_[row3 + 1] * new_matrix_[8])/div_factor_;

	    new_green =
	      (green_raster_[row1 - 1] * new_matrix_[0]
	      + green_raster_[row1] * new_matrix_[1]
	      + green_raster_[row1 + 1] * new_matrix_[2]
	      + green_raster_[row2 - 1] * new_matrix_[3]
	      + green_raster_[row2] * new_matrix_[4]
	      + green_raster_[row2 + 1] * new_matrix_[5]
	      + green_raster_[row3 - 1] * new_matrix_[6]
	      + green_raster_[row3] * new_matrix_[7]
	      + green_raster_[row3 + 1] * new_matrix_[8])/div_factor_;

	    new_blue =
	      (blue_raster_[row1 - 1] * new_matrix_[0]
	      + blue_raster_[row1] * new_matrix_[1]
	      + blue_raster_[row1 + 1] * new_matrix_[2]
	      + blue_raster_[row2 - 1] * new_matrix_[3]
	      + blue_raster_[row2] * new_matrix_[4]
	      + blue_raster_[row2 + 1] * new_matrix_[5]
	      + blue_raster_[row3 - 1] * new_matrix_[6]
	      + blue_raster_[row3] * new_matrix_[7]
	      + blue_raster_[row3 + 1] * new_matrix_[8])/div_factor_;

	    if (new_red > 255)
	      new_red = 255;

	    if (new_green > 255)
	      new_green = 255;
	    
	    if (new_blue > 255)
	      new_blue = 255;

	    pixels[y - 1] = (alpha << 24) | (new_red << 16) | (new_green << 8) | new_blue;

	  }
	consumer.setPixels(0, x-1, columns_, 1, defaultRGB_, pixels, 0, columns_);
      }
    //    System.out.println(timer);
    consumer.imageComplete(status);
  }

  protected void createColorRasters()
  {
    int off = 0;
    int index;
    int pixel;
    red_raster_ = new int[(rows_ + 2)*(columns_ + 2)];
    green_raster_ = new int[red_raster_.length];
    blue_raster_ = new int[red_raster_.length];
    
    for (int x = 1; x < rows_ + 1; x++)
      {
	for (int y = 1; y < columns_ + 1; y ++)
	  {
	    index = x*columns_ + y;
	    pixel = raster_[off];
	    red_raster_[index] = defaultRGB_.getRed(pixel);
	    green_raster_[index] = defaultRGB_.getGreen(pixel);
	    blue_raster_[index] = defaultRGB_.getBlue(pixel);
	    off++;
	  }
      }
  }
}
