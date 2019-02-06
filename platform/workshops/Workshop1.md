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

### Arduino IDE



### C++

### Data-Centric Design Hub

Per group:

* Register as user
* Create a thing
* Keep thing id and token

### Grafana

* Connect
* Look at the live data of the demo

## Raspberry Pi

* Main Components

* Architecture

* Setting up the Raspberry Pi

* Network

* Bash commands

## Arduino



## Visualisation

* Create a dashboard
* Build query