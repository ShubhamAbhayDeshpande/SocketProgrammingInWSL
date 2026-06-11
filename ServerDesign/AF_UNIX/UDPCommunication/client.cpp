/* 
This is the client side functionality of the Unix Domain UDP process.
*/
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SERVER_SOCKET_NAME "/tmp/UDPServerSocket"
#define CLIENT_SOCKET_NAME "/tmp/UDPClientSocket" // Client needs its own path!
#define BUFFER_SIZE 1024

using namespace std;

int main() 
{
    int client_socket;
    int ret;
    int final_result;

    // 1. Create Unix Domain UDP Socket
    client_socket = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (client_socket < 0) {
        cerr << "Client socket creation failed" << endl;
        return EXIT_FAILURE;
    }

    // 2. Bind the client to its own unique filesystem path
    // CRITICAL: Without this, the server cannot see who sent the data!
    struct sockaddr_un client_name;
    memset(&client_name, 0, sizeof(client_name));
    client_name.sun_family = AF_UNIX;
    strcpy(client_name.sun_path, CLIENT_SOCKET_NAME);

    unlink(CLIENT_SOCKET_NAME); // Remove old client socket if it exists
    
    ret = bind(client_socket, (const struct sockaddr *)&client_name, sizeof(struct sockaddr_un));
    if (ret < 0) {
        cerr << "Binding client socket failed" << endl;
        close(client_socket);
        return EXIT_FAILURE;
    }

    // 3. Configure target server address layout
    struct sockaddr_un server_name;
    memset(&server_name, 0, sizeof(server_name));
    server_name.sun_family = AF_UNIX;
    strcpy(server_name.sun_path, SERVER_SOCKET_NAME);

    // 4. Send structured sequence of integer payloads
    int inputs[] = {10, 20, 30, 0}; // 0 tells the server to sum up and reply
    socklen_t server_len = sizeof(struct sockaddr_un);

    for (int i = 0; i < 4; i++) {
        cout << "Sending data packet: " << inputs[i] << endl;
        
        ret = sendto(client_socket, &inputs[i], sizeof(int), 0,
                     (const struct sockaddr *)&server_name, server_len);
                     
        if (ret < 0) {
            cerr << "sendto() system call failed" << endl;
            break;
        }
        usleep(100000); // Small 100ms pause to ensure orderly arrival
    }

    // 5. Block to read the processed summary back from the server
    cout << "Waiting for calculation reply from server..." << endl;
    
    ret = recvfrom(client_socket, &final_result, sizeof(int), 0,
                   (struct sockaddr *)&server_name, &server_len);

    if (ret > 0) {
        cout << "====== SUCCESS ======" << endl;
        cout << "Summation received from server: " << final_result << endl;
    } else {
        cerr << "recvfrom() failed to capture server reply" << endl;
    }

    // 6. Housekeeping cleanup
    close(client_socket);
    unlink(CLIENT_SOCKET_NAME); // Delete client's temporary filesystem node
    
    return 0;
}
