package gjt.test;

import java.net.URL;
import java.awt.*;
import java.applet.Applet;
import gjt.ExclusiveImageButtonPanel;
import gjt.ImageButton;
import gjt.ImageButtonEvent;
import gjt.Orientation;
import gjt.Toolbar;
import gjt.Separator;

/**
 * A Toolbar to the north, and an ExclusiveImageButtonPanel on 
 * the west give this little applet its own unique charm.  
 * Owner is motivated.<p>
 *
 * @version 1.0, April 25, 1996
 * @author  David Geary
 * @see     gjt.test.UnitTest
 * @see     gjt.ExclusiveImageButtonPanel
 * @see     gjt.ImageButton
 * @see     gjt.Toolbar
 */
public class ToolbarTest extends UnitTest {
    public String title() { 
        return "Toolbar/ImageButtonPanel Test"; 
    }
    public Panel centerPanel() { 
        return new ToolbarTestPanel(this);      
    }
}

class ToolbarTestPanel extends Panel {
    ImageButton     newButton,   openButton, diskButton, 
                    printButton, cutButton,  copyButton,
                    pasteButton;

    public ToolbarTestPanel(Applet app) {
        setLayout(new BorderLayout());  
        add("North", makeToolbar(app, app.getCodeBase()));
        add("West",  makePalette(app, app.getCodeBase()));
    }
    public boolean handleEvent(Event event) {
        if(event instanceof ImageButtonEvent) {
            ImageButtonEvent ibevent = (ImageButtonEvent)event;
          
            if(ibevent.isActivated()) {
                if(event.target == newButton)
                    System.out.println("New Button Activated");
                if(event.target == openButton)
                    System.out.println("Open Button Activated");
                if(event.target == diskButton)
                    System.out.println("Disk Button Activated");
                if(event.target == printButton)
                    System.out.println("Print Button Activated");
                if(event.target == cutButton)
                    System.out.println("Cut Button Activated");
                if(event.target == copyButton)
                    System.out.println("Copy Button Activated");
                if(event.target == pasteButton)
                    System.out.println("Paste Button Activated");
            
                return true;
            }
        }

        return super.handleEvent(event);
    }
    private Toolbar makeToolbar(Applet app, URL cb) {
        Toolbar tb  = new Toolbar(10, 0);

        newButton   = tb.add(app.getImage(cb, "gifs/new.gif"));
        openButton  = tb.add(app.getImage(cb, "gifs/open.gif"));
        diskButton  = tb.add(app.getImage(cb, "gifs/disk.gif"));

        tb.addSpacer(newButton.preferredSize().width);

        printButton = tb.add(app.getImage(cb, "gifs/print.gif"));

        tb.addSpacer(newButton.preferredSize().width);

        cutButton   = tb.add(app.getImage(cb, "gifs/cut.gif"));
        copyButton  = tb.add(app.getImage(cb, "gifs/copy.gif"));
        pasteButton = tb.add(app.getImage(cb, "gifs/paste.gif"));

        return tb;
    }
    private Panel makePalette(Applet app, URL cb) {
        ExclusiveImageButtonPanel iconPalette;
        Panel                     iconPalettePanel = new Panel();

        iconPalette = new ExclusiveImageButtonPanel(
                              Orientation.VERTICAL,
                              Orientation.CENTER,
                              Orientation.TOP, 10);

        iconPalette.add(app.getImage(cb,"gifs/ballot_box.gif"));
        iconPalette.add(app.getImage(cb,"gifs/palette.gif"));
        iconPalette.add(app.getImage(cb,"gifs/light_bulb1.gif"));
        iconPalette.add(app.getImage(cb,"gifs/Dining.gif"));
        iconPalette.add(app.getImage(cb,"gifs/scissors.gif"));
        iconPalette.add(app.getImage(cb,"gifs/tricycle.gif"));

        iconPalettePanel = new Panel();
        iconPalettePanel.setLayout(new BorderLayout());
        iconPalettePanel.add      ("Center", iconPalette);
        iconPalettePanel.add      ("East",   new Separator());
        return iconPalettePanel;
    }
}
