import socket
import threading
from queue import Queue

# Define the host and port to listen on
host = '192.168.17.128'  # Change this to your desired host
port = 8484  # Change this to your desired port
max_connections = 5  # Maximum number of connections in the pool

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

def worker(connection_pool):
    while True:
        # Get a client connection from the connection pool
        client_socket, client_address = connection_pool.get()

        # Handle the client connection
        handle_client(client_socket, client_address)

        # Mark the task as done for the connection pool
        connection_pool.task_done()

def main():
    # Create a socket object
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    # Set the SO_REUSEADDR option
    sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

    # Bind the socket to the host and port
    sock.bind((host, port))

    # Listen for incoming connections
    sock.listen(max_connections)

    print("Server is listening on {}:{}".format(host, port))

    # Create a connection pool as a thread-safe queue
    connection_pool = Queue(max_connections)

    # Create worker threads
    for _ in range(max_connections):
        t = threading.Thread(target=worker, args=(connection_pool,))
        t.daemon = True
        t.start()

    while True:
        # Accept a connection from a client
        client_socket, client_address = sock.accept()
        print("Connected to {}:{}".format(client_address[0], client_address[1]))

        # Put the client connection in the connection pool
        connection_pool.put((client_socket, client_address))

if __name__ == '__main__':
    main()
