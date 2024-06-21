# Rice Runtime Documentation

Rice runtime is a cross-platform runtime for multiple languages, that helps them to interconnect, and almost zero-cost in comparing to just native executable.

## Possible Features

* Reflection and Serialization for C and C++

    > That can be obtained in Rice Runtime CLI by using LLVM or Clang to analyze AST and generate and compile source file by this bindings.

* Automatic API generation

    > That also can be obtained by analyzing AST and generating API definitions for a certain language.

* Extensions

    List of languages is not fixed, supported languages can be extended by CLI and Runner Packages. CLI package will help to create API for a certain language, and Runner Package will help to execute a certain language by adding certain language runtime.

    Without extensions at the moment planned support is for C, C++, Rust and Lua.

* Deployment

    After work is done application and be deployed as a native binary with all dependencies. Also deployment options can be configured.

## Concepts
* **Rice Runtime API**

    The programming interface that will help package code to interact with Runtime.

* **Rice Runtime CLI**

    The console line interface that will help making packages for the runtime. Will be developed after runtime first builds, using runtime as base.

* **Package**

    File that contains runtime data that can be executed or called from other package. Package can be dependent from other packages

    Runtime package can contain an initialization point in which package interacts with runtime to initialize all the necessary things such as create threads, or get some data from dependent packages

    **Package Types:**

    - **.nrp - Native Runtime Package**

        Type of package that contains native platform data such as shared library and some additional metadata (class and functions descriptions), and also resources

        Suitable for languages that compiles into native code such as:
        - C
        - C++
        - Carbon
        - Rust
        - Go 

    - **.urp - Universal Runtime Package**

        Type of package that can be executed at any platform or operating system. But it can be restricted in package description.

        It requires runner to be executed.

        Suitable for languages that are JIT-compiled or interpreted such as:
        - C#
        - Lua
        - Python
        - JavaScript

    - **.rrp - Resource Runtime Package**

        Type of package that contains only resource data

    - **.${language}.drp - Development Rice Package**

        Package that contains development data for a certain language (such as headers, or API)
    
    - **.srp - Source Rice Package**

        Package that contains source code that can be compiled into .nrp or .urp