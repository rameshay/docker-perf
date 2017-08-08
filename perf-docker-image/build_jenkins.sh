#!/bin/bash
set -e
function build_image() {
    echo "Building docker image with tag $1"
    sudo docker build --rm . -t $1
}

function push_image() {
    echo "Pushing docker image with tag $1 to registry"
    # Commenting out this until the images are build clean
    sudo docker push $1
    if [ $? -eq 0 ]; then
        sudo docker rmi $1
    fi
}


function main() {
    IMAGE_NAME=`basename $PWD`
    PERF_DOCKER_IMAGE_TAG=$1
    PERF_POLYGRAPH_VERSION=$2
    if [ -z $PERF_DOCKER_IMAGE_TAG ]; then
        echo "Please provide a proper tag value to the image that is being generated."
        echo "Exiting ...."
        exit 1
    fi

    if [ -z $PERF_POLYGRAPH_VERSION ] ; then
        ELASTICA_VERSION="latest"
    else
        ELASTICA_VERSION=$PERF_POLYGRAPH_VERSION
    fi
    DOCKER_IMAGE_TAG="${PERF_DOCKER_IMAGE_TAG}/${IMAGE_NAME}":"${ELASTICA_VERSION}"
    build_image $DOCKER_IMAGE_TAG
    push_image $DOCKER_IMAGE_TAG
}
main $@
