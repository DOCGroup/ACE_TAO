package imaging.filters;

import java.awt.image.*;

public class UnsharpFilter extends SpatialFilter
{

  int[] red_raster_;
  int[] green_raster_;
  int[] blue_raster_;
  
  private int sharp_ = 1;
  private int smooth_ = 5;
  private float frac_ = (float)((float).8/(float)2);
  
  public UnsharpFilter()
  {
  }

  public UnsharpFilter(int sharp, int smooth, float frac)
  {
    float div = (float)2.0;
    sharp_ = sharp;
    smooth_ = smooth;
    frac_ = frac/div;
  }

  public String info ()
  {
    return "Sharpens an image.";
  }

  public void imageComplete(int status)
  {
    if (status == IMAGEERROR || status == IMAGEABORTED)
      {
	consumer.imageComplete(status);
	System.out.println("Image Error");
	return;
      }

    int[] pixels = new int[columns_*rows_], temp;
    int[] blurred_red = new int[columns_* rows_];
    int[] blurred_blue = new int[columns_* rows_];
    int[] blurred_green = new int[columns_* rows_];
    int pixel = 0;
    int red, green, blue;
    int row1, row2, row3;
    int index, sum, index_pixels;
    int new_red = 0, new_green = 0, new_blue = 0;
    int alpha, iterations;

    if (sharp_ > smooth_)
      iterations = sharp_;
    else
      iterations = smooth_;
    
    createColorRasters();
    for (int i = 0; i < iterations; i++)
      {
	for (int x = 1; x < rows_ - 1; x++)
	  {
	    for (int y = 1; y < columns_ - 1; y++)
	      {		
		row1 = columns_*(x - 1) + y;
		row2 = row1 + columns_;
		row3 = row2 + columns_;
		
		alpha = defaultRGB_.getAlpha(raster_[row2]);

		new_red =
		  (red_raster_[row1 - 1]
		   + red_raster_[row1 + 1]
		   + red_raster_[row3 - 1]
		   + red_raster_[row3 + 1]
		   + ((red_raster_[row1]
		      + red_raster_[row2 - 1]
		      + red_raster_[row2 + 1]
		      + red_raster_[row3]) << 1)
		   + (red_raster_[row2] << 2)) >> 4;

		new_green =
		  (green_raster_[row1 - 1]
		   + green_raster_[row1 + 1]
		   + green_raster_[row3 - 1]
		   + green_raster_[row3 + 1]
		   + ((green_raster_[row1]
		      + green_raster_[row2 - 1]
		      + green_raster_[row2 + 1]
		      + green_raster_[row3]) << 1)
		   + (green_raster_[row2] << 2)) >> 4;
		
		new_blue =
		  (blue_raster_[row1 - 1]
		   + blue_raster_[row1 + 1]
		   + blue_raster_[row3 - 1]
		   + blue_raster_[row3 + 1]
		   + ((blue_raster_[row1]
		      + blue_raster_[row2 - 1]
		      + blue_raster_[row2 + 1]
		      + blue_raster_[row3]) << 1)
		   + (blue_raster_[row2] << 2)) >> 4;

		pixels[x*columns_ + y] = (alpha << 24) | (new_red << 16) | (new_green << 8) | new_blue;
		
	      }
	  }

	sum = columns_ - 1;
	for (int y = 1; y < rows_ + 1; y++)
	  {
	    index = y*columns_;
	    index_pixels = (y-1)*columns_;

	    alpha = defaultRGB_.getAlpha(raster_[index_pixels]);	    
	    new_red = red_raster_[index];
	    new_blue = blue_raster_[index];
	    new_green = green_raster_[index];
	    pixels[index_pixels] = (alpha << 24) | (new_red << 16) | (new_green << 8) | new_blue;

	    index += sum;
	    index_pixels += sum;

	    alpha = defaultRGB_.getAlpha(raster_[index_pixels]);
	    new_red = red_raster_[index];
	    new_blue = blue_raster_[index];
	    new_green = green_raster_[index];
	    pixels[index_pixels] = (alpha << 24) | (new_red << 16) | (new_green << 8) | new_blue;
	  }

	sum = columns_*(rows_ -1);
	for (int x = 1; x < columns_ + 1; x++)
	  {
	    alpha = defaultRGB_.getAlpha(raster_[x-1]);	    
	    new_red = red_raster_[x];
	    new_blue = blue_raster_[x];
	    new_green = green_raster_[x];
	    pixels[x-1] = (alpha << 24) | (new_red << 16) | (new_green << 8) | new_blue;

	    index = x + sum;

	    alpha = defaultRGB_.getAlpha(raster_[index-1]);	    
	    new_red = red_raster_[index];
	    new_blue = blue_raster_[index];
	    new_green = green_raster_[index];

	    pixels[index-1] = (alpha << 24) | (new_red << 16) | (new_green << 8) | new_blue;
	  }

	temp = pixels;
	pixels = raster_;
	raster_ = temp;

	createColorRasters();
		
	if (i == sharp_ - 1)
	  {
	    sum = 0;
	    for (int x = 1; x < rows_ + 1; x++)
	      {
		for (int y = 1; y < columns_ + 1; y ++)
		  {
		    index = x*columns_ + y;
		    blurred_red[sum] = red_raster_[index];
		    blurred_blue[sum] = blue_raster_[index];
		    blurred_green[sum] = green_raster_[index];
		    sum++;
		  }
	      }
	  }
      }

    sum = 0;
    for (int x = 1; x < rows_ + 1; x++)
      {
	for (int y = 1; y < columns_ + 1; y++)
	  {
	    index = x*columns_ + y;
	    
	    alpha = defaultRGB_.getAlpha(raster_[sum]);
	    
	    new_red = blurred_red[sum] - (int)((float)red_raster_[index]*frac_);
	    if (new_red < 0)
	      new_red = 0;
	    else if (new_red > 255)
	      new_red = 255;

	    blurred_red[sum] = new_red;
	    
	    new_blue = blurred_blue[sum] - (int)((float)blue_raster_[index]*frac_);
	    if (new_blue < 0)
	      new_blue = 0;
	    else if (new_blue > 255)
	      new_blue = 255;

	    blurred_blue[sum] = new_blue;

	    new_green = blurred_green[sum] - (int)((float)green_raster_[index]*frac_);
	    if (new_green < 0)
	      new_green = 0;
	    else if (new_green > 255)
	      new_green = 255;
	    
	    blurred_green[sum] = new_green;

	    pixels[sum] = (alpha << 24) | (new_red << 16) | (new_green << 8) | new_blue;
	    sum++;
	  }
      }

    consumer.setPixels(0, 0, columns_, rows_, defaultRGB_, pixels, 0, columns_);
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




