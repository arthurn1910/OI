#!/bin/bash
sudo apt install qt5-qmake qtbase5-dev
echo "export QT_SELECT=qt5" >> ~/.bashrc
source ~/.bashrc
