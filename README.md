# QuickShell

[简体中文](./assets/README_zh_CN.md)

## Introduction

QuickShell is a graphical user interface (GUI) script runner developed based on Qt6, designed to provide a simple and intuitive environment for writing, editing, and running various scripts. Whether you are a user of Python, Bash, Perl, or other scripting languages, QuickShell can help you manage and execute scripts more efficiently.

![Screenshot](./assets/screenshot.png)

> **Warning:**
>
> This project is still under development, and some features are still in the works!

## Installation

Currently, you can install and use it in the following two ways:

### Installer Package

You can visit the [Github Release](https://github.com/CatIsNotFound/QuickShell/releases/latest) page to get the latest installer package.

### Building from Source Code

#### Step 1: Clone

Execute the following command in the terminal:

```sh
git clone https://github.com/CatIsNotFound/QuickShell.git
```

#### Step 2: Build the Project

>**Note:**
>
> Before building, please ensure that **the necessary components for Qt6.5 or higher and their required dependencies are installed**! If you have Qt Creator installed, please use Cmake for Qt 6.x in Qt Creator to build the project.

```sh
cd QuickShell
mkdir build && cd build
cmake ..
```
