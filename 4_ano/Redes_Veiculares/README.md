# Intelligent Road Safety Based on Fog Computing

Implementation of a collision avoidance algorithm to improve mobility.

# Technologies Used
- Eclipse Mosaic
- Sumo and NetEdit
- Java


# Implementation
- Entities: 
  - RSU
  - TrafficLight
  - Fog Node
  - Vehicles
  
Configuration in the applications (Java) of each entity to allow Ad-Hoc communication between Vehicles and RSU and cellular communication between RSU and Fog Node. 

- The Fog Node is where vehicle information is processed and traffic metrics such as number of vehicles, average speed and average delay are calculated. 

  
### Messages
- Car characteristics
- Traffic metrics
- Speeding alert
- Collision warning. 

### Collision Risk Detection Algorithm

The algorithm simulates the future position of vehicles and generates alerts if the distance between them is less than 1 meter. 

The traffic light is forced to turn red if there is a risk of collision.  



  
