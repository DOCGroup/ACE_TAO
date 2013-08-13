// $Id$
package demo;

import Demo.BaseValue;
import Demo.BaseValueValueFactory;
import org.omg.CORBA_2_3.portable.InputStream;

import java.io.Serializable;

/**
 * Created with IntelliJ IDEA.
 * User: gaow
 * Date: 11/28/12
 * Time: 5:00 PM
 * To change this template use File | Settings | File Templates.
 */
public class BaseValueValueFactoryImpl implements BaseValueValueFactory {
  @Override
  public BaseValue create(String str) {
    BaseValueImpl foo = new BaseValueImpl();
    foo.str = str;
    return foo;
  }

  @Override
  public Serializable read_value(InputStream inputStream) {
    return inputStream.read_value(new BaseValueImpl());
  }
}
