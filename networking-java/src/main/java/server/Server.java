package server;

import java.io.IOException;

public interface Server {
    void startServer() throws IOException;
    void stopServer();
}
