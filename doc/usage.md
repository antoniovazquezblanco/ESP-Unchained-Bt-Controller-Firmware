# Usage

## What this is

The firmware turns the board into a **bare Bluetooth HCI controller** that speaks the standard **HCI** transport.
There is no on-chip host stack meaning that to the computer it is simply another Bluetooth controller.

The advantages is that this Bluetooth controller has some vendor commands that allow us to do things such as changing the MAC address of the device.

This means that you will be able to use this as any other serial Bluetooth controller in your machine.
