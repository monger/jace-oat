package org.jace.util;

public class ShutdownHook extends Thread
{
	private static ShutdownHook instance;
	private boolean registered;

    /**
     * Creates the shutdown hook
     */
    private ShutdownHook() {}
    
	/**
	 * Registers the shutdown hook
	 */
	public static synchronized void register() {
		if (instance == null) {
			instance = new ShutdownHook();
        }
        instance.registerIfNecessary();
	}

	/**
	 * Signal to Jace that the JVM is shutting down.
	 */
	private static native void signalVMShutdown();

	/**
	 * The first time this method is invoked per JVM, it registers the shutdown hook. Any subsequent
	 * invocations do nothing. This method is invoked internally by Jace and is not meant for end-users.
	 */
	private synchronized void registerIfNecessary() {
		if (!registered) {
			Runtime.getRuntime().addShutdownHook(this);
			registered = true;
		}
	}

	@Override public synchronized void run() {
		registered = false;
		signalVMShutdown();
	}
    
}
