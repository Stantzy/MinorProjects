package client;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.net.SocketException;

public interface Client {
    void connect(InetSocketAddress serverAddress) throws SocketException;
    void send(byte[] data) throws IOException;
    byte[] receive() throws IOException;
    void disconnect();
}
