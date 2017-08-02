#!/bin/bash
set -e
IMAGE_NAME=`basename $PWD`
if [ -z ${PERF_DOCKER_IMAGE_TAG} ]; then 
    echo "Please provide a proper tag value to the image that is being generated."
    echo "Exiting ...."
    exit 1
fi

if [ -z ${PERF_POLYGRAPH_VERSION} ] ; then
    ELASTICA_VERSION="latest"
else
    ELASTICA_VERSION=${PERF_POLYGRAPH_VERSION}
fi
sudo docker build --rm . -t "${PERF_DOCKER_IMAGE_TAG}/${IMAGE_NAME}":"${ELASTICA_VERSION}"
exit $?
