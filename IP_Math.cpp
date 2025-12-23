/*
This file contains the function for calculating all the subnet information such as:
1. Broadcast address
2. All possible IP addresses
3. Network address

*/

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <bitset>
#include<cmath>

using namespace std;

class IP_Math {
    private:
        

    public:
        // Define parameterized constructor 
        IP_Math() {}
        
        // Function to parse the given IP-Address
        vector<int> parseIP(string ip)
        {
            vector <int> octates;
            string temp ;

            for(int i=0; i < ip.length(); i++)
            {
                if (ip[i] == '.')
                {
                    octates.push_back(stoi(temp));
                    temp = "";
                }
                else
                {
                    temp += ip[i];
                }
            }
            // For last octate of the IP-Address
            octates.push_back(stoi(temp));

            return octates;
        }

        // Function to convert any integer to 8 bit binary
        vector<int> toBinary(int num)
        {
            vector<int> binary(8, 0);
            for (int b = 7; b >=0; b--)
            {
                binary[b] = num%2;
                num = num/2;
            }
            return binary;
        }

        // Function to convert any binary integer to decimal
        int toDecimal (vector<int> binary)
        {
            int decimal = 0;
            int pwr = 1;

            for (int d=7; d>=0; d--)
            {
                decimal += binary[d]*pwr;
                pwr *=2;
            }
            return decimal;
        }

        // Function for doing bitwise AND operation
        vector<int> bitwiseAND(vector<int> a, vector<int> b)
        {
            vector<int> result(8);
            for (int i =0; i<8; i++)
            {
                result[i] = a[i] & b[i];
            }

            return result;
        }

        // Function for bitwise OR operation
        vector<int> bitwiseOR(vector<int> a, vector<int> b)
        {
            vector<int> result(8);
            for(int i=0; i<8; i++)
            {
                result[i] = a[i] | b[i];
            }
            return result;
        }

        // Function to invert the bits of the array
        vector<int> invertBits(vector<int> a)
        {
            vector<int> result(8);
            for (int k = 0; k<8; k++)
            {
                if (a[k] == 0)
                {
                    result[k] = 1;
                }

                else
                {
                    result[k] = 0;
                }
                
            }
            return result;
        }
    };

int main()
{
    // Testcase for the class and functions
    vector<int> IPOctates;
    IP_Math ip_math;
    IPOctates = ip_math.parseIP("192.168.2.10");

    // Calculate the total possible number of IP addresses
    int ones = 0;
    vector<int> maskParts = ip_math.parseIP("255.255.255.0");
    // calculate the number of 1s in the binary IP mask
    for (int m=0; m<4; m++)
    {
        vector<int> binaryPart = ip_math.toBinary(maskParts[m]);
        for (int binary=0; binary<8; binary++)
        {
            if (binaryPart[binary] == 1)
            {
                ones +=1;
            }
        }
    }
    // Calculate the possible number of IP addresses:
    cout<<"Possible number of IP addresses: "<<pow(2, (32-ones))-2<<endl;

    // Calculate the broadcast address
    //1. Parse the IP and mask address
    vector<int> octateIP = ip_math.parseIP("192.168.2.10");
    vector<int> octateMask = ip_math.parseIP("255.255.240.0");
    
    //2.convert the parsed IP and Mask to binary
    vector<vector<int>> binaryIP (4);
    vector<vector<int>> binaryMask(4);
    vector<vector<int>> invertBinaryMask(4);

    // Vector to store the broadcast Address
    vector<vector<int>> broadCastBinary (4);
    vector<int> broadcastAddress(4);
    // Empty string to display final broadcast address
    string BroadCastAddress = ""; 

    // Vectors to store the Network Address
    vector<vector<int>> networkAddressBinary(4);
    vector<int> networkAddress(4);
    // Empty string to store the fnal network address
    string NetworkAddress = "";
    
    for (int i=0; i<4; i++)
    {
        binaryIP[i] = ip_math.toBinary(octateIP[i]);
        binaryMask[i] = ip_math.toBinary(octateMask[i]);

        // For getting the network address
        // 3. Bitwise AND operation
        networkAddressBinary[i] = ip_math.bitwiseAND(binaryIP[i], binaryMask[i]);

        // 4. Convert binary to decimal
        networkAddress[i] = ip_math.toDecimal(networkAddressBinary[i]);
        
        // 5. Convert the decimal integer to string
        NetworkAddress += to_string(networkAddress[i]);
        if (i < 3)
        {
            NetworkAddress += ".";
        }

        // 6. Invert the binary of the mask
        invertBinaryMask[i] = ip_math.invertBits(binaryMask[i]);

        // 7. Do bitwise OR operation and store the individual results in 'broadcast' vector
        broadCastBinary[i] = ip_math.bitwiseOR(binaryIP[i], invertBinaryMask[i]);

        // 8. Convert individual binary octates to decimal octates
        broadcastAddress[i] = ip_math.toDecimal(broadCastBinary[i]);
        
        // 9. Add Octates to final broadcast address
        BroadCastAddress += to_string(broadcastAddress[i]);
        if (i < 3)
        {
            BroadCastAddress += '.';
        }
        
    }

    cout<<"Network address: "<<NetworkAddress<<endl;

    cout<<"Broadcast address for the network: "<< BroadCastAddress<<endl;

    cout<<"Success"<<endl;
    return 0;
}