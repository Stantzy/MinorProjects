package server;

import java.io.IOException;
import java.net.*;

public class UdpEchoServer implements Server {
    private static final String DEFAULT_ADDRESS = "localhost";
    private static final int DEFAULT_PORT = 7788;
    private static final int DEFAULT_BUFSIZE = 1024;

    private final DatagramSocket serverSocket;
    private boolean running;

    public UdpEchoServer() throws SocketException, UnknownHostException {
        this(DEFAULT_ADDRESS, DEFAULT_PORT);
    }

    public UdpEchoServer(int port) throws SocketException, UnknownHostException {
        this(DEFAULT_ADDRESS, port);
    }

    public UdpEchoServer(String address, int port)
    throws UnknownHostException, SocketException {
        InetAddress serverAddress = InetAddress.getByName(address);
        serverSocket = new DatagramSocket(port, serverAddress);
    }

    @Override
    public void startServer() throws IOException {
        logStartServer();

        try {
            this.running = true;
            byte[] buffer = new byte[DEFAULT_BUFSIZE];
            while(running) {
                DatagramPacket packet = new DatagramPacket(buffer, buffer.length);
                receiveMessage(packet);
                sendEcho(packet, buffer);
                checkShutdownRequest(packet);
            }
        } finally {
            stopServer();
        }
    }

    @Override
    public void stopServer() {
        serverSocket.close();
        logStopServer();
    }

    private void receiveMessage(DatagramPacket packet) throws IOException {
        serverSocket.receive(packet);
        logReceivedMessage(packet);
    }

    private void sendEcho(DatagramPacket packet, byte[] data)
    throws IOException {
        serverSocket.send(packet);
        logEcho(packet);
    }

    private void checkShutdownRequest(DatagramPacket packet) {
        String received =
            new String(packet.getData(), 0, packet.getLength());

        if(received.equals("shutdown")) {
            this.running = false;
            logShutdownRequest(packet);
        }
    }

    private void logStartServer() {
        String ip = serverSocket.getLocalAddress().getHostAddress();
        int port = serverSocket.getLocalPort();

        System.out.printf(
            "[INFO] UDP Echo Server started on %s:%d\n", ip, port
        );
    }

    private void logReceivedMessage(DatagramPacket packet) {
        String clientIp = packet.getAddress().getHostAddress();
        int clientPort = packet.getPort();

        System.out.printf(
            "[DEBUG] Received %d bytes from %s:%d: \"%s\"\n",
            packet.getLength(),
            clientIp,
            clientPort,
            new String(packet.getData(), 0, packet.getLength())
        );
    }

    private void logEcho(DatagramPacket echo) {
        String clientIp = echo.getAddress().getHostAddress();
        int clientPort = echo.getPort();
        System.out.printf(
            "[DEBUG] Echoed %d bytes to %s:%d\n",
            echo.getLength(),
            clientIp,
            clientPort
        );
    }

    private void logShutdownRequest(DatagramPacket request) {
        String clientIp = request.getAddress().getHostAddress();
        int clientPort = request.getPort();
        System.out.printf(
            "[INFO] Server shutdown requested by %s:%d\n",
            clientIp,
            clientPort
        );
    }

    private void logStopServer() {
        System.out.println("[INFO] UDP Echo Server stopped");
    }
}
