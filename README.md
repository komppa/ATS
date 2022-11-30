# ATS - Automatic Transfer Switch

![tests](https://github.com/komppa/ATS/actions/workflows/test.yaml/badge.svg)

Intelligent power switching between two power sources. Feed load from both grid and generator without manual hassle!

## Install

Automatic transfer switch requires following to be installed
- python3
- pip3
- platformio

Platfrmio can be installed by using pip
```bash
pip3 install -U platformio
```

After that you can install dependencies of the project
```bash
pio lib install
```

## Running tests

Tests are allowed to be executed only for the native environment that does not require real hardware to be connected to the tester.
Run tests by executing the following command
```bash
pio test -e native
```

## Building

Since platformio.ini includes two environmets (native & megaatmega2560 for Arduino Mega) you need to specify for which target you want to build project. To build for ATmega2560:
```bash
pio run -r megaatmega2560
```

## Sketch about the idea

<br />
<div>
    <img width="800" src="images/fsm.png">
<div/>
<br />


**Happy switching!**