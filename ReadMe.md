# Simple Exam Browser

Simple web browser that makes it difficult to access unauthorized resources during exams or tests, and blocks screenshots.

## Features

* Show on screen only browser if more than one screen make it black
* Block make screenshots
* Block typical actions (alt+tab, open menu start, etc)
* There are no address bar, user can only use page with which the browser was opened
* Display the number of prohibited actions performed
* Configured by `config.ini` file

## Install & usage

### Runtime requirements

* Windows 64bit (currently only supported)

### Install

Download browser from releases or build.

Update configuration file (`config.ini`) which is located next to exe file.

### Configuration file

| option  |               Description                |   Type   |    Example value    |
|:-------:|:----------------------------------------:|:--------:|:-------------------:|
| baseUrl | Page which will be open on start browser | `string` | `https://pk.edu.pl` |

## Build

### Build requirements

Application requires:

* **CMake** (version 3.16+)
* **MSVC c++ compilator** (MSVC 2019+)
* **Qt6.8**
  * Web engine addon

Application uses windows low level actions to catch keyboard events, that make MSVC is required.

### Build

Clone repository then run following commands:

```console
mkdir build
cd build
cmake ../
cmake --build . --target simpleExamBrowser
```

## Roadmap

The planned roadmap can be found on the [project board](https://github.com/users/FilipKrzysiek/projects/4/). 

## Authors

mgr inż. Krzysztof Swałdek - Politechnika Krakowska

## License

The Simple Exam Browser is available under the GNU license. 

If you modify the application, add a link to the source version.

