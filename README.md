# Info
Library is provided by Loupe  
https://loupe.team  
info@loupe.team  
1-800-240-7042  

# Description
ABB provides a REST API for interfacing with its robots, called Robot Web Services. This library implements an HTTP client to handle communication between a B&R PLC and an ABB IRC’s Robot Web Services server. It thereby provides read and write access to a wide range of settings and parameters on the IRC, as ABB has exposed most of its inner workings via this API. 

# Use Cases
Many different types of process-level communication between the two systems are possible using this library:

-Retrieve the current state of the IRC (RAPID state, program execution state, etc)  
-Read/write the value of an IO signal on the IRC  
-Read/write the value of a data symbol (i.e. RAPID var) on the IRC  
-Control program execution (start, stop, reset PP)  

For more documentation and examples, see https://loupeteam.github.io/LoupeDocs/libraries/a3br.html

# Installation
To install using the Loupe Package Manager (LPM), in an initialized Automation Studio project directory run `lpm install a3br`. For more information about LPM, see https://loupeteam.github.io/LoupeDocs/tools/lpm.html

## Licensing

This project is primarily licensed under the [MIT License](LICENSE.md). However, it includes components under the [Apple Public Source License (APSL)](LICENSE-APSL.md) and the [Apache Group License](LICENSE-Apache.md). Please refer to the respective files for full license texts.

base64.c : [Apple Public Source License (APSL)](LICENSE-APSL.md) AND [Apache Group License](LICENSE-Apache.md)

base64.h : [Apple Public Source License (APSL)](LICENSE-APSL.md) AND [Apache Group License](LICENSE-Apache.md)