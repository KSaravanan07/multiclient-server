/*
Assignment 2 - CS5060: ACN
Socket Programming

Name     : Manan Bharatkumar Darji
Roll No. : CS22MTECH14004

Question : 
b. Create a client program which interacts with the file server on TCP. Client
TCP connects to the file server to fetch a file given the file path in the
server. Assume that a valid client knows the ip address, port number of
the file server.
*/

#include<iostream>
#include<fstream>
#include<stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <vector>
#include <sstream>
#include <bits/stdc++.h>

using namespace std;

class FileClientSocket {

    int PORT;
    int MySocket;
    int Address_length;
    int ByteRecvAtOnce = 1024 * 50;

    struct sockaddr_in Address;
    FILE * file;

    public:
        FileClientSocket() {
            CreateClientSocket();
            PORT = 8000;
            cout << "Please Enter Port Number Of Server : ";
            cin >> PORT;
            Address.sin_family = AF_INET;
            Address.sin_port = htons(PORT);
            Address_length = sizeof(Address);
            if (inet_pton(AF_INET, "127.0.0.1", & Address.sin_addr) <= 0) {
                cout << "||Client Side Error Occured|| : Connection Address Invalid :(" << endl;
            }
        }

    vector < string > FileNameTokenizer(string InFileLs, char Sep) {
        // This Function is used for seperating the string by some char which i used to seprate the list of files
        stringstream InFileLsStream(InFileLs);
        string File;
        vector < string > v;
        while (!InFileLsStream.eof()) {
            getline(InFileLsStream, File, Sep);
            v.push_back(File);
        }
        return v;
    }

    void CreateClientSocket() {
        // This Function is used to create socket
        MySocket = socket(AF_INET, SOCK_STREAM, 0);
        if (MySocket < 0) {
            cout << "||Client Side Error Occured|| : Socket Creation Failed, Exiting Program Now..." << endl;
            exit(EXIT_FAILURE);
        }
        cout << "||Client Log|| : Client Socket Created Successfully." << endl;
    }

    void CreateConnection() {
        // This Function is used for creating the connection with the server
        if (connect(MySocket, (struct sockaddr * ) & Address, sizeof(Address)) < 0) {
            cout << "||Client Side Error Occured|| : connection attempt failed, Exiting Program Now..." << endl;
            exit(EXIT_FAILURE);
        }
        cout << "||Client Log|| : Connection Successfull." << endl;
    }

    string WaitForMessage() {
        // This Function is used for recving the message/command from server
        char MessageBuf[ByteRecvAtOnce];
        memset(MessageBuf, '\0', sizeof(MessageBuf));
        int datarev = recv(MySocket, & MessageBuf, sizeof(MessageBuf), 0);
        cout << "||Resv LOG|| : DATA Received - " << datarev << endl;
        return MessageBuf;
    }

    void SendMessage(string msg) {
        // This Function is used for sending the message/command to server
        int response = send(MySocket, msg.c_str(), sizeof(msg), 0);
        cout << "||Send LOG|| : Response - " << response << endl;
    }

    int StrToInt(string n) {
        // This Function is used for converting string to int
        stringstream ss;
        ss << n;
        int i;
        ss >> i;
        return i;
    }

    string IntToStr(int n) {
        // This Function is used for converting the int to string
        stringstream ss;
        ss << n;
        string s;
        ss >> s;
        return s;
    }

    void ReceiveSelectedFile(string FileName) {
        // This Function is used for recving the selected file which will be sent by server
        string ClientPath = ".//Storage//Client//";
        ClientPath.append(FileName);
        file = fopen(ClientPath.c_str(), "wb");
        if (file) {
            cout << "||Client Log|| : File Creted." << endl;
        } else {
            cout << "||Client Side Error Occured|| : File creation failed, Exiting Program Now..." << endl;
            exit(EXIT_FAILURE);
        }

        int dataRecvd = 0, c = 0, flags = 0;
        long long size, received = 0;
        long long data = recv(MySocket, & size, sizeof(long long), 0);
        long long sizeleft = size;
        int temp = ByteRecvAtOnce;
        if (ByteRecvAtOnce > size) {
            temp = size;
        }
        char buffer[temp];
        cout << "||Client Log|| : Total Size Of File to be Receive " << size << endl;
        while (sizeleft > 0) {
            memset(buffer, '\0', sizeof(buffer));
            dataRecvd = recv(MySocket, & buffer, sizeof(buffer), flags);
            if (dataRecvd == -1) {
                cout << "Errorno : " << errno << endl;
            }
            received += dataRecvd;
            sizeleft -= dataRecvd;
            cout << "||Client Log|| : Number of Bytes Receive  : " << dataRecvd << " | Total Data Sent " << received << endl;
            c++;
            fwrite(buffer, 1, sizeof(buffer), file);
        }
        cout << endl;
        cout << "||Client Log|| : Received " << received << " Bytes of Data" << endl;
        cout << "||Client Log|| : Data Not Received " << max(0LL, size - received) << " Bytes" << endl;
        cin.clear();
        fclose(file);
        if (max(0LL, size - received) != 0LL)
            cout << "||Client Log|| : Error Occured In Transmition, File Might be Currupted." << endl;
        else
            cout << "||Client Log|| : File Saved Successfully." << endl;
    }
};

int main() {
    FileClientSocket C;
    C.CreateConnection();
    vector < string > v;

    // This is main infinite loop
    for (;;) {
        string val;
        cout << "------------------------------------------------------" << endl;
        cout << "Commands : " << endl <<
            " GETFL -  To Get a File List from Server" << endl <<
            " GET   -  To Get a File" << endl <<
            " BYE   -  To TERMINATE Program on Client Side" << endl <<
            endl;
        cout << "Enter Command : " << endl;

        cin >> val;
        if (val == "GETFL") {
            C.SendMessage("GETFL");
            string rev = C.WaitForMessage();
            v = C.FileNameTokenizer(rev, '/');
            cout << "------------------------------------------------------" << endl;
            int j = 0;
            cout << "index " << "name" << endl;
            for (auto a: v) {
                cout << j << "   " << a << endl;
                j++;
            }
            cout << "------------------------------------------------------" << endl;
            cout << "USE Command GET inorder to get that file" << endl;
        } else if (val == "GET") {
            string str = "";
            C.SendMessage("GET");
            cout << "PLEASE ENTER INDEX OF FILE : ";
            cin.clear();
            string in ;
            cin >> in ;
            cout << in << endl;
            C.SendMessage( in );
            C.ReceiveSelectedFile(v.at(C.StrToInt( in )));
        } else if (val == "BYE") {
            C.SendMessage("BYE");
            break;
        } else {
            C.SendMessage(val);
            string rev = C.WaitForMessage();
            cout << rev << endl;
            if (rev == "BYE") {
                C.SendMessage("BYE");
                break;
            }
        }
    }
    return 0;
}