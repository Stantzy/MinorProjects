package client;

import java.io.IOException;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.util.Arrays;
import java.util.Scanner;

public class TestClient {
    public static void main(String[] args) {
        try {
            Client client = new UdpEchoClient(
                InetAddress.getByName("localhost")
            );

            Scanner scanner = new Scanner(System.in);
            byte[] buffer;
            while(scanner.hasNext()) {
                String message = scanner.nextLine();
                client.connect(
                    new InetSocketAddress(
                        InetAddress.getByName("localhost"),
                        7788
                    )
                );
                client.send(message.getBytes());
                buffer = client.receive();
                System.out.println(Arrays.toString(buffer));
            }

        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }
}
