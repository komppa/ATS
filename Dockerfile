# syntax=docker/dockerfile:1

FROM ubuntu

WORKDIR /home/ci/project

RUN apt update

RUN apt install -y python3 python3-pip curl

RUN pip3 install -U platformio

COPY entrypoint.sh /.
COPY entrypointRelease.sh /.
RUN chmod +x /entrypoint.sh
RUN chmod +x /entrypointRelease.sh

COPY . /home/ci/project/.

ENTRYPOINT ["/entrypoint.sh"]