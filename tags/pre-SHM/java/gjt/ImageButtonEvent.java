package gjt;

import java.awt.Event;

/**
 * An extension of java.awt.Event, specifically designed for
 * ImageButton events.<p>
 *
 * ImageButtonEvents are constructed with 3 arguments:
 * <dl>
 * <dd> ImageButton in which the event occurred
 * <dd> The AWT event that triggered the image button event
 * <dd> The id of the event.
 * </dl>
 *
 * An ImageButtonEvent's id (the constructor's 3rd argument),
 * must be one of the following:
 *
 * <dl>
 * <dd> ImageButtonEvent.ARM
 * <dd> ImageButtonEvent.DISARM
 * <dd> ImageButtonEvent.ACTIVATE
 * </dl>
 *
 * ImageButtonEvent has only a constructor and a paramString() 
 * method.  Containers that contain ImageButtons should check 
 * for ImageButtonEvents like so: <p>
 * 
 * <pre>
 *      // handleEvent(Event) method of a container that
 *      // contains ImageButtons.
 *
 *      public boolean handleEvent(Event event) {
 *              if(event instanceof ImageButtonEvent) {
 *                  ImageButtonEvent ibevent = 
 *                  (ImageButtonEvent)event;
 *
 *                  if(ibevent.isArmed()) {
 *                      // do something for arming
 *                  }
 *                  if(ibevent.isDisarmed()) {
 *                      // do something for disarming
 *                  }
 *                  if(ibevent.isActivated()) {
 *                      // do something for activation
 *                  }
 *              }
 *      }
 * </pre>
 *
 * ImageButtonController is the only GJT class that creates
 * ImageButtonEvents.
 * 
 * @version 1.0, Apr 1 1996
 * @author  David Geary
 * @see     ImageButton
 * @see     ImageButtonController
 * @see     SpringyImageButtonController
 * @see     StickyImageButtonController
 * @see     gjt.test.ImageButtonTest
 */
public class ImageButtonEvent extends Event {
    public static final int ARM      = 1;
    public static final int DISARM   = 2;
    public static final int ACTIVATE = 3;

    private int eventType;

    public ImageButtonEvent(ImageButton button,
                            Event       event, 
                            int         type) {
        super(button, event.when, event.id, event.x, event.y,
                      event.key,  event.modifiers, event.arg);

        Assert.notFalse(type == ARM    ||
                        type == DISARM ||
                        type == ACTIVATE);

        eventType = type;
        id = -1;
    }
    public boolean isArmed() {
        return eventType == ARM;
    }
    public boolean isDisarmed() {
        return eventType == DISARM;
    }
    public boolean isActivated() {
        return eventType == ACTIVATE;
    }
    protected String paramString() {
        String str = new String();

        if(eventType == ImageButtonEvent.ARM)         
            str = "ARM";
        else if(eventType == ImageButtonEvent.DISARM) 
            str = "DISARM";
        else if(eventType == ImageButtonEvent.ACTIVATE) 
            str = "ACTIVATE";

        return super.paramString() + str;
    }
}
