package imaging.framework;

import java.awt.*;
import java.awt.image.*;
import java.util.Vector;

public class ImageProcessor
{
  private Vector pipeline_ = new Vector();

  
  public ImageProcessor()
  {
  }

  public void addFilter(String filter_name, ImageFilter filter)
  {
    pipeline_.addElement(new FilterNode(filter_name, filter));
  }

  public boolean removeFilter(String name)
  {
    FilterNode filter_node;
    
    for (int i = 0; i < pipeline_.size(); i++)
      {
	filter_node =  (FilterNode)pipeline_.elementAt(i);
	if (name.equals(filter_node.getName()))
	  {
	    pipeline_.removeElementAt(i);
	    pipeline_.trimToSize();
	    return true;
	  }
      }

    return false;
  }

  public Image processImage(Image image, ImageFilter filter, Component component)
  {
    Image old_image = image, new_image = old_image;
    MediaTracker tracker = new MediaTracker(component);

    try
      {
	new_image = component.createImage(new FilteredImageSource(old_image.getSource(), filter));
	tracker.addImage(new_image, 0);
	tracker.waitForID(0);
      }
    catch(InterruptedException excp)
      {
	System.out.println("ImageProcessor::processImage Image failed to load.");
	System.out.println(excp);
	return null;
      }

    return new_image;
  }
  
  public Image processImage(Image image, Component component)
  {
    Image old_image = image, new_image = null;
    ImageFilter image_filter;
    FilterNode filter_node;
    MediaTracker tracker = new MediaTracker(component);

    try
      {
	for (int i = 0; i < pipeline_.size(); i++)
	  {
	    filter_node = (FilterNode) pipeline_.elementAt(i);
	    image_filter = filter_node.getFilter();
	    new_image = component.createImage(new FilteredImageSource(old_image.getSource(), image_filter));
	    old_image = new_image;
	    tracker.addImage(new_image, 0);
	    tracker.waitForID(0);
	  }
      }
    catch(InterruptedException excp)
      {
	System.out.println("ImageProcessor::processImage Image failed to load.");
	System.out.println(excp);
	return null;
      }

    return new_image;
  }
}

  
class FilterNode
{
  private String name_;
  private ImageFilter filter_;

  FilterNode(String name, ImageFilter filter)
  {
    name_ = name;
    filter_ = filter;
  }

  String getName()
  {
    return name_;
  }

  ImageFilter getFilter()
  {
    return filter_;
  }
}
