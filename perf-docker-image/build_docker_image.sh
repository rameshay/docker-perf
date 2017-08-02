#!/bin/bash
set -e
IMAGE_NAME=`basename $PWD`
if [ -z $1 ] ; then
    ELASTICA_VERSION="latest"
else
    ELASTICA_VERSION="$1"
fi
sudo docker build --rm . -t "${IMAGE_NAME}":"${ELASTICA_VERSION}"
