#!/usr/bin/env python3
"""

Things that we can measure:
    CLOUD-BY-CLOUD SCAN:
    - number of clouds (total)
    - number of point (total)

    - number of points per cloud (the latest one)
    - number of points per cloud (mean)
    - bad points ratio for cloud (the latest one)
    - bad points ratio for cloud (mean)

    - time per cloud (the latest one)
    - time per cloud (mean)
    
    - angle step (mean for the latest cloud)
    - time step (mean for the latest cloud)
    
    POINT-BY-POINT SCAN:

"""
import sys

PREFIX = "lidar-diag:"



class Diagnose:
    def __init__(self):
        self.points = 0
        self.clouds = 0
        self.lines = 0

        self.points_in_cloud = 0
        self.points_per_cloud = 0
        self.points_per_cloud_avg = 0
        self.bad_points_per_cloud = 0
        self.bad_points_per_cloud_avg = 0
        
        self.time_per_cloud = 0
        self.time_per_cloud_avg = 0
        
        self.angle_step_sum = 0
        self.angle_step_avg = 0
        self.time_step_sum = 0
        self.time_step_avg = 0

    def parse(self, line):
        raise NotImplementedError()

    def write(self, refreshable=False):
        raise NotImplementedError()


class DiagnosePointByPoint(Diagnose):
    def __init__(self):
        super().__init__()
    
    def parse(self, line):
        pass

    def write(self, refreshable=False):
        pass

class DiagnoseCloudByCloud(Diagnose):
    def __init__(self):
        super().__init__()
    
    def parse(self, line):
        split = line.split()
        
        # Empty line
        if not line:
            return

        # Commnets
        if line[0] == "#":
            return

        # Cloud start
        if line[0] == "!":
            clo_no, clo_timestamp = split[1], split[2]
            self.clouds += 1
            self.points_per_cloud = self.points_in_cloud
            self.points_per_cloud_avg = self.points / self.clouds
            self.points_in_cloud = 0
            return
        
        # Point
        angle, dist = float(split[0]), float(split[1])
        self.points += 1
        self.points_in_cloud += 1
        if dist == 0:
            self.bad_points_per_cloud += 1
    
    def write(self, refreshable=False):
        line = "{:5}c  {:5} ({:5.2f})p/c".format(self.clouds, self.points_per_cloud, self.points_per_cloud_avg)
        print(PREFIX, line, end="\r" if refreshable else "\n")

def main():
    print(PREFIX, "Start.")
    diag = DiagnoseCloudByCloud()
    try:
        while True:
            line = input()
            diag.parse(line)
            diag.write(True)
    except KeyboardInterrupt:
        diag.write()
        print(PREFIX, "Exit signal recieved.")

if __name__ == "__main__":
        main()