/**
 * Title:        ResourcePool
 * Description:  An abstraction of a collection of resources that can be
 *               reserved and released by real-time components
 */
package EAC;

public class ResourcePool {

  // percentage of resources reserved
  protected int reserved;

  public ResourcePool()
  {
    reserved = 0;
  } /* default constructor */

  public ResourcePool(int r) {
    reserved = r;
  } /* constructor */

  public int usage() {
    return reserved;
  } /* usage */

  public boolean reserve(int r) {
    if (reserved + r > 100)
      return false;
    else {
      //System.out.println("Reservation ok: " + java.lang.Integer.toString(r));
      reserved += r;
      return true;
    } /* else */
  } /* reserve */

  public boolean release(int r) {
    if (reserved - r < 0)
      return false;
    else {
      //System.out.println("Release ok: " + java.lang.Integer.toString(r));
      reserved -= r;
      return true;
    } /* else */
  } /* release */

}