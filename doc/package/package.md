# Rice Runtime Packages

**Package** is a file that contains runtime data that can be executed or called from other package.

Runtime package can contain an initialization point in which package interacts with runtime to initialize all the necessary things such as create threads, or get some data from dependent packages.

Also package can have dependencies from other packages or native libraries.

**Package Types:**

- .nrp - [Native Runtime Package](./nrp.md)

- .urp - [Universal Runtime Package](./urp.md) 

- **.rrp - Resource Runtime Package**

    Type of package that contains only resource data

- **.${language}.drp - Development Rice Package**

    Package that contains development data for a certain language (such as headers, or API)
    
- **.srp - Source Rice Package**

    Package that contains source code that can be compiled into .nrp or .urp