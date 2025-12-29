
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <algorithm>
#include <string>
#include <limits>
#include "Graph.h"
#include "types.h"
#include "MatchingCommand.h"


void loadFullyDynamicGraphStream(const std::string& file_path, const std::string& output_file_path, Graph*& data_graph){
    
    std::ifstream infile(file_path);
    std::ofstream outputfile;
    outputfile.open(output_file_path, std::ios::app);
    std::string line;

    if (!infile.is_open()) {
        std::cout << "Can not open the graph file " << file_path << " ." << std::endl;
        exit(-1);
    }

    char type;
    std::string input_line;
    ui label = 0;

    std::cout << "Reading File............ " << std::endl;

    VertexID begin, end;
    int rating, linesToSkip = 3;
    long long timestamp;

    ui time_serial = 0;

    long long* temporal_stats = new long long[3];
    ui* stats = new ui[3];

    long long total_bfy_count = 0;

    temporal_stats[0] = 0;
    temporal_stats[1] = 0;
    temporal_stats[2] = 0;

    for (int i = 0; i < linesToSkip; ++i) {
        if (!std::getline(infile, line)) {
            break;
        }
    }


    while(infile >> begin) {

        infile >> end;
        infile >> rating;
        infile >> timestamp;

        if(begin == end){
            time_serial++;
            continue;
        }

        data_graph->add_edge(begin, end, time_serial);

        std::cout << "Inserted Edge.......... " << std::endl;

        total_bfy_count += data_graph->get_temporal_stats_for_butterfly(begin, end, temporal_stats, stats);

        time_serial++;

        if(time_serial % 1000 == 0){
            outputfile << (temporal_stats[0] / total_bfy_count) << "  " << (temporal_stats[1] / total_bfy_count) << "  " <<  (temporal_stats[2] / total_bfy_count) << "  " << total_bfy_count << std::endl;
            outputfile.flush();
        }

        if(time_serial >= 190000){
            break;
        }
    }

    infile.close();
    outputfile.close();
}


int main(int argc, char** argv){

    MatchingCommand command(argc, argv);
    
    std::string input_data_graph_file = command.getDataGraphFilePath();
    std::string output_file = command.getOutputFilePath();

    std::cout << "Input File : " << input_data_graph_file << std::endl;    

    Graph* data_graph = new Graph();
    loadFullyDynamicGraphStream(input_data_graph_file, output_file, data_graph);

    std::cout << "Output Written to : " << output_file << std::endl;    
}
