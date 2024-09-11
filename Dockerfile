# syntax=docker/dockerfile:1

FROM ubuntu

WORKDIR /home/ci/app

RUN apt update

RUN apt install -y python3 python3-pip python3-platformio curl jq

COPY entrypoint.sh /.
COPY entrypointRelease.sh /.
RUN chmod +x /entrypoint.sh
RUN chmod +x /entrypointRelease.sh

COPY . /home/ci/app/.

ENTRYPOINT ["/entrypoint.sh"]