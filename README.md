# Starter module

Goal of this project is to make a single module (aka Starter) that you can use on any race in the world for starting your robot. At the same time the module aims to be reliable as it is a safety critical part in some cases.

# Arduino remote
As of 1/5/2017 A simple to use library is available so anyone with an Arduino or compatible board can make a remote in a few minutes. Just copy the "StarterRemote" folder to your Arduino library folder and restart the Arduino IDE.

# The software project:
## Summary:
The software was started on the base of this module: http://startmodule.com/ .
Most of the SW had to be rewritten as it was far from clear to read code, nor it compiled to a good size (initial size was >1KB, and Attiny13A has just 1KB). Currently the approach is to make an event based application (combining the two protocols for start modules I am aware of for now), and a driver layer that sends the events when needed. Future updates will contain WD for safety and some UI functions that I expect to make the module easier to use.
## Updates:
### 1/5/2017 - Refactoring in progress
- New RC-5 decoding algorithm as the old one just took too much space (still to be tested).
- Event based main logic, made so expanding with more start protocols can be done.
- Update on production was made again as I had forgotten the BOD protection, now the fuses are set right after module is reprogrammed.
### 17/1/2017 - Update on production files and some bugs
#### Progress:
- we had a bug where KILL set LOW when starting in STARTED mode - FIXED
- the new batch of ATTINY13A IC's was with a not working 4.8MHz crystal, the issue was fixed by using the 9.6MHz clock and adjusting some constants in the code
- some minor refactoring was made

#### Issues / TODO's:
- the code is still unclear to read and messy more refactoring is needed
- the RC5 decoding is taking too much program space as it is now, it needs to be made in assembly and integrated

# The hardware project:
Can be seen here with full designs and production files: http://circuitmaker.com/Projects/Details/Mihail-Vrachanski/Start-Module