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
	System.out.println("Image Error: " + status);
	return;
      }

    int[] pixels = new int[columns_];
    int[] nnrect = new int[degree_*degree_];
    int offset = degree_/2, cnt = 0, maxcnt = 0, col = 0;

    for (int y = offset; y < rows_ - offset; y++)
      {
	for (int x = offset; x < columns_ - offset; x++)
	  {
	    
	    for (int i = 0; i < degree_*degree_; i++)
	      nnrect[i] = raster_[(y + (i / degree_ - 1))*columns_ + (x + (i % degree_ - 1)) ];

	    maxcnt = 0;
	    col = 0;
	    for (int i = 0; i < degree_*degree_; i++)
	      {
		cnt = 1;

		for (int j = i+1; j < degree_*degree_; j++)
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

	    pixels[x] = col;
	  }

	consumer.setPixels(0, y, columns_, 1, defaultRGB_, pixels, 0, columns_);
	
      }
    
    consumer.imageComplete(status);
  }

}
