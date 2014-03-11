package org.jace.util;

import java.lang.CloneNotSupportedException;
import java.lang.Object;
import java.lang.Override;
import java.lang.String;
import java.lang.Throwable;
import java.lang.reflect.InvocationHandler;
import java.lang.reflect.Method;
import java.lang.reflect.Proxy;
import java.util.Map;
import java.util.HashMap;

public class NativeInvocation implements InvocationHandler {
    private static final String EQUALS = "equals";
    private static final String HASH_CODE = "hashCode";
    private static final String TO_STRING = "toString";

    /** The class that we are proxying */
    private final Class clazz;
    
    /** A mapping of our methods to references */
    private final Map<String, Callback> callbackMap = new HashMap<String, Callback>();

    public static int defaultHashCode(final Object proxy) {
        return System.identityHashCode(proxy);
    }

    public static boolean defaultEquals(final Object proxy, Object obj) {
        return proxy.hashCode() == obj.hashCode();
    }

    public static String defaultToString(final Object proxy) {
        return proxy.getClass().getName() + "@" + Integer.toHexString(proxy.hashCode());
    }

    /**
     * Constructs a new invocation
     */
    public NativeInvocation(final String className) throws ClassNotFoundException {
        this.clazz = Class.forName(className);
    }
    
    /**
     * Registers a method
     */
    public void registerNative(final String name, final long ref, final int idx) { 
        callbackMap.put(name, new Callback(ref, idx));
    }
    
	/**
	 * Creates a new instance of a proxy class for this invocation
	 */
	public Object createProxy() {
        return Proxy.newProxyInstance(clazz.getClassLoader(), new Class[] { clazz }, this);
	}

    @Override public Object invoke(final Object proxy, final Method method, final Object[] args) {
        if (!callbackMap.containsKey(method.getName())) {
            switch(method.getName()) {
                case EQUALS:
                    return defaultEquals(proxy, args[0]);
                case HASH_CODE:
                    return defaultHashCode(proxy);
                case TO_STRING:
                    return defaultToString(proxy);
                default:
                    throw new UnsupportedOperationException("No method " + method.getName() + " registered");
            }
        }
        Callback c = callbackMap.get(method.getName());
        return invokeNative(c.ref, c.idx, proxy, args);
    }

	/**
	 * The native callback our invocation handler makes
	 */
	private native Object invokeNative(final long ref, final int idx, final Object proxy, final Object[] args);
    
    /**
     * A class to hold our callback information
     */
    private class Callback {
        final long ref;
        final int idx;
        private Callback(final long ref, final int idx) { 
            this.ref = ref;
            this.idx = idx;
        }
    }
}
