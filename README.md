![](inc/turkey.png)

# StuffedTurkey

This is a package for Distributed Embedding aggregation (e.g. *average*, *weighted average* or *log weighted average*). The name comes from the fact that the package can be used to bundle vectors (e.g. distributed word representations). Therefore in a certain sense to "stuff" them into a more general form. Besides, the name is also easy to remember, cool and it just happened to be Thanksgiving.


## Usage
Currently, we only support a minimal CLI interface. However, R and python packages are on the TODO-list. To use the software you have to compile it first. For this you need a current C++17 compiler (clang or GNU g++) and the classic build tools (e.g. make).

```bash
make
sudo make install
stuffedturkey --help 
```

Altough, there is no Python package yet, it can be quite easily used in `jupyter(lab|hub)?` using the bash-syntax. Here is a minimalistic example how it could be used in combination with gensim:

```py
# TODO: insert paths
vec_path, agg_path, mapping = './...', './...', './...' 

# save trained gensim model
model.wv.save_word2vec_format(vec_path, binary=False)

# call stuffedturkey (you have to install it first)
!stuffedturkey agg {vec_path} {agg_path} {mapping} --mode log_weighted

# now you can load the aggregated embedding
agg_model = KeyedVectors.load_word2vec_format(agg_path, binary=False)
```


### TODOs:
 - add other input and output formats (e.g., json, bin)
 - python package
 - R package
 - bigquery format export
 - process gensim/word2vec-bin format
