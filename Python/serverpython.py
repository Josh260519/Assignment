import socket
import threading

# Defining host and port to listen for connections
host = '192.168.17.128'  # IP Address of host
port = 8484  # Port to listen to

def handle_client(client_socket, client_address):
    # Receive data from the client
    while True:
        data = client_socket.recv(1024).decode('utf-8')
        if not data:
            break
        print("Received data from {}: {}: {}".format(client_address[0], client_address[1], data))

        # Echo the received data back to the client
        client_socket.sendall(data.encode('utf-8'))

    # Close the client socket
    client_socket.close()
    print("Connection closed with {}:{}".format(client_address[0], client_address[1]))

def main():
    # Create a socket object
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    # Enabling SO_REUSEADDR option
    sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

    # Bind the socket to the host and port
    sock.bind((host, port))

    # Listen for incoming connections
    sock.listen(5)

    print("Server is listening on {}:{}".format(host, port))

    while True:
        # Accept a connection from a client
        client_socket, client_address = sock.accept()
        print("Connected to {}:{}".format(client_address[0], client_address[1]))

        # Creates a thread to handle the client
        client_thread = threading.Thread(target=handle_client, args=(client_socket, client_address))
        client_thread.start()

if __name__ == '__main__':
    main()
