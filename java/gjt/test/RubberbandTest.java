package gjt.test;

import java.awt.*;
import gjt.DrawingPanel;
import gjt.Separator;
import gjt.RowLayout;
import gjt.rubberband.*;

/**
 * A simple drawing applet that demonstrates the utility of 
 * the gjt.rubberband package.<p>
 *
 * Note that this unit test also serves as the unit test for
 * gjt.DrawingPanel.<p>
 *
 * @version 1.0, April 25, 1996
 * @author  David Geary
 * @see     gjt.test.UnitTest
 * @see     gjt.DrawingPanel
 * @see     gjt.rubberband.Rubberband
 * @see     gjt.rubberband.RubberbandLine
 * @see     gjt.rubberband.RubberbandRectangle
 * @see     gjt.rubberband.RubberbandEllipse
 * @see     gjt.rubberband.RubberbandPanel
 */
public class RubberbandTest extends UnitTest {
    public String title() { 
        return "Rubberband Test"; 
    }
    public Panel centerPanel() { 
        return new RubberbandTestPanel(); 
    }
}

class RubberbandTestPanel extends Panel {
    private DrawingPanel drawingPanel;
    private ChoicePanel  choicePanel;

    public RubberbandTestPanel() {
        drawingPanel = new DrawingPanel();
        choicePanel  = new ChoicePanel(drawingPanel);

        setLayout(new BorderLayout());
        add("North",  choicePanel);
        add("Center", drawingPanel);
    }
}

class ChoicePanel extends Panel {
    private DrawingPanel drawingPanel;
    private Color        color;
    private Checkbox     fillCheckbox = new Checkbox();

    public ChoicePanel(DrawingPanel drawingPanel) {
        Panel    choicePanel     = new Panel();
        Choice   geometricChoice = new Choice();
        Choice   colorChoice     = new Choice();

        this.drawingPanel = drawingPanel;
    
        geometricChoice.addItem("Lines");
        geometricChoice.addItem("Rectangles");
        geometricChoice.addItem("Ellipses");

        colorChoice.addItem("Black");
        colorChoice.addItem("Red");
        colorChoice.addItem("Blue");
        colorChoice.addItem("Gray");
        colorChoice.addItem("White");

        choicePanel.setLayout(new RowLayout(10));
        choicePanel.add(new Label("Shape:"));
        choicePanel.add(geometricChoice);
        choicePanel.add(new Label("Color:"));
        choicePanel.add(colorChoice);
        choicePanel.add(new Label("Fill:"));
        choicePanel.add(fillCheckbox);

        setLayout(new BorderLayout());
        add("Center", choicePanel);
        add("South",  new Separator());
    }
    public boolean action(Event event, Object what) {
        if(event.target instanceof Checkbox) {
            drawingPanel.setFill(fillCheckbox.getState());
        }
        else if(event.target instanceof Choice) {
            if(((String)what).equals("Lines")) {     
                fillCheckbox.setState(false);
                drawingPanel.drawLines();
            }
            else if(((String)what).equals("Rectangles")) {
                System.out.println("Rectangles");
                drawingPanel.drawRectangles();
            }
            else if(((String)what).equals("Ellipses"))   
                drawingPanel.drawEllipses  ();
            else if(((String)what).equals("Black")) 
                drawingPanel.setColor(Color.black);
            else if(((String)what).equals("Red"))   
                drawingPanel.setColor(Color.red);
            else if(((String)what).equals("Blue"))  
                drawingPanel.setColor(Color.blue);
            else if(((String)what).equals("Gray"))  
                drawingPanel.setColor(Color.gray);
            else if(((String)what).equals("White")) 
                drawingPanel.setColor(Color.white);
        }
        return true;
    }
    public Insets insets() { return new Insets(5,0,5,0); }
}
