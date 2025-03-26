# KKAM Engine

KKAM is a 2D/3D game engine. Currently a passion solo project making out of love for video games and the efforts done to alleviate those purposes.

# What do I wish to achieve with KKAM? (InDev)
### Accessible for *(almost)* all developers
I have seen different kinds of game engines, ranging from professional, more complex development (Unreal), to more friendly and indie focused (Godot). **I want to provide that all in one engine.**
* Engine expects to use multiple languages, compiled and interpreted.
* This aims to be flexible for developers, giving them an option to develop stuff that is expensive on compiled languages for preformance and leaving other aspects to interpreted languages as these are much faster to develop with.

## Languages
**Developers should use a mix of compiled languages and interpreted languages. Compiled languages help to push out preformance given they are compiled before runtime and interpreted languages are ran and decided during runtime.**
* A good way to use both is leaving important/expensive operations to compiled languages for preformance, and features that aren't super expensive to the interpreted languages for faster development.

## Platform-Support
As my main goal is to provide an engine that is accessible to as much developers as I can reach. This engine suffers from the fact that I am excluding Darwin users (Unix OS for devices like MacOS).
* This project was made for Windows in mind, using DirectX and Native Window Handles
* Developing for multiple platforms seems unbeneficial for me as Windows makes up a large majority of game developers and gamers
* MacOS's Objective C++ language is confusing and I am unable to figure out how to support .mm files

### Modding
Modding is a big thing for many games and communities, I have seen mods for games that blow my mind and it shows how dedicated people are to making something for the game they love
* Editor mods (add features to your editor, appearance, libraries? tools?)
* Game mods, allow users to modify your game or extend your game for others.
* Outline what mods are able to and aren't able to access (Script Environments)
* Developers can override and customize their class's binding functions to customize the way other languages interact with your class (C++ only)

### Customization
Customization of your project is a key aspect of game engines. KKAM wishes to provide very important customization features that are easy to understand and navigate without all the clutter.
* Developers can choose what languages they want to use for their project (from project creation wizard to their project settings)
* Developers can choose what languages they allow modders to use. (Only applies to languages that are being used for the project)

## Third-Party Libraries

### Qt Framework
This application uses **Qt 6.8.2** under the LGPL v3 license, this license is found under **Docs/lgpl-3.0.txt**. The Qt libraries are dynamically linked and can be replaced with compatible versions. You can obtain the Qt source code from [https://www.qt.io/download-qt-installer](https://www.qt.io/download-qt-installer).

The specific Qt version used is **Qt 6.8.2**, compiled with **MSVC 2022 x64**.

For more information about Qt licensing, please visit: [https://www.qt.io/licensing/](https://www.qt.io/licensing/)
