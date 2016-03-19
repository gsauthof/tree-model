#!/bin/bash

set -eux

# for travis, it is not really necessary to do some cleanup,
# because travis wipes the environment anyway, after each job

docker stop cxx-runtime

docker rm cxx-runtime cxx-devel
