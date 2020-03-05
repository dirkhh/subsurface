package org.subsurfacedivelog.mobile;

import com.hoho.android.usbserial.driver.UsbSerialDriver;
import com.hoho.android.usbserial.driver.UsbSerialPort;
import com.hoho.android.usbserial.driver.UsbSerialProber;

import android.hardware.usb.UsbDeviceConnection;
import android.hardware.usb.UsbManager;
import android.hardware.usb.UsbDevice;

import android.content.Context;
import android.util.Log;
import android.app.PendingIntent;
import android.content.Intent;

import org.subsurfacedivelog.mobile.SubsurfaceMobileActivity;

import java.lang.System;
import java.lang.Thread;
import java.util.Queue;
import java.util.List;
import java.util.LinkedList;
import java.lang.Math;

public class AndroidSerial {

	private static final String TAG = "AndroidSerial";

	private static final int DC_STATUS_SUCCESS = 0;
	private static final int DC_STATUS_DONE = 1;
	private static final int DC_STATUS_UNSUPPORTED = -1;
	private static final int DC_STATUS_INVALIDARGS = -2;
	private static final int DC_STATUS_NOMEMORY = -3;
	private static final int DC_STATUS_NODEVICE = -4;
	private static final int DC_STATUS_NOACCESS = -5;
	private static final int DC_STATUS_IO = -6;
	private static final int DC_STATUS_TIMEOUT = -7;
	private static final int DC_STATUS_PROTOCOL = -8;
	private static final int DC_STATUS_DATAFORMAT = -9;
	private static final int DC_STATUS_CANCELLED = -1;

	/**
	 * The parity checking scheme.
	 * Matches us-serial-for-android
	 */
	private static final int DC_PARITY_NONE = 0;
	private static final int DC_PARITY_ODD = 1;
	private static final int DC_PARITY_EVEN = 2;
	private static final int DC_PARITY_MARK = 3;
	private static final int DC_PARITY_SPACE = 4;

	/**
	 * The number of stop bits.
	 * Has to be translated for usb-serial-for-android
	 */
	private static final int DC_STOPBITS_ONE = 0;          /**< 1 stop bit */
	private static final int DC_STOPBITS_ONEPOINTFIVE = 1; /**< 1.5 stop bits*/
    private static final int DC_STOPBITS_TWO = 2;              /**< 2 stop bits */

	/**
	 * The direction of the data transmission.
	 */
	private static final int DC_DIRECTION_INPUT = 1;  /**< Input direction */
	private static final int DC_DIRECTION_OUTPUT = 2; /**< Output direction */
	private static final int DC_DIRECTION_ALL = 3;    /**< All directions */


	private UsbSerialPort usbSerialPort;
	private int timeout = 0;
	private LinkedList<Byte> readBuffer = new LinkedList<Byte>();

	private static String printQueue(LinkedList<Byte> readBuffer)
	{
		String str = "" + readBuffer.size() + " elements: ";
		for(byte element : readBuffer) {
			str = str + String.format("%02X", element);
		}
		return str;
	}


	private AndroidSerial(UsbSerialPort usbSerialPort) {
		this.usbSerialPort = usbSerialPort;
	}

	public static AndroidSerial open_android_serial() {
		try {
			Log.d(TAG, "in " + Thread.currentThread().getStackTrace()[2].getMethodName());
			// Find all available drivers from attached devices.
			Context context = SubsurfaceMobileActivity.getAppContext();
			UsbManager manager = (UsbManager) context.getSystemService(Context.USB_SERVICE);
			UsbSerialProber usbSerialProber = UsbSerialProber.getDefaultProber();

			// TODO attach custom VID / PID / Drivers

			List<UsbSerialDriver> availableDrivers = usbSerialProber.findAllDrivers(manager);
			if (availableDrivers.isEmpty()) {
				Log.w(TAG, "no usb-to-serial devices found!");
				return null;
			}

			// Open a connection to the first available driver.
			UsbSerialDriver driver = availableDrivers.get(0);
			UsbDeviceConnection connection = manager.openDevice(driver.getDevice());

			if (connection == null) {
				manager.requestPermission(driver.getDevice(), PendingIntent.getBroadcast(context, 0, new Intent("org.subsurfacedivelog.mobile.USB_PERMISSION"), 0));
				Log.w(TAG, "Could not open device!");
				return null;
			}

			Log.i(TAG, "Num ports: " + driver.getPorts().size());

			UsbSerialPort usbSerialPort = driver.getPorts().get(0); // Most devices have just one port (port 0)
			usbSerialPort.open(connection);
			Log.i(TAG, "Opened serial device " + usbSerialPort);

			return new AndroidSerial(usbSerialPort);

		}
		catch (Exception e)
		{
			Log.e(TAG, "Error in " + Thread.currentThread().getStackTrace()[2].getMethodName(), e);
			return null;
		}
	}

	/* dc_status_t */
	public int set_timeout(int timeout) {
		Log.d(TAG, "in " + Thread.currentThread().getStackTrace()[2].getMethodName());
		this.timeout = timeout;
		return AndroidSerial.DC_STATUS_SUCCESS;
	}

	/* dc_status_t */
	public int set_dtr(boolean value) {
		Log.d(TAG, "in " + Thread.currentThread().getStackTrace()[2].getMethodName());
		try {
			usbSerialPort.setDTR(value);
		}
		catch (Exception e) {
			Log.e(TAG, "Error in " + Thread.currentThread().getStackTrace()[2].getMethodName(), e);
			return AndroidSerial.DC_STATUS_IO;
		}
		return AndroidSerial.DC_STATUS_SUCCESS;
	}

	/* dc_status_t */
	public int set_rts(boolean value) {
		Log.d(TAG, "in " + Thread.currentThread().getStackTrace()[2].getMethodName());
		try {
			usbSerialPort.setRTS(value);
		}
		catch (Exception e) {
			Log.e(TAG, "Error in " + Thread.currentThread().getStackTrace()[2].getMethodName(), e);
			return AndroidSerial.DC_STATUS_IO;
		}
		return AndroidSerial.DC_STATUS_SUCCESS;
	}

	/* length of read data, or dc_status_t if not successful */
	/*
	// Currently commented out since a non-blocking get_available is not possible due to usb-serial-for-android limitations
	public int get_available() {
		Log.d(TAG, "in " + Thread.currentThread().getStackTrace()[2].getMethodName());
		try {
			byte[] readData = new byte[64];  // TODO magic number
			int len = usbSerialPort.read(readData, 250); // TODO magic number
			for(int i = 0; i < len; i++) {
				readBuffer.add(readData[i]);
			}
			return readBuffer.size();
		}
		catch (Exception e) {
			Log.e(TAG, "Error in " + Thread.currentThread().getStackTrace()[2].getMethodName(), e);
			return AndroidSerial.DC_STATUS_IO;
		}
	}
	*/

	// Flow control is not implemented in usb-serial-for-android. It's not used by any dive computer anyways...
	/* dc_status_t */
	public int configure(int baudrate, int databits, /*dc_parity_t*/ int parity, /*dc_stopbits_t*/ int stopbits) {
		Log.d(TAG, "in " + Thread.currentThread().getStackTrace()[2].getMethodName() + ", baudrate=" + baudrate + ", databits=" + databits + ", parity=" + parity + ", stopbits=" + stopbits);
		int translated_stopbits = 0;
		switch(stopbits) {
			case AndroidSerial.DC_STOPBITS_ONE:
				translated_stopbits = UsbSerialPort.STOPBITS_1;
				break;
			case AndroidSerial.DC_STOPBITS_ONEPOINTFIVE:
				translated_stopbits = UsbSerialPort.STOPBITS_1_5;
				break;
			case AndroidSerial.DC_STOPBITS_TWO:
				translated_stopbits = UsbSerialPort.STOPBITS_2;
				break;
		}

		try {
			usbSerialPort.setParameters(baudrate, databits, translated_stopbits, parity);
		}
		catch (Exception e) {
			Log.e(TAG, "Error in " + Thread.currentThread().getStackTrace()[2].getMethodName(), e);
			return AndroidSerial.DC_STATUS_IO;
		}
		return AndroidSerial.DC_STATUS_SUCCESS;
	}

	/* length of read data, or dc_status_t if not successful */
	public int read(byte[] data) {
		Log.d(TAG, "in " + Thread.currentThread().getStackTrace()[2].getMethodName());

		try {

			Log.d(TAG, "read length: " + data.length);

			int toReadFromHwLength = data.length - readBuffer.size();

			int arraylength = (toReadFromHwLength % 64) != 0 ? toReadFromHwLength + (64 - (toReadFromHwLength % 64)): toReadFromHwLength; // use blocks of 64 for reading

			// When we don't have enough in the buffer, try to read from HW
			if(toReadFromHwLength > 0)
			{
				// Read and append to buffer
				byte[] readFromHwData = new byte[arraylength];
				int actuallyReadFromHwLength = usbSerialPort.read(readFromHwData, 0); // With this it works... But the timeout is ignored! Fix this!
				for(int i = 0; i < actuallyReadFromHwLength; i++ ) {
					readBuffer.add(readFromHwData[i]);
				}
			}

			//Log.d(TAG, "read buffer: " + printQueue(readBuffer));
			int returnLength = 0;
			for(returnLength = 0; returnLength < data.length; returnLength++) {
				if(readBuffer.isEmpty()) break;
				data[returnLength] = readBuffer.remove();
			}

			Log.d(TAG, "return length: " + returnLength);
			return returnLength;
		}
		catch (Exception e) {
			Log.e(TAG, "Error in " + Thread.currentThread().getStackTrace()[2].getMethodName(), e);
			return AndroidSerial.DC_STATUS_IO;
		}
	}

	/* number of bytes written, or dc_status_t */
	public int write (byte[] data) {
		Log.d(TAG, "in " + Thread.currentThread().getStackTrace()[2].getMethodName());
		try {
		Log.d(TAG, "write length: " + data.length);
			int retval =  usbSerialPort.write(data, timeout);
			Log.d(TAG, "actual write length: " + retval);
			return retval;
		}
		catch (Exception e) {
			Log.e(TAG, "Error in " + Thread.currentThread().getStackTrace()[2].getMethodName(), e);
			return AndroidSerial.DC_STATUS_IO;
		}
	}

	/* dc_status_t */
	public int purge (/*dc_direction_t*/ int direction) {
		Log.d(TAG, "in " + Thread.currentThread().getStackTrace()[2].getMethodName());
		try {
			if((direction | AndroidSerial.DC_DIRECTION_INPUT) > 0) readBuffer.clear();
			boolean retval = this.usbSerialPort.purgeHwBuffers((direction | AndroidSerial.DC_DIRECTION_OUTPUT) > 0, (direction | AndroidSerial.DC_DIRECTION_INPUT) > 0);
			return retval ? AndroidSerial.DC_STATUS_SUCCESS : AndroidSerial.DC_STATUS_IO;
		}
		catch (Exception e) {
			Log.e(TAG, "Error in " + Thread.currentThread().getStackTrace()[2].getMethodName(), e);
			return AndroidSerial.DC_STATUS_IO;
		}
	}

	/*dc_status_t*/
	public int close () {
		Log.d(TAG, "in " + Thread.currentThread().getStackTrace()[2].getMethodName());
		try {
			usbSerialPort.close();
			return AndroidSerial.DC_STATUS_SUCCESS;
		}
		catch (Exception e) {
			Log.e(TAG, "Error in " + Thread.currentThread().getStackTrace()[2].getMethodName(), e);
			return AndroidSerial.DC_STATUS_IO;
		}
	}
}
