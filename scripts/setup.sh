#!/bin/bash

sudo service xencommons start
sudo service xend start
sudo service libvirtd restart
reset

