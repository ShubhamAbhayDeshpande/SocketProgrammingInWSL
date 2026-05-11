/* 
This is srever side functionality of the process. 

When the connection is made to the client, 

*/
#include<iostream>
#include<cstring>
#include<cstdlib>
#include<sys/socket.h> // Used for system calls bind(), accept(), listen() and unlink().
#include<sys/un.h>
#include<unistd.h>

#define SOCKET_NAME "/tmp/DemoSocket"
#define BUFFER_SIZE 1024

using namespace std;

int main()
{
    // ======================================================================
    // 0. Creating variables to be used when accept() system call will be called
    // ======================================================================
    int data_socket;
    int result;
    char buffer[BUFFER_SIZE];
    int ret;
    int data;

    // ======================================================================
    // 1. Creating socket on the server side
    // ======================================================================
    
    // The following int variable will be used as a file descriptor
    int connectionSocket;
    connectionSocket = socket(AF_UNIX, SOCK_STREAM, 0);

    // Check if the socket connection was successful
    if (connectionSocket< 0)
    {
        cerr<<"Connection to the server failed";
        return(EXIT_FAILURE);
    }

    cout<<"Socket connction to server successfull"<<endl;

    // ======================================================================
    // 2. Creating address structure and memory for the socket connection
    // ======================================================================

    struct sockaddr_un name; // This creates memory to store all the information about the socket address. This information can be thought of as the metadata for the socket memory. 

    // The following check is implemented to see if the created memory is correct. This is done using the #if and #endif directives
    #if 0
    struct sockaddr_un {
        sa_family_t sun_family;
        char sun_path[108];
    };
    #endif

    // Clear the created memory if the initialization was successfull
    memset(&name, 0, sizeof(name));

    // Specify the address family name in the memory
    name.sun_family = AF_UNIX;

    // Specify the path of the socket(i.e. the location where the socket object is located)
    strcpy(name.sun_path, SOCKET_NAME);

    // ======================================================================
    // 3. Binding the socket using socket address to the socket name
    // ======================================================================

    // If any previous connection still exists, this will remove it. 
    unlink(SOCKET_NAME);

    ret = bind(connectionSocket, (const struct sockaddr *)&name, sizeof(struct sockaddr_un));

    // Check if the bind() was successfull. If not, raise error, exit program
    if (ret<0)
    {
        cerr<<("Binding the socket to socket address was not succesfull.")<<endl;
        return(EXIT_FAILURE);
    }

    // ======================================================================
    //4. Listening to the client and accepting the messages. 
    // ======================================================================
    /*
    This is the main loop where the server is online 24x7. This is one of the main
    quality of a good server.

    There will be a continuous cycle of accept() and read() system calls to read the 
    data from the client.
    */

    ret = listen(connectionSocket, 5);
    if (ret <0)
    {
        cerr<<"listen system call filed"<<endl;
        return(EXIT_FAILURE);
    }

    for (;;)
    {
        // Wait for the incoming connection request
        cout<<"Waiting for accept() system call."<<endl;

        // Using accept() system call 
        data_socket = accept(connectionSocket, NULL, NULL);

        // Raise an error if the accept() system call fails
        if(data_socket<0)
        {
            cerr<<"accept() system call failed"<<endl;
            return(EXIT_FAILURE);
        }
        cout<<"Connection from the client accepted with acceept() system call"<<endl;

        result = 0;

        for (;;)
        {
            // Prepare buffer for receiving data from client by eraising the existing values in buffer
            memset(buffer, 0, BUFFER_SIZE);

            // Wait for the next data packet
            cout<<"Waiting for the next data packet to arrive"<<endl;

            // Using the read() system call to read the data from the client
            ret = read(data_socket, buffer, sizeof(int));

            // If the read() fails, return error and end program. Note here, that read can return 0. Hence, the error is called at -1.
            if (ret <= 0)
            {
                cerr<<"read() function call failed. "<<endl;
                return(EXIT_FAILURE);
            }

            // Copy the received data from the buffer to an integer
            memcpy(&data, buffer, sizeof(int));
            if (data==0)
            {
                break;
            }
            result+=data;            
        }

        // Send the information back to the client
        // Reset the buffer
        // Copy result into buffer
        cout << "Sending result back to client" << endl;

        ret = write(data_socket, &result, sizeof(result));

        if (ret < 0)
        {
            cerr << "write() failed" << endl;
        }

        // Close the socket
        close(data_socket);
    
    }

    // Close connectionSocket
    close(connectionSocket);

    // Unlink the socket. This is needed because, if for any reason, the server crashes, the next time bind() tries to find the file system object, the object already exists. unlink() is used to delete the existing file sytem object.
    unlink(SOCKET_NAME);
    
    return 0;
}

