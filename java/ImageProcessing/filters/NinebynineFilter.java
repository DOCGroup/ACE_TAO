package imaging.filters;

import java.awt.image.*;

public class NinebynineFilter extends SpatialFilter
{
  int[] red_raster_;
  int[] green_raster_;
  int[] blue_raster_;
  int[] new_matrix_;
  
  public NinebynineFilter(int[] matrix, int degree, int div_factor, int offset)
  {
    new_matrix_ = matrix;
    degree_ = degree;
    div_factor_ = div_factor;
    offset_ = offset;
  }

  public String info ()
  {
    return "Base class filter. Doesn't do much";
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
    int row1, row2, row3, row4, row5, row6, row7, row8, row9;
    int new_red = 0, new_green = 0, new_blue = 0;
    int alpha;

    for (int x = 4; x < rows_ + 4; x++)
      {
	for (int y = 4; y < columns_ + 4; y++)
	  {
	    alpha = defaultRGB_.getAlpha(raster_[pixel++]);

	    row1 = columns_*(x - 4) + y;
	    row2 = row1 + columns_;
	    row3 = row2 + columns_;
	    row4 = row3 + columns_;
	    row5 = row4 + columns_;
	    row6 = row5 + columns_;
	    row7 = row6 + columns_;
	    row8 = row7 + columns_;
	    row9 = row8 + columns_;
	    
	    new_red =
	      (red_raster_[row1 - 4] * new_matrix_[0]
	       + red_raster_[row1 - 3] * new_matrix_[1]
	       + red_raster_[row1 - 2] * new_matrix_[2]
	       + red_raster_[row1 - 1] * new_matrix_[3]
	       + red_raster_[row1] * new_matrix_[4]
	       + red_raster_[row1 + 1] * new_matrix_[5]
	       + red_raster_[row1 + 2] * new_matrix_[6]
	       + red_raster_[row1 + 3] * new_matrix_[7]
	       + red_raster_[row1 + 4] * new_matrix_[8]
	       + red_raster_[row2 - 4] * new_matrix_[9]
	       + red_raster_[row2 - 3] * new_matrix_[10]
	       + red_raster_[row2 - 2] * new_matrix_[11]
	       + red_raster_[row2 - 1] * new_matrix_[12]
	       + red_raster_[row2] * new_matrix_[13]
	       + red_raster_[row2 + 1] * new_matrix_[14]
	       + red_raster_[row2 + 2] * new_matrix_[15]
	       + red_raster_[row2 + 3] * new_matrix_[16]
	       + red_raster_[row2 + 4] * new_matrix_[17]
	       + red_raster_[row3 - 4] * new_matrix_[18]
	       + red_raster_[row3 - 3] * new_matrix_[19]
	       + red_raster_[row3 - 2] * new_matrix_[20]
	       + red_raster_[row3 - 1] * new_matrix_[21]
	       + red_raster_[row3] * new_matrix_[22]
	       + red_raster_[row3 + 1] * new_matrix_[23]
	       + red_raster_[row3 + 2] * new_matrix_[24]
	       + red_raster_[row3 + 3] * new_matrix_[25]
	       + red_raster_[row3 + 4] * new_matrix_[26]
	       + red_raster_[row4 - 4] * new_matrix_[27]
	       + red_raster_[row4 - 3] * new_matrix_[28]
	       + red_raster_[row4 - 2] * new_matrix_[29]
	       + red_raster_[row4 - 1] * new_matrix_[30]
	       + red_raster_[row4] * new_matrix_[31]
	       + red_raster_[row4 + 1] * new_matrix_[32]
	       + red_raster_[row4 + 2] * new_matrix_[33]
	       + red_raster_[row4 + 3] * new_matrix_[34]
	       + red_raster_[row4 + 4] * new_matrix_[35]
	       + red_raster_[row5 - 4] * new_matrix_[36]
	       + red_raster_[row5 - 3] * new_matrix_[37]
	       + red_raster_[row5 - 2] * new_matrix_[38]
	       + red_raster_[row5 - 1] * new_matrix_[39]
	       + red_raster_[row5] * new_matrix_[40]
	       + red_raster_[row5 + 1] * new_matrix_[41]
	       + red_raster_[row5 + 2] * new_matrix_[42]
	       + red_raster_[row5 + 3] * new_matrix_[43]
	       + red_raster_[row5 + 4] * new_matrix_[44]
	       + red_raster_[row6 - 4] * new_matrix_[45]
	       + red_raster_[row6 - 3] * new_matrix_[46]
	       + red_raster_[row6 - 2] * new_matrix_[47]
	       + red_raster_[row6 - 1] * new_matrix_[48]
	       + red_raster_[row6] * new_matrix_[49]
	       + red_raster_[row6 + 1] * new_matrix_[50]
	       + red_raster_[row6 + 2] * new_matrix_[51]
	       + red_raster_[row6 + 3] * new_matrix_[52]
	       + red_raster_[row6 + 4] * new_matrix_[53]
	       + red_raster_[row7 - 4] * new_matrix_[54]
	       + red_raster_[row7 - 3] * new_matrix_[55]
	       + red_raster_[row7 - 2] * new_matrix_[56]
	       + red_raster_[row7 - 1] * new_matrix_[57]
	       + red_raster_[row7] * new_matrix_[58]
	       + red_raster_[row7 + 1] * new_matrix_[59]
	       + red_raster_[row7 + 2] * new_matrix_[60]
	       + red_raster_[row7 + 3] * new_matrix_[61]
	       + red_raster_[row7 + 4] * new_matrix_[62]
	       + red_raster_[row1 - 4] * new_matrix_[63]
	       + red_raster_[row1 - 3] * new_matrix_[64]
	       + red_raster_[row1 - 2] * new_matrix_[65]
	       + red_raster_[row1 - 1] * new_matrix_[66]
	       + red_raster_[row1] * new_matrix_[67]
	       + red_raster_[row1 + 1] * new_matrix_[68]
	       + red_raster_[row1 + 2] * new_matrix_[69]
	       + red_raster_[row1 + 3] * new_matrix_[70]
	       + red_raster_[row1 + 4] * new_matrix_[71]
	       + red_raster_[row1 - 4] * new_matrix_[72]
	       + red_raster_[row1 - 3] * new_matrix_[73]
	       + red_raster_[row1 - 2] * new_matrix_[74]
	       + red_raster_[row1 - 1] * new_matrix_[75]
	       + red_raster_[row1] * new_matrix_[76]
	       + red_raster_[row1 + 1] * new_matrix_[77]
	       + red_raster_[row1 + 2] * new_matrix_[78]
	       + red_raster_[row1 + 3] * new_matrix_[79]
	       + red_raster_[row1 + 4] * new_matrix_[80])/div_factor_;

	    new_green =
	      (green_raster_[row1 - 4] * new_matrix_[0]
	       + green_raster_[row1 - 3] * new_matrix_[1]
	       + green_raster_[row1 - 2] * new_matrix_[2]
	       + green_raster_[row1 - 1] * new_matrix_[3]
	       + green_raster_[row1] * new_matrix_[4]
	       + green_raster_[row1 + 1] * new_matrix_[5]
	       + green_raster_[row1 + 2] * new_matrix_[6]
	       + green_raster_[row1 + 3] * new_matrix_[7]
	       + green_raster_[row1 + 4] * new_matrix_[8]
	       + green_raster_[row2 - 4] * new_matrix_[9]
	       + green_raster_[row2 - 3] * new_matrix_[10]
	       + green_raster_[row2 - 2] * new_matrix_[11]
	       + green_raster_[row2 - 1] * new_matrix_[12]
	       + green_raster_[row2] * new_matrix_[13]
	       + green_raster_[row2 + 1] * new_matrix_[14]
	       + green_raster_[row2 + 2] * new_matrix_[15]
	       + green_raster_[row2 + 3] * new_matrix_[16]
	       + green_raster_[row2 + 4] * new_matrix_[17]
	       + green_raster_[row3 - 4] * new_matrix_[18]
	       + green_raster_[row3 - 3] * new_matrix_[19]
	       + green_raster_[row3 - 2] * new_matrix_[20]
	       + green_raster_[row3 - 1] * new_matrix_[21]
	       + green_raster_[row3] * new_matrix_[22]
	       + green_raster_[row3 + 1] * new_matrix_[23]
	       + green_raster_[row3 + 2] * new_matrix_[24]
	       + green_raster_[row3 + 3] * new_matrix_[25]
	       + green_raster_[row3 + 4] * new_matrix_[26]
	       + green_raster_[row4 - 4] * new_matrix_[27]
	       + green_raster_[row4 - 3] * new_matrix_[28]
	       + green_raster_[row4 - 2] * new_matrix_[29]
	       + green_raster_[row4 - 1] * new_matrix_[30]
	       + green_raster_[row4] * new_matrix_[31]
	       + green_raster_[row4 + 1] * new_matrix_[32]
	       + green_raster_[row4 + 2] * new_matrix_[33]
	       + green_raster_[row4 + 3] * new_matrix_[34]
	       + green_raster_[row4 + 4] * new_matrix_[35]
	       + green_raster_[row5 - 4] * new_matrix_[36]
	       + green_raster_[row5 - 3] * new_matrix_[37]
	       + green_raster_[row5 - 2] * new_matrix_[38]
	       + green_raster_[row5 - 1] * new_matrix_[39]
	       + green_raster_[row5] * new_matrix_[40]
	       + green_raster_[row5 + 1] * new_matrix_[41]
	       + green_raster_[row5 + 2] * new_matrix_[42]
	       + green_raster_[row5 + 3] * new_matrix_[43]
	       + green_raster_[row5 + 4] * new_matrix_[44]
	       + green_raster_[row6 - 4] * new_matrix_[45]
	       + green_raster_[row6 - 3] * new_matrix_[46]
	       + green_raster_[row6 - 2] * new_matrix_[47]
	       + green_raster_[row6 - 1] * new_matrix_[48]
	       + green_raster_[row6] * new_matrix_[49]
	       + green_raster_[row6 + 1] * new_matrix_[50]
	       + green_raster_[row6 + 2] * new_matrix_[51]
	       + green_raster_[row6 + 3] * new_matrix_[52]
	       + green_raster_[row6 + 4] * new_matrix_[53]
	       + green_raster_[row7 - 4] * new_matrix_[54]
	       + green_raster_[row7 - 3] * new_matrix_[55]
	       + green_raster_[row7 - 2] * new_matrix_[56]
	       + green_raster_[row7 - 1] * new_matrix_[57]
	       + green_raster_[row7] * new_matrix_[58]
	       + green_raster_[row7 + 1] * new_matrix_[59]
	       + green_raster_[row7 + 2] * new_matrix_[60]
	       + green_raster_[row7 + 3] * new_matrix_[61]
	       + green_raster_[row7 + 4] * new_matrix_[62]
	       + green_raster_[row1 - 4] * new_matrix_[63]
	       + green_raster_[row1 - 3] * new_matrix_[64]
	       + green_raster_[row1 - 2] * new_matrix_[65]
	       + green_raster_[row1 - 1] * new_matrix_[66]
	       + green_raster_[row1] * new_matrix_[67]
	       + green_raster_[row1 + 1] * new_matrix_[68]
	       + green_raster_[row1 + 2] * new_matrix_[69]
	       + green_raster_[row1 + 3] * new_matrix_[70]
	       + green_raster_[row1 + 4] * new_matrix_[71]
	       + green_raster_[row1 - 4] * new_matrix_[72]
	       + green_raster_[row1 - 3] * new_matrix_[73]
	       + green_raster_[row1 - 2] * new_matrix_[74]
	       + green_raster_[row1 - 1] * new_matrix_[75]
	       + green_raster_[row1] * new_matrix_[76]
	       + green_raster_[row1 + 1] * new_matrix_[77]
	       + green_raster_[row1 + 2] * new_matrix_[78]
	       + green_raster_[row1 + 3] * new_matrix_[79]
	       + green_raster_[row1 + 4] * new_matrix_[80])/div_factor_;

	    new_blue =
	      (blue_raster_[row1 - 4] * new_matrix_[0]
	       + blue_raster_[row1 - 3] * new_matrix_[1]
	       + blue_raster_[row1 - 2] * new_matrix_[2]
	       + blue_raster_[row1 - 1] * new_matrix_[3]
	       + blue_raster_[row1] * new_matrix_[4]
	       + blue_raster_[row1 + 1] * new_matrix_[5]
	       + blue_raster_[row1 + 2] * new_matrix_[6]
	       + blue_raster_[row1 + 3] * new_matrix_[7]
	       + blue_raster_[row1 + 4] * new_matrix_[8]
	       + blue_raster_[row2 - 4] * new_matrix_[9]
	       + blue_raster_[row2 - 3] * new_matrix_[10]
	       + blue_raster_[row2 - 2] * new_matrix_[11]
	       + blue_raster_[row2 - 1] * new_matrix_[12]
	       + blue_raster_[row2] * new_matrix_[13]
	       + blue_raster_[row2 + 1] * new_matrix_[14]
	       + blue_raster_[row2 + 2] * new_matrix_[15]
	       + blue_raster_[row2 + 3] * new_matrix_[16]
	       + blue_raster_[row2 + 4] * new_matrix_[17]
	       + blue_raster_[row3 - 4] * new_matrix_[18]
	       + blue_raster_[row3 - 3] * new_matrix_[19]
	       + blue_raster_[row3 - 2] * new_matrix_[20]
	       + blue_raster_[row3 - 1] * new_matrix_[21]
	       + blue_raster_[row3] * new_matrix_[22]
	       + blue_raster_[row3 + 1] * new_matrix_[23]
	       + blue_raster_[row3 + 2] * new_matrix_[24]
	       + blue_raster_[row3 + 3] * new_matrix_[25]
	       + blue_raster_[row3 + 4] * new_matrix_[26]
	       + blue_raster_[row4 - 4] * new_matrix_[27]
	       + blue_raster_[row4 - 3] * new_matrix_[28]
	       + blue_raster_[row4 - 2] * new_matrix_[29]
	       + blue_raster_[row4 - 1] * new_matrix_[30]
	       + blue_raster_[row4] * new_matrix_[31]
	       + blue_raster_[row4 + 1] * new_matrix_[32]
	       + blue_raster_[row4 + 2] * new_matrix_[33]
	       + blue_raster_[row4 + 3] * new_matrix_[34]
	       + blue_raster_[row4 + 4] * new_matrix_[35]
	       + blue_raster_[row5 - 4] * new_matrix_[36]
	       + blue_raster_[row5 - 3] * new_matrix_[37]
	       + blue_raster_[row5 - 2] * new_matrix_[38]
	       + blue_raster_[row5 - 1] * new_matrix_[39]
	       + blue_raster_[row5] * new_matrix_[40]
	       + blue_raster_[row5 + 1] * new_matrix_[41]
	       + blue_raster_[row5 + 2] * new_matrix_[42]
	       + blue_raster_[row5 + 3] * new_matrix_[43]
	       + blue_raster_[row5 + 4] * new_matrix_[44]
	       + blue_raster_[row6 - 4] * new_matrix_[45]
	       + blue_raster_[row6 - 3] * new_matrix_[46]
	       + blue_raster_[row6 - 2] * new_matrix_[47]
	       + blue_raster_[row6 - 1] * new_matrix_[48]
	       + blue_raster_[row6] * new_matrix_[49]
	       + blue_raster_[row6 + 1] * new_matrix_[50]
	       + blue_raster_[row6 + 2] * new_matrix_[51]
	       + blue_raster_[row6 + 3] * new_matrix_[52]
	       + blue_raster_[row6 + 4] * new_matrix_[53]
	       + blue_raster_[row7 - 4] * new_matrix_[54]
	       + blue_raster_[row7 - 3] * new_matrix_[55]
	       + blue_raster_[row7 - 2] * new_matrix_[56]
	       + blue_raster_[row7 - 1] * new_matrix_[57]
	       + blue_raster_[row7] * new_matrix_[58]
	       + blue_raster_[row7 + 1] * new_matrix_[59]
	       + blue_raster_[row7 + 2] * new_matrix_[60]
	       + blue_raster_[row7 + 3] * new_matrix_[61]
	       + blue_raster_[row7 + 4] * new_matrix_[62]
	       + blue_raster_[row1 - 4] * new_matrix_[63]
	       + blue_raster_[row1 - 3] * new_matrix_[64]
	       + blue_raster_[row1 - 2] * new_matrix_[65]
	       + blue_raster_[row1 - 1] * new_matrix_[66]
	       + blue_raster_[row1] * new_matrix_[67]
	       + blue_raster_[row1 + 1] * new_matrix_[68]
	       + blue_raster_[row1 + 2] * new_matrix_[69]
	       + blue_raster_[row1 + 3] * new_matrix_[70]
	       + blue_raster_[row1 + 4] * new_matrix_[71]
	       + blue_raster_[row1 - 4] * new_matrix_[72]
	       + blue_raster_[row1 - 3] * new_matrix_[73]
	       + blue_raster_[row1 - 2] * new_matrix_[74]
	       + blue_raster_[row1 - 1] * new_matrix_[75]
	       + blue_raster_[row1] * new_matrix_[76]
	       + blue_raster_[row1 + 1] * new_matrix_[77]
	       + blue_raster_[row1 + 2] * new_matrix_[78]
	       + blue_raster_[row1 + 3] * new_matrix_[79]
	       + blue_raster_[row1 + 4] * new_matrix_[80])/div_factor_;

	    if (new_red > 255)
	      new_red = 255;

	    if (new_green > 255)
	      new_green = 255;
	    
	    if (new_blue > 255)
	      new_blue = 255;

	    pixels[y - 4] = (alpha << 24) | (new_red << 16) | (new_green << 8) | new_blue;

	  }
	consumer.setPixels(0, x-5, columns_, 1, defaultRGB_, pixels, 0, columns_);
      }
    //    System.out.println(timer);
    consumer.imageComplete(status);
  }

  protected void createColorRasters()
  {
    int off = 0;
    int index;
    int pixel;
    red_raster_ = new int[(rows_ + 4)*(columns_ + 4)];
    green_raster_ = new int[red_raster_.length];
    blue_raster_ = new int[red_raster_.length];
    
    for (int x = 2; x < rows_ + 2; x++)
      {
	for (int y = 2; y < columns_ + 2; y ++)
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
