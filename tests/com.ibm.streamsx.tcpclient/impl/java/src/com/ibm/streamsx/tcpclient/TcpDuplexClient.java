/*
 Copyright (C)2015, International Business Machines Corporation and
 others. All Rights Reserved.
 */
package com.ibm.streamsx.tcpclient;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.Reader;
import java.math.BigInteger;
import java.net.Socket;
import java.util.List;

import org.apache.log4j.Logger;

import com.ibm.streams.operator.AbstractOperator;
import com.ibm.streams.operator.OperatorContext;
import com.ibm.streams.operator.OutputTuple;
import com.ibm.streams.operator.StreamingData.Punctuation;
import com.ibm.streams.operator.StreamingInput;
import com.ibm.streams.operator.StreamingOutput;
import com.ibm.streams.operator.Tuple;

/**
 * Class for an operator that receives a tuple and then optionally submits a
 * tuple. This pattern supports a number of input streams and a single output
 * stream.
 * <P>
 * The following event methods from the Operator interface can be called:
 * </p>
 * <ul>
 * <li><code>initialize()</code> to perform operator initialization</li>
 * <li>allPortsReady() notification indicates the operator's ports are ready to
 * process and submit tuples</li>
 * <li>process() handles a tuple arriving on an input port
 * <li>processPuncuation() handles a punctuation mark arriving on an input port
 * <li>shutdown() to shutdown the operator. A shutdown request may occur at any
 * time, such as a request to stop a PE or cancel a job. Thus the shutdown() may
 * occur while the operator is processing tuples, punctuation marks, or even
 * during port ready notification.</li>
 * </ul>
 * <p>
 * With the exception of operator initialization, all the other events may occur
 * concurrently with each other, which lead to these methods being called
 * concurrently by different threads.
 * </p>
 */
public class TcpDuplexClient extends AbstractOperator {

	private Socket socket;
	private Reader inReader;
	private BufferedWriter outWriter;

	private Thread readThread;

	private class ReadRunnable implements Runnable {
		@Override
		public void run() {
			if (inReader instanceof BufferedReader) {
				readAsString();
			}
			else
			{
				while (true)
				{
					readResponse();
				}
			}

		}

	}

	/**
	 * Initialize this operator. Called once before any tuples are processed.
	 * 
	 * @param context
	 *            OperatorContext for this operator.
	 * @throws Exception
	 *             Operator failure, will cause the enclosing PE to terminate.
	 */
	@Override
	public synchronized void initialize(OperatorContext context)
			throws Exception {
		// Must call super.initialize(context) to correctly setup an operator.
		super.initialize(context);
		Logger.getLogger(this.getClass()).trace(
				"Operator " + context.getName() + " initializing in PE: "
						+ context.getPE().getPEId() + " in Job: "
						+ context.getPE().getJobId());

		String host = getOperatorContext().getParameterValues("host").get(0);
		int port = Integer.valueOf(getOperatorContext().getParameterValues(
				"port").get(0));

		socket = new Socket(host, port);	
		
		outWriter = new BufferedWriter(new OutputStreamWriter(
				socket.getOutputStream()));
		
//		inReader = new BufferedReader(new InputStreamReader(
//				socket.getInputStream()));
		
		inReader = new InputStreamReader(socket.getInputStream());

	}

	/**
	 * Notification that initialization is complete and all input and output
	 * ports are connected and ready to receive and submit tuples.
	 * 
	 * @throws Exception
	 *             Operator failure, will cause the enclosing PE to terminate.
	 */
	@Override
	public synchronized void allPortsReady() throws Exception {
		// This method is commonly used by source operators.
		// Operators that process incoming tuples generally do not need this
		// notification.
		OperatorContext context = getOperatorContext();
		Logger.getLogger(this.getClass()).trace(
				"Operator " + context.getName()
						+ " all ports are ready in PE: "
						+ context.getPE().getPEId() + " in Job: "
						+ context.getPE().getJobId());

		List<String> values = context.getParameterValues("readResponse");
		boolean startReadThread = true;
		if (values.size() > 0) {
			Boolean read = Boolean.valueOf(values.get(0));
			startReadThread = read.booleanValue();
		}

		if (startReadThread) {
			readThread = context.getThreadFactory().newThread(
					new ReadRunnable());
			readThread.start();
		}
	}

	/**
	 * Process an incoming tuple that arrived on the specified port.
	 * <P>
	 * Copy the incoming tuple to a new output tuple and submit to the output
	 * port.
	 * </P>
	 * 
	 * @param inputStream
	 *            Port the tuple is arriving on.
	 * @param tuple
	 *            Object representing the incoming tuple.
	 * @throws Exception
	 *             Operator failure, will cause the enclosing PE to terminate.
	 */
	@Override
	public final void process(StreamingInput<Tuple> inputStream, Tuple tuple)
			throws Exception {

		// take the input tuple
		String line = tuple.getString(0);

		// send it through the socket
		outWriter.write(line);
		outWriter.flush();
		
//		readResponse();

	}
	
	private void readResponse() {
		System.out.println("Read Response");
		try {
			byte[] len = new byte[8];
			char[] buffer = new char[1024*1024];
			
			InputStream inStream = socket.getInputStream();
			
			int numRead = inStream.read(len);
			
			if (numRead != -1) {
				
				System.out.println(len);

				// convert length to a number
				BigInteger msgLen = new BigInteger(len);
				
				System.out.println("Buffer length: " + msgLen);
				
				int numBytes = msgLen.intValue();
				if (numBytes > 0)
				{
					// read the next numBytes

					numRead = inReader.read(buffer, 0,  numBytes);
					
					if (numRead != -1)
					{
						System.out.println("Buffer content: " + String.valueOf(buffer));
						// output response
						// Create a new tuple for output port 0
						StreamingOutput<OutputTuple> outStream = getOutput(0);
						OutputTuple outTuple = outStream.newTuple();
						outTuple.setString(0, new String(buffer));

						// Submit new tuple to output port 0

						getOperatorContext().getStreamingOutputs().get(0)
								.submit(outTuple);
											
					}
				}						
			}
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	/**
	 * Process an incoming punctuation that arrived on the specified port.
	 * 
	 * @param stream
	 *            Port the punctuation is arriving on.
	 * @param mark
	 *            The punctuation mark
	 * @throws Exception
	 *             Operator failure, will cause the enclosing PE to terminate.
	 */
	@Override
	public void processPunctuation(StreamingInput<Tuple> stream,
			Punctuation mark) throws Exception {
		// For window markers, punctuate all output ports
		super.processPunctuation(stream, mark);
	}

	/**
	 * Shutdown this operator.
	 * 
	 * @throws Exception
	 *             Operator failure, will cause the enclosing PE to terminate.
	 */
	public synchronized void shutdown() throws Exception {
		OperatorContext context = getOperatorContext();
		Logger.getLogger(this.getClass()).trace(
				"Operator " + context.getName() + " shutting down in PE: "
						+ context.getPE().getPEId() + " in Job: "
						+ context.getPE().getJobId());

		inReader.close();
		outWriter.close();
		socket.close();

		super.shutdown();
	}

	private void readAsString() {
		BufferedReader bReader = (BufferedReader) inReader;
		try {
			// read response from socket

			String response = bReader.readLine();

			while (response != null) {
				// output response
				// Create a new tuple for output port 0
				StreamingOutput<OutputTuple> outStream = getOutput(0);
				OutputTuple outTuple = outStream.newTuple();
				outTuple.setString(0, response);

				// Submit new tuple to output port 0

				getOperatorContext().getStreamingOutputs().get(0)
						.submit(outTuple);

				response = bReader.readLine();
			}
		} catch (Exception e) {
		}
	}
}
