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
    private final Map<String, Long> methodMap = new HashMap<String, Long>();
    
    /**
     * Constructs a new invocation
     */
    public NativeInvocation(final String className) throws ClassNotFoundException {
        this.clazz = Class.forName(className);
    }
    
    /**
     * Registers a method
     */
    public void registerNative(final String name, final long ref) { 
        methodMap.put(name, ref);
    }
    
	/**
	 * Creates a new instance of a proxy class for this invocation
	 */
	public Object createProxy() {
        return Proxy.newProxyInstance(clazz.getClassLoader(), new Class[] { clazz }, this);
	}

    @Override public Object invoke(final Object proxy, final Method method, final Object[] args) {
        if (!methodMap.containsKey(method.getName())) {
            throw new IllegalStateException("No method " + method.getName() + " registered");
        }
        return invokeNative(proxy, methodMap.get(method.getName()), args);
    }

	/**
	 * The native callback our invocation handler makes
	 */
	private native Object invokeNative(final Object proxy, final long ref, final Object[] args);
}
