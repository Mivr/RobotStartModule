# RobotStartModule

Our goal is to make the start protocol originally used by RobotChallenge in Vienna, a worldwide available and affordable start stop method for robotic races.

## The software project:
### 17/1/2017 - Update on production files and some bugs
#### Progress:
- we had a bug where KILL set LOW when starting in STARTED mode - FIXED
- the new batch of ATTINY13A IC's was with a not working 4.8MHz crystal, the issue was fixed by using the 9.6MHz clock and adjusting some constants in the code
- some minor refactoring was made

#### Issues / TODO's:
- the code is still unclear to read and messy more refactoring is needed
- the RC5 decoding is taking too much program space as it is now, it needs to be made in assembly and integrated

## The hardware project:
Can be seen here with full designs and production files: http://circuitmaker.com/Projects/Details/Mihail-Vrachanski/Start-Module

## The business project:
### Goal
To make a set of products that allow beginning and professional users to utilize the start protocol.

#### Strategy
Lean approach - develop the most needed part -> sell to customers -> get feedback -> start over
With this strategy we expect to provide just the product that is needed on the market.

### Short term goals
Based on feedback a remote is needed as most users do not obtain a phone that can serve as a remote.
    -> The task is to provide the users with a remote.
        ->  MVP will contain just an Arduino sketch and an IR diode.

#### MVP - Arduino remote
As most users obtain an Arduino or compatible board it is needed to make a board independent sample code that is capable of sending basic functions as program start and stop with given by the user settings. For better availability the IR diode needed will be added to a start module purchase.

### Marketing
Current marketing of the product is just a listing in Ebay: http://www.ebay.com/itm/142152118677

#### Future plans
The goal is to start supplying web shops for robotics with the module, for this it is needed the whole project to gain some popularity and the module to send some counts so the web shops can see the use of selling it.
