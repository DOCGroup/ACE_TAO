package imaging.framework;

import java.awt.*;
import gjt.Separator;

class StatusDisplay extends Panel
{
  public StatusDisplay ()
  {
    this.setLayout (new BorderLayout ());
    this.textField_.setEditable (false);
    this.add ("North", new Separator ());
    this.add ("Center", this.textField_);
    this.add ("South", new Separator ());
  }

  public void setText (String s)
  {
    this.textField_.setText (s);
  }

  private TextField textField_ = new TextField ();
}
