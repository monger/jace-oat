package org.jace.util;

import java.lang.reflect.InvocationHandler;
import java.lang.reflect.Method;
import java.lang.reflect.Proxy;
import java.util.Map;
import java.util.HashMap;

public class NativeInvocation implements InvocationHandler {
    /** The class that we are proxying */
    private final Class clazz;
    
    /** A mapping of our methods to references */
    private final Map<String, Callback> callbackMap = new HashMap<String, Callback>();
    
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
            throw new UnsupportedOperationException("No method " + method.getName() + " registered");
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
