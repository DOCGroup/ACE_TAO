package imaging.filters;

import java.awt.image.*;
import java.util.*;

public class SharpenFilter extends SpatialFilter
{
  double percentage_ = 75.0;
  
  public SharpenFilter()
    {
    }
  
  public SharpenFilter(double percentage)
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
      
      HSV hsv;
      RGB rgb;
      int pixel, alpha;
      int[] pixels = new int[columns_];
      double fact, ifact, hue,sat,val, vsum;
      double[] line0 = new double[columns_], 
	linep1 = new double[columns_],
	linem1 = new double[columns_],
	tmpptr;

      fact = percentage_/100.0;
      ifact = 1.0 - fact;
      
      for (int x = 0; x < columns_; x++)
	{
	  pixel = raster_[x];
	  hsv = getHSV(((pixel >> 16) & 0xff), ((pixel >> 8) & 0xff), (pixel & 0xff));
	  line0[x] = hsv.val_;
	}
      
      for (int x = columns_, index = 0; x < 2*columns_; x++, index++)
	{
	  pixel = raster_[x];
	  hsv = getHSV(((pixel >> 16) & 0xff), ((pixel >> 8) & 0xff), (pixel & 0xff));
	  linep1[index] = hsv.val_;
	}

      for (int y = 1; y < rows_ - 1; y++)
	{
	  tmpptr = linem1;
	  linem1 = line0;
	  line0 = linep1;
	  linep1 = tmpptr;

	  for (int x = columns_*(y+1), index= 0; x < columns_*(y+2); x++, index++)
	    {
	      pixel = raster_[x];
	      hsv = getHSV(((pixel >> 16) & 0xff), ((pixel >> 8) & 0xff), (pixel & 0xff));
	      linep1[index] = hsv.val_;			   
	    }

	  for (int x = 1; x < columns_ - 1; x++)
	    {
	      vsum = 0.0;
	      vsum = linem1[x-1] + linem1[x] + linem1[x+1] +
		line0[x-1] + line0[x] + line0[x + 1] + 
		linep1[x-1] + linep1[x] + linep1[x + 1];

	      pixel = raster_[y*columns_ + x];
	      alpha = (pixel >> 24) & 0xff;
	      hsv = getHSV(((pixel >> 16) & 0xff), ((pixel >> 8) & 0xff), (pixel & 0xff));
	      hsv.val_ = ((hsv.val_ - (fact * vsum) / 9) / ifact);
	      if (hsv.val_ < 1.0)
		{
		  if (hsv.val_ < 0.0)
		    hsv.val_ = 0.0;
		}
	      else
		hsv.val_ = 1.0;

	      rgb = getRGB(hsv.hue_, hsv.sat_, hsv.val_);

	      if (rgb.red_ < 0) rgb.red_ = 0;
	      if (rgb.green_ < 0) rgb.green_ = 0;
	      if (rgb.blue_ < 0) rgb.blue_ = 0 ;
	      
	      if (rgb.red_ > 255) rgb.red_ = 255;
	      if (rgb.green_ > 255) rgb.green_ = 255;
	      if (rgb.blue_ > 255) rgb.blue_ = 255;

	      pixels[x] =
		(alpha << 24) | (rgb.red_ << 16) | (rgb.green_ << 8) | rgb.blue_;
	    }
	  
	  consumer.setPixels(0, y, columns_, 1, defaultRGB_, pixels, 0, columns_); 	  
	}
     
  
      System.out.println("Finished altering image");
      consumer.imageComplete(status);
    }
  
  
  static private HSV getHSV(int red, int green, int blue)
    {
      double rd, gd, bd, max, min, del, rc, gc, bc; 
      HSV hsv = new HSV(); 
      
      /* convert RGB to HSV */ 
      rd = red / 255.0;            /* rd,gd,bd range 0-1 instead of 0-255 */
      gd = green / 255.0; 
      bd = blue / 255.0; 

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
      hsv.val_ = max;

      if (max != 0.0)
	hsv.sat_ = (del) / max;
      else
	hsv.sat_ = 0.0;
      
      hsv.hue_ = -1;
      
      if (hsv.sat_ != 0.0)
	{
	  rc = (max - rd) / del;
	  gc = (max - gd) / del;
	  bc = (max - bd) / del;

	  if (rd == max)
	    hsv.hue_ = bc - gc;
	  else
	    if (gd == max)
	      hsv.hue_ = 2 + rc - bc;
	    else
	      if (bd == max)
		hsv.hue_ = 4 + gc - rc;

	  hsv.hue_ *= 60;
	  if (hsv.hue_<0)
	    hsv.hue_ += 360;
	}
      
      return hsv;
    }

  static private RGB getRGB(double hue, double sat, double val)
    {
      int    j;
      double rd, gd, bd;
      double f, p, q, t;
      RGB rgb = new RGB();
      
      /* convert HSV back to RGB */
      if (hue == -1 || sat == 0.0)
	{
	  rd = val;  gd = val;  bd = val;
	}
      else
	{
	  if (hue==360.0) hue = 0.0;
	  hue = hue / 60.0;
	  j = (int) Math.floor(hue);
	  if (j<0) j=0;          /* either hue or floor seem to go neg on some sys */
	  f = hue - j;
	  p = val * (1-sat);
	  q = val * (1 - (sat*f));
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
      
      rgb.red_ = (int) Math.floor((rd * 255.0) + 0.5);
      rgb.green_ = (int) Math.floor((gd * 255.0) + 0.5);
      rgb.blue_ = (int) Math.floor((bd * 255.0) + 0.5);

      
      return rgb;
    }
}  

class HSV
{
  public double hue_;
  public double sat_;
  public double val_;
}

class RGB
{
  public int red_;
  public int green_;
  public int blue_;
}













