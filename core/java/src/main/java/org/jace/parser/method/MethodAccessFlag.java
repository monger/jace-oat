package org.jace.parser.method;

import com.google.common.collect.Lists;
import java.util.Collection;

/**
 * Represents an access flag for a ClassMethod.
 *
 * @author Toby Reyelts
 *
 */
public class MethodAccessFlag
{
	// Declared public; may be accessed from outside its package
	public static final MethodAccessFlag PUBLIC = new MethodAccessFlag("public", 0x0001);
	// Declared private; accessible only within the defining class
	public static final MethodAccessFlag PRIVATE = new MethodAccessFlag("private", 0x0002);
	// Declared protected; may be accessed within subclasses
	public static final MethodAccessFlag PROTECTED = new MethodAccessFlag("protected", 0x0004);
	// Declared static
	public static final MethodAccessFlag STATIC = new MethodAccessFlag("static", 0x0008);
	// Declared final; may not be overridden.
	public static final MethodAccessFlag FINAL = new MethodAccessFlag("final", 0x0010);
	// Declared synchronized; invocation is wrapped in a monitor lock.
	public static final MethodAccessFlag SYNCHRONIZED = new MethodAccessFlag("synchronized", 0x0020);
	// A bridge method, generated by the compiler.
	public static final MethodAccessFlag BRIDGE = new MethodAccessFlag("bridge", 0x0040);
	// Declared with variable number of arguments.
	public static final MethodAccessFlag VARARGS = new MethodAccessFlag("varargs", 0x0080);
	// Declared native; implemented in a language other than Java.
	public static final MethodAccessFlag NATIVE = new MethodAccessFlag("native", 0x0100);
	// Declared abstract; no implementation is provided.
	public static final MethodAccessFlag ABSTRACT = new MethodAccessFlag("abstract", 0x0400);
	// Declared strictfp; floating-point mode is FP-strict
	public static final MethodAccessFlag STRICT = new MethodAccessFlag("strict", 0x0800);
	// Declared synthetic; Not present in the source code.
	public static final MethodAccessFlag SYNTHETIC = new MethodAccessFlag("synthetic", 0x1000);
	private final int value;
	private final String name;

	/**
	 * Creates a new MethodAccessFlag with the given name and value.
	 *
	 * @param name the flag name
	 * @param value the flag value
	 */
	protected MethodAccessFlag(String name, int value)
	{
		this.value = value;
		this.name = name;
	}

	/**
	 * Returns all possible MethodAccessFlags.
	 *
	 * @return all possible MethodAccessFlags
	 */
	public static Collection<MethodAccessFlag> getFlags()
	{
		return Lists.newArrayList(PUBLIC, PRIVATE, PROTECTED, STATIC, FINAL, SYNCHRONIZED, BRIDGE,
			VARARGS, NATIVE, ABSTRACT, STRICT, SYNTHETIC);
	}

	/**
	 * Returns the name used to represent the flag in Java source code.
	 * For example, "public", "protected", etc ...
	 *
	 * @return the flag name
	 */
	public String getName()
	{
		return name;
	}

	/**
	 * Returns the value used to represent the flag in a Java class file.
	 *
	 * @return the flag value
	 */
	public int getValue()
	{
		return value;
	}

	@Override
	public boolean equals(Object obj)
	{
		if (!(obj instanceof MethodAccessFlag))
			return false;
		MethodAccessFlag other = (MethodAccessFlag) obj;
		return value == other.getValue();
	}

	@Override
	public int hashCode()
	{
		int hash = 7;
		hash = 53 * hash + this.value;
		hash = 53 * hash + (this.name != null ? this.name.hashCode() : 0);
		return hash;
	}
}
