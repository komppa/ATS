#!/bin/bash

cd /home/ci/app
pio lib install
pio test -e native --junit-output-path /home/ci/app/output