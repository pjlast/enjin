# Enjin

[![Build Status](https://travis-ci.org/pjlast/enjin.svg?branch=master)](https://travis-ci.org/pjlast/enjin)

## Requirements
- SDL2
- SDL2_image

Install these libraries with your package manager of choice.
e.g. Fedora:
> sudo dnf install SDL2-devel SDL2_image-devel

## Running the stuffs
Building and running the example program:
> make build

> cd src

> ./program

Running the tests:

Make sure the unity submodule cloned correctly:
> git submodule init

> git submodule update

> make test

## What is an ECS game engine?
The architecture is based on the following talk by Catherine West at RustConf:

https://www.youtube.com/watch?v=aKLntZcp27M
change
Hello Worlds
