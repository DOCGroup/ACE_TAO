import java.awt.*;
import java.awt.event.*;
import java.util.*;
import com.sun.java.swing.*;
import com.sun.java.swing.event.*;
import com.sun.java.swing.border.*;
import com.sun.java.swing.tree.*;
import com.sun.java.swing.table.*;
import com.sun.java.swing.border.*;
import Server_Discovery;
import Server_Discovery_Util;

public class Server_Discovery_Selection
  extends JInternalFrame
{
  JTree tree_;
  Server_Discovery sd_;
  JScrollPane table_scroller_ = new JScrollPane ();
  DefaultMutableTreeNode root_ = new DefaultMutableTreeNode ("Movies");
  String selected_movie_ = null, selected_server_ = null;
  Hashtable movie_map_ = new Hashtable ();
  
  public Server_Discovery_Selection (Server_Discovery sd)
    {
      super ("Movie Selection", true, true, true, true);
      this.sd_ = sd;

      tree_ = new JTree (this.root_);
      JScrollPane tree_scroller = new JScrollPane (this.tree_);
      JSplitPane split_pane =
        new JSplitPane (JSplitPane.HORIZONTAL_SPLIT, false,
                        tree_scroller, this.table_scroller_);

      TreeSelectionListener tsl = new TreeSelectionListener ()
        {
          public void valueChanged (TreeSelectionEvent e)
            {
              TreePath tree_path = e.getNewLeadSelectionPath ();

              if (tree_path.getPath ().length == 3)
                {
                  DefaultMutableTreeNode tree_node =
                    (DefaultMutableTreeNode) tree_path.getLastPathComponent ();
                  String host_name = (String) tree_node.getUserObject ();
                  DefaultMutableTreeNode parent_node =
                    (DefaultMutableTreeNode) tree_node.getParent ();
                  String movie_name = (String) parent_node.getUserObject ();

                  String key = host_name + movie_name;
                  TAO_VR.Movie movie = (TAO_VR.Movie) movie_map_.get (key);
                  generate_table (host_name, movie);
                }
            }
        };
        
      this.tree_.addTreeSelectionListener (tsl);
      tree_scroller.setBackground (this.tree_.getBackground ());

      JPanel content_pane = new JPanel ();
      JPanel button_pane = new JPanel ();
      ImageIcon mmedia = new ImageIcon ("select", "mmedia3.gif");
      JButton select_button = new JButton ("View This Selection", mmedia);
      button_pane.setBorder (new EtchedBorder ());
      button_pane.add (select_button);
      content_pane.setLayout (new BorderLayout ());
      content_pane.add ("Center", split_pane);
      content_pane.add ("South", button_pane);

      ActionListener al = new ActionListener ()
        {
          public void actionPerformed (ActionEvent e)
            {
              if (selected_server_ != null && selected_movie_ != null)
                {
                  Server_Discovery_Util.load_movie (selected_server_,
                                                    selected_movie_);
                }
            }
        };
      
      select_button.addActionListener (al);      
      
      this.setContentPane (content_pane);
      this.setSize (350, 400);
      this.setLocation (15, 15);
    }

  public void add_movie (String host_name, TAO_VR.Movie movie)
    {
      // @ TODO: Order the movies alphabetically by name.
      DefaultMutableTreeNode category =
        new DefaultMutableTreeNode (movie.category_);
      DefaultMutableTreeNode movie_name =
        new DefaultMutableTreeNode (movie.name_);
      DefaultMutableTreeNode server_name =
        new DefaultMutableTreeNode (host_name);

      Object[] path = { category, movie_name, server_name };
      TreePath tree_path = new TreePath (path);

      String key = host_name + movie.name_;
      this.movie_map_.put (key, movie);
      this.tree_.addSelectionPath (tree_path);      
    }

  public void flush ()
    {
    }

  private void generate_table (String host_name, TAO_VR.Movie movie)
    {
      // The user object here is going to be a JTable instead of a
      // simply a string. The table will contain values for each of
      // the fields of the TAO_VR.Movie struct, where the description
      // URL will be hightlighted as a link. Clicking on that link in
      // the table will call up a JEditorPane with the html file in
      // it. In addition, there will be a button in one of table
      // entries for viewing performance numbers.
      DefaultTableModel table_model = new DefaultTableModel (10, 2);

      // Set the column values:
      table_model.setValueAt ("Name", 0, 0);
      table_model.setValueAt ("File Name", 1, 0);
      table_model.setValueAt ("Category", 2, 0);
      table_model.setValueAt ("Description", 3, 0);
      table_model.setValueAt ("Format", 4, 0);
      table_model.setValueAt ("File Size", 5, 0);
      table_model.setValueAt ("Duration", 6, 0);
      table_model.setValueAt ("Frame Rate", 7, 0);
      table_model.setValueAt ("Frame Size", 8, 0);
      table_model.setValueAt ("Server Performance", 9, 0);

      table_model.setValueAt (movie.name_, 0, 1);
      table_model.setValueAt (movie.filename_, 1, 1);
      table_model.setValueAt (movie.category_, 2, 1);
      table_model.setValueAt ("0", 4, 1);
      table_model.setValueAt ("MPEG", 5, 1);
      table_model.setValueAt ("0", 6, 1);
      table_model.setValueAt ("0", 7, 1);
      table_model.setValueAt ("0", 8, 1);

      // Special cases:
      Font label_font = new Font ("Dialog", Font.BOLD, 12);
      ImageIcon link_icon = new ImageIcon ("link", "www1.gif");
      JLabel url_label = new JLabel (movie.description_, link_icon, JLabel.RIGHT);
      url_label.setFont (label_font);
      url_label.setForeground (Color.blue);
      url_label.addMouseListener (new Clicked_URL (movie.description_));      
      table_model.setValueAt (url_label, 3, 1);

      ImageIcon graph_icon = new ImageIcon ("graph", "graph02.gif");
      JLabel graph_label = new JLabel ("Display server performance", graph_icon, JLabel.RIGHT); 
      graph_label.setFont (label_font);
      graph_label.setForeground (Color.blue);
      graph_label.addMouseListener (new Clicked_Graph (movie.name_, host_name));
      table_model.setValueAt (graph_label, 9, 1);

      JTable movie_table = new JTable (table_model);
      DefaultMutableTreeNode table_node = new DefaultMutableTreeNode (movie_table, false);

      JViewport viewport = new JViewport ();
      viewport.setView (movie_table);
      this.table_scroller_.setViewport (viewport);
      this.selected_movie_ = movie.name_;
      this.selected_server_ = host_name;
    }

  class Clicked_URL extends MouseAdapter
  {
    private String url_;

    public Clicked_URL (String url)
      {
        this.url_ = url;
      }

    public void mouseClicked (MouseEvent e)
      {
        sd_.load_page (this.url_);
      }    
  }

  class Clicked_Graph extends MouseAdapter
  {
    private String movie_, host_;

    public Clicked_Graph (String movie, String host)
      {
        this.movie_ = movie;
        this.host_ = host;
      }

    public void mouseClicked (MouseEvent e)
      {
        sd_.display_performance (this.movie_, this.host_);
      }
  }
}
  
