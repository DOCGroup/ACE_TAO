package imaging.framework;

import java.io.*;
import java.awt.*;

public class StatusIndicator extends Frame
{
  public StatusIndicator (String title, Image image)
  {
    super (title);
    this.image_ = image;
    //    statusCanvas_ = new CounterCanvas (image);
    this.setLayout (new BorderLayout ());
    this.text_ = new TextField ();
    this.label_ = new TextField ("Generating");

    this.add ("North", this.label_);
    this.add ("Center", text_);
    //    this.add ("Center", statusCanvas_);
    //    this.add ("Center", new Button ("Hello"));
    //    this.add ("South", new Button ("Hello"));
    this.resize (300,200);
    this.show ();
  }

  public void update (int count)
  {
    //    this.label_.setText ("Generating...");
    this.text_.setText ((new Integer (count++)).toString ());
    //    statusCanvas_.update ();
    //    textCanvas_.setCount (count);
  }

  TextField text_;
  TextField label_;
  Image image_;
  //  CounterCanvas statusCanvas_ = null;
}

class CounterCanvas extends Canvas
{
  public CounterCanvas (Image image)
  {
    this.image_ = image;
  }

  public void paint (Graphics g)
  {
    g.clearRect (0, 0, this.size ().width, this.size ().height);
    this.setBackground (Color.white);

    if (this.image_ != null)
      g.drawImage(this.image_, 
		  40, 40,
		  this.image_.getWidth (this),
		  this.image_.getHeight (this),
		  this);

    g.setFont (new Font ("TimesRoman", Font.PLAIN, 18));
    g.drawString ("Generating GIF format: ", 120, 120);
  }

  public void update ()
  {
    repaint ();
  }

  Image image_ = null;
}

