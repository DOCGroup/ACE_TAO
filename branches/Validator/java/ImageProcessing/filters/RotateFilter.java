package imaging.filters;

import java.awt.image.*;

public class RotateFilter extends SpatialFilter
{
  private double angle_;
  private int rotx_, roty_;
  private int rotheight_, rotwidth_;
  
  public RotateFilter ()
  {
    this.angle_ = 90.0;
  }

  public RotateFilter(double angle)
    {
      this.angle_ = angle;
    }

  public String info ()
    {
      return "Rotates an image";
    }

    public void setDimensions(int width, int height)
    {
      DoublePoint temp;

      rows_ = height;
      columns_ = width;
      
      // Convert the angle into radians
      double rotrad = angle_ * Math.PI / (double)180.0;
      
      // Compute the corner points after rotation
      double center_x = (double)(columns_ - 1) / (double)2.0;
      double center_y = (double)(rows_ - 1) / (double)2.0;
      
      // The new top left corner
      temp = rotatePoint(0, 0, center_x, center_y, rotrad);
      int rtl_x = (int)(temp.x_ + ((temp.x_ < 0) ? -0.5 : 0.5));
      int rtl_y = (int)(temp.y_ + ((temp.y_ < 0) ? -0.5 : 0.5));
      
      // the new top right corner
      temp = rotatePoint(0, rows_, center_x, center_y, rotrad);
      int rtr_x = (int)(temp.x_ + ((temp.x_ < 0) ? -0.5 : 0.5));
      int rtr_y = (int)(temp.y_ + ((temp.y_ < 0) ? -0.5 : 0.5));
      
      // the new bottom left corner
      temp = rotatePoint(columns_, 0, center_x, center_y, rotrad);
      int rbl_x = (int)(temp.x_ + ((temp.x_ < 0) ? -0.5 : 0.5));
      int rbl_y = (int)(temp.y_ + ((temp.y_ < 0) ? -0.5 : 0.5));
      
      // the new bottom right corner
      temp = rotatePoint(columns_, rows_, center_x, center_y, rotrad);
      int rbr_x = (int)(temp.x_ + ((temp.x_ < 0) ? -0.5 : 0.5));
      int rbr_y = (int)(temp.y_ + ((temp.y_ < 0) ? -0.5 : 0.5));

      //System.out.println(" ( " + center_x + "," + center_y + " ) ");
      //System.out.println(" ( " + rtl_x + "," + rtl_y + " ) ");
      //System.out.println(" ( " + rtr_x + "," + rtr_y + " ) ");
      //System.out.println(" ( " + rbr_x + "," + rbr_y + " ) ");
      //System.out.println(" ( " + rbl_x + "," + rbl_y + " ) ");       
      
      // rotated bounding box
      int rbbx1, rbby1;
      int rbbx = rbbx1 = rtl_x;
      int rbby = rbby1 = rtl_y;
      
      if (rtr_x < rbbx) rbbx = rtr_x;
      if (rtr_x > rbbx1) rbbx1 = rtr_x;
      if (rtr_y < rbby) rbby = rtr_y;
      if (rtr_y > rbby1) rbby1 = rtr_y;
      
      if (rbl_x < rbbx) rbbx = rbl_x;
      if (rbl_x > rbbx1) rbbx1 = rbl_x;
      if (rbl_y < rbby) rbby = rbl_y;
      if (rbl_y > rbby1) rbby1 = rbl_y;
      
      if (rbr_x < rbbx) rbbx = rbr_x;
      if (rbr_x > rbbx1) rbbx1 = rbr_x;
      if (rbr_y < rbby) rbby = rbr_y;
      if (rbr_y > rbby1) rbby1 = rbr_y;
      
      int rbbw = rbbx1 - rbbx;
      int rbbh = rbby1 - rbby;

      //System.out.println("(rbbx, rbby): " + rbbx + " " + rbby);
      //System.out.println("(rbbx1, rbby1): " + rbbx1 + " " + rbby1);
      //System.out.println("(rbbw, rbbh): " + rbbw + " " + rbbh);

      
      //rbbx--; rbby--; rbbw+=2; rbbh+=2;

      
      // Ensure we haven't increased the size of the image

      /*
      int x = rbbx + rbbw - 1, y = rbby + rbbh - 1;
      if (rbbx < 0) rbbx = 0;
      if (rbbx > columns_ - 1 ) rbbx = columns_ - 1;
      if (rbby < 0) rbby = 0;
      if (rbby > rows_ - 1) rbby = rows_ - 1;
      */
      
      /*
      
      if (x < 0) x = 0;
      if (x > columns_ - 1) x = columns_ - 1;
      if (y < 0) y = 0;
      if (y > rows_ - 1) y = rows_ - 1;

      if (x < rbbx) x = rbbx;
      if (y < rbby) y = rbby;
      rbbw = (x - rbbx) + 1;
      rbbh = (y - rbby) + 1;

      */
      
      rotx_ = rbbx;
      roty_ = rbby;
      rotheight_ = rbbh;
      rotwidth_ = rbbw;

      //System.out.println("(rotx,roty): " + rotx_ + " " +
      //		 roty_);
      //System.out.println("(rotwidth,rotheight): " + rotwidth_ + " " +
      //		 rotheight_);
      //System.out.println("(columns,rows): " + columns_ +
      //		 " " + rows_);
	    
      
      raster_ = new int[columns_*rows_];
      consumer.setDimensions(rotwidth_, rotheight_);
    }

  
  public void imageComplete(int status)
  {
    if (status == IMAGEERROR || status == IMAGEABORTED)
      {
	consumer.imageComplete(status);
	System.out.println("Image Error: " + status);
	return;
      }

    // For each pixel in the dimensions of the rotated image, if the
    // inverse rotation falls in the bounds of the original image. If
    // it does, compute and store an appropriate color, otherwise skip
    // it.

    System.gc();
    
    double xf, yf, px, py, apx, apy;
    int[] pixels = new int[rotwidth_*rotheight_];
    int ox, oy, ox1, oy1, index, pixel;
    double cx = (columns_ - 1) / 2;
    double cy = (rows_ - 1) / 2;
    double rotrad = angle_ * Math.PI / 180.0, ang, d;
    int p0r = 0, p0g = 0, p0b = 0,
      p1r = 0, p1g = 0,p1b = 0,
      p2r = 0, p2g = 0, p2b = 0,
      p3r = 0, p3g = 0, p3b = 0, lcv, lcv2;
    int rv,gv,bv, alpha;
    double rd,gd,bd, p0wgt = 0,
      p1wgt = 0, p2wgt = 0, p3wgt = 0, xfrac, yfrac;

    profile_timer_.start();

    lcv = roty_ + rotheight_;
    lcv2 = rotx_ + rotwidth_;
    for (int y = roty_, i = 0; y < lcv; y++)
      {
	for (int x = rotx_; x < lcv2; x++, i++)
	  {
	    // Inverse rotate the point (x,y)
	    // Inlining the call to rotatePoint
	    xf = (double)x;
	    yf = (double)y;
	    d = Math.sqrt((xf - cx) * (xf - cx) + (yf - cy) * (yf - cy));
	    
	    if ((xf - cx) != 0.0)
	      {
		ang = Math.atan((cy-yf)/(xf-cx));
		if ((xf - cx) < 0)
		  ang += Math.PI;
	      }
	    else
	      {
		if ((yf - cy) > 0.0)
		  ang = (Math.PI * 3.0) / 2;
		else
		  ang = Math.PI / 2;
	      }

	    xf = cx + (d * Math.cos(ang - rotrad));
	    yf = cy - (d * Math.sin(ang - rotrad));
	    // end inline
	    
	    // Cheat a little
	    if (xf < 0.0 && xf > -0.5) xf = 0.0;
	    if (yf < 0.0 && yf > -0.5) yf = 0.0;

	    ox = (int)Math.floor(xf);
	    oy = (int)Math.floor(yf);

	    if ((ox >= 0) && (oy >= 0) &&
		(ox < columns_) && (oy < rows_) )
	      {
		// The color will be a linear combination of the colors of
		// the center pixel, its left or right neighbor, its top
		// or bottom neighbor, and its corner neighbor. Which
		// neighbors are used is determined by the position of
		// the fractional part of xf, xy within the 1-unit square
		// of the pixel.

		/* compute px,py: fractional offset from center of pixel (x.5,y.5) */
		xfrac = xf - ox;        /* 0 - .9999 */
		yfrac = yf - oy;
		px = ((xfrac >= .5) ? (xfrac - .5) : (-.5 + xfrac));
		py = ((yfrac >= .5) ? (yfrac - .5) : (-.5 + yfrac));
		apx = ((px < 0) ? -px : px);
		apy = ((py < 0) ? -py : py);
		
		/* get neighbor colors:  p0col, p1col, p2col, p3col */
		ox1 = ox + ((px < 0.0) ? -1 : 1);
		oy1 = oy + ((py < 0.0) ? -1 : 1);

		index = oy * columns_ + ox;
		pixel = raster_[index];
		alpha = (pixel >> 24) & 0xff;
		p0r = (pixel >> 16) & 0xff;
		p0g = (pixel >> 8) & 0xff;
		p0b = pixel & 0xff;

		if (ox1 >= 0 && ox1 < columns_)
		  {
		    index = oy*columns_ + ox1;
		    pixel = raster_[index];
		    p1r = (pixel >> 16) & 0xff;
		    p1g = (pixel >> 8) & 0xff;
		    p1b = pixel & 0xff;
		    p1wgt = apx * (1.0 - apy);
		  }
		else { p1r=p1g=p1b=0;  p1wgt = 0.0; }
		
		if (oy1 >= 0 && oy1 < rows_)
		  {
		    index = oy1*columns_ + ox;
		    pixel = raster_[index];
		    p2r = (pixel >> 16) & 0xff;
		    p2g = (pixel >> 8) & 0xff;
		    p2b = pixel & 0xff;
		    p2wgt = apx * (1.0 - apy);
		  }
		else { p2r=p2g=p2b=0;  p2wgt = 0.0; }
		
		if (ox1 >= 0 &&  ox1 < columns_ &&
		    oy1 >= 0 && oy1 < rows_)
		  {
		    index = oy1 * columns_ + ox1;
		    pixel = raster_[index];
		    p3r = (pixel >> 16) & 0xff;
		    p3g = (pixel >> 8) & 0xff;
		    p3b = pixel & 0xff;
		    p3wgt = apx * (1.0 - apy);
		  }
		else { p3r=p3g=p3b=0;  p3wgt = 0.0; }

		p1wgt = p1wgt * .7;        /* black art */
		p2wgt = p2wgt * .7;
		p3wgt = p3wgt * .7;
		
		p0wgt = 1.0 - (p1wgt + p2wgt + p3wgt);
		
		/* okay, compute and store resulting color */
		rd = p0r * p0wgt + p1r * p1wgt + p2r * p2wgt + p3r * p3wgt;
		gd = p0g * p0wgt + p1g * p1wgt + p2g * p2wgt + p3g * p3wgt;
		bd = p0b * p0wgt + p1b * p1wgt + p2b * p2wgt + p3b * p3wgt;
		
		rv = (int) (rd + 0.5);
		gv = (int) (gd + 0.5);
		bv = (int) (bd + 0.5);
		
		if (rv < 0) rv = 0;
		if (gv < 0) gv = 0;
		if (bv < 0) bv = 0 ;
		
		if (rv > 255) rv = 255;
		if (gv > 255) gv = 255;
		if (bv > 255) bv = 255;

		pixels[i] = (alpha << 24) | (rv << 16) | (gv << 8) | bv;
	      }	       
	  }
      }

    profile_timer_.stop();
    
    consumer.setPixels(0, 0, rotwidth_, rotheight_, defaultRGB_,
		       pixels, 0, rotwidth_);

    consumer.imageComplete(status);
  }

  private final static DoublePoint rotatePoint(int x, int y,
				  double cx, double cy, double rad)
    {
      /* rotate point x, y 'rad' radians around cx, cy, return rx, ry */
      double d, xf, yf, ang, rx, ry;

      xf = (double)x;
      yf = (double)y;

      // d = distance from the point to the center
      d = Math.sqrt((xf - cx) * (xf - cx) + (yf - cy) * (yf - cy));

      if ((xf - cx) != 0.0)
	{
	  // Compute the angle between the axis and the point in radians
	  // using the inverse tangent function.
	  ang = Math.atan((cy-yf)/(xf-cx));

	  // If the x value falls below the axis the angle is between
	  // PI and 2*PI.
	  if ((xf - cx) < 0)
	    ang += Math.PI;
	}
      else
	{
	  // The point is on the positive y-axis so its angle is 3/2*PI
	  // I'm guessing this the cooridnate system is flipped in
	  // screen graphics.
	  if ((yf - cy) > 0.0)
	    ang = (Math.PI * 3.0) / 2;
	  else
	    ang = Math.PI / 2;
	}

      rx = cx + (d * Math.cos(ang + rad));
      ry = cy - (d * Math.sin(ang + rad));
      
      return new DoublePoint(rx, ry);
    }
}

class DoublePoint
{
  public double x_;
  public double y_;

  DoublePoint(double x, double y)
    {
      x_ = x;
      y_ = y;
    }
}
