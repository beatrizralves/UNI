# Smart Home Monitoring and Control System

The project aims to simulate a **smart home** by collecting data from sensors and controlling actuators to ensure comfort, energy efficiency, and safety. The system follows an IoT-based architecture, allowing remote control and monitoring through a central web server and mobile/desktop applications.

## Project Objectives

* Monitor environmental variables (e.g., temperature, humidity, rain detection).
* Implement a central system that aggregates and stores sensor data.
* Create a responsive and intuitive **web interface** for administrators and clients.
* Develop a **mobile application** allowing:

  * User registration
  * Sensor system registration
  * Real-time data access
  * Visualizations via charts


## Tools & Technologies

| Tool/Platform          | Purpose                                 |
| ---------------------- | --------------------------------------- |
| **Arduino IDE**        | Programming ESP32 boards (sensor logic) |
| **Visual Studio Code** | Development of web server/API/app code  |
| **MySQL**              | Relational database                     |
| **Draw\.io**           | Block and architecture diagrams         |
| **GanttProject**       | Project scheduling                      |
| **Discord**            | Team communication                      |
| **Overleaf**           | Reporting and documentation             |


## Architecture

### Web Server

* Serves both admin and client users.
* Displays real-time sensor data.
* Handles user authentication and authorization.
* Shows charts (e.g., temperature over time).

### Relational Database

* Links users, sensor systems, and individual sensors.
* Stores sensor readings and user metadata.
* Entity-relationship model implemented in Phase B, expanded in Phase C.

### API Endpoints (examples):

| Route                | Method | Description                    |
| -------------------- | ------ | ------------------------------ |
| `/ss/:idSS`          | GET    | Get info about a sensor system |
| `/ss/:idSS/sensors`  | GET    | List sensors in the system     |
| `/ss/:idSS/readings` | GET    | Fetch readings for sensors     |
| `/ss/`               | GET    | List all sensor systems        |


## Client Application 

* Allows users to:

  * Register and log in
  * Link sensor systems to their account
  * View live and historical data
* Cross-platform UI (mobile-first), implemented with technologies TBD
* Integration with the existing API

