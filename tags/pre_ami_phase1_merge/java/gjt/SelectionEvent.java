package gjt;

import java.awt.Event;

/**
 * An extension of java.awt.Event, specifically designed for
 * selection events.<p>
 *
 * SelectionEvents are constructed with arguments:<p>
 * <dl>
 * <dd> The AWT event that triggered the image button event
 * <dd> The id of the event.
 * </dl>
 *
 * An SelectionEvent's id (the constructor's 3rd argument),
 * must be one of the following:<p>
 * <dl>
 * <dd> SelectionEvent.SELECT
 * <dd> SelectionEvent.DESELECT
 * </dl>
 *
 * SelectionEvent has only a constructor and a paramString() 
 * method.  Containers that contain objects which are capable
 * of generating SelectionEvents should check the events
 * like so:
 * 
 * <pre>
 *      // handleEvent(Event) method of a container that
 *      // contain objects that generate SelectionEvents
 *
 *      public boolean handleEvent(Event event) {
 *              if(event instanceof SelectionEvent) {
 *                  SelectionEvent sevent = 
 *                  (SelectionEvent)event;
 *
 *                  if(sevent.isSelected()) {
 *                      // do something for selection
 *                  }
 *                  else {
 *                      // do something for deselection
 *                  }
 *              }
 *      }
 * </pre>
 *
 * @version 1.0, Apr 1 1996
 * @author  David Geary
 * @see     LabelCanvas
 * @see     ImageButtonEvent
 * @see     gjt.test.LabelCanvasTest
 */
public class SelectionEvent extends Event {
    public static final int SELECT   = 1;
    public static final int DESELECT = 2;

    private int eventType;

    public SelectionEvent(Object target,
                          Event  event, 
                          int    type) {
        super(target, event.when, event.id, event.x, event.y,
                      event.key,  event.modifiers, event.arg);

        Assert.notFalse(type == SELECT || type == DESELECT);

        eventType = type;
        id = -1;
    }
    public boolean isSelected() {
        return eventType == SELECT;
    }
    protected String paramString() {
        String typeString = new String();

        if(eventType == SelectionEvent.SELECT)    
            typeString = "SELECT";
        else if(eventType == SelectionEvent.DESELECT) 
            typeString = "DESELECT";

        return super.paramString() + typeString;
    }
}
