package imaging.filters;

import java.awt.image.*;
import JACE.Timers.ProfileTimer;

public class SpatialFilter extends ImageFilter implements MedFilter
{
  public SpatialFilter()
  {
  }
  
  public SpatialFilter(int[][] matrix, int degree)
  {
    this(matrix, degree, 1, 0);
  }
  
  public SpatialFilter(int[][] matrix, int degree, int div_factor, int offset)
  {
    matrix_ = matrix;
    div_factor_ = div_factor;
    offset_ = offset;
    degree_ = degree;
  }

  public void setFilter(int[][] matrix, int degree, int div_factor)
    {
      matrix_ = matrix;
      degree_ = degree;
      div_factor_ = div_factor;
    }
  
  public String info ()
  {
    return "Base Filter class. Doesn't do much";
  }

  public long filterTime()
    {
      return profile_timer_.elapsedTime();
    }

  public static void setTimer(int internal_iterations)
    {
      iterations_ = internal_iterations;
    }
  
  public void setDimensions(int width, int height)
  {
    rows_ = height;
    columns_ = width;
    raster_ = new int[width * height];
    consumer.setDimensions(width, height);
  }
  
  public void setPixels(int x, int y, int w, int h, ColorModel model,
                          byte pixels[], int off, int scansize)
  {
    int source_offset = off;
    int dest_offset = y * columns_ + x;

    for (int y_ind = 0; y_ind < h; y_ind++)
      {
	for (int x_ind = 0; x_ind < w; x_ind++)
	  {
	    raster_[dest_offset] = model.getRGB(pixels[source_offset] & 0xff);
	    dest_offset++;
	    source_offset++;
	  }

	source_offset += (scansize - w);
	dest_offset += (columns_ - w);
      }
  }

  public void setPixels(int x, int y, int w, int h, ColorModel model,
                          int pixels[], int off, int scansize)
  {
    int source_offset = off;
    int dest_offset = y * columns_ + x;
    
    if (model == defaultRGB_)
      {
	for (int yc = 0; yc < h; yc++)
	  {
	    System.arraycopy(pixels, source_offset, raster_, dest_offset, w);
	    source_offset += scansize;
	    dest_offset += columns_;
	  }
      }
    else
      {

	for (int yc = 0; yc < h; yc++)
	  {
	    for (int xc = 0; xc < w; xc++)
	      {
		raster_[dest_offset] = model.getRGB(pixels[source_offset]);
		dest_offset++;
		source_offset++;
	      }
	    source_offset += (scansize - w);
	    dest_offset += (columns_ - w);
	  }
      }
  }
  
  public void setColorModel(ColorModel model)
  {
    consumer.setColorModel(defaultRGB_);
  }

  public void setHints(int hintflags)
  {
    consumer.setHints(TOPDOWNLEFTRIGHT
		      | COMPLETESCANLINES
		      | SINGLEPASS
		      | (hintflags & SINGLEFRAME));
  }


  public void imageComplete(int status)
  {
    System.out.println("Image Complete called");
    
    if (status == IMAGEERROR || status == IMAGEABORTED)
      {
	consumer.imageComplete(status);
	System.out.println("Image Error");
	return;
      }
    
    int[] pixels = new int[columns_];
    int[][] new_raster= expandRaster();
    int pixel = 0;
    int red, green, blue;
    int new_red = 0, new_green = 0, new_blue = 0;
    int alpha;

    for (int x = raster_offset_; x < rows_; x++)
      {
	for (int y = raster_offset_; y < columns_; y++)
	  {
	    new_red = 0; new_green = 0; new_blue = 0;
	    alpha = defaultRGB_.getAlpha(new_raster[x][y]);
	    for (int i = 0; i < degree_; i++)
	      {
		for (int j = 0; j < degree_; j++)
		  {
		    pixel = new_raster[x + (i - raster_offset_)][y + (j - raster_offset_)];
		    
		    red = defaultRGB_.getRed(pixel) * matrix_[i][j];
		    blue = defaultRGB_.getBlue(pixel) * matrix_[i][j];
		    green = defaultRGB_.getGreen(pixel) * matrix_[i][j];
		    
		    new_red += red;
		    new_green += green;
		    new_blue += blue;
		  }
	      }
	    
	    new_red /= div_factor_;
	    new_green /= div_factor_;
	    new_blue /= div_factor_;
	    
	    new_red = Math.min(Math.abs(new_red), 255);
	    new_green = Math.min(Math.abs(new_green), 255);
	    new_blue = Math.min(Math.abs(new_blue), 255);

	    pixels[y - raster_offset_] =
	      (alpha << 24) | (new_red << 16) | (new_green << 8) | new_blue;
	  }
	
	consumer.setPixels(0, x - raster_offset_, columns_, 1, defaultRGB_, pixels, 0, columns_);
      }

    System.out.println("Finished altering image");
    consumer.imageComplete(status);
  }

  protected int[][] expandRaster()
  {
    int[][] new_raster;
    int index = 0;

    raster_offset_ = degree_ / 2;
    new_raster = new int[rows_ + raster_offset_*2][columns_ + raster_offset_*2];
    
    for (int x = 0; x < rows_; x++)
      {
	for (int y = 0; y < columns_; y++)
	  {
	    new_raster[x + raster_offset_][y + raster_offset_] = raster_[index];
	    index++;
	  }
      }

    return new_raster;
  }

  protected static final int intensity(int rd, int gn, int bl)
    {
      return (((int)(rd)*11 + (int)(gn)*16 + (int)(bl)*5) >> 5);
    }

  protected static int iterations_ = 1;
  protected static final  ColorModel defaultRGB_ = ColorModel.getRGBdefault();
  protected int[][] matrix_;
  protected int[] raster_;
  protected int rows_ = 0, columns_ = 0;
  protected int div_factor_ = 1, offset_, degree_;
  protected int raster_offset_ = 0;
  protected final ProfileTimer profile_timer_ = new ProfileTimer();
}
