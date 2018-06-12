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

## Dependencies

  - [Qt](https://www.qt.io/)
  - [SMTP client for Qt](https://github.com/bluetiger9/SmtpClient-for-Qt) - included as submodule
  - [OpenSSL](https://slproweb.com/products/Win32OpenSSL.html) - only needs to be installed on Windows. Warning: Use a version < 1.1, since some needed libs are missing in version 1.1.
  
![](https://img1.picload.org/image/dogggpcw/image18.png)  
