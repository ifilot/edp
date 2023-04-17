#!/bin/bash

# Perform Docker compilation under Windows using Git BASH

# provide the path to the location where the paper is stored; note that under
# Windows Git Bash, two slashes need to be used instead of one
PAPERPATH="//d//PROGRAMMING//CPP//edp//paper"

docker run --rm \
--volume $PAPERPATH:/data \
--user $(id -u):$(id -g) \
--env JOURNAL=joss \
openjournals/inara
