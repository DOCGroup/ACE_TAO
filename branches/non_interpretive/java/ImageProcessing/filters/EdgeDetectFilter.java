package imaging.filters;

import java.awt.image.*;

public class EdgeDetectFilter extends SpatialFilter
{
  public EdgeDetectFilter()
  {
  }
  
  public String info ()
  {
    return "Edge detection filter.";
  }

  public void imageComplete(int status)
    {
    if (status == IMAGEERROR || status == IMAGEABORTED)
      {
	consumer.imageComplete(status);
	System.out.println("Image Error");
	return;
      }

    System.gc();
    
    int[] pixels = new int[columns_*rows_];
    int pixel = 0;
    int red, green, blue;
    int row1, row2, row3;
    int new_red1 = 0, new_green1 = 0, new_blue1 = 0,
      new_red2 = 0, new_green2 = 0, new_blue2 = 0;
    int ul, um, ur, ll, lm, lr, ml, mr;
    int alpha, lcv, lcv2;

    profile_timer_.start();

    for (int z = SpatialFilter.iterations_; z-- > 0;)
      {
	lcv = rows_ - 1;
	lcv2 = columns_ - 1;
	for (int x = 1, index = 0; x < lcv; x++, index += columns_)
	  {
	    for (int y = 1; y < lcv2; y++)
	      {
		row1 = index + y;
		row2 = row1 + columns_;
		row3 = row2 + columns_;
		
		ul = raster_[row1 - 1];
		um = raster_[row1];
		ur = raster_[row1 + 1];
		ml = raster_[row2 - 1];
		mr = raster_[row2 + 1];
		ll = raster_[row3 - 1];
		lm = raster_[row3];
		lr = raster_[row3 + 1];
		
		alpha = (raster_[row2] >> 24) & 0xff;
		
		new_red1 =
		  (((ul >> 16) & 0xff)
		   + (((um >> 16) & 0xff) << 1)
		   + ((ur >> 16) & 0xff)
		   - ((ll >> 16) & 0xff)
		   - (((lm >> 16) & 0xff) << 1)
		   - ((lr >> 16) & 0xff));
		
		new_green1 =
		  (((ul >> 8) & 0xff)
		   + (((um >> 8) & 0xff) << 1)
		   + ((ur >> 8) & 0xff)
		   - ((ll >> 8) & 0xff)
		   - (((lm >> 8) & 0xff) << 1)
		   - ((lr >> 8) & 0xff));
		
		new_blue1 =
		  (((ul) & 0xff)
		   + (((um) & 0xff) << 1)
		   + ((ur) & 0xff)
		   - ((ll) & 0xff)
		   - (((lm) & 0xff) << 1)
		   - ((lr) & 0xff));
		
		new_red2 =
		  (- ((ul >> 16) & 0xff)
		   + ((ur >> 16) & 0xff)
		   - (((ml >> 16) & 0xff) << 1)
		   + (((mr >> 16) & 0xff) << 1)
		   - ((ll >> 16) & 0xff)
		   + ((lr >> 16) & 0xff));
		
		new_green2 =
		  (- ((ul >> 8) & 0xff)
		   + ((ur >> 8) & 0xff)
		   - (((ml >> 8) & 0xff) << 1)
		   + (((mr >> 8) & 0xff) << 1)
		   - ((ll >> 8) & 0xff)
		   + ((lr >> 8) & 0xff));
		
		new_blue2 =
		  (- ((ul) & 0xff)
		   + ((ur) & 0xff)
		   - (((ml) & 0xff) << 1)
		   + (((mr) & 0xff) << 1)
		   - ((ll) & 0xff)
		   + ((lr) & 0xff));
		
		
		new_red1 = (int)Math.sqrt(new_red1*new_red1 + new_red2*new_red2);
		new_green1 = (int)Math.sqrt(new_green1*new_green1 + new_green2*new_green2);
		new_blue1 = (int)Math.sqrt(new_blue1*new_blue1 + new_blue2*new_blue2);
		
		if (new_red1 < 0) red = 0;
		if (new_green1 < 0) green = 0;
		if (new_blue1 < 0) blue = 0 ;
		
		
		if (new_red1 > 255)
		  new_red1 = 255;
		
		if (new_green1 > 255)
		  new_green1 = 255;
		
		if (new_blue1 > 255)
		  new_blue1 = 255;
		
		pixels[row2] = (alpha << 24) | (new_red1 << 16) | (new_green1 << 8) | new_blue1;		
	      }
	  }
      }

    profile_timer_.stop();
    
    consumer.setPixels(0, 0, columns_, rows_, defaultRGB_, pixels, 0, columns_);
    consumer.imageComplete(status);
  }

  /*
  public void imageComplete(int status)
  {
    if (status == IMAGEERROR || status == IMAGEABORTED)
      {
	consumer.imageComplete(status);
	System.out.println("Image Error");
	return;
      }
    
    int[][] pixels = new int[rows_*columns_];
    int pixel = 0;
    int red, green, blue;
    int row1, row2, row3, row4, row5;
    int a, b, c, d, rsum, gsum, bsum, v, maxv = 0;
    int ul, um, ur, ml, mr, ll, lm, lr, lcv, lcv2;
    int alpha;

    lcv = rows_ -1;
    for (int y = 1; y < lcv; y++)
      {
	lcv2 = columns_ -1;
	for (int x = 1; x < lcv2; x++)
	  {
	    row1 = columns_*(y-1) + x;
	    row2 = row1 + columns_;
	    row3 = row2 + columns_;
	    ul = raster_[row1 - 1];
	    um = raster_[row1];
	    ur = raster_[row1 + 1];
	    ml = raster_[row2 - 1];
	    mr = raster_[row2 + 1];
	    ll = raster_[row3 - 1];
	    lm = raster_[row3];
	    lr = raster_[row3 + 1];

	    alpha = defaultRGB_.getAlpha(raster_[row2]);

	    a = ((lr >> 16) & 0xff) - ((ul >> 16) & 0xff);
	    b = ((mr >> 16) & 0xff) - ((ml >> 16) & 0xff);
	    c = ((ur >> 16) & 0xff) - ((ll >> 16) & 0xff);
	    d = ((um >> 16) & 0xff) - ((lm >> 16) & 0xff);

	    rsum = a + b + c;
	    if (rsum < 0) rsum = -rsum;
	    a = a - c - d;
	    if (a < 0) a  = -a;
	    if (rsum > a) rsum = a;
	    rsum /= 3;
	    
	    a = ((lr >> 8) & 0xff) - ((ul >> 8) & 0xff);
	    b = ((mr >> 8) & 0xff) - ((ml >> 8) & 0xff);
	    c = ((ur >> 8) & 0xff) - ((ll >> 8) & 0xff);
	    d = ((um >> 8) & 0xff) - ((lm >> 8) & 0xff);

	    gsum = a + b + c;
	    if (gsum < 0) gsum = -gsum;
	    a = a - c - d;
	    if (a < 0) a  = -a;
	    if (gsum > a) gsum = a;
	    gsum /= 3;
	    
	    a = ((lr) & 0xff) - ((ul) & 0xff);
	    b = ((mr) & 0xff) - ((ml) & 0xff);
	    c = ((ur) & 0xff) - ((ll) & 0xff);
	    d = ((um) & 0xff) - ((lm) & 0xff);

	    bsum = a + b + c;
	    if (bsum < 0) bsum = -bsum;
	    a = a - c - d;
	    if (a < 0) a  = -a;
	    if (bsum > a) bsum = a;
	    bsum /= 3;

	    pixels[row2] = (alpha << 24) | (rsum << 16) | (gsum << 8) | bsum;

	    v = (((int)(rsum)*11 + ((int)(gsum) << 4) + (int)(bsum)*5) >> 5);
	    
	    if (maxv < v)
	      maxv = v;
	  }
	
      }

    for (int y = 1; y < rows_ - 1; y++)
      {
	for (int x = 1; x < columns_ - 1; x++)
	  {
	    pixel = pixels[y][x];
	    alpha = (pixel >> 24) & 0xff;
	    red = (((pixel >> 16) & 0xff) * 255) / maxv;
	    green = (((pixel >> 8) & 0xff) * 255) / maxv;
	    blue = ((pixel & 0xff) * 255) / maxv;

	    if (red < 0) red = 0;
	    if (green < 0) green = 0;
	    if (blue < 0) blue = 0 ;

	    if (red > 255) red = 255;
	    if (green > 255) green = 255;
	    if (blue > 255) blue = 255;

	    pixels[y*x] = (alpha << 24) | (red << 16) | (green << 8) | blue;
	  }

	consumer.setPixels(0, y, columns_, 1, defaultRGB_, pixels[y], 0, columns_);
	}
      
    consumer.imageComplete(status);
  }
  */
}
