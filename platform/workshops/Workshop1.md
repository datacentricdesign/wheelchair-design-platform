# Workshop 1: Building an Internet-connected Wheelchair



## Activities

* Tools
* Setup Raspberry Pi
* Setup Arduino

## Tools

### Terminal

A terminal is a text-based interface we use to enter and execute commands on a computer.
While we will maximise the use of graphical tools, we will also use the terminal
in some occasion.

* On Windows, it is called 'Command Prompt'.
* On Mac, it is called 'Terminal'.

Go back to this text-based interface each time we refer to the 'terminal'.

### Git & GitHub

Git is a version control system (VCS). It helps keeping track of all changes
in your project and share it. While it is heavily used by software developer to
track and share code, it is also useful to track and share the progress of any
design or research process. The principle is as follows: you share a remote
repository, you 'pull' from this repository the latest changes (any files, code,
doc...) from your peers, you make changes on your machine, and you 'push' your
changes on the remote repository, making them available for your peers.

Here are four short videos with more details:

* https://git-scm.com/video/what-is-version-control
* https://git-scm.com/video/what-is-git
* https://git-scm.com/video/get-going
* https://git-scm.com/video/quick-wins

* Install Git

Windows / Mac

GitHub is a popular online platform that host remote Git repositories.
We will use it throughout the course to host your repo

* Sign up on GitHub

If you do not have GitHub account, sign up here: https://github.com/

* Fork repository (only one member of the group)

'A fork is a copy of a repository. Forking a repository allows you to freely
experiment with changes without affecting the original project.'
(GitHub help https://help.github.com/articles/fork-a-repo/)

On the top-right corner of this GitHub page, click on Fork. In the dialog,
select your account. You have now a copy of the wheelchair-design-platform
repository on your own account.

* Add members to repo (only one member of the group)

Your repository is publicly accessible for reading. However, you need to give other member of the group 
access to this repository. On the top menu, click 'Settings' > 'Collaborators'.
Search the username of your group members and add them.

* Create a project

GitHub offers a convenient way of managing your projects with a Kanban style
board. On the top menu, click on 'Projects' and click on the green button 'New
Project'. Fill in a project name and click 'Create project'. You can use this
space to map your tasks and your progress.

* Clone repository (all group members)

'When you create a repository on GitHub, it exists as a remote repository. You
can clone your repository to create a local copy on your computer and sync
between the two locations.'
(GitHub help https://help.github.com/articles/cloning-a-repository/)

On GitHub, at the top of you repository, click on the green button 'Clone or
download' and copy the provided link.

Go back to the terminal, and type in 'git clone' followed by the link you copied.
For example:

```bash
git clone https://github.com/example/wheelchair-design-platform.git
```

### Python

To run Python code, we need to install the Python interpreter on your machine. 
We will use Python 3.x (as opposed to 2.x).

Back to the terminal, type in the following command

```bash
python3 --version
```

If the command results is 'Not found', you need to install Python 3.
Otherwise, skip the rest of this section.

Select, download and install the latest version of Python 3 for Windows or Mac
on this page: https://www.python.org/downloads/release/python-372/

Close and reopen the terminal to check the installation

```bash
python3 --version
```

In python Pip is a tool that manages packages for us. We will use it to install
and update any Python library our project relies on. You can install it with
the following command:

```bash
python get-pip.py
```

### Atom

Another tool we need is a code editor, commonly called IDE for integrated
development editor. For this we will use Atom, as it is open source and popular 
for its modularity. 

Download and start Atom: https://atom.io/

* ide-python

The next step is the python plugin, to get some help from in Atom specifically
for Python. Go to the terminal and type:

```bash
python3 -m pip install 'python-language-server[all]'
```

When it is installed, go back to Atom. On the top menu, click on 'Atom' > 
'Preferences' > 'Packages'. Search and install 'atom-ide-ui' and 'ide-python'.

* Open your project

To open the project you cloned, on the top menu click on 'File' > 'Open...' and
select the folder you cloned. On the left panel, you can see the files of your
project. Let's have a closer look:

* .git folder contains the all history of your project, managed by git. Do not
edit or delete files in this folder;
* platform contains all documents and code examples to get started;
* .gitignore contains the list of files/file extensions you do not want to track
with Git. It is especially convenient for local configuration or passwords not 
to be shared;
* LICENSE contains the MIT license for this project;
* README.md contains a short introduction to the platform in markdown, a simple
text formatting language;
* requirements.txt contains the list of python library you need to install for
the project to work;


### Git Flow

To experience Git flow of updating and sharing progress, open README.md and add
a title at the top of the file, for example:

```markdown
# Noisy Wheelchair

A short description...

# Wheelchair Design Platform
...

```

In markdown, # represents headers and * represents bullet points. More
formatting can be found here: https://guides.github.com/features/mastering-markdown/


Note: A blue dot appear next to your file name README.md at the top of the page,
letting you know it is not saved. Press Command+S (or Ctrl+S) to save.

Your file appears in yellow in the left panel. It means that there are changes
in this file that are not yet tracked by Git (unstaged). Click on Git in the
bottom-right corner.

* In the 'Unstaged changes', double-click on the README.md to 'stage' it, i.e. 
prepare this file to track its changes. The middle tab show you what are the
changes;

* Then we add a 'Commit message' to briefly explain the nature of those changes,
e.g. 'a test of Git and Markdown'. Click on 'Commit to Master' to track the changes.
You have made one change (commit) to your local repository;

* The final step consists in sharing this change with your peers. In the bottom-right
corner, click on 'push'.


Other members of the group can now press 'Fetch' in the bottom-right corner to 
update their local repository with the latest version.

### Data-Centric Design Hub (Per group)

In the cloud we will use our prototyped cloud platform for designer we call 
Data-Centric Design Hub.

* Person have a collection of Things;
* Things are physical or virtual entities with Properties.
* Properties are one-to-many dimensional data points

In our wheelchair case, we have one Thing (the wheelchair) with properties such
as acceleration (3 values for x, y and z) or sit pressure (a value for each force
sensors on the sit).

Go to the hub manager via https://dwd.tudelft.nl/manager and sign up as a group
with an email address, a name and a password. The sign up process create an
account, then standard OAuth2 process start with a consent: you need to let the
manager access your Things, so that it can help you manage them.

Once the consent succeed, you can click on 'My Things' and create a first one.
For example with the name 'My wheelchair', type 'Wheelchair',
description 'An Internet-connected wheelchair'

The process take a few seconds as the hub generate an access token for your Thing.

COPY A SAVE THIS TOKEN in a file, it will be shown only once and enable your wheelchair
to communicate with the hub. You can also save the thing id, but you can always
go back to the manager to retrieve this id.


### Python example

Back to Atom and your project, let's create a first Python example.

We use Pip to install the dependencies we need, listed in the file requirements.txt.
This file contains a dependence to the library writen for the Data-Centric Design
Hub as well as the a dependence for MQTT, a communication protocol we use to
talk to the hub.

In Atom, open a terminal from the top menu 'View > Terminal > New Terminal Window'
and execute the following command.

```bash
pip3 install -r requirements.txt --user
```

Here we 'install', the option -r indicates we provide a file name that contains 
the required dependencies, the option --user indicates we install the dependencies
in a dependency folder specific for the current users.

On the left panel, right click on the root folder > New Folder and name it 'wheelchair'.

Copy the file platform > examples > raspberrypi > get_started.py 
in your 'wheelchair' folder.

Opening this file, this Python code import the necessary library, then we can see
the following lines:

```python
THING_ID = os.environ['THING_ID']
THING_TOKEN = os.environ['THING_TOKEN']
```

In Python, it means we look at the environment variables to read the id and
access token of our thing. To provide these information as environment variable,
right click at the root of your project (left panel) and create a file '.env'.

In this file, type in the following and paste your id and access token after
the equal signs.

```bash
THING_ID=
THING_TOKEN=
```

Going back to the get_started.py Python script, read through the code and comments
to capture the main steps:

* Create and connect a Thing to the Hub
* Retrieve and display the Thing details
* If the Thing is not containing any property, create a dum property
* In any case, retrieve this dum property
* Continuously call a method that generate dum data and send them to the hub

Here are a few Python elements to note:

* Use # in Python to comment your code
* my_thing and my_property are variables
* To display information in the terminal, we use the method print()

```python
print("show text")
```

* Conditional statement: In Python we express the condition with 'if', elif
(else if) and else (more here https://www.tutorialspoint.com/python/python_if_else.htm).

In our case, we check if the thing details we retrieved from the server has no property
(a newly created Thing) and then create a dum property.

```python
if len(my_thing.properties) == 0:
    # create a dum property
```

* Indentation is key in Python. Take the previous example of condition, the indentation
define what is in the condition. Any following line align with the if would be
considered outside the condition.


## Execute the Python code

Let's execute this code. Go to the Atom terminal and type in the following command:

```bash
python3 wheelchair/getStarted.py
```

If the example run properly you should see a log generated every two seconds,
indicating dum data is being sent to the Hub.

### Visualise data on Grafana

To visualise this data, we use Grafana.

Go to the https://dwd.tudelft.nl/grafana, click on Sign in with OAuth. Fill in
your Data-Centric design Hub email and password. Similarly to the manager,
consent to let Grafana access your data.


Go to 'Dashboard > Manage' and create a new folder for your project.
Then, create a new Dashboard and select a new panel 'Graph'. At the top of this new panel, click on 'Panel Title > Edit'

At the bottom, in the query element GROUP BY, click on time and 'remove'.

In FROM, click on 'Select Measurement' and select your Property ID. If your 
Property ID is not appearing in the list, the hub is not receiving data from
your python code.

In SELECT, click on field and select Value1. Then click on the 
+ sign > Fields > Field to add Value2 and Value3.

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

## Arduino



## Visualisation

* Create a dashboard
* Build query