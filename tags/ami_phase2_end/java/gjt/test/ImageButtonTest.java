package gjt.test;

import java.applet.Applet;
import java.awt.*;
import gjt.Box;
import gjt.ImageButton;
import gjt.ImageButtonEvent;
import gjt.SpringyImageButtonController;
import gjt.StickyImageButtonController;

/**
 * 2 ImageButtons, one springy and the other sticky, both 
 * crabby.<p>
 *
 * Both ImageButtons come with an awt.Button that is used to 
 * enable/disable the ImageButton it's associated with.<p>
 *
 * ImageButtonEvents, along with mouse enter and mouse exit
 * events for the two image buttons are printed out.<p>
 *
 * @version 1.0, Apr 25, 1996
 * @author  David Geary
 * @see     gjt.test.UnitTest
 * @see     gjt.ImageButton
 * @see     gjt.ImageButtonEvent
 * @see     gjt.SpringyImageButtonController
 * @see     gjt.StickyImageButtonController
 */
public class ImageButtonTest extends UnitTest {
    public String title() { 
        return "ImageButton Test"; 
    }
    public Panel centerPanel() { 
        return new ImageButtonTestPanel(this); 
    }
}

class ImageButtonTestPanel extends Panel {
    private ImageButton springyButton;
    private Button      springyButtonEnabler;
    private ImageButton stickyButton;
    private Button      stickyButtonEnabler;

    public ImageButtonTestPanel(Applet applet) {
        Image              image;
        Box                springyBox, stickyBox;
        GridBagLayout      gbl = new GridBagLayout();
        GridBagConstraints gbc = new GridBagConstraints();

        image = 
        applet.getImage(applet.getCodeBase(), "gifs/crab.gif");

        springyButton        = new ImageButton(image);
        springyButtonEnabler = new Button     ("Disable");
        stickyButton         = new ImageButton(image);
        stickyButtonEnabler  = new Button     ("Disable");

        stickyButton.setController(
			new StickyImageButtonController(stickyButton));

        setLayout(gbl);

        gbc.anchor    = GridBagConstraints.NORTH;
        springyBox    = new Box(springyButton, "Springy");
        gbc.insets    = new Insets(10,0,0,0);
        gbl.setConstraints(springyBox, gbc); add(springyBox);

        gbc.gridwidth = GridBagConstraints.REMAINDER;
        gbc.insets    = new Insets(45,10,0,0);
        gbl.setConstraints(springyButtonEnabler, gbc); 
        add(springyButtonEnabler);

        gbc.anchor    = GridBagConstraints.NORTH;
        gbc.gridwidth = 1;
        stickyBox     = new Box(stickyButton, "Sticky");
        gbc.insets    = new Insets(10,0,0,0);
        gbc.weighty   = 1.0;
        gbl.setConstraints(stickyBox, gbc); add(stickyBox);

        gbc.gridwidth = GridBagConstraints.REMAINDER;
        gbc.insets    = new Insets(45,10,0,0);
        gbl.setConstraints(stickyButtonEnabler, gbc); 
        add(stickyButtonEnabler);
    }
    public boolean action(Event event, Object what) {
        Button button = (Button)event.target;
        String label  = (String)what;

        if(button == stickyButtonEnabler) {
            if(label.equals("Disable")) stickyButton.disable();
            else                        stickyButton.enable();
        }
        else {
            if(label.equals("Disable")) springyButton.disable();
            else                        springyButton.enable();
        }
        if(label.equals("Disable")) button.setLabel("Enable");
        else                        button.setLabel("Disable");

        return true;
    }
    public boolean handleEvent(Event event) {
        boolean eventHandled = false;
        
        if(event instanceof ImageButtonEvent) {
            System.out.println("ImageButton " + event);
            eventHandled = true;
        }
        if(event.id == Event.MOUSE_ENTER) {
            if(event.target == stickyButton)
                System.out.println("Sticky Button Entered");

            else if(event.target == springyButton)
                System.out.println("Springy Button Entered");

            eventHandled = true;
        }
        if(event.id == Event.MOUSE_EXIT) {
            if(event.target == stickyButton)
                System.out.println("Sticky Button Exited");

            else if(event.target == springyButton)
                System.out.println("Springy Button Exited");

            eventHandled = true;
        }
        if(eventHandled) return true;
        else             return super.handleEvent(event);
    }
}
