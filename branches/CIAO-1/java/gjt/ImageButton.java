package gjt;

import java.awt.*;
import java.awt.image.FilteredImageSource;

import gjt.image.BleachImageFilter;

/**
 * An Image painted in a Canvas, bordered by a ThreeDRectangle.
 * <p>
 *
 * ImageButtons have two constructors, both of which take an 
 * Image.  The Image passed to the constructor must not be null;
 * this is enforced by an assertion.<p>
 *
 * Default border thickness is 2 pixels - thickness may be set 
 * at construction time only.<p>
 *
 * Event handling is delegated to an ImageButtonController.  By 
 * default, all ImageButtons are fitted with an instance of 
 * SpringyImageButtonController, however, 
 * setController(ImageButtonController) may be used to fit an
 * ImageButton with a different derivation of 
 * ImageButtonController after construction.<p>
 *
 * ImageButtons ensure that their Images are completely loaded 
 * before they are displayed.<p>
 * 
 * Drawn either raised or inset, current state may be queried 
 * via the isRaised() method.<p>
 *
 * disable() disables response to input and repaints the image 
 * with a bleached version.  enable() restores the original 
 * image and enables response to input.  The intensity of the 
 * bleaching effect may be controlled (for all ImageButtons) 
 * via the static setBleachPercent(int) method.<p>
 *
 * @version 1.0, Apr 1 1996
 * @author  David Geary
 * @see     ThreeDRectangle
 * @see     ImageButtonController
 * @see     ImageButtonEvent
 * @see     SpringyImageButtonController
 * @see     StickyImageButtonController
 * @see     BleachImageFilter
 * @see     gjt.test.ImageButtonTest
 */
public class ImageButton extends Canvas {
    private static BleachImageFilter _bleachFilter;
    private static int               _bleachPercent    = 50;
    private static int               _offset           = 1;
    private static int               _defaultThickness = 2;

    private ThreeDRectangle border  = new ThreeDRectangle(this);
    private boolean         isDisabled    = false;
    private Dimension       preferredSize = new Dimension(0,0);
    private int             thickness;
    private Image           image, disabledImage;
    private ImageButtonController controller;

    public static int setBleachPercent() { 
        return _bleachPercent; 
    }
    public static void getBleachPercent(int p) { 
        _bleachPercent = p;    
    }
    public ImageButton(Image image) {
        this(image, _defaultThickness, null);
    }
    public ImageButton(Image image, 
                       ImageButtonController controller) {
        this(image, _defaultThickness, controller);
    }
    public ImageButton(Image image, int thickness, 
                       ImageButtonController controller) {
        Assert.notNull(image);
        Assert.notFalse(thickness > 0);

        if(controller == null) 
            this.controller = 
                new SpringyImageButtonController(this);
        else                   
            this.controller = controller;

        border.setThickness(this.thickness = thickness);  
        setImage(image);
    }
    public void setImage(Image image) {
        Util.waitForImage(this, this.image = image);

        preferredSize.width  = image.getWidth (this) + 
                               (2*thickness);
        preferredSize.height = image.getHeight(this) + 
                               (2*thickness);
    }
    public Dimension minimumSize() { 
        return preferredSize; 
    }
    public Dimension preferredSize() { 
        return preferredSize;                
    }
    public boolean isRaised  () { return border.isRaised();    }
    public boolean isDisabled() { return isDisabled;           }

    public void enable() { 
        isDisabled = false; 
        repaint();
    }
    public void disable() { 
        isDisabled = true;  

        if(disabledImage == null) 
            createDisabledImage();

        repaint();       
    }
    public void resize(int w, int h) { 
        reshape(location().x, location().y, w, h);
    }
    public void reshape(int x, int y, int w, int h) { 
        super.reshape(x,y,w,h);
        border.resize(w,h);
    }
    public void paint(Graphics g) {
        if(isRaised()) paintRaised();
        else           paintInset ();
    }
    public void paintInset() {
        Point     upperLeft = findUpperLeft();
        Graphics  g         = getGraphics();
        Image     image     = isDisabled() ? 
                              disabledImage : this.image;
        Dimension size      = size();

        if(g != null) {
            border.clearInterior();
            g.drawImage(image, 
                        upperLeft.x + thickness + _offset, 
                        upperLeft.y + thickness + _offset,this);

            g.setColor(getBackground().darker());
            for(int i=0; i < _offset; ++i) {
                g.drawLine(thickness+i,thickness+i,
                           size.width-thickness-i,thickness+i);
                g.drawLine(thickness+i,thickness+i,
                           thickness+i,size.height-thickness-i);
            }
            border.paintInset();
        }
    }
    public void paintRaised() {
        Point    upperLeft = findUpperLeft();
        Graphics g         = getGraphics();
        Image    image     = isDisabled() ? 
                             disabledImage : this.image;

        if(g != null) {
            border.clearInterior();
            g.drawImage(image, upperLeft.x + thickness, 
                               upperLeft.y + thickness, this);
            border.paintRaised();
        }
    }
    public boolean isInside(int x, int y) {
      Dimension size = size();
      return x >= 0 && x < size.width && y >= 0 && 
             y < size.height;
    }
    public void setController(ImageButtonController controller){
        this.controller = controller;
    }
    public ImageButtonController getController() {
        return controller;
    }
    public boolean mouseDown(Event event, int x, int y) {
        if(isDisabled()) return false;
        else             return controller.mouseDown(event,x,y);
    }
    public boolean mouseUp(Event event, int x, int y) {
        if(isDisabled()) return false;
        else             return controller.mouseUp(event,x,y);
    }
    public boolean mouseDrag(Event event, int x, int y) {
        if(isDisabled()) return false;
        else             return controller.mouseDrag(event,x,y);
    }

    private void createDisabledImage() {
        if(_bleachFilter == null)
            _bleachFilter = 
                new BleachImageFilter(_bleachPercent);

        if(_bleachPercent != _bleachFilter.percent())
            _bleachFilter.percent(_bleachPercent);

        FilteredImageSource fis = 
            new FilteredImageSource(image.getSource(), 
                                    _bleachFilter);

        Util.waitForImage(this, disabledImage=createImage(fis));
    }
    private Point findUpperLeft() {
        Dimension size = size();
        return new Point((size.width/2) - 
                         (preferredSize.width/2), 
                         (size.height/2) - 
                         (preferredSize.height/2));
    }
}
