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

Demo Setup
----------

1. Download the most recent release ZIP archive ("Cmod-A7-35T-OOB-2018.2-*.zip") from the repo's [releases page](https://github.com/Digilent/Cmod-A7-35T-OOB/releases).
2. Extract the downloaded ZIP.
3. Open the XPR project file, found at \<archive extracted location\>/vivado_proj/Cmod-A7-35T-OOB.xpr, included in the extracted release archive in Vivado 2020.1.
4. In the toolbar at the top of the Vivado window, select **File -> Export -> Export Hardware**. Select **\<Local to Project\>** as the Exported Location and make sure that the **Include bitstream** box is checked, then click **OK**.
5. In the toolbar at the top of the Vivado window, select **File -> Launch SDK**. Select **\<Local to Project\>** as both the workspace location and exported location, then click **OK**.
6. With Vivado SDK opened, wait for the hardware platform exported by Vivado to be imported.
7. In the toolbar at the top of the SDK window, select **File -> New -> Application Project**.
8. Fill out the fields in the first page of the New Application Project Wizard as in the table below. Most of the listed values will be the wizard's defaults, but are included in the table for completeness.

| Setting                                 | Value                             |
| --------------------------------------- | --------------------------------- |
| Project Name                            | Cmod-A7-35T-OOB                   |
| Use default location                    | Checked box                       |
| OS Platform                             | standalone                        |
| Target Hardware: Hardware Platform      | user_35t_wrapper_hw_platform_0    |
| Target Hardware: Processor              | microblaze_0                      |
| Target Software: Language               | C                                 |
| Target Software: Board Support Package  | Create New (Cmod-A7-35T-OOB_bsp)  |

9. Click **Next**.
10. From the list of template applications, select "Empty Application", then click **Finish**.
11. In the Project Explorer pane to the left of the SDK window, expand the new application project (named "Cmod-A7-35T-OOB").
12. Right click on the "src" subdirectory of the application project and select **Import**.
13. In the "Select an import wizard" pane of the window that pops up, expand **General** and select **File System**. Then click **Next**.
14. Fill out the fields of the "File system" screen as in the table below. Most of the listed values will be the defaults, but are included in the table for completeness.

| Setting                                                | Value                                      |
| -                                                      | -                                          |
| From directory                                         | \<archive extracted location\>/sdk_appsrc  |
| Files to import pane: sdk_appsrc                       | Checked box                                |
| Into folder                                            | Cmod-A7-35T-OOB/src                        |
| Options: Overwrite existing resources without warning  | Checked box                                |
| Options: Create top-level folder                       | Unchecked box                              |

15. Click **Finish**.

<Note for maintainers: This project does not require any additional configuration of application or bsp projects. Projects that require any of this configuration should have the steps required to do so described here.>

16. Open a serial terminal application (such as TeraTerm) and connect it to the Cmod A7's serial port, using a baud rate of 9600.
17. In the toolbar at the top of the SDK window, select **Xilinx -> Program FPGA**. Leave all fields as their defaults and click "Program".
18. In the Project Explorer pane, right click on the "Cmod-A7-35T-OOB" application project and select "Run As -> Launch on Hardware (System Debugger)".
19.The application will now be running on the Cmod A7. It can be interacted with as described in the first section of this README.

Next Steps
----------
This demo can be used as a basis for other projects by modifying the hardware platform in the Vivado project's block design or by modifying the SDK application project.

Additional Notes
--------------
For more information on how this project is version controlled, refer to the [Digilent Vivado Scripts Repository](https://github.com/digilent/digilent-vivado-scripts)

