package imaging.filters; 

import java.awt.image.*; 

public class OilPaintFilter extends SpatialFilter 
{
  private int degree_ = 3; 

  public OilPaintFilter() 
    {
    }

  public OilPaintFilter(int degree)
    {
      degree_ = degree;
    }

  public String info ()
  {
    return "Converts an image into an oil painting.";
  }

  public void imageComplete(int status)
  {
    if (status == IMAGEERROR || status == IMAGEABORTED)
      {
	consumer.imageComplete(status);
	System.out.println("Oil Paint FilterImage Error: " + status);
	return;
      }

    System.gc();
    
    int[] pixels = new int[columns_*rows_];
    int[] nnrect = new int[degree_*degree_];
    int offset = degree_/2, cnt = 0, maxcnt = 0, col = 0, tmp;
    int lcv1 = rows_,
      lcv2 = columns_,
      lcv3 = degree_*degree_;
    
    profile_timer_.start();
    
    for (int z = SpatialFilter.iterations_; z-- > 0;)
      {
	for (int y = 0, index = 0; y < lcv1; y++)
	  {
	    for (int x = 0; x < lcv2; x++, index++)
	      {
		cnt = 0;
		for (int i = y - offset; i < y + offset; i++)
		  {
		    tmp = i*columns_;
		    for (int j = x - offset; j < x + offset; j++, cnt++)
		      {
			if (i >= 0 && i < lcv1 && j >= 0 && j < lcv2)
			  nnrect[cnt] = raster_[tmp + j];
			else
			  nnrect[cnt] = -1;
		      }		    
		  }
		
		maxcnt = 0;
		col = 0;
		cnt = 0;
		for (int i = 0; i < lcv3; i++)
		  {
		    if (nnrect[i] != -1)
		      {
			cnt = 1;
			
			for (int j = i+1; j < lcv3; j++)
			  {
			    if (nnrect[i] == nnrect[j])
			      cnt++;
			  } 
			
			if (cnt > maxcnt)
			  {
			    col = nnrect[i];
			    maxcnt = cnt;
			  }
		      }
		  }
		
		pixels[index] = col;
	      }
	  }
      }

    System.err.println("Oil Paint should be finished.");
    
    profile_timer_.stop();
    
    consumer.setPixels(0, 0, columns_, rows_, defaultRGB_, pixels, 0, columns_);
    consumer.imageComplete(status);
  }
}
