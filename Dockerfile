FROM node:alpine

RUN apk upgrade --update \
  && apk add --virtual \
    build-dependencies \
    build-base \
    perl \
    git \
    make \
    gcc \
    python \
    bash \
    sox \
  && npm install -g bs-platform --unsafe-perm
