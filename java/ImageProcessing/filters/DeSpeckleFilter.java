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
	System.out.println("Image Error: " + status);
	return;
      }

    int rsum, bsum, gsum, r, g, b, a;
    int count, n2, nsq, c2;
    int[] rtab, gtab, btab;
    int[] pixels = new int[columns_];
    
    n2 = degree_/2;
    nsq = degree_ * degree_;

    rtab = new int[nsq];
    gtab = new int[nsq];
    btab = new int[nsq];

    for (int y = 0; y < rows_; y++)
      {		
	for (int x = 0; x < columns_; x++)
	  {	    
	    rsum = gsum = bsum = 0;
	    count = 0;

	    for (int y1 = y - n2; y1 < y + n2; y1++)
	      {
		if (y1 >= 0 && y1 < rows_)
		  {
		    for (int x1 = x - n2; x1 < x + n2; x1++)
		      {
			if (x1 >= 0 && x1 < columns_)
			  {
			    int pixel = raster_[y1*columns_ + x1];
			    rtab[count] = (pixel >> 16) & 0xff;
			    gtab[count] = (pixel >> 8) & 0xff;
			    btab[count] = (pixel) & 0xff;
			    count++;
			  }
		      }
		  }
	      }
	    
	    for (int d = count/2; d > 0; d=d/2)
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

	    c2 = count/2;
	    
	    a = (raster_[y*columns_ + x] >> 24) & 0xff;
	    r = ( (count % 2 == 0) ? (rtab[c2] + rtab[c2 - 1])/2 : rtab[c2]);
	    g = ( (count % 2 == 0) ? (gtab[c2] + gtab[c2 - 1])/2 : gtab[c2]);
	    b = ( (count % 2 == 0) ? (btab[c2] + btab[c2 - 1])/2 : btab[c2]);

	    pixels[x] = (a << 24) | (r << 16) | (g << 8) | b;
	  }

	consumer.setPixels(0, y, columns_, 1, defaultRGB_, pixels, 0, columns_);	
      }
    
    consumer.imageComplete(status);
  }

}
