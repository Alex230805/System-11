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

System-11 supporta immediatamente una semplice integrazione della scheda CF a livello di file system (<strong>ZenithFS</strong>).

Il driver del dispositivo viene caricato all'avvio come modulo del kernel. Lo stato del dispositivo viene salvato nella ram bassa dove sono archiviate la variabile del kernel e la variabile dello stato del kernel.

Il driver è progettato per utilizzare uno spazio di indirizzi fino a 32 bit, che consente al sistema di archiviare fino a 4 GB di dati nel disco rigido.

Il dispositivo viene montato nella sessione utente BASH dopo che il kernel ha inizializzato sia il driver che il supporto zenithFS.

Il dispositivo è utilizzabile in 2 diverse modalità: 16bit e 32bit. Questo tipo di modalità è gestita dal File System ed è possibile modificare il suo funzionamento con un semplice comando tramite bash.


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