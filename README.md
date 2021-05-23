# lidar-scan

Simple and cross-platform tool for grabbing data from RPLIDAR A3A1 scanner. Based on [lidar-visualizations](https://github.com/knei-knurow/lidar-visualizations).

## Gallery

Video: [https://www.youtube.com/watch?v=MQhYTqz40xI](https://www.youtube.com/watch?v=MQhYTqz40xI)

Indoor – walking around the house:

![doc/screenshots/garden.gif](https://raw.githubusercontent.com/knei-knurow/lidar-visualizations/main/doc/screenshots/room.gif)

Outdoor near street – passing cars can be seen:

![doc/screenshots/cars.gif](https://raw.githubusercontent.com/knei-knurow/lidar-visualizations/main/doc/screenshots/cars.gif)

Outdoor – lots of trees and shrubs around:

![doc/screenshots/garden.gif](https://raw.githubusercontent.com/knei-knurow/lidar-visualizations/main/doc/screenshots/garden.gif)

## About RPLIDAR

This project has been created with the low-cost **Slamtec RPLIDAR A3M1** device, and SDK
provided by its manufacturers. We haven't tested it, but should be compatible with other
related Slamtec models. A3M1 supports two important scanning modes depending on a scanning
environment (indoor/outdoor).

| Property                | A3M1         |
| :---------------------- | ------------ |
| Distance Range          | up to 25 m   |
| Sample Rate             | up to 16 kHz |
| Scan Rate               | 5 Hz - 20 Hz |
| Angular Resolution      | up to 0.225° |
| Communication Interface | TTL UART     |
| Communication Speed     | 256000 bps   |

The complete RPLIDAR specification and documentation is available below:

- [A3M1 website](https://www.slamtec.com/en/Lidar/A3)
- [SDK GitHub](https://github.com/Slamtec/rplidar_sdk)
- [RPLIDAR A3 User Manual](https://download.kamami.pl/p573426-LM310_SLAMTEC_rplidarkit_usermanual_A3M1_v1.0_en.pdf)
- [RPLIDAR A3 Introduction and Datasheet](https://download.kamami.pl/p573426-LD310_SLAMTEC_rplidar_datasheet_A3M1_v1.3_en.pdf)

### RPLIDAR Modes

RPLIDAR supports several scanning modes which differs by its application (indoor/outdoor),
distance range, and sample rate. More details can be found in the documentation of the
product. You should only consider the option 3 and 4, because the first three ones are
here due to compatibility reasons and don't produce spectacular results.

| ID   | Scan mode        | Sample time [us] | Frequency |
| ---- | ---------------- | ---------------- | --------- |
| 0    | Standard         | 252              | 0.484406  |
| 1    | Express          | 126              | 0.968812  |
| 2    | Boost            | 63               | 1.93762   |
| 3    | Sensitivity      | 63               | 1.93762   |
| 4    | Stability        | 100              | 1.2207    |

### RPLIDAR software RPM vs actual RPM

| Software RPM | Actual RPM       | Software RPS | Actual RPS       | Error  |
| ------------ | ------------------ | ------------ | ------------------ | ------ |
| 200          | 196.62426442087457 | 3.33...      | 3.2770710736812427 | ~1.7%  |
| 400          | 432.0221331536197  | 6.66...      | 7.200368885893662  | ~8.0%  |
| 600          | 684.5153998304605  | 10.00        | 11.408589997174342 | ~14.0% |
| 800          | 965.1414495782639  | 13.33...     | 16.085690826304397 | ~20.6% |
| 1000         | 1268.981667825387  | 16.66...     | 21.14969446375645  | ~26.8% |

RPM to RPS:
```
RPM = (1 / 360_CLOUD_TIME) * 60
```

## Usage

`lidar-scan port [options]`

### Options

```
-h --help       Print this message
-m --mode       RPLIDAR mode (0 - 4)
-r --rpm        RPLIDAR revolutions* per minute (200 - 1023)
--reset         Try to reset the RPLIDAR driver and exit
```
\* [the actual rpm might differ](#rplidar-software-rpm-vs-actual-rpm).

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

## Usage

When you have the `lidarvis` executable , you are able to start some scanning and visualizing.
The program can be controlled via command line in such a way:

```sh
lidar [options]
```

### Point cloud output

Output contains a series of point clouds. Each line (except for comments which must start with `#`) represents a single point which consists of an **angle value [°]** and a **distance value [mm]**. Both may be a floating
point number, and have to be separated by any kind of white characters. Lines
starting with `!` separates two point clouds. Each line marked with `!` should consist
of the **ID number of the following point cloud** and **number of milliseconds elapsed from grabbing the previous one**. Clouds should be sorted by their ID number.

Example:

```
# A comment
# ! ID Number   Elapsed time [ms]
# Angle [°]   Distance [mm]
! 0 0
120  100
240  100
360  100
! 1 500
120  200
240  200
360  200
! 2 500
120  300
240  300
360  300
! 3 500
```

Preview:

```
lidar -fs datasets/example-series.txt
```

![doc/screenshots/example.gif](https://raw.githubusercontent.com/knei-knurow/lidar-visualizations/main/doc/screenshots/example-series.gif)

## Thanks

This project was developed within Electronics and Computer Science Club in Knurów
(KNEI for short) where lots of amazing projects and ideas come from. Have a look at
our website - [https://knei.pl/](https://knei.pl/) - unfortunately, at the moment,
only available in Polish. Check out our GitHub too - https://github.com/knei-knurow.

Numerous packages with colourful electronic gadgets like RPLIDAR have been granted to
us by our friends from [KAMAMI.pl](http://kamami.pl).
