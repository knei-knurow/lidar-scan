# lidar-scan

Simple and cross-platform tool for grabbing data from RPLIDAR A3A1 scanner.

## Usage

`lidar-scan port [options]`

### Options

```
-h --help       Print this message
-m --mode       RPLIDAR mode (0 - 4)
-r --rpm        RPLIDAR revolutions per minute (200 - 1023)
--reset         Try to reset the RPLIDAR driver and exit
```

## Compilation

### Linux, macOS

1. Install [rplidar_sdk](https://github.com/Slamtec/rplidar_sdk):

   `$ ./install_rplidar`

2. Build:

   `$ make`

3. Run:

   `$ ./lidar-scan`

4. Install (Optional):

   `$ make install`

### Windows (Visual Studio)

coming soon (or never)
