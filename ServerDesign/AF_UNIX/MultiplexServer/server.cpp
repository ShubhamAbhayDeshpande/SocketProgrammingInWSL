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

        select(get_max_fd()+1, &readfds, NULL, NULL, NULL);


    }



    return 0;
}
