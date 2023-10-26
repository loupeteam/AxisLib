# Info
Library is provided by Loupe  
https://loupe.team  
info@loupe.team  
1-800-240-7042  

# Description
The AxisLib library provides a clean interface for basic motion control.

Every motion axis has a set of basic functionality in common. This includes powering and homing the axis, performing basic moves, and reading back status information. In addition to this basic functionality, every axis has its own set of specific functionality. Since it is impossible to make one piece of software that can handle both the common and specific functionality of any arbitrary axis, the software that handles common functionality must not interfere with axis-specific functionality.

The AxisLib library bundles the most common motion control commands and status information together into one clean interface. This leads to significantly faster development of motion applications. Also, the library is designed to work in parallel with any other motion functionality. This allows any application specific motion code to be layered on top of the basic functionality while still taking advantage of all the library has to offer.

# Use Cases
The AxisLib functionality can be integrated into any project using a data structure and a function call.

For more documentation and examples, see https://loupeteam.github.io/LoupeDocs/libraries/axislib.html

# Installation
To install using the Loupe Package Manager (LPM), in an initialized Automation Studio project directory run `lpm install axislib`. For more information about LPM, see https://loupeteam.github.io/LoupeDocs/tools/lpm.html

## Licensing

This project is licensed under the [MIT License](LICENSE.md).