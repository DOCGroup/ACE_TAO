package imaging.filters;

import java.awt.Color;
import java.awt.image.*;
import java.util.*;

public class SharpenFilter extends SpatialFilter
{
  float percentage_ = (float)75.0;
  
  public SharpenFilter()
    {
    }
  
  public SharpenFilter(float percentage)
    {
      percentage_ = percentage;
    }
  
  public String info ()
    {
      return "Sharpens an image.";
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

      System.gc();
      
      float[] hsv = new float[3];
      int rgb, red, green, blue;
      int pixel, alpha, lcv, lcv2, lcv3;
      int[] pixels = new int[columns_*rows_];
      float fact, ifact, hue, sat, val, vsum;
      float[] line0 = new float[columns_], 
	linep1 = new float[columns_],
	linem1 = new float[columns_],
	tmpptr;
      
      profile_timer_.start();
      
      for (int z = 0; z < SpatialFilter.iterations_; z++)
	{          
	  fact = percentage_/(float)100.0;
	  ifact = (float)1.0 - fact;
	  
	  for (int x = 0; x < columns_; x++)
	    {
	      pixel = raster_[x];
	      hsv = getHSV(((pixel >> 16) & 0xff), ((pixel >> 8) & 0xff), (pixel & 0xff),hsv);
	      line0[x] = hsv[2];
	    }
	  
	  lcv = columns_ << 1;
	  for (int x = columns_, index = 0; x < lcv; x++, index++)
	    {
	      pixel = raster_[x];
	      hsv = getHSV(((pixel >> 16) & 0xff), ((pixel >> 8) & 0xff), (pixel & 0xff),hsv);
	      linep1[index] = hsv[2];
	    }
	  
	  lcv = columns_ - 1;
	  lcv2 = rows_ - 1;
	  for (int y = 1, i = columns_; y < lcv2; y++)
	    {
	      tmpptr = linem1;
	      linem1 = line0;
	      line0 = linep1;
	      linep1 = tmpptr;
	      
	      lcv3 = columns_*(y+2);
	      for (int x = columns_*(y+1), index= 0; x < lcv; x++, index++)
		{
		  pixel = raster_[x];
		  // Note hsv is only instantiated once
		  hsv = getHSV(((pixel >> 16) & 0xff), ((pixel >> 8) & 0xff), (pixel & 0xff), hsv);
		  linep1[index] = hsv[2];			   
		}
	      
	      i++;
	      for (int x = 1; x < lcv; x++, i++)
		{
		  vsum = (float)0.0;
		  vsum = linem1[x-1] + linem1[x] + linem1[x+1] +
		    line0[x-1] + line0[x] + line0[x + 1] + 
		    linep1[x-1] + linep1[x] + linep1[x + 1];
		  
		  pixel = raster_[i];
		  alpha = (pixel >> 24) & 0xff;
		  hsv = getHSV(((pixel >> 16) & 0xff), ((pixel >> 8) & 0xff), (pixel & 0xff), hsv);
		  
		  val = ((hsv[2] - (fact * vsum) / 9) / ifact);	      
		  if (val < 1.0)
		    {
		      if (val < 0.0)
			val = (float)0.0;
		    }
		  else
		    val = (float)1.0;
		  
		  hsv[2] = val;
		  rgb = getRGB(hsv[0], hsv[1], hsv[2]);
		  
		  red = (rgb >> 16) & 0xff;
		  green = (rgb >> 8) & 0xff;
		  blue = rgb & 0xff;
		  
		  if (red < 0) red = 0;
		  if (green < 0) green = 0;
		  if (blue < 0) blue = 0 ;
		  
		  if (red > 255) red = 255;
		  if (green > 255) green = 255;
		  if (blue > 255) blue = 255;
		  
		  pixels[i] =
		    (alpha << 24) | (red << 16) | (green << 8) | blue;
		}
	      i++;
	    }
	}
      
      profile_timer_.stop();

      consumer.setPixels(0, 0, columns_, rows_, defaultRGB_, pixels, 0, columns_);
      
      System.out.println("Finished altering image");
      consumer.imageComplete(status);
    }
  
  
  static private float[] getHSV(int red, int green, int blue, float[] hsv)
    {
      float rd, gd, bd, max, min, del, rc, gc, bc, hue, sat;
      if (hsv == null)
	hsv = new float[3];
      
      /* convert RGB to HSV */ 
      rd = red / (float)255.0;            /* rd,gd,bd range 0-1 instead of 0-255 */
      gd = green / (float)255.0; 
      bd = blue / (float)255.0; 

      /* compute maximum of rd,gd,bd */
      if (rd >= gd)
	{
	  if (rd >= bd)
	    max = rd;
	  else
	    max = bd;
	}
      else
	{
	  if (gd >= bd)
	    max = gd;
	  else
	    max = bd;
	}
      
      /* compute minimum of rd,gd,bd */
      if (rd<=gd)
	{
	  if (rd<=bd)
	    min = rd;
	  else
	    min = bd;
	}
      else 
	{
	  if (gd<=bd)
	    min = gd;
	  else
	    min = bd;
	}

      del = max - min;
      hsv[2] = max;

      if (max != 0.0)
	sat = (del) / max;
      else
	sat = (float)0.0;
      
      hue = -1;
      
      if (sat != 0.0)
	{
	  rc = (max - rd) / del;
	  gc = (max - gd) / del;
	  bc = (max - bd) / del;

	  if (rd == max)
	    hue = bc - gc;
	  else
	    if (gd == max)
	      hue = 2 + rc - bc;
	    else
	      if (bd == max)
		hue = 4 + gc - rc;

	  hue *= 60;
	  if (hue<0)
	    hue += 360;
	}

      hsv[0] = hue;
      hsv[1] = sat;
      return hsv;
    }

  static private int getRGB(float hue, float sat, float val)
    {
      int    j, r, g, b;
      float rd, gd, bd;
      float f, p, q, t;
      
      /* convert HSV back to RGB */
      if (hue == -1 || sat == 0.0)
	{
	  rd = val;  gd = val;  bd = val;
	}
      else
	{
	  if (hue==(float)360.0) hue = (float)0.0;
	  hue = hue / (float)60.0;
	  j = (int) hue;
	  if (j<0) j=0;          /* either hue or floor seem to go neg on some sys */
	  f = hue - j;
	  p = val * (1-sat); // val - sat* val
	  q = val * (1 - (sat*f)); // val - val*sat*f
	  t = val * (1 - (sat*(1 - f)));
	  
	  switch (j) {
	  case 0:  rd = val;  gd = t;  bd = p;  break;
	  case 1:  rd = q;  gd = val;  bd = p;  break;
	  case 2:  rd = p;  gd = val;  bd = t;  break;
	  case 3:  rd = p;  gd = q;  bd = val;  break;
	  case 4:  rd = t;  gd = p;  bd = val;  break;
	  case 5:  rd = val;  gd = p;  bd = q;  break;
	  default: rd = val;  gd = t;  bd = p;  break;  /* never happen */
	  }
	}
      r = (int)(rd * 255.0 + 0.5);
      g = (int)(gd * 255.0 + 0.5);
      b = (int)(bd * 255.0 + 0.5);
      
      return 0xff000000 | (r << 16) | (g << 8) | b;
    }
}  















