#!/usr/bin/env python3
import sys

PREFIX = "lidar-diag:"

def main():
    pts_count = 0
    pts_empty = 0
    
    clo_count = 0


    print(PREFIX, "Start.")
    try:
        while True:
            line = input().split()
            if line:
                pts_count += 1
            angle, dist, 

            print(PREFIX, pts_count, end="\r")
    except KeyboardInterrupt:
        print(PREFIX, count)
        print(PREFIX, "Exit signal recieved.")

if __name__ == "__main__":
        main()