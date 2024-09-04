/*
Assignment 2 - CS5060: ACN
Socket Programming

Name     : Manan Bharatkumar Darji
Roll No. : CS22MTECH14004

Question : 
b. Create one server program as per #a above and many instances of valid
clients mentioned in Part A and check the working of parallel file download
from the server. Check for all possible errors for which the server may give
error message/error code to the client. E.g., file doesn’t exist. Check the 
situation when the server is busy serving other clients and one client has
to wait for the server to start serving it after serving other clients.
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
                cout << "||Client Side Error|| : Connection Address Invalid :(" << endl;
            }
        }

    vector < string > FileNameTokenizer(string InFileLs, char Sep) {
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
        MySocket = socket(AF_INET, SOCK_STREAM, 0);
        if (MySocket < 0) {
            cout << "||Client Side Error|| : Socket Creation Failed, Exiting Program Now..." << endl;
            exit(EXIT_FAILURE);
        }
        cout << "||Client Log|| : Client Socket Created Successfully." << endl;
    }

    void CreateConnection() {
        if (connect(MySocket, (struct sockaddr * ) & Address, sizeof(Address)) < 0) {
            cout << "||Client Side Error|| : connection attempt failed, Exiting Program Now..." << endl;
            exit(EXIT_FAILURE);
        }
        cout << "||Client Log|| : Connection Successfull." << endl;
    }

    string WaitForMessage() {
        char MessageBuf[ByteRecvAtOnce];
        memset(MessageBuf, '\0', sizeof(MessageBuf));
        int datarev = recv(MySocket, & MessageBuf, sizeof(MessageBuf), 0);
        cout << "||Resv LOG|| : DATA Received - " << datarev << endl;
        return MessageBuf;
    }

    void SendMessage(string msg) {
        int response = send(MySocket, msg.c_str(), sizeof(msg), 0);
        cout << "||Send LOG|| : Response - " << response << endl;
    }

    int StrToInt(string n) {
        stringstream ss;
        ss << n;
        int i;
        ss >> i;
        return i;
    }

    string IntToStr(int n) {
        stringstream ss;
        ss << n;
        string s;
        ss >> s;
        return s;
    }

    void ReceiveSelectedFile(string FileName) {
        string ClientPath = ".//Storage//Client//";
        ClientPath.append(FileName);
        file = fopen(ClientPath.c_str(), "wb");
        if (file) {
            cout << "||Client Log|| : File Creted." << endl;
        } else {
            cout << "||Client Side Error|| : File creation failed, Exiting Program Now..." << endl;
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
                // printf("__errno2 = %08x\n", errno);
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
        // cin.clear();
        // getline(cin, val);

        if (val == "GETFL") {
            C.SendMessage("GETFL");
            string rev = C.WaitForMessage();
            // cout<<rev<<endl;
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
            // str.append(""+in);
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