/**
 * Title:        InputMode
 * Description:  Maintains Configurator state for deciding how to handle input
 */
package EAC;

public class InputMode {

  public final int COMMAND = 0;
  public final int SELECT = 1;
  public final int CUT = 2;
  public final int COPY = 3;
  public final int PASTE = 4;
  public final int EDIT = 5;
  public final int RESIZE = 6;

  protected int mode;

  protected Primitive current_shape;

  public InputMode() {
    mode = COMMAND;
  } /* constructor */

  public int getMode() {
    return mode;
  } /* getMode */

  protected void setMode(int m) {
    mode = m;
  } /* setMode */
}