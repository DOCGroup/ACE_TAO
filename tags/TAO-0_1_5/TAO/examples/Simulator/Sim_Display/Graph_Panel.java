// $Id$ 

import java.awt.*;
import java.awt.event.*;
import java.util.*;

class Graph_Panel
  extends Panel	  
  implements ComponentListener,
	     ContainerListener
{
  private final static int VPAD = 4, HPAD = 4,
    MIN_WIDTH = 100,
    SUG_WIDTH = 175;
  private final static Random RAND = new Random (System.currentTimeMillis());
  private static float hue_ = RAND.nextFloat();
  
  private Hashtable graphs_ = new Hashtable();
  
  public Graph_Panel()
    {
      addComponentListener (this);
      addContainerListener (this);
    }  
  
  public void addGraph(String name, int max)
    {
      int index = name.lastIndexOf('/');
      String short_name;      
      
      if (index != -1)
	short_name = name.substring(index+1);
      else
	short_name = name;
      
      Graph graph = constructGraph(short_name, max);
      graphs_.put(name, graph);
    }

  public void updateGraph(String name, float new_value)
    {
      Graph graph = (Graph)graphs_.get(name);

      if(graph != null)
	graph.addNewData(new_value);
    }

  public Graph getGraph(String name)
    {
      return (Graph)((Container)graphs_.get(name)).getComponent(0);
    }
  
  public void removeGraph(String name)
    {
      graphs_.remove(name);

      if (graphs_.size() > 0)
	layoutGraphs();
      else
	removeAll();
    }

  public int numGraphs()
    {
      return graphs_.size();
    }

  public void clear()
    {
      removeAll();
    }

  public void componentResized(ComponentEvent e)
    {
      System.out.println ("Component resized.");
      removeContainerListener (this);
      removeComponentListener (this);
      layoutGraphs ();
      addComponentListener (this);
      addContainerListener (this);
    }

  public void componentMoved(ComponentEvent e) {}
  public void componentShown(ComponentEvent e) {}
  public void componentHidden(ComponentEvent e) {}

  public void componentAdded(ContainerEvent e)
    {
      System.out.println ("component added.");
      removeContainerListener (this);
      removeComponentListener (this);
      layoutGraphs ();
      addComponentListener (this);
      addContainerListener (this);
    }
  
  public void componentRemoved(ContainerEvent e)
    {
      System.out.println ("component removed.");
    }
  
  private void layoutGraphs ()
    {
      Dimension current_size = getSize ();
      int total_area = current_size.width * current_size.height;
      int num_rows, graphs_per_row, min_width = MIN_WIDTH;
      int graph_area = graphs_.size () *
	(SUG_WIDTH + HPAD) *
	(SUG_WIDTH + VPAD);
      Enumeration graph_enum = graphs_.elements ();

      removeAll ();

      System.out.println ("Total area: " + total_area + " Graph area: " + graph_area);
      
      if ((total_area < graph_area) ||
	  (current_size.width < MIN_WIDTH) ||
	  (current_size.height < MIN_WIDTH))
	{
	  System.out.println ("adjusting graph size");
	  min_width = total_area / graphs_.size ();
	  min_width = (int) Math.round (Math.sqrt (min_width));

	  if (min_width > current_size.height)
      	    min_width = current_size.height;
	  if (min_width > current_size.width)
	    min_width = current_size.width;

	  // Ah, hopeless. Too narrow to draw graphs in this panel.
	  if (min_width < MIN_WIDTH)
	    {
	      System.err.println ("Graph_Panel.layoutGraphs: Giving up!");
	      return;
	    }
	}

      graphs_per_row = current_size.width / (min_width + HPAD);
      num_rows = (graphs_.size () / graphs_per_row) + 1;

      setLayout (new GridLayout (num_rows, 1, 0, VPAD));
      for (int i = 0; i < num_rows; i++)
	{
	  Panel row = new Panel ();
	  row.setLayout (new GridLayout (1, graphs_per_row, HPAD, 0));
	  
	  for (int j = 0;
	       j < graphs_per_row && graph_enum.hasMoreElements ();
	       j++)
	    {
	      Component graph = (Component) graph_enum.nextElement ();		  
	      row.add (graph);
	    }

	  add (row);
	}

      validate ();
    }
  
  private final static Graph constructGraph(String label, int max)
    {
      Graph graph = new Graph(label, max);
      
      float brightness = RAND.nextFloat();

      hue_ += .075;

      if (hue_ > 1.0)
	hue_ -= 1.0;
      
      if (brightness > 0.75)
	brightness -= 0.25;
      
      Color new_color = Color.getHSBColor(hue_, 1, brightness);
      
      graph.setBackground(new_color);
      graph.setForeground(Color.white);
      
      return graph;
    }
}
