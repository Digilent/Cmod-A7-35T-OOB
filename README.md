Cmod A7-35T Out of Box Demo 
====================

Description
-----------

This project demonstrates how to use the Cmod A7-35T's Artix 7 FPGA's Microblaze processor with the SRAM, LED's, RGB LED's, Pushbuttons and the USB UART bridge. Vivado is used to build the demo's hardware platform, and Xilinx SDK is used to program the bitstream onto the board and to build and deploy a C application. 

To use this demo, the Cmod A7-35T must be connected to a computer over MicroUSB, which must be running a serial terminal. For more information on how to set up and use a serial terminal, such as Tera Term or PuTTY, refer to [this tutorial](https://reference.digilentinc.com/vivado/installing-vivado/start).

Whenever the demo is started from Vivado SDK, the board will go through a memory test and will print out the result to the serial terminal. Whenever one of the buttons is pressed, the line “Button Pressed” is sent. Every time BTN1 is pressed, the LED state changes. The user LEDs create a 2 bit binary counter that increments every time the button is pressed.  The RGB LED smoothly transitions between colors.

Requirements
------------
* **Cmod A7-35T**: To purchase a Cmod A7-35T, see the [Digilent Store](https://store.digilentinc.com/cmod-a7-breadboardable-artix-7-fpga-module/).
* **Vivado 2020.1 Installation with Xilinx SDK**: To set up Vivado, see the [Installing Vivado and Digilent Board Files Tutorial](https://reference.digilentinc.com/vivado/installing-vivado/start).
* **Serial Terminal Emulator**: 
* **MicroUSB Cable**

Demo Setup (v2020.1-1)
----------
**Note:** *Other releases may require other steps be taken to use them. Make sure to check the version of this README found in the commit associated with that release's tag for instructions.*

1. Download the most recent release ZIP archives from the repo's [releases page](https://github.com/Digilent/Cmod-A7-35T-OOB/releases). 
2. Open Vivado 2020.1.
3. Open the XPR project file, found at \<archive extracted location\>/release/vivado-proj/Cmod-A7-35T-OOB.xpr, included in the extracted hardware release in Vivado 2020.1.
4. No additional steps are required within Vivado. The project can be viewed, modified, and rebuilt, and a new platform can be exported, as desired.
5. Open Vitis 2020.1. Choose an empty folder as the *Workspace* to launch into.
6. With Vitis opened, click the **Import Project** button, under **PROJECT** in the welcome screen.
7. Choose *Vitis project exported zip file* as the Import type, then click **Next**.
8. **Browse** -sdk_appsrc- folder (found in the downloaded archive) **Open** it.
9. Make sure that all boxes are checked in order to import each of the projects present in the archive will be imported, then click **Finish**.
10. Connect the CMOD A7 to your computer via the MicroUSB programming cable, make sure the power source select jumper is set to USB or WALL (depending on whether you are using an external supply), then power on the board by flipping the power switch to the ON position.
11. Open a serial terminal application (such as TeraTerm or PuTTY) and connect it to the CMOD-A7-35T's serial port, using a baud rate of 9600.
12. In the *Assistant* pane at the bottom left of the Vitis window, right click on the project marked `[System]`, and select **Run** -> **Launch Hardware**. When the demo is finished launching, messages will be able to be seen through the serial terminal, and the buttons can be used as described in this document's *Description* section, above.

Next Steps
----------
This demo can be used as a basis for other projects by modifying the hardware platform in the Vivado project's block design or by modifying the Vitis application project.

For technical support or questions, please post on the [Digilent Forum](forum.digilentinc.com).

Additional Notes
----------------
For more information on how this project is version controlled, refer to the [digilent-vivado-scripts](https://github.com/digilent/digilent-vivado-scripts) and [digilent-vitis-scripts](https://github.com/digilent/digilent-vitis-scripts) repositories.