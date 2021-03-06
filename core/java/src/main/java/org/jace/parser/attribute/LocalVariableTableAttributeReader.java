package org.jace.parser.attribute;

import java.io.IOException;
import java.io.InputStream;
import org.jace.parser.ConstantPool;

/**
 * Reads LocalVariableTableAttributes.
 *
 * @author Toby Reyelts
 */
public class LocalVariableTableAttributeReader implements AttributeReader
{
	/**
	 * Reads a LocalVariableTableAttribute from a class file InputStream.
	 *
	 * @return the LocalVariableTableAttribute
	 * @throws IOException if an I/O error occurs while reading the attribute
	 */
	@Override
	public Attribute readAttribute(InputStream input, int nameIndex, ConstantPool pool)
		throws IOException
	{
		return new LocalVariableTableAttribute(input, nameIndex, pool);
	}

	@Override
	public String getName()
	{
		return "LocalVariableTable";
	}
}
