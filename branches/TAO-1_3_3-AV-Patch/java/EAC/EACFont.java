/**
 * Title:        EACFont
 * Description:  The font to be used for on-screen text
 */
package EAC;
import java.awt.*;

public class EACFont {

  // Use 9-point Courier New
  public final int FONT_SIZE = 10;
  protected Font eac_font = new Font("Courier New",Font.PLAIN,FONT_SIZE);

  public EACFont() {
  }

  public Font getFont() {
    return eac_font;
  } /* getFont */

}