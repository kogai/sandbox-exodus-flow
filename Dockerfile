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
  sox

RUN mkdir ~/.npm-global \
  && npm config set prefix '~/.npm-global' \
  && echo "export PATH=~/.npm-global/bin:$PATH" >> ~/.profile \
  && source ~/.profile

RUN npm install -g bs-platform --unsafe-perm
