# syntax=docker/dockerfile:1

FROM ubuntu

WORKDIR /home/ci/app

RUN apt update

RUN apt install -y python3-full python3-pip curl jq

RUN pip3 install -U platformio

COPY entrypoint.sh /.
COPY entrypointRelease.sh /.
RUN chmod +x /entrypoint.sh
RUN chmod +x /entrypointRelease.sh

COPY . /home/ci/app/.

ENTRYPOINT ["/entrypoint.sh"]