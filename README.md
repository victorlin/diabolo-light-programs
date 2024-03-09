# diabolo light programs

A collection of programs using [ChaosNuggets/Diabolo_Light](https://github.com/ChaosNuggets/Diabolo_Light).

## Usage

PlatformIO is configured to upload a `src/main.cpp` file. Choose which program to use by symlinking:

    ln -Fs performance.cpp src/main.cpp

## `src/color_lab.cpp`

This program is used to pick colors that look good on the LED kit.

## `src/single_color.cpp`

This program shows a color on initial button press, and will turn off on the 2nd press.

## `src/rainbow_picker.cpp`

<a title="Gringer, Public domain, via Wikimedia Commons" href="https://commons.wikimedia.org/wiki/File:Blended_colour_wheel.svg">
    <img width="128" alt="Blended colour wheel" src="https://upload.wikimedia.org/wikipedia/commons/thumb/c/c7/Blended_colour_wheel.svg/128px-Blended_colour_wheel.svg.png">
</a>

This program cycles through colors of the color wheel. Usage:

1. Press to begin color cycling.
2. Press to stop at a color.
3. Press again to resume color cycling.
4. Long press to turn off.

## `src/performance.cpp`

This program defines a performance consisting of scenes which are defined by colors and transitions between colors. Contains generic structs which I hope to pull out into a separate file eventually.

I use Final Cut Pro's Export XML functionality and a custom script `src/read_fcp_xml.py` to facilitate definition of scenes for this program. Not enough time to document now, but here's a screenshot:

![fcp.jpeg](./fcp.jpeg)
