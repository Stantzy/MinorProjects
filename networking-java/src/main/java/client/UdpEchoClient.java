package client;

import java.io.IOException;
import java.net.*;

public class UdpEchoClient implements Client {
    private static final int DEFAULT_BUFSIZE = 1024;

    private final InetSocketAddress clientAddress;
    private final DatagramSocket socket;
    byte[] buffer;

    public UdpEchoClient(InetAddress address) throws SocketException {
        this.socket = new DatagramSocket();
        clientAddress = new InetSocketAddress(address, socket.getLocalPort());
        this.buffer = new byte[DEFAULT_BUFSIZE];
    }

    @Override
    public void connect(InetSocketAddress serverAddress) throws SocketException {
        socket.connect(serverAddress);
    }

    @Override
    public void send(byte[] data) throws IOException {
        DatagramPacket packet = new DatagramPacket(
            data,
            data.length
        );

        socket.send(packet);
    }

    @Override
    public byte[] receive() throws IOException {
        DatagramPacket packet = new DatagramPacket(buffer, buffer.length);

        socket.receive(packet);

        byte[] data = new byte[packet.getLength()];
        System.arraycopy(buffer, 0, data, 0, packet.getLength());

        return data;
    }

    @Override
    public void disconnect() {
        socket.close();
    }
}
