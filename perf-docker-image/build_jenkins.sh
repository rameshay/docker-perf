#!/bin/bash
set -e
function build_image() {
    sudo docker build --rm . -t "${PERF_DOCKER_IMAGE_TAG}/${IMAGE_NAME}":"${ELASTICA_VERSION}"
    exit $?
}

function push_image() {
    # Commenting out this until the images are build clean
    #sudo docker push "${PERF_DOCKER_IMAGE_TAG}/${IMAGE_NAME}":"${ELASTICA_VERSION}"
    #if [ $? -eq 0 ];
        sudo docker rmi "${PERF_DOCKER_IMAGE_TAG}/${IMAGE_NAME}":"${ELASTICA_VERSION}"
    #fi
    return $?
}


function main() {
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
}
main $@
