package imaging.framework;

import java.io.*;
import java.awt.*;
import JACE.OS.*;

public class ImageByteCounter
{
  public ImageByteCounter (String title, Image image)
  {
    this.image_ = image;
  }

  public int count ()
  {
    indicator_ = new StatusIndicator ("");
    int length = 0;
    try
      {
	//	GIFOutputStream ostream = new GIFOutputStream (statusIndicator_);
	GIFOutputStream ostream = new GIFOutputStream (null);
	GifEncoder encoder = new GifEncoder (this.image_, ostream);
	encoder.encode ();

	length = ostream.count ();
	System.out.println ("send: " + length);
      }
    catch (IOException e)
      {
	ACE.ERROR ("Exception generating gif");
      }
    //    indicator_.dispose ();
    return length;
  }

  Image image_ = null;
  StatusIndicator indicator_ = null;
}

class GIFOutputStream extends OutputStream
{
  public GIFOutputStream (StatusIndicator indicator)
  {
    super ();
    this.indicator_ = indicator;
  }

  public synchronized void write (int b) throws IOException 
    {
      bytesWritten_++;
      if (this.indicator_ != null)
	this.indicator_.update (bytesWritten_);
    }

  public synchronized void write (byte buf[]) throws IOException 
    {
      bytesWritten_ += buf.length;
      if (this.indicator_ != null)
	this.indicator_.update (bytesWritten_);
    }

  public synchronized void write (byte buf[], int offset, int length) throws IOException 
    {
      bytesWritten_ += length;
      if (this.indicator_ != null)
	this.indicator_.update (bytesWritten_);
    }

  public int count ()
    {
      return this.bytesWritten_;
    }

  private int bytesWritten_ = 0;
  private StatusIndicator indicator_ = null;
}
