package imaging.framework;

import java.awt.*;

class HierarchyAdapter extends Hierarchy
{
  public HierarchyAdapter(String name, boolean root)
  {
    super(name, root);
  }
  
  public HierarchyAdapter(String name, Icon icon, boolean root)
  {
    super(name, icon, root);
  }

  public HierarchyAdapter(String name)
  {
    super(name);
  }
  
  public HierarchyAdapter(String name, Icon icon)
  {
    super(name, icon);
  }

  public void paint(Graphics g)
    {
      super.paint(g);

      getParent().getParent().validate();
    }
}
