package imaging.filters;

import java.awt.image.*;

public class PixelizeFilter extends SpatialFilter
{
  private int pwidth_ = 4;
  private int pheight_ = 4;

  public PixelizeFilter()
    {
    }
  
  public PixelizeFilter(int pwidth, int pheight)
    {
      pwidth_ = pwidth;
      pheight_ = pheight;
    }

  public String info ()
  {
    return "Pixelizes the image.";
  }

  public void imageComplete(int status)
  {
    if (status == IMAGEERROR || status == IMAGEABORTED)
      {
	consumer.imageComplete(status);
	System.out.println("Image Error: " + status);
	return;
      }

    int pixel;
    int nsum, asum, rsum, gsum, bsum, nwide, nhigh, stx, sty, x, y;
    int x_offset, y_offset, x_extent, y_extent;
    int[] results = new int[pwidth_*pheight_];
    
    nwide = (columns_ + pwidth_ - 1) / pwidth_;
    nhigh = (rows_ + pheight_ - 1) / pheight_;

    stx = -(nwide*pwidth_ - columns_)/2;
    sty = -(nhigh*pheight_ - rows_)/2;

    y = sty;
    for (int i = 0; i < nhigh; i++, y += pheight_)
      {
	x = stx;

	for (int j = 0; j < nwide; j++, x += pwidth_)
	  {
	    nsum = asum = rsum = bsum = gsum = 0;
	    
	    for (int y1 = y; y1 < y + pheight_; y1++)
	      {
		for (int x1 = x; x1 < x + pwidth_; x1++)
		  {
		    if (x1 >= 0 && y1 >= 0 && x1 < columns_ && y1 < rows_)
		      {
			nsum++;
			pixel = raster_[y1*columns_ + x1];
			
			asum += (pixel >> 24) & 0xff;
			rsum += (pixel >> 16) & 0xff;
			gsum += (pixel >> 8 ) & 0xff;
			bsum += pixel & 0xff;
		      }
		  }
	      }

	    if (nsum > 0)
	      {
		rsum /= nsum;
		gsum /= nsum;
		bsum /= nsum;

		if (asum < 0) asum = 0;
		if (rsum < 0) rsum = 0;
		if (gsum < 0) gsum = 0;
		if (bsum < 0) bsum = 0 ;

		if (asum > 255) asum = 255;
		if (rsum > 255) rsum = 255;
		if (gsum > 255) gsum = 255;
		if (bsum > 255) bsum = 255;

	      }
	    
	    for (int k = 0; k < pwidth_*pheight_; k++)
	      results[k] =  (asum << 24) | (rsum << 16) | (gsum << 8) | bsum;

	    x_offset = x;
	    y_offset = y;
	    x_extent = pwidth_;
	    y_extent = pheight_;
	    
	    if (x < 0)
	      x_offset = -x;

	    if (y < 0)
	      y_offset = -y;

	    if (x + pwidth_ > columns_)
	      x_extent = x - columns_;

	    if (y + pheight_ > rows_)
	      y_extent = y - rows_;
	    
	    consumer.setPixels(x_offset, y_offset, x_extent, y_extent, defaultRGB_, results, 0, x_extent);
	    	    
	  }	
      }

    consumer.imageComplete(status);
  }
}
