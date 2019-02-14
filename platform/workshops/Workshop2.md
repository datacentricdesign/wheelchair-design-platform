


## Raspberry Pi

The next step consists in setting up the RaspberryPi and running your code on it.

Insert the SD card in your laptop.

Download and install the software 

Get the lattest Raspbian here: https://www.raspberrypi.org/downloads/raspbian/

Unzip the file, you obtain an image file (extension .img)

To install this image on the SD card, download and install Etcher: https://www.balena.io/etcher/

Starting Etcher, you first select your image file, then your SD card, and 'Flash'.

To connect to the Raspberry Pi without monitor, mouse and keyboard, we use directly your laptop. To do this, we need to enable the ssh protocol on the Raspberry Pi (secure remote access to a computer).

On your laptop, open text editor (TextEdit on Mac, Editor on Windows) and save an empty file named 'ssh' (without extension). This file will indicate that we want to enable ssh.

(add process for netword auto config)

Eject the SD card and insert it in on the Raspberry Pi, then power the Pi

(Scan and find your IP address)

Open a terminal (or 'command prompt' on Windows). Type in

```
ssh pi@<your ip address>
```

Update

```
sudo apt-get update
sudo apt-get upgrade
```

Create a folder

```
mkdir wheelchair
cd wheelchair
```

Set up Git

```
sudo apt-get git
```

Clone your GitHub repository

git clone


* Main Components

* Architecture

* Setting up the Raspberry Pi

* Network

* Bash commands