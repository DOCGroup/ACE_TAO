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
	System.out.println("Pixelize Image Error: " + status);
	return;
      }

    System.gc();
    
    int pixel;
    int nsum, asum, rsum, gsum, bsum, nwide, nhigh, stx, sty, x, y;
    int x_offset, y_offset, x_extent, y_extent, lcv, lcv2, lcv3, index;
    int[] results = new int[rows_*columns_];

    profile_timer_.start();

    for (int z = SpatialFilter.iterations_; z-- > 0;)
      {
	lcv3 = pwidth_*pheight_;
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
		
		lcv = y + pheight_;
		lcv2 = x + pwidth_;
		for (int y1 = y; y1 < lcv; y1++)
		  {
		    index = y1*columns_;
		    for (int x1 = x; x1 < lcv2; x1++)
		      {
			if (x1 >= 0 && y1 >= 0 && x1 < columns_ && y1 < rows_)
			  {
			    nsum++;
			    pixel = raster_[index + x1];
			    
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
		    if (bsum < 0) bsum = 0;

		    if (asum > 255) asum = 255;
		    if (rsum > 255) rsum = 255;
		    if (gsum > 255) gsum = 255;
		    if (bsum > 255) bsum = 255;
		    
		  }
		
		for (int y1 = y; y1< lcv; y1++)
		  {
		    index = y1*columns_;
		    for (int x1 = x; x1 < lcv2; x1++)
		      {
			if (x1 >= 0 && y1 >= 0 && x1 < columns_ && y1 < rows_)
			  results[index + x1]
			    = (asum << 24) | (rsum << 16) | (gsum << 8) | bsum;
		      }
		  }	    
	      }	
	  }
      }
    
    profile_timer_.stop();
    
    consumer.setPixels(0, 0, columns_, rows_, defaultRGB_, results, 0, columns_);
    consumer.imageComplete(status);
  }
}
