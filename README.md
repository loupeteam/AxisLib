# Info
Package is provided by Loupe  
https://loupe.team  
info@loupe.team  
1-800-240-7042  

# Description
This repo provides packages that allow basic control of B&R servo axes.  
### Program Package
This program package will install the Servo B&R package that will contain the ServoMgr program for a Automation Studio project and all its dependencies.
The ServoMgrInit program will perform simple initialization of parameters if needed.
The ServoMgrCyclic program will call the AxisBasicFn_Cyclic function block which handles much of the basic axis control.
The cyclic program also calls the axisErrorCollector function which will support axis error handling.

### Library Package
This library package allows access to the servo basic function blocks.

# Installation
To install using the Loupe Package Manager (LPM), in an initialized Automation Studio project directory run `lpm install servomgracp10prog`.  Note that this will also pull in the library package as a dependency.  
If you only want to install the library package, run `lpm install axislib`.  
For more information about LPM, see https://loupeteam.github.io/LoupeDocs/tools/lpm.html.

# Documentation
For more documentation and examples, see https://loupeteam.github.io/LoupeDocs/libraries/axislib.html (or you can run `lpm docs axislib`).
# Licensing

This project is licensed under the [MIT License](LICENSE).