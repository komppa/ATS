#!/bin/bash

cd /home/ci/project
pio lib install
pio test -e native --junit-output-path /home/ci/project/output