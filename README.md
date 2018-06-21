# SnailSnap
Muscheln und Schnecken des Museums für Naturkunde Berlin freistellen und neue Bilder generieren. Erweiterung, dass man ein Selfie von sich nutzen kann und Metainfos durch Antippen auslesen kann.

## Daten

  - [Coding da Vinci Daten Seite](https://codingdavinci.de/daten/#museum-f%C3%BCr-naturkunde-berlin)
  - [Metadaten](http://gbif.naturkundemuseum-berlin.de/CDV2018/Mollusken/Metadaten/)
  - [Mediendateien](http://gbif.naturkundemuseum-berlin.de/CDV2018/Mollusken/)
  - [Gallerie-Ansicht](http://gbif.naturkundemuseum-berlin.de/CDV2018/mollusken-gallery.html)
  
## Build Status

|Branch      |Linux     |Windows   |
|------------|----------|----------|
|master      | [![Build Status](https://travis-ci.com/snailsnap/mosaic.svg?branch=master)](https://travis-ci.com/snailsnap/mosaic) | [![Build status](https://ci.appveyor.com/api/projects/status/5g1x1yk1wvlohao2/branch/master?svg=true)](https://ci.appveyor.com/project/lukaswagner/mosaic/branch/master) |
|develop     | [![Build Status](https://travis-ci.com/snailsnap/mosaic.svg?branch=develop)](https://travis-ci.com/snailsnap/mosaic) | [![Build status](https://ci.appveyor.com/api/projects/status/5g1x1yk1wvlohao2/branch/develop?svg=true)](https://ci.appveyor.com/project/lukaswagner/mosaic/branch/develop) |

## How to Run

  - Install Qt
  - Clone it
  - Initialize submodules (if SmtpClient or JCVoronoi fail to build, you're missing this)
  - Run CMake
  - Build it
  - Place the data folder (containing the snail images, the meta file and the optional descriptions) next to the built executable
  - Run it
  - If it doesn't work for some reason, check these steps again, and if it still doesn't work create an issue where you tell [Lukas](https://github.com/lukaswagner) to improve the readme.

## Command Line Arguments

  - Important
    - `-n` or `--maxNumOfMolluscs` - Number of snails to build the mosaic from. A good value is 7500.
  - Optional
    - `-d` or `--data` - Path to the data folder, if it's not placed directly next to the executable.
    - `-c` or `--useCam` - Whether the camera should be used to capture images. Pass `false` to use a file dialog to open an existing image. Default is `true`.
  - Unnecessary
    - `-o` or `--output` - Output of the processed image. Just for debugging.

## Dependencies

  - [Qt](https://www.qt.io/)
  - [SMTP client for Qt](https://github.com/bluetiger9/SmtpClient-for-Qt) - included as submodule
  - [OpenSSL](https://slproweb.com/products/Win32OpenSSL.html) - only needs to be installed on Windows. Warning: Use a version < 1.1, since some needed libs are missing in version 1.1.
  
![](https://img1.picload.org/image/dogggpcw/image18.png)  
