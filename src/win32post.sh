#!/bin/bash

zip -j mrgui-win32.zip release/mrgui.exe
cd avrdude/i686-mingw32/avrdude-6.3.0/
zip -g ../../../mrgui-win32.zip bin/avrdude.exe etc/avrdude.conf
cd ../../../zadic/
zip -g ../mrgui-win32.zip bin/libusb0.dll bin/zadic.exe bin/install-usbtiny.bat bin/install-iseavrprog.bat
