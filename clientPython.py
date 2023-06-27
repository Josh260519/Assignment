import socket

# Prompt the user for server IP address and port number
server_ip = input("Enter the server IP address: ")
server_port = int(input("Enter the server port number: "))

# Create a TCP/IP socket
client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Connect to the server
server_address = (server_ip, server_port)
client_socket.connect(server_address)

# Prompt the user for input
user_input = input("Enter a string: ")

# Send the user input to the server
client_socket.sendall(user_input.encode('utf-8'))

# Receive the reply from the server
reply = client_socket.recv(1024).decode('utf-8')

# Print the reply
print("Reply from server:", reply)

# Close the connection
client_socket.close()
