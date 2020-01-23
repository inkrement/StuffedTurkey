![](inc/turkey.png)

# StuffedTurkey

This is a package for Distributed Embedding aggregation (e.g. *average*, *weighted average* or *log weighted average*). The name comes from the fact that the package is there to bundle (aggregate) vectors (e.g. distributed word representations). Therefore in a certain sense to "stuff" them into a more general form. Besides, the name is also easy to remember, cool and it just happened to be Thanksgiving.


## Usage
Currently we only support a minimal CLI interface. R and python packages are on the TODO list. To use the software you have to compile it first. For this you need a current C++17 compiler (clang or GNU g++) and the classic build tools (make).

```bash
make
./stuffedturkey --help 
```

### TODOs:
 - add other input and output formats (e.g., json, bin)
 - python package
 - R package
 - bigquery format export
 - process bin format
