# Fire Detection System – Operating Systems Project

The objective is to implement a system in **C (on UNIX-based systems)** that simulates a **fire detection system using binary image analysis**, **multi-process coordination**, **inter-process communication (IPC)** via **pipes**, and basic **client-server architecture** using **named pipes (FIFOs)**.


### Objective 1 – Binary Image Analysis and Alarm Detection

* Reads lines from a text file specifying binary files and coordinates.
* Creates child processes to scan each binary file for fire pixel patterns.
* Child processes report alarms via anonymous pipes.
* The parent manages up to **4 concurrent processes**, synchronizing using `wait()`.

### Objective 2 – Alarm Coordination and Email Notification

* Receives a maximum number of alarms to process.
* Uses a pipe and a child process to launch an email-sending script.
* Demonstrates inter-process data exchange via redirection and `execl()`.

### Objective 3 – Client-Server Model (Partially Implemented)

* Uses named pipes (FIFOs) for communication between `pesquisacli` (client) and `pesquisasrv` (server).
* Client sends the number of alarms and file to scan.
* Server uses the logic from Objective 1 to analyze the file and return results.
* Implementation was started but not finalized due to complexity.

