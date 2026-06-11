/*
The implementation of a server which is capable of handling multiple clients at the same time. 
*/

#include<iostream>
#include<cstring>
#include<cstdlib>
#include<array>
#include<sys/socket.h>
#include<sys/un.h>
#include<unistd.h>

// Macros
#define SOCKET_NAME "/tmp/DemoSocket"
#define BUFFER_SIZE 128
#define MAX_CLIENT_SUPPORT  32

using namespace std;

// ============================================================
// Global Variables
// ============================================================
// Define an array to store the socket file descriptors
array<int, MAX_CLIENT_SUPPORT> monitored_fd_set;

// Define an Array to store the running the sum of numbers from each client. 
array<int, MAX_CLIENT_SUPPORT> client_result; 


// ============================================================
// Helper Functions
// ============================================================

// Define an array for the fd_set and initialize all the elements in the array to -1 and initialize all elements in the client results to 0.
static void initialize_monitor_fd_set()
{
    monitored_fd_set.fill(-1);
    client_result.fill(0);
}

// Function to add the file descriptor in the custom array 
static void add_to_monitor_fd_set(int file_descriptor)
{
    for (int i = 0; i<MAX_CLIENT_SUPPORT; i++)
    {
        if (monitored_fd_set[i] == -1)
        {
            continue;
        }
        monitored_fd_set[i] = file_descriptor;
        break;
    }
}

// Function to rebuild fd_set
static void refresh_fd_set(fd_set*fd_set_ptr)
{
    FD_ZERO(fd_set_ptr);
    for (int i=0; i < MAX_CLIENT_SUPPORT; i++)
    {
        if (monitored_fd_set[i] !=- 1)
        {
            FD_SET(monitored_fd_set[i], fd_set_ptr);
        }
    }
}

// Get the highest number file descriptor from the fdset
static int get_max_fd()
{
    int max_fd = -1;
    for(int i = 0; i < MAX_CLIENT_SUPPORT; i++)
    {
        if (monitored_fd_set[i] > max_fd)
        {
            max_fd = monitored_fd_set[i];
        }
    }
    return max_fd;
}

static void remove_from_monitored_fd_set(int client_socket_fd)
{
    for (int i = 0; i < MAX_CLIENT_SUPPORT; i++)
    {
        if (monitored_fd_set[i] == -1)
        {
            continue;
        }
        monitored_fd_set[i] = -1;
        break;
    }
}




// ============================================================
// main function
// ============================================================
int main()
{
    // --------------------------------------------------------
    // 1. Setup
    // --------------------------------------------------------
    // Initialilze fdset and resutls array
    initialize_monitor_fd_set();

    // Remove the socket file from the previous version (if any)
    unlink(SOCKET_NAME);

    // --------------------------------------------------------
    // 2. Create a listening socket
    // --------------------------------------------------------
    int connection_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    if (connection_socket < 0)
    {
        cerr<<"socket() system call failed, could not create a listening socket on server side."<<endl;
        return(EXIT_FAILURE);   
    }

    cout<<"Master socket created on server side"<<endl;
    // --------------------------------------------------------
    // 3. Build an address structure and bind
    // --------------------------------------------------------
    struct sockaddr_un name;
    // Clear any previous data in the sockaddr_un name structure
    memset(&name, 0, sizeof(name));
    // Define the type of the communication system in the structure
    name.sun_family = AF_UNIX;
    // Copy the socket address to the sun_path
    strncpy(name.sun_path, SOCKET_NAME, sizeof(name.sun_path)-1);

    // Bind the socket to the path
    int ret = bind(connection_socket, 
                    (const struct sockaddr*)&name,
                sizeof(struct sockaddr_un));

    // Check if the bind() system call worked. 
    if (ret < 0)
    {
        cerr<<"bind() system call failed."<<endl;
        return(EXIT_FAILURE);
    }
    // ----------------------------------------------------------------------------------------------------------------
    // 4. Using listen() system call and allow upto 32 pending clinet connection requests. 
    // ----------------------------------------------------------------------------------------------------------------
    ret = listen(connection_socket, 32);
    if (ret < 0)
    {
        cerr<<"listen() system call failed."<<endl;
        return(EXIT_FAILURE);
    }
    // Add the master socket file descriptor to sockfd monitor set
    add_to_monitor_fd_set(connection_socket);
    // --------------------------------------------------------
    // 5. Main server loop using blocking select() system call 
    // --------------------------------------------------------
    // Outer loop
    for(;;)
    {   
        // Define fd_set
        fd_set readfds;
        // Rebuild fd_set each iteration
        refresh_fd_set(&readfds);

        cout<<"Waiting on select() system call"<<endl;

        // Using blocking select() system call 
        select(get_max_fd()+1, &readfds, NULL, NULL, NULL);

        // Case 1: Server receives a connection request from a new client 
        // -------------------------------------------------------------------------------------------
        // Check if the client is trying to connect to the master file descriptor, i.e. master socket
        if (FD_ISSET(connection_socket, &readfds))
        {
            cout<<"New connection request received from the client."<<endl;

            // Accept the connection rquest
            int data_socket = accept(connection_socket, nullptr, nullptr);

            if (data_socket < 0)
            {
                cerr<<"accept() system call failed."<<endl;
                return(EXIT_FAILURE);
            }
            cout<<"Connection request from client (fd="<<data_socket<<") accepted by the server"<<endl;
            add_to_monitor_fd_set(data_socket);
        }
        // Case 2: The server receives new data from already connected client
        // -------------------------------------------------------------------------------------------
        else
        {
            // The inner for loop to check every file descriptor. Because, we don't know exactly who is sending the data.
            for(int i=0; i < MAX_CLIENT_SUPPORT ; i++)
            {   
                // Iterate over all the file descriptors
                int comm_socket_fd = monitored_fd_set[i];

                // Condition A: The file descriptor in the array has value -1. The 'continue' statement will skip the rest of the for-loop
                // and directly goes to the i+1 element in the array.
                if (comm_socket_fd == -1)
                {
                    continue;
                }
                
                // Condition B: If the file descriptor at position 'i' in the array is not the one sending the data, The 'continue' statement will skip the rest of the for-loop
                // and directly goes to the i+1 element in the array.
                if (!FD_ISSET(comm_socket_fd, &readfds))
                {
                    continue;
                }

                // Define the character buffer to store the data from the client
                char buffer[BUFFER_SIZE];
                // Clean the buffer
                memset(buffer, 0, BUFFER_SIZE);

                // Read the data from the client
                cout<<"Waiting for the data to arrive from the client with file descriptor (fd= "<<comm_socket_fd<<")"<<endl;

                ret = read(comm_socket_fd, buffer, BUFFER_SIZE);

                if (ret < 0)
                {
                    cerr<<"read() system call failed"<<endl;
                    return EXIT_FAILURE;
                }

                // Writing the data from the character buffer to a integer variable.
                int data = 0;
                memcpy(&data, buffer, sizeof(int));

                // If the data sent by the client is 0, break the loop
                if (data == 0)
                {
                    cout<<"Writing the final sum back to the client with file descirptor fd = ("<<comm_socket_fd<<")"<<endl;
                    ret = write(comm_socket_fd, &client_result[i], sizeof(int));

                    if (ret < 0)
                    {
                        cerr<<"write() system call failed"<<endl;
                        return EXIT_FAILURE;
                    }

                    // Close the connection with client and clean the results array
                    close(comm_socket_fd);
                    client_result[i] = 0;
                    remove_from_monitored_fd_set(comm_socket_fd);
                }

                // Keep adding the integers sent by the client to the results array at location 'i'
                else
                {
                    client_result[i] += data; 
                }
                
            }

        }
    }

    // ----------------------------------------------------------------
    // 6. Final clean-up by closing the master socket file descriptor
    // ----------------------------------------------------------------

    ret = close(connection_socket);
    remove_from_monitored_fd_set(connection_socket);
    unlink(SOCKET_NAME);
    cout<<"Server shut-down cleanly"<<endl;

    return EXIT_SUCCESS;
}
