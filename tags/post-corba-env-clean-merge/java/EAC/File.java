/**
 *  Title:        File
 *  Description:  A file I/O class for the Event Analysis Configurator
 */
package EAC;

import java.io.RandomAccessFile;

public class File extends RandomAccessFile {

  public final int SOURCE = 1;
  public final int SINK = 2;
  public final int COMPONENT = 3;
  public final int CONNECTOR = 4;
  public final int UNION = 5;
  public final int INTERSECTION = 6;
  public final int LABEL = 7;

  protected Configuration config;

  protected EACPanel canvas;

  public File(String filename, String mode, Configuration c, EACPanel p)
    throws java.io.FileNotFoundException {
      super(filename,mode);
      config = c;
      canvas = p;
  } /* constructor */

  public void writeConfiguration() throws java.io.IOException {
    int i;

    // write number of primitives
    writeInt(config.getPrimitiveCount());

    // write primitives
    for (i = 0; i < config.getPrimitiveCount(); i++)
      config.getPrimitive(i).write(this);

  } /* writeConfiguration */

  public void readConfiguration() throws java.io.IOException, TooManyPrimitivesException {
    int i,
        num,
        label_length,  // for reading the number of chars in an EACLabel
        primitiveID;

    Primitive next_primitive = new Union(canvas);  // dummy initialization

    Connector next_connector;

    // read number of primitives
    num = readInt();

    for (i = 0; i < num; i++) {
      primitiveID = readInt();
      switch(primitiveID) {
        case SOURCE:
          next_primitive = new Source(canvas,config);
          next_primitive.read(this);
          i++; // Add one more to the loop counter 'cause we read our own label
          break;
        case SINK:
          next_primitive = new Sink(canvas);
          next_primitive.read(this);
          break;
        case COMPONENT:
          next_primitive = new EACComponent(canvas,config);
          next_primitive.read(this);
          i++; // Add one more to the loop counter 'cause we read our own label
          break;
        case CONNECTOR:
          next_connector = new Connector(canvas);
          next_primitive = next_connector; // so it can be added to the config
          next_connector.read(this);
          try {
            int connector_start_index = config.leftSideOverlaps(next_connector);
            int connector_end_index = config.rightSideOverlaps(next_connector);

            Primitive startP = config.getPrimitive(connector_start_index);
            Primitive endP = config.getPrimitive(connector_end_index);

            // reinstate connection
            startP.addOutput(next_connector);
            next_connector.setInput(startP);
            next_connector.setOutput(endP);
            endP.addInput(next_connector);

          } catch (ConnectionException ce) {
            // can't happen (right?)
          } /* try */
          break;
        case UNION:
          next_primitive = new Union(canvas);
          next_primitive.read(this);
          break;
        case INTERSECTION:
          next_primitive = new Intersection(canvas);
          next_primitive.read(this);
          break;
        case LABEL:
          label_length = readInt();
          next_primitive = new EACLabel(canvas,label_length);
          next_primitive.read(this);
          break;
      } /* switch */

      // add to configuration
      config.addPrimitive(next_primitive);

    } /* for */

    // draw the configuration
    config.draw();

  } /* readConfiguration */

}