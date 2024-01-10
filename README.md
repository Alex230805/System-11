# Hardware Architecture

-   $0000-$2fff = low ram:
    <strong>used for System variable</strong>

-   $3000-$3fff = Device:
    <strong>Io port</strong>

-   $4000-$8fff = Hight ram:
    <strong>Banked 512kb-1MB of Hight ram shared with the graphics card</strong>

-   $9000-$ffff = OS Rom
    <strong>Rom with the os and the default software</strong>

# CF Card Support

System-11 support out of the box a cf card simple support with an integration at file system level (<strong>ZenithFS</strong>).


The device driver is loaded on boot as a kernel module.The Device status is saved in the low ram where kernel variable and kernel state variable are stored.


The Driver is designed to use an address space up to 32bit, wich allow the system to store at least 4gb of data into the hard drive. 


The device is mounted in the user session of BASH after the kernel initialize both the Driver and the zenithFS support. 


The device is usable in 2 different mode: 16bit and 32bit.
Those type of mode are handled by the File System and it's possible to modify is operation by a simple command via bash.


# SD card support


On the other side the SD card support allow the user to store data on it only as an external memory support. 


The system use by default the CF Card support and at the moment it's not possible to use the SD as a main storage.

# System Capabilites:

#### I2C Serial Bus Support

A i2c serial bus support is present by default in the driver module loaded on the bootstrap process.


- It use the 65c22 interface


#### General Serial Port

Serial port are supported by default


- It use the 65c22


- Allow the integration of the i2c portocol


# Kernel layout