package imaging.framework;

import java.awt.*;
import java.awt.image.*;
import gjt.Util;
import JACE.OS.*;

class ImageCanvas extends Canvas
{
  public static final double MAX_ZOOM = 4.0;
  public static final double MIN_ZOOM = 0.5;

  public int setImage (Image image)
  {
    int flags = 0;
    if (image != null)
      {
	// Load the image
	this.tracker_.addImage(image, 0);
	try { this.tracker_.waitForID(0); } catch(InterruptedException excp) {}

	// Check if the image was actually loaded. Note that we have
	// to wait for the potential image to finish loading before we
	// know if it is a valid image.
	flags = this.checkImage (image, this);
	if (ACE.BIT_ENABLED (flags, ImageObserver.ERROR))
	  return -1;

	// If we reached here, it means image was loaded successfully so cache it
	this.image_ = image;
	this.originalImage_ = this.image_;
	
	this.x_ = (this.size ().width - this.image_.getWidth (this))/2;
	this.y_ = (this.size ().height - this.image_.getHeight (this))/2;
	this.original_x_ = this.x_;
	this.original_y_ = this.y_;
	repaint ();
      }
    return 0;
  }

  public Image getImage ()
  {
    return this.image_;
  }

  public void paint (Graphics g)
  {
    this.setBackground (Color.white);
    if (this.image_ != null)
      g.drawImage(this.image_, 
		  this.x_, this.y_,
		  (int) (this.image_.getWidth (this) * this.zoom_),
		  (int) (this.image_.getHeight (this) * this.zoom_),
		  this);
  }

  public void applyFilter (ImageFilter filter)
  {
    if (this.image_ != null)
      {
	Image temp;
	if (filter == null)
	  {
	    temp = this.originalImage_;
	    this.x_ = this.original_x_;
	    this.y_ = this.original_y_;
	    this.zoom_ = 1.0;
	  }
	else
	  temp = this.ip_.processImage(this.image_, filter, this);
	
	this.tracker_.addImage(temp, 0);
	try { this.tracker_.waitForID(0); } catch(InterruptedException excp) {}
	
	this.image_ = temp;

	// Originally I needed to flush the pixel data for the image to be
	// drawn properly. When running the applet in appletviewer, the
	// image used to jump around, but running in a browser seems to be
	// ok.  
	//this.image_.flush();
	repaint ();
      }
  }

  public void zoomFactor (double zoom)
  {
    this.zoom_ *= zoom;
    if (this.zoom_ > ImageCanvas.MAX_ZOOM)
      this.zoom_ = ImageCanvas.MAX_ZOOM;
    else if (this.zoom_ < ImageCanvas.MIN_ZOOM)
      this.zoom_ = ImageCanvas.MIN_ZOOM;

    repaint ();
  }

  public boolean mouseDown (Event evt, int x, int y)
  {
    if (inBounds (x, y))
      {
	this.selected_ = true;
	this.last_x_ = x;
	this.last_y_ = y;
      }
    return true;
  }

  public boolean mouseUp (Event evt, int x, int y)
  {
    this.selected_ = false;
    return true;
  }

  public boolean mouseDrag (Event evt, int x, int y)
  {
    if (this.selected_)
      {
	this.x_ = x - (this.last_x_ - this.x_);
	this.y_ = y - (this.last_y_ - this.y_);
	this.last_x_ = x;
	this.last_y_ = y;
	repaint ();
      }
    return true;
  }

  public boolean mouseMove (Event evt, int x, int y)
  {
    if (this.image_ != null && inBounds (x, y))
      Util.getFrame (this).setCursor (Frame.HAND_CURSOR);
    else
      Util.getFrame (this).setCursor (Frame.DEFAULT_CURSOR);
    return true;	
  }

  public boolean mouseExit (Event evt, int x, int y)
  {
    Util.getFrame (this).setCursor (Frame.DEFAULT_CURSOR);
    return true;
  }

  // Check if  mouse is within the bounds of the image
  private boolean inBounds (int x, int y)
  {
    if (this.image_ == null)
      return false;
    else
      return (x >= this.x_) &&
	     (y >= this.y_) &&
             (x <= (this.x_ + this.zoom_ * this.image_.getWidth (this))) &&
             (y <= (this.y_ + this.zoom_ * this.image_.getHeight (this)));
  }

  private MediaTracker tracker_ = new MediaTracker(this);
  private Image image_, originalImage_;
  private int x_ = 0, y_ = 0;
  private int original_x_ = 0, original_y_ = 0;
  private int width_ = 0, height_ = 0;
  private ImageProcessor ip_ = new ImageProcessor ();
  private boolean selected_ = false;
  private int last_x_ = 0, last_y_ = 0;
  private double zoom_ = 1.0;

}

