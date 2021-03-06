#!/bin/bash
# This file is distributed under the BSD 3-Clause License. See LICENSE for details.

# Defaults for configuration variables used by script
: ${LGRAPH_SRC:=${HOME}/lgraph}
: ${LGRAPH_BUILD_MODE:=fastbuild}

if [ ! -e ${LGRAPH_SRC}/WORKSPACE ]; then
  echo "ERROR: '${LGRAPH_SRC}' does not contain LGRAPH source code"
  exit -1
fi

cd ${LGRAPH_SRC}

bazel test -c ${LGRAPH_BUILD_MODE} //...
if [ $? -ne 0 ]; then
  exit 1
fi

if [ "$TRAVIS_EVENT_TYPE" == "cron" ]; then
  bazel test --test_tag_filters "long1" //...

  if [ $? -ne 0 ]; then
    exit 1
  fi
fi


exit 0
