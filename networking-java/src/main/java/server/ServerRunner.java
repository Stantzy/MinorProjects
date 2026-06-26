package server;

public class ServerRunner {
    public static void main(String[] args) {
        try {
            Server server = new UdpEchoServer();
            server.startServer();
            server.stopServer();
        } catch(Exception e) {
            throw new RuntimeException(e);
        }
    }
}
