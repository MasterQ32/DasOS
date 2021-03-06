# DasOS

## Goal

Simple, single task, single thread OS with a focus on graphics and gaming with support of networking,
graphics and sound. All programs will run in fullscreen with a small OS status bar at the bottom.

## Planned Features

- FAT12 Driver (*done*)
- FAT16 Driver (*pending*)
- Keyboard Input (*done*)
- Sound Driver (*pending*)
- Network (IP)
	- TCP (*pending*)
	- UDP (*pending*)
- Graphical Interface
	- VBE (*done*)
	- TinyGL (*pending*)
	- Font Rendering (*done*)
- Graphical Shell
	- Start program (*pending*)
	- Options (*pending*)
	- Shutdown (*pending*)

## Architecture

### Memory Layout
Everything in the first gigabyte of virtual memory is designated for the kernel,
everything above is available for the user programs.

### User Programs
User Programs will have their entry point at 0x40000000, all memory above is available for the user.

### Kernel API
The kernel will export all required functions via simple calls, no syscall interface required.

## Roadmap

### 0.1
- System Initialization
- Memory Layout

### 0.2
- Userspace ELF Loading

### 0.3 
- Keyboard Driver
- Kernel Keyboard API

### 0.4
- Graphics Driver
- -TinyGL- (too many dependencies for this version)
- Kernel Graphics and GL API

### 0.5
- Block Device Driver
- FAT12/16 Driver (FAT32 is done later)
- Kernel File System API

### 0.55
- Font Rendering
- Font Loading

### 0.6
- Shell with
	- File Listing
	- Program Execution
	- Debug Interface
	- System Options
	- Shutdown

### 0.7
- PCI Driver
- Sound Driver (dev:???)
- Kernel Sound API

### 0.8
- Network Driver (dev:???)
- TCP/IPv4 Stack
- UDP/IPv4 Stack
- Kernel Network API
	- UDP
	- TCP
	- DNS Query

### 0.9
- Text Editor
- Web Browser (HTTP → Text only)
- $(BASIC) Interpreter

### 1.0
- Finishing Touches
- Fixes of Known Bugs

