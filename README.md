# Rice Runtime

![Onigiri](./doc/res/icon.svg)

## About

Rice Runtime is a project that goal is to make a simple programming interface that will help to interact different languages and runtimes.

## Features

* **Cross-platform**

    The runtime will running at any platform and any architecture. 

* **New functionality for old languages** 

    For C and C++ the runtime will add reflection and serialization. 

* **Almost zero cost in efficiency**

    This runtime will take time only to package initialization. All other code will work as native.

* **Converting of non-rice projects into rice via CLI**

    The runtime will have CLI that will have a lot of useful features.

* **Extensions**

    List of languages is not fixed, supported languages can be extended by CLI and Runner Packages. CLI package will help to create API for a certain language, and Runner Package will help to execute a certain language by adding certain language runtime.

    Without extensions at the moment planned support is for C, C++ and Rust.

* **Deployment**

    After work is done application and be deployed as a native binary with all dependencies. Also deployment options can be configured.

## References

* Get started page will developed after first releases of runtime

* [Documentation](./doc/doc.md)