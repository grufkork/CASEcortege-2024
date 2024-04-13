
# CASE CORTEGE 2024 

This repository contains the code used for one of the CASE-lab's vehicles for the Chalmers cortège. The vehicle is built primarily by "aspiring" (ASP) members.

**The Cortège starts april 30th, at 18:15**

The vehicle is a moving "Dance Dance Revolution" platform. The vehicle is controlled remotely using the ESPNOW protocol.

With minor modification, the platform can be reused for other purposes. 


## Repo info
### Vehicle control
The folders cortege_remote and cortege_on_esp contain the code used to control the vehicle. 

**Cortege_on_esp** contains the onboard firmware. The program recieves data from the remote and controls the motors. It applies heavy smoothing to make vehicle control less jerky.

**cortege_remote** is the software used for the remote. The program sends data retrieved from two linear potentiometers. Some deadzone and a "dead man's switch" is applied for ease of use and safety.

### Dancing platform
