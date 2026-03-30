# Lab 1 – System Fingerprint

Course: Software Security

## Objective

The goal of this lab is to obtain system hardware information and
understand how hardware parameters can be used to create a system fingerprint
for software licensing and protection.

## Collected Information

The following parameters were collected:

- CPU ID
- RAM information
- Disk information
- MAC address
- Operating System version

## Windows Commands

The following commands were used in Windows:

wmic cpu get ProcessorId
systeminfo
wmic diskdrive get serialnumber
getmac

## Bash Script

The Linux script collects system information automatically.

File: system_info.sh

## C++ Program

The C++ program executes system commands and prints hardware information.

File: system_info.cpp

## System Fingerprint

Hardware parameters can be combined and hashed (SHA256) to create a
unique identifier for the system.

Example command:

echo "$(lscpu | grep 'Model name')$(ip link | grep ether)" | sha256sum

## Result

The collected data demonstrates how software licensing systems
can bind applications to specific hardware.
