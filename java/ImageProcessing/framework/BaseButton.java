package imaging.framework;

import java.awt.*;
import imaging.filters.*;

public class BaseButton extends Panel
{
  public BaseButton (String title, String description, ImageApp parent)
  {
    this.setLayout (new BorderLayout ());
    this.button_ = new Button (title);
    this.add ("Center", this.button_);
    this.resize (100, 100);
    this.description_ = description;
    this.parent_ = parent;
  }

  public boolean mouseEnter(Event evt, int x, int y)
  {
    this.parent_.displayStatus (this.description_);
    return true;
  }

  public boolean mouseExit(Event evt, int x, int y)
  {
    this.parent_.displayStatus ("");
    return true;
  }

  protected ImageApp parent_;
  private String description_;
  private Button button_;
}

class URLDialogButton extends BaseButton
{
  public URLDialogButton (String title, String desc, ImageApp parent)
  {
    super (title, desc, parent);
    this.openURLFrame_ = new URLFrame ("Open URL", this.parent_);
  }

  public boolean action (Event e, Object arg)
  {
    this.openURLFrame_.show ();
    return true;
  }
  private URLFrame openURLFrame_;
}

class SaveButton extends BaseButton
{
  public SaveButton (String title, String desc, ImageApp parent)
  {
    super (title, desc, parent);
  }

  public boolean action (Event e, Object arg)
  {
    this.parent_.saveFile ();
    return true;
  }
}

class ReloadButton extends BaseButton
{
  public ReloadButton (String title, String desc, ImageApp parent)
  {
    super (title, desc, parent);
  }

  public boolean action (Event e, Object arg)
  {
    this.parent_.reloadFilters ();
    return true;
  }
}

class ApplyButton extends BaseButton
{
  public ApplyButton (String title, String desc, ImageApp parent)
  {
    super (title, desc, parent);
  }

  public boolean action (Event e, Object arg)
  {
    this.parent_.apply ();
    return true;
  }
}

class ResetButton extends BaseButton
{
  public ResetButton (String title, String desc, ImageApp parent)
  {
    super (title, desc, parent);
  }

  public boolean action (Event e, Object arg)
  {
    this.parent_.resetImage ();
    return true;
  }
}

class ZoomInButton extends BaseButton
{
  public ZoomInButton (String title, String desc, ImageApp parent)
  {
    super (title, desc, parent);
  }

  public boolean action (Event e, Object arg)
  {
    this.parent_.zoomFactor (1.6);
    return true;
  }
}

class ZoomOutButton extends BaseButton
{
  public ZoomOutButton (String title, String desc, ImageApp parent)
  {
    super (title, desc, parent);
  }

  public boolean action (Event e, Object arg)
  {
    this.parent_.zoomFactor (0.625);
    return true;
  }
}


class AboutButton extends BaseButton
{
  public AboutButton (String title, String desc, ImageApp parent)
  {
    super (title, desc, parent);
  }

  public boolean action (Event e, Object arg)
  {
    DialogManager.popDialog (DialogType.ABOUT, null);
    return true;
  }
}

class HelpButton extends BaseButton
{
  public HelpButton (String title, String desc, ImageApp parent)
  {
    super (title, desc, parent);
  }

  public boolean action (Event e, Object arg)
  {
    System.out.println ("Help selected");
    return true;
  }
}

class ChoicePanel extends Panel
{
  public ChoicePanel (String desc, ImageApp parent)
  {
    this.description_ = desc;
    this.parent_ = parent;

    this.loadFilters ();
    // Set the layout of the Choice Panel. Note that the Choice Panel
    // holds the choice button of filters.
    this.setLayout (new FlowLayout ());
    this.resize (150, 100);
  }

  public void choice (Choice choice)
  {
    this.choice_ = choice;
  }

  public Choice choice ()
  {
    return this.choice_;
  }

  public void loadFilters ()
  {
    // First remove all components of the panel including the
    // choices of filters
    this.removeAll ();

    // Now create new choices
    this.choice_ = this.parent_.getFilters ();

    // Add the choices to our choice panel
    this.add (this.choice_);
  }

  public boolean mouseEnter(Event evt, int x, int y)
  {
    MedFilter filter = null;
    String displayString = null;
    String filterName = this.choice_.getSelectedItem ();

    if (filterName.compareTo ("None") == 0)
      displayString = "No filter selected";
    else
      {
	filter = (MedFilter) this.parent_.getFilter (filterName);
	displayString = filter.info ();
      }
    this.parent_.displayStatus (displayString);
    //    this.parent_.displayStatus (this.description_);
    return true;
  }

  public boolean mouseExit(Event evt, int x, int y)
  {
    this.parent_.displayStatus ("");
    return true;
  }
  
  private Choice choice_;
  private ImageApp parent_;
  String description_;
}

