package imaging.framework;

import java.awt.*;

// Create a panel for all the buttons
class FilePanel extends Panel
{
  FilePanel (ImageApp parent)
  {
    this.parent_ = parent;

    this.setLayout (new GridLayout (2, 1));

    // First create all the buttons
    this.URLDialogButton_ = new URLDialogButton ("Open URL", "Download an image", this.parent_);
    this.saveButton_ = new SaveButton ("Save", "Upload an image", this.parent_);

    // Place the created buttons in the panel
    this.add (this.URLDialogButton_);
    this.add (this.saveButton_);

    // Disable the save button for now
    this.disableSaveButton ();
    this.resize (400, 400);
  }

  public void enableSaveButton ()
  {
    this.saveButton_.enable ();
  }

  public void disableSaveButton ()
  {
    this.saveButton_.disable ();
  }

  // All the created buttons
  private URLDialogButton URLDialogButton_;
  private SaveButton saveButton_;

  private ImageApp parent_;
}

// Create a panel for all the buttons
class ResetPanel extends Panel
{
  ResetPanel (ImageApp parent)
  {
    this.parent_ = parent;

    this.setLayout (new GridLayout (2, 1));

    // First create all the buttons
    this.reloadButton_ = new ReloadButton ("Reload Filters", "Reload all filters", this.parent_);
    this.resetButton_ = new ResetButton ("Reset", "Reset the image", this.parent_);


    // Place the created buttons in the panel
    this.add (this.resetButton_);
    this.add (this.reloadButton_);

    this.resize (400, 400);
  }

  // All the created buttons
  private ReloadButton reloadButton_;
  private ResetButton resetButton_;

  private ImageApp parent_;
}


class ZoomPanel extends Panel
{
  ZoomPanel (ImageApp parent)
  {
    this.parent_ = parent;

    this.setLayout (new GridLayout (2, 1));

    // First create the two zoom buttons
    this.zoomInButton_ = new ZoomInButton ("<< Zoom in", "Zoom into the image", this.parent_);
    this.zoomOutButton_ = new ZoomOutButton ("Zoom out >>", "Zoom out of the image", this.parent_);

    // Now add the buttons to the panel
    this.add (this.zoomInButton_);
    this.add (this.zoomOutButton_);

    this.resize (100, 100);
  }

  private ZoomInButton zoomInButton_;
  private ZoomOutButton zoomOutButton_;

  private ImageApp parent_;
}


class FilterPanel extends Panel
{
  FilterPanel (ImageApp parent)
  {
    this.parent_ = parent;

    this.setLayout (new GridLayout (2, 1));
    this.applyButton_ = new ApplyButton ("Apply", "Apply the selected filter", this.parent_);

    // Set the layout of the Choice Panel. Note that the Choice Panel
    // holds the choice button of filters.
    this.choicePanel_ = new ChoicePanel ("Select filter", this.parent_);

    this.add (this.applyButton_);
    this.add (this.choicePanel_);
    this.resize (200,200);
  }

  public Choice choice ()
  {
    return this.choicePanel_.choice ();
  }

  public void loadFilters ()
  {
    this.choicePanel_.loadFilters ();
  }

  private ChoicePanel choicePanel_;
  private ApplyButton applyButton_;
  private ImageApp parent_;
}

class HelpPanel extends Panel
{
  HelpPanel (ImageApp parent)
  {
    this.parent_ = parent;

    this.setLayout (new GridLayout (2, 1));

    this.aboutButton_ = new AboutButton ("About", "About the applet", this.parent_);
    this.helpButton_ = new HelpButton ("Help", "Help on how to use the applet", this.parent_);

    // Now add the buttons to the panel
    this.add (this.aboutButton_);
    this.add (this.helpButton_);

    this.resize (100, 100);
  }

  private AboutButton aboutButton_;
  private HelpButton helpButton_;

  private ImageApp parent_;
}
