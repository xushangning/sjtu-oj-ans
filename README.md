# sjtu-oj-ans

C++ programs written for SJTU OJ

## Usage Notes

- The `src` directory contains all sources. Headers for data structures reside under the `src/include` directory.
- All classes are encapsulated in the `sx` namespace to avoid name conflicts.
- Each commit will be marked with its corresponding problem ID, except for the first few commits.
- There may be multiple cpp files for a problem. These files may be moved under their own directory.

Initially, headers are `include`d in cpp files. In the final version (usually the version submitted for online judging), headers `include`d are copy-pasted into the cpp file, with unused class methods removed, thus creating a "static" version of the headers. That's why the implementation of a particular class may differ in different cpp files. Just remember that *that* implementation **works** for the problem.
