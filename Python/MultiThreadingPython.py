import socket
import multiprocessing

# Define the host and port to listen on
host = '192.168.17.128'  # Change this to your desired host
port = 8484  # Change this to your desired port

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

        # Create a new process to handle the client
        client_process = multiprocessing.Process(target=handle_client, args=(client_socket, client_address))
        client_process.start()

if __name__ == '__main__':
    main()
