import com.sun.java.swing.*;
import java.awt.*;
import java.net.URL;
import java.net.MalformedURLException;
import java.io.*;
import com.sun.java.swing.text.*;
import com.sun.java.swing.event.*;

public class Server_Discovery_Browser
  extends JInternalFrame
{
  public Server_Discovery_Browser (String initial_url)
    {
      super ("Movie Description", true, true, true, true);
      
      HtmlPane html = new HtmlPane (initial_url);
      this.setBounds (200, 25, 400, 400);
      this.getContentPane ().add (html);
    }

  public void pack()
    {
      Dimension size = getPreferredSize ();
      this.setSize (size.width, size.height);
    }
}

// This code was liberated from the jfc demo "Metalworks", which is
// included with the jdk 1.2 beta 3 distribution.
class HtmlPane
  extends JPanel
  implements HyperlinkListener
{
  JEditorPane html_;
  
  public HtmlPane (String initial_url)
    {
      this.setLayout( new BorderLayout() );

      try
        {
          URL url = new URL (initial_url);
          this.html_ = new JEditorPane (initial_url);
          this.html_.setEditable (false);
          this.html_.addHyperlinkListener (this);
        }
      catch (Exception excp)
        {
          System.err.println (excp);
        }
          
      JScrollPane scroller = new JScrollPane ();
      JViewport vp = scroller.getViewport ();
      vp.add (this.html_);
      this.add (scroller, BorderLayout.CENTER);
    }

    /**
     * Notification of a change relative to a 
     * hyperlink.
     */
    public void hyperlinkUpdate (HyperlinkEvent e)
    {
      if (e.getEventType () == HyperlinkEvent.EventType.ACTIVATED)
        {
          this.linkActivated (e.getURL ());
	}
    }

    /**
     * Follows the reference in an
     * link.  The given url is the requested reference.
     * By default this calls <a href="#setPage">setPage</a>,
     * and if an exception is thrown the original previous
     * document is restored and a beep sounded.  If an 
     * attempt was made to follow a link, but it represented
     * a malformed url, this method will be called with a
     * null argument.
     *
     * @param u the URL to follow
     */
    protected void linkActivated (URL u)
    {
      Cursor c = this.html_.getCursor ();
      Cursor waitCursor = Cursor.getPredefinedCursor (Cursor.WAIT_CURSOR);
      this.html_.setCursor (waitCursor);
      SwingUtilities.invokeLater (new PageLoader(u, c));
    }

    /**
     * temporary class that loads synchronously (although
     * later than the request so that a cursor change
     * can be done).
     */
    class PageLoader implements Runnable
    {      
      PageLoader (URL u, Cursor c)
        {
          this.url_ = u;
          this.cursor_ = c;
	}
      
      public void run ()
        {
          if (this.url_ == null)
            {
              // restore the original cursor
              html_.setCursor (this.cursor_);
              
              // PENDING(prinz) remove this hack when 
              // automatic validation is activated.
              Container parent = html_.getParent ();
              parent.repaint ();
            }
          else
            {
              Document doc = html_.getDocument ();
              try
                {
                  html_.setPage (this.url_);
                }
              catch (IOException ioe)
                {
                  html_.setDocument (doc);
                  getToolkit ().beep ();
                }
              finally
                {
                  // schedule the cursor to revert after
                  // the paint has happended.
                  this.url_ = null;
                  SwingUtilities.invokeLater (this);
		}
	    }
	}
      
      URL url_;
      Cursor cursor_;
    }
}

