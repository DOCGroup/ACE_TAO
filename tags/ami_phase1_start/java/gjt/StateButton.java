package gjt;

import java.awt.Image;

/**
 * An ImageButton that cycles through a series of images.  The 
 * image advances to the next image in the series every time 
 * the button is activated.<p>
 *
 * Note that the cycling is actually performed by the buttons' 
 * controller - a StateButtonController.<p>
 *
 * @version 1.0, Apr 1 1996
 * @author  David Geary
 * @see     ImageButton
 * @see     StateButtonController
 * @see     gjt.test.StateButtonTest
 */
public class StateButton extends ImageButton {
    private Image[] images;
    private int     state = 0;
    private int     numStates;

    public StateButton(Image[] images) {
        super(images[0]);

        this.images = images;
        numStates   = images.length;
        setController(new StateButtonController(this));
        waitForImages();
    }
    public Image nextImage() {
        if(state + 1 < numStates) state++;
        else                      state = 0;

        return images[state];
    }
    public int state() {
        return state;
    }
    private void waitForImages() {
        for(int i=0; i < images.length; ++i)
            Util.waitForImage(this, images[i]);
    }
}
