package imaging.framework;

import java.io.*;
import java.awt.*;

public class StatusIndicator extends Frame
{
  public StatusIndicator (String title)
  {
    super (title);

    this.setLayout (new BorderLayout ());
    this.add ("Center", textCanvas_);
    //    this.add ("Center", new Button ("Hello"));
    this.add ("South", new Button ("Hello"));
    this.resize (300,200);
    this.show ();
  }

  public void update (int count)
  {
    textCanvas_.setCount (count);
  }

  CounterCanvas textCanvas_ = new CounterCanvas ();
}

class CounterCanvas extends Canvas
{
  public void paint (Graphics g)
  {
    g.clearRect (0, 0, this.size ().width, this.size ().height);
    g.setFont (new Font ("TimesRoman", Font.PLAIN, 18));
    this.setBackground (Color.white);
    g.drawString ("Generating GIF format: ", 20, 20);
    g.drawString ((new Integer (count_)).toString (), 200, 20);
  }

  public void setCount (int count)
  {
    count_ = count;
    repaint ();
  }

  int count_ = 0;
}

