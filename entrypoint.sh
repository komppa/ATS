#!/bin/bash

cd /home/ci/project
pio lib install || true
ls /home/ci/project/output || true
pio test -e native --junit-output-path /home/ci/project/output || true