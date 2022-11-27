# syntax=docker/dockerfile:1

FROM ubuntu

WORKDIR /home/ci/project

RUN apt update

RUN apt install -y python3 python3-pip

RUN pip3 install -U platformio

COPY entrypoint.sh /.
RUN chmod +x /entrypoint.sh

COPY . /home/ci/project/.

ENTRYPOINT ["/entrypoint.sh"]