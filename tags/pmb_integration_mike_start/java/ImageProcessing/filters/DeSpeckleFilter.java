package imaging.filters;

import java.awt.image.*;

public class DeSpeckleFilter extends SpatialFilter
{
  private int degree_ = 3;
  
  public DeSpeckleFilter()
    {
    }

  public DeSpeckleFilter(int degree)
    {
      degree_ = degree;
    }
  
  public String info ()
  {
    return "Removes speckles from an image.";
  }

  public void imageComplete(int status)
  {
    if (status == IMAGEERROR || status == IMAGEABORTED)
      {
	consumer.imageComplete(status);
	System.out.println("DeSpeckleFilter Image Error: " + status);
	return;
      }

    System.gc();
    
    int rsum, bsum, gsum, r, g, b, a;
    int count, n2, nsq, c2, lcv, lcv2, index;
    int[] rtab, gtab, btab;
    int[] pixels = new int[columns_*rows_];

    profile_timer_.start();
    
    n2 = degree_/2;
    nsq = degree_ * degree_;

    rtab = new int[nsq];
    gtab = new int[nsq];
    btab = new int[nsq];

    for (int z = SpatialFilter.iterations_; z-- > 0;)
      {    
	for (int y = 0, k = 0; y < rows_; y++)
	  {
	    lcv = y + n2;
	    for (int x = 0; x < columns_; x++, k++)
	      {	    
		rsum = gsum = bsum = 0;
		count = 0;
		
		lcv2 = x + n2;
		for (int y1 = y - n2; y1 < lcv; y1++)
		  {
		    index = y1*columns_; 
		    if (y1 >= 0 && y1 < rows_)
		      {
			for (int x1 = x - n2; x1 < lcv2; x1++)
			  {
			    if (x1 >= 0 && x1 < columns_)
			      {
				int pixel = raster_[index + x1];
				rtab[count] = (pixel >> 16) & 0xff;
				gtab[count] = (pixel >> 8) & 0xff;
				btab[count] = (pixel) & 0xff;
				count++;
			      }
			  }
		      }
		  }
		
		for (int d = count >> 1; d > 0; d >>= 1)
		  {
		    for (int i = d; i < count; i++)
		      {		    
			for (int j = i - d; j >= 0 && rtab[j] > rtab[j+d]; j -= d)
			  {
			    int t = rtab[j];
			    rtab[j] = rtab[j + d];
			    rtab[j + d] = t;			
			  }
			
			for (int j = i - d; j >= 0 && gtab[j] > gtab[j+d]; j -= d)
			  {
			    int t = gtab[j];
			    gtab[j] = gtab[j + d];
			    gtab[j + d] = t;			
			  }
			
			for (int j = i - d; j >= 0 && btab[j] > btab[j+d]; j -= d)
			  {
			    int t = btab[j];
			    btab[j] = btab[j + d];
			    btab[j + d] = t;			
			  }
		      }
		  }
		
		c2 = count >> 1;
		
		a = (raster_[k] >> 24) & 0xff;
		r = ( (count % 2 == 0) ? (rtab[c2] + rtab[c2 - 1]) >> 1 : rtab[c2]);
		g = ( (count % 2 == 0) ? (gtab[c2] + gtab[c2 - 1]) >> 1 : gtab[c2]);
		b = ( (count % 2 == 0) ? (btab[c2] + btab[c2 - 1]) >> 1 : btab[c2]);
		
		pixels[k] = (a << 24) | (r << 16) | (g << 8) | b;
	      }
	  }
      }

    profile_timer_.stop();
    
    consumer.setPixels(0, 0, columns_, rows_, defaultRGB_, pixels, 0, columns_);	
    consumer.imageComplete(status);
  }

}
