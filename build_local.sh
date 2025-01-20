#!/bin/bash

USE_BAZEL_VERSION=7.4.1 bazel build //src:gcp-traffic-extension \
    --compilation_mode=fastbuild