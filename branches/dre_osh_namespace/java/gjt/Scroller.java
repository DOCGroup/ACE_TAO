package gjt;

import java.awt.*;

/**
 * Each Scroller contains a Panel (viewport) and two Scrollbars 
 * (horizontal and vertical).  Works in conjunction with a 
 * ScrollerLayout, that lays out the viewport and two 
 * scrollbars.<p>
 * 
 * Subclasses must override:<p>
 * <dl>
 * <dd> abstract public void      scrollTo(int x, int y)
 * <dd> abstract public Dimension getScrollAreaSize()
 * </dl>
 * 
 * @version 1.0, Apr 1 1996
 * @author  David Geary
 * @see     ComponentScroller
 * @see     ImageScroller
 * @see     ScrollerLayout
 * @see     gjt.test.ComponentScrollerTest
 * @see     gjt.test.ImageScrollerTest
 */
public abstract class Scroller extends Panel {
    protected Panel     viewport;
    protected Scrollbar hbar, vbar;

    abstract public void      scrollTo(int x, int y);
    abstract public Dimension getScrollAreaSize();

    public Scroller() {
        setLayout(new ScrollerLayout(this));
        add("Scroll", viewport = new Panel());
        add("East", vbar = new Scrollbar(Scrollbar.VERTICAL));
        add("South",hbar = new Scrollbar(Scrollbar.HORIZONTAL));
    }
    public Scrollbar getHorizontalScrollbar() {return hbar;    }
    public Scrollbar getVerticalScrollbar  () {return vbar;    }
    public Panel     getViewport           () {return viewport;}

    public boolean handleEvent(Event event) {
        boolean handledEvent;

        switch(event.id) {
            case Event.SCROLL_LINE_UP:   scrollLineUp(event); 
            break;
            case Event.SCROLL_LINE_DOWN: scrollLineDown(event); 
            break;
            case Event.SCROLL_PAGE_UP:   scrollPageUp  (event); 
            break;
            case Event.SCROLL_PAGE_DOWN: scrollPageDown(event); 
            break;
            case Event.SCROLL_ABSOLUTE:  scrollAbsolute(event); 
            break;
        }
        handledEvent = event.id == Event.SCROLL_LINE_UP   ||
                       event.id == Event.SCROLL_LINE_DOWN ||
                       event.id == Event.SCROLL_PAGE_UP   ||
                       event.id == Event.SCROLL_PAGE_DOWN ||
                       event.id == Event.SCROLL_ABSOLUTE;

        if(handledEvent) return true;
        else             return super.handleEvent(event);
    }
    public void paint (Graphics g) { scroll(); }
    public void update(Graphics g) { paint(g); }

    public void manageScrollbars() { 
        manageHorizontalScrollbar();
        manageVerticalScrollbar  ();
    }
    protected void manageHorizontalScrollbar() {
        Dimension size           = size();
        Dimension scrollAreaSize = getScrollAreaSize();

        if(vbar.isVisible())
            size.width -= vbar.size().width;

        if(scrollAreaSize.width > size.width) {
            if( ! hbar.isVisible())
                hbar.show();
        }
        else if(hbar.isVisible()) {
            hbar.hide();
            hbar.setValue(0);
            repaint();
        }
    }
    protected void manageVerticalScrollbar() {
        Dimension size           = size();
        Dimension scrollAreaSize = getScrollAreaSize();

        if(hbar.isVisible())
            size.height -= hbar.size().height;

        if(scrollAreaSize.height > size.height) {
            if( ! vbar.isVisible())
                vbar.show();
        }
        else if(vbar.isVisible()) {
            vbar.hide();
            vbar.setValue(0);
            repaint();
        }
    }
    public void setScrollbarValues() {
        if(hbar.isVisible()) setHorizontalScrollbarValues();
        if(vbar.isVisible()) setVerticalScrollbarValues();
    }
    protected void setHorizontalScrollbarValues() {
        Dimension vsize          = viewport.size();
        Dimension scrollAreaSize = getScrollAreaSize(); 
        int max = scrollAreaSize.width - vsize.width;

        hbar.setValues(hbar.getValue(), // value
                       vsize.width,     // amt visible/page
                       0,               // minimum
                       max);            // maximum

        setHorizontalLineAndPageIncrements();
    }
    protected void setVerticalScrollbarValues() {
        Dimension vsize          = viewport.size();
        Dimension scrollAreaSize = getScrollAreaSize(); 
        int max = scrollAreaSize.height - vsize.height;

        vbar.setValues(vbar.getValue(), // value
                       vsize.height,    // amt visible/page
                       0,               // minimum
                       max);            // maximum

        setVerticalLineAndPageIncrements();
    }
    protected void scrollLineUp  (Event event) { scroll(); }
    protected void scrollLineDown(Event event) { scroll(); }
    protected void scrollPageUp  (Event event) { scroll(); }
    protected void scrollPageDown(Event event) { scroll(); }
    protected void scrollAbsolute(Event event) { scroll(); }

    protected void setHorizontalLineAndPageIncrements() {
        Dimension size = getScrollAreaSize();
        hbar.setLineIncrement(size.width/10);
        hbar.setPageIncrement(size.width/5);
    }
    protected void setVerticalLineAndPageIncrements() {
        Dimension size = getScrollAreaSize();
        vbar.setLineIncrement(size.height/10);
        vbar.setPageIncrement(size.height/5);
    }
    protected void scroll() {
        scrollTo(hbar.getValue(), vbar.getValue());
    }
}
