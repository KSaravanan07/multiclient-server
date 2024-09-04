
# Socket-Programming-Multiple-Client-in-c++
---

## Assignment 2 - CS5060: ACN
### Socket Programming: Multi Client Server using C++

```
Name     : K Saravanan
Roll No. : ************
```

* Both Part A & B folders Contains files for server & client.
* Consider folder named Storage which is there in Both Part A & B 
* It has a sub folders
  * Server - this is where server files are stored.
  * Client - this is where client will save downloaded files.
  
<br/>

# ``How To Run Code?``
## `Part A`


 * First we Have to compile the code in PartA folder
```bash
g++ -o server Server.cpp 
g++ -o client Client.cpp 
```

<br/>

### Run Server with
---  
```
./server
```
* follow this step by step
  * Server will run on 127.0.0.1 by default
  * Enter Port No to host server on
  * it will start listing on that port No. 

<br/>

### Run Client with
---
```
./client
```

* follow this step by step
  * Server will run on 127.0.0.1 by default
  * 3 option Will be shown
  
  * ```
    ------------------------------------------------------
    Commands : 
    GETFL -  To Get a File List from Server
    GET   -  To Get a File
    BYE   -  To TERMINATE Program on Client Side

    Enter Command :
    ```
  
  * First Enter `GETFL` in order to get list of files available on server (must)
  * you will get something like this
  * ```
    index name
    0   .
    1   ..
    2   Assignment2.pdf
    3   test3.png
    4   test2.jpg
    5   test1.png
    6   CPP-Cheatsheet.pdf
    7   server_text.txt
    8 
    ------------------------------------------------------
    USE Command GET inorder to get that file
    ------------------------------------------------------
    ```
  * Now Use `GET` Command to Get Requird File with appropriate Index Number
  * It will ask for Index of file
  * ```
    PLEASE ENTER INDEX OF FILE :  
    ```
  * Enter proper index number - `EX : 3 for test3.png`
  * Please do not enter improper index number `EX : 0,1,9,10,22`
  * it will Download that file like
  * ```
    ||Client Log|| : File Creted.
    ||Client Log|| : Total Size Of File to be Receive 3459455
    ||Client Log|| : Number of Bytes Receive  : 51200 | Total Data Sent 51200
    ||Client Log|| : Number of Bytes Receive  : 51200 | Total Data Sent 102400
    ||Client Log|| : Number of Bytes Receive  : 51200 | Total Data Sent 153600
    .
    .
    .
    ||Client Log|| : Number of Bytes Receive  : 51200 | Total Data Sent 3430400
    ||Client Log|| : Number of Bytes Receive  : 51200 | Total Data Sent 3481600

    ||Client Log|| : Received 3481600 Bytes of Data
    ||Client Log|| : Data Not Received 0 Bytes
    ||Client Log|| : File Saved Successfully.
    ```
  * Ypu can Use `BYE` Command to Terminate the connection
  
---
## ``Part B``

 * First we Have to compile the code in PartB folder
```bash
g++ -o server Server.cpp -lpthread 
g++ -o client Client.cpp 
```

<br/>

### Run Server with
---  
```
./server
```
* follow this step by step
  * Server will run on 127.0.0.1 by default
  * Enter Port No to host server on
  * it will start listing on that port No. 

<br/>

### Run Client with
---
## Here we have to run Multiple Clients, Here max Limit is set to `3`. To test this open 2 terminals and run this in both of them
```
./client
```

* follow this step by step
  * Server will run on 127.0.0.1 by default
  * 3 option Will be shown
  
  * ```
    ------------------------------------------------------
    Commands : 
    GETFL -  To Get a File List from Server
    GET   -  To Get a File
    BYE   -  To TERMINATE Program on Client Side

    Enter Command :
    ```
  
  * First Enter `GETFL` in order to get list of files available on server
  * you will get something like this
  * ```
    index name
    0   .
    1   ..
    2   Assignment2.pdf
    3   test3.png
    4   test2.jpg
    5   test1.png
    6   CPP-Cheatsheet.pdf
    7   server_text.txt
    8 
    ------------------------------------------------------
    USE Command GET inorder to get that file
    ------------------------------------------------------
    ```
  * Now Use `GET` Command to Get Requird File with appropriate Index Number
  * It will ask for Index of file
  * ```
    PLEASE ENTER INDEX OF FILE :  
    ```
  * Enter proper index number - `EX : 3 for test3.png`
  * it will Download that file like
  * ```
    ||Client Log|| : File Creted.
    ||Client Log|| : Total Size Of File to be Receive 3459455
    ||Client Log|| : Number of Bytes Receive  : 51200 | Total Data Sent 51200
    ||Client Log|| : Number of Bytes Receive  : 51200 | Total Data Sent 102400
    ||Client Log|| : Number of Bytes Receive  : 51200 | Total Data Sent 153600
    .
    .
    .
    ||Client Log|| : Number of Bytes Receive  : 51200 | Total Data Sent 3430400
    ||Client Log|| : Number of Bytes Receive  : 51200 | Total Data Sent 3481600

    ||Client Log|| : Received 3481600 Bytes of Data
    ||Client Log|| : Data Not Received 0 Bytes
    ||Client Log|| : File Saved Successfully.
    ```
  * You can Use `BYE` Command to Terminate the connection
  
---


### PLAGIARISM STATEMENT 
I certify that this assignment/report is my own work, based on my personal study and/or research and that I have acknowledged all material and sources used in its preparation, whether they be books, articles, reports, lecture notes, and any other kind of document, electronic or personal ommunication. I also certify that this assignment/report has not reviously been submitted for assessment in any other course, except where specific permission has been granted from all course instructors involved, or at any other time in this course, and that I have not copied in part or whole or otherwise plagiarized the work of other students and/or persons. I pledge to uphold the principles of honesty and responsibility at CSE@IITH. In addition, I understand my responsibility to report honor violations by other students if I become aware of it.

### `Name of the student : K Saravanan`
### `Roll No : *************`

---

---
## PS : IF SOME ERROR OCCURED OR DATA IS CORRUPTED PLEASE DO RESTART THE SERVER, AND TRY AGAIN  :) 
# `Thank You!`
---

