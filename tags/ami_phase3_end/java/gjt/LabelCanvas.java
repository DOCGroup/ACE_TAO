package gjt;

import java.awt.*;

/**
 * A selectable label.  Clients can set the insets around the 
 * label via setInsets(Insets).
 *
 * LabelCanvases generate SelectionEvents when they are
 * selected or deselected.<p>
 *
 * @version 1.0, Apr 1 1996
 * @author  David Geary
 * @see     SelectionEvent
 * @see     gjt.test.LabelCanvasTest
 */
public class LabelCanvas extends Canvas {
    private String  label;
    private boolean selected = false;
    private Insets  insets   = new Insets(2,2,2,2);
    private Point   labelLoc = new Point(0,0);

    public LabelCanvas(String label) {
        this.label = label;
    }
    public void paint(Graphics g) {
        if(selected == true) paintSelected(g);
        else                 
            g.drawString(label, labelLoc.x, labelLoc.y);
    }
    public void setInsets(Insets insets) { 
        this.insets = insets;   
        repaint();
    }
    public String  getLabel  () { return label;                }
    public boolean isSelected() { return selected;             }
    public void    select    () { selected = true;  repaint(); }
    public void    deselect  () { selected = false; repaint(); }

    public void resize(int w, int h) { 
        reshape(location().x, location().y, w, h); 
    }
    public void reshape(int x, int y, int w, int h) {
        super.reshape(x, y, w, h);
        labelLoc = labelLocation(getGraphics());
    }
    public Dimension minimumSize() { 
        return preferredSize(); 
    }
    public Dimension preferredSize() {
        FontMetrics fm = getFontMetrics(getFont());
            return new Dimension(
                     insets.left + fm.stringWidth(label) + 
                     insets.right, 
                     insets.top  + fm.getHeight() + 
                     insets.bottom);
    }
    public boolean mouseDown(Event event, int x, int y) {
        if(selected) deselect();
        else         select  ();

        int eventType = isSelected() ? 
                        SelectionEvent.SELECT : 
                        SelectionEvent.DESELECT;

        Event newEvent = new SelectionEvent(this, 
                                            event, 
                                            eventType);
        deliverEvent(newEvent);

        return true;
    }
    protected void paintSelected(Graphics g) {
        Point labelLoc = labelLocation(g);

        g.setColor(getForeground());
        g.fillRect(0,0,size().width,size().height);
        g.setColor(getBackground());
        g.drawString(label, labelLoc.x, labelLoc.y);
    }
    protected String paramString() {
        return super.paramString() + ",text=" + label;
    }
    private Point labelLocation(Graphics g) {
        Dimension   size = size();
        FontMetrics fm   = g.getFontMetrics();

        int x = (size.width/2) - (fm.stringWidth(label)/2);
        int y = (size.height/2) + (fm.getAscent()/2) - 
                                   fm.getLeading();
        return new Point(x,y);
    }
}
