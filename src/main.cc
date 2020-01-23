#include <iostream>
#include <string>
#include <cstdint>
#include <stdlib.h> 
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <assert.h>
#include <memory>

#include "vendor/json.hpp"
#include "vendor/clipp.hpp"

#include "vector.h"
#include "embedding.h"

using json = nlohmann::json;

using namespace StuffedTurkey;
using namespace clipp; 

enum class AggregationMode {uniform,weighted,log_weighted};

inline bool ends_with(const std::string& value, const std::string& ending) {
    if (ending.size() > value.size()) return false;
    return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

std::unique_ptr<Embedding> loadEmbedding(const std::string& filename){
    std::unique_ptr<Embedding> e(new Embedding());

    if (ends_with(filename, ".vec")){
        std::cout << "read vec-model" << std::endl;
        e->loadvec(filename);
    } else if (ends_with(filename, ".bin")) {
        std::cout << "read binary-model" << std::endl;
        e->loadbin(filename);
    } else {
        throw std::runtime_error("unknown format");
    }

    return e;
}

void aggEmbedding(std::string in_file, std::string out_file, const std::string map_file, AggregationMode agg_mode) {
    auto emb = loadEmbedding(in_file);
    Embedding out_emb = Embedding(emb->dim());

    std::ifstream map_stream(map_file);
    std::string line;
    json j;

    Item (*agg_fn)(std::vector<Item>);

    switch (agg_mode)
    {
        case AggregationMode::uniform:
            agg_fn = Item::avg;
            break;
        case AggregationMode::weighted:
            agg_fn = Item::weighted_avg;
            break;
        case AggregationMode::log_weighted:
            agg_fn = Item::log_weighted_avg;
    }

    while (std::getline(map_stream, line)) {
        j = json::parse(line);

        json::iterator it = j.begin();

        if ( it != j.end()) {
            std::vector<Item> oitems = {};

            for (auto v: it.value()){
                if (emb->contains(v)){
                    oitems.push_back(emb->get(v));
                }
            }

            Item new_item = Item::aggregate(oitems, agg_fn);
            out_emb.add(it.key(), new_item);
        }
    }
    
    out_emb.unit();
    
    std::ofstream ofs(out_file);
    out_emb.dump(ofs);
}

void printInfo(std::string filename){
    auto emb = loadEmbedding(filename); 

    std::cout << "Dimensions: " << emb->dim() << std::endl;
    std::cout << "Vocab size: " << emb->len() << std::endl;
    std::cout << "normalized: " << (emb->is_unit() ? "true" : "false") << std::endl;

    std::map<std::string, Item>::iterator iter = emb->begin();

    for (uint8_t i = 0; i < 10 && iter != emb->end(); i++, iter++){
        std::cout << iter->first;

        if (i != 9) {
            std::cout << ", ";
        }
    }
    std::cout << std::endl;
}

void unitEmbedding(std::string in_file, std::string out_file){
    auto emb = loadEmbedding(in_file);

    emb->unit();

    std::ofstream ofs(out_file);
    emb->dump(ofs);
}

int main(int argc, char * argv[]){
    std::string infile, outfile, cnt_file, mapping_file = "";
    std::string agg_mode = "log_weighted";
    std::string in_format = "auto";
    std::string out_format = "vec";

    enum class cmd {info,help,unit,agg};
    cmd selected = cmd::help;
    AggregationMode amode = AggregationMode::log_weighted;

    auto cli = (
        (
            (
                (command("info").set(selected, cmd::info), 
                    value("input file", infile)) |
                (command("unit").set(selected, cmd::unit), 
                    value("input file", infile), 
                    value("output file", outfile)) | 
                (command("agg", "aggregate").set(selected, cmd::agg),
                    value("input file", infile), 
                    value("output file", outfile),
                    value("mapping", mapping_file) % "json file which sates which items belong together",
                    option("--item_counts") & value("count_file", cnt_file) % "item counts to calculate frequencies",
                    option("--mode", "-m") & value("agg_mode", agg_mode) % 
                        "aggregation mode (uniform, weighted or log_weighted). Defaults to log_weigted mean"
                )
            ),
            option("--in_format") & value("in_format", in_format) % "input format: bin, vec or auto (default)",
            option("--out_format") & value("out_format", out_format) % "output format bin or vec (default)"
        ) | (command("help").set(selected, cmd::help))
    );

    if(!parse(argc, argv, cli)) {
        std::cout << make_man_page(cli, "stuffedturkey");
        return 1;
    } 

    switch(selected) {
        case cmd::info: 
            printInfo(infile);
            break;
        case cmd::unit:
            unitEmbedding(infile, outfile);
            break;
        case cmd::agg:
            
            if(agg_mode == "uniform"){
                amode = AggregationMode::uniform;
            } else if (agg_mode == "weighted") {
                amode = AggregationMode::weighted;
            }

            //Embedding
            aggEmbedding(infile, outfile, mapping_file, amode);
            break;
        case cmd::help:
            std::cout << make_man_page(cli, "stuffedturkey");
            break;
    }
    
    return 0;
}
