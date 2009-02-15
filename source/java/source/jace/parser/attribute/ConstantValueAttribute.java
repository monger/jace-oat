package jace.parser.attribute;

import jace.parser.ConstantPool;
import jace.parser.constant.Constant;
import jace.parser.constant.UTF8Constant;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStream;

/**
 * A ConstantValueAttribute represents a constant value for a static ClassField.
 *
 * A ConstantValueAttribute may represent any constant type. 
 *
 * @author Toby Reyelts
 * @author Gili Tzabari
 */
public class ConstantValueAttribute implements Attribute {

  /* From the JVM specification.
   *
   * (u1 represents an unsigned byte)
   * (u2 represents an unsigned short)
   * (u4 represents an unsigned int)
   *
   * attribute_info {
   *   u2 attribute_name_index;
   *   u4 attribute_length;
   *   u2 constantvalue_index;
   * }
   */
  /**
   * Creates a new ConstantValueAttribute.
   *
   */
  public ConstantValueAttribute(InputStream stream, int nameIndex, ConstantPool pool) throws IOException {

    mPool = pool;
    mNameIndex = nameIndex;

    /* Read the name for this constant.
     * From the VM spec, we know it must be equal to "ConstantValue".
     */
    Constant c = mPool.getConstantAt(mNameIndex);

    if (c instanceof UTF8Constant) {

      String name = c.getValue().toString();

      if (!name.equals("ConstantValue")) {
        throw new ClassFormatError("While reading a ConstantValueAttribute, the name ConstantValue was expected, " +
          "but the name " + name + " was encountered.");
      }
    } else {
      throw new ClassFormatError("While reading a ConstantValueAttribute, a UTF8Constant was expected, " +
        "but a constant of type " + c.getClass().getName() + " was encountered.");
    }

    DataInputStream input = new DataInputStream(stream);

    /* Read the length of the attribute.
     * From the VM spec, we know that length must = 2.
     */
    mLength = input.readInt();

    if (mLength != 2) {
      throw new ClassFormatError("While reading a ConstantValueAttribute, an attribute length of size 2 was expected, " +
        "but an attribute length of size " + mLength + " was encountered.");
    }

    /*
     */
    mValueIndex = input.readUnsignedShort();
  }

  public String getName() {
    return mPool.getConstantAt(mNameIndex).toString();
  }

  /**
   * Returns the Constant that represents the value of this ConstantValueAttribute.
   *
   * Based on the type of the ClassField this ConstantValueAttribute belongs to,
   * the Constant may be one of the following:
   *
   *   long - LongConstant
   *   float - FloatConstant
   *   double - DoubleConstant
   *   int, short, char, byte, boolean - IntegerConstant
   *   String - StringConstant
   *
   * @return the Constant that represents the value of this ConstantValueAttribute
   */
  public Constant getValue() {
    return mPool.getConstantAt(mValueIndex);
  }

  /**
   * Returns the length of this Attribute.
   *
   * @return the length of this Attribute
   */
  public int getLength() {
    return mLength;
  }

  public void write(DataOutputStream output) throws IOException {
    output.writeShort(mNameIndex);
    output.writeInt(mLength);
    output.writeShort(mValueIndex);
  }

  @Override
  public String toString() {
    return getClass() + " " + getValue();
  }
  private int mNameIndex;
  private int mLength;
  private int mValueIndex;
  private ConstantPool mPool;
}
