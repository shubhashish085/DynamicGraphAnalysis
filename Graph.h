#ifndef DGA_GRAPH_H
#define DGA_GRAPH_H

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <set>
#include <algorithm>
#include <limits>
#include "types.h"

class Graph{

private:

    ui vertices_count;
    ui edges_count;
    ui max_degree;

    std::unordered_map<VertexID, std::unordered_set<VertexID>> srcToDsts;
    std::unordered_map<VertexID, double> nodeToButterflies;

    double globalButterfly = 0;

    VertexID** edge_array;
    ui* degrees;
    ui* temporal_array;
    std::unordered_map<KeyID, ui> edgeToIndex;

public:

    long long global_butterfly_cnt = 0;

    Graph() {

        vertices_count = 0;
        edges_count = 0;
        max_degree = 0;
        global_butterfly_cnt = 0;
        edge_array = new VertexID *[2];
        for(ui i = 0; i < 2; i++){
            edge_array[i] = new VertexID[200000];
        }
        temporal_array = new ui[200000];

    }

    ~Graph() {

    }
 

    const ui getVerticesCount() const {
        return vertices_count;
    }

    const ui getEdgesCount() const {
        return edges_count;
    }


    void add_edge(VertexID u, VertexID v, ui time_serial) {


        if(u > v){
            VertexID temp = u;
            u = v;
            v = temp;
        }

        ui size = edgeToIndex.size();
        edge_array[0][size] = u;
        edge_array[1][size] = v;
        temporal_array[size] = time_serial;
        KeyID key = ((KeyID)u * std::numeric_limits<unsigned int>::max()) + v;

        edgeToIndex.emplace(key, size);
        if (srcToDsts.find(u) == srcToDsts.end()){
            std::unordered_set<VertexID> src_set;
            srcToDsts.emplace(u, src_set);
        }
        std::unordered_map<VertexID, std::unordered_set<VertexID>>::iterator itr = srcToDsts.find(u);
        (itr->second).insert(v);

        if (srcToDsts.find(v) == srcToDsts.end()){
            std::unordered_set<VertexID> dst_set;
            srcToDsts.emplace(v, dst_set);
        }

        itr = srcToDsts.find(v);
        (itr->second).insert(u);        
    }


    bool delete_edge(VertexID u, VertexID v){

        ui size = edgeToIndex.size(), index;

        if(u > v){
            VertexID temp = u;
            u = v;
            v = temp;
        }

        KeyID key = ((KeyID)u * std::numeric_limits<unsigned int>::max()) + v;
        std::unordered_map<KeyID, ui>::iterator edge_idx_itr = edgeToIndex.find(key);
        if (edge_idx_itr != edgeToIndex.end()){
            index = edge_idx_itr->second;
            edgeToIndex.erase(key);
        }

        std::unordered_map<VertexID, std::unordered_set<VertexID>>::iterator itr = srcToDsts.find(u);
        if (itr != srcToDsts.end()){
            (itr->second).erase(v);
            if ((itr->second).empty()){
                srcToDsts.erase(u);
            }
        }

        itr = srcToDsts.find(v);
        if (itr != srcToDsts.end())
        {
            (itr->second).erase(u);
            if ((itr->second).empty()){
                srcToDsts.erase(v);
            }
        }

        if (index < size - 1)
        {
            VertexID newSrc = edge_array[0][index] = edge_array[0][size - 1];
            VertexID newDst = edge_array[1][index] = edge_array[1][size - 1];
            temporal_array[index] = temporal_array[size - 1];
            KeyID newKey = ((KeyID)newSrc * std::numeric_limits<unsigned int>::max()) + newDst;
            edgeToIndex.erase(newKey);
            edgeToIndex.emplace(newKey, index);
        }        

        return true;
    }


    long long get_global_butterfly_count(){
        return global_butterfly_cnt;
    }

    KeyID get_key(VertexID u, VertexID v){

        if(u > v){
            VertexID temp = u;
            u = v;
            v = temp;
        }

        KeyID key = ((KeyID)u * std::numeric_limits<unsigned int>::max()) + v;
        return key;
    }


    ui get_temporal_stats_for_butterfly(VertexID u, VertexID v, long long*& temporal_stats, ui* stats){

        ui butterfly_count = 0, index = 0;

        std::unordered_map<VertexID, std::unordered_set<VertexID>>::iterator src_itr = srcToDsts.find(u);
        std::unordered_map<VertexID, std::unordered_set<VertexID>>::iterator dst_itr = srcToDsts.find(v);
        std::unordered_map<VertexID, std::unordered_set<VertexID>>::iterator two_hop_itr;
        KeyID key;

        std::unordered_set<VertexID>::iterator itr, inner_itr;

        stats[0] = 0, stats[1] = 0, stats[2] = 0;
        
        for (itr = (src_itr->second).begin(); itr != (src_itr->second).end(); itr++) {
            two_hop_itr = srcToDsts.find(*itr);
            for(inner_itr = (two_hop_itr->second).begin(); inner_itr != (two_hop_itr->second).end(); inner_itr++){
                if(*inner_itr != u && (dst_itr->second).find(*inner_itr) != (dst_itr->second).end()){
                    
                    butterfly_count += 1;
                    
                    key = get_key(u, *itr);
                    index = edgeToIndex[key];
                    stats[0] = temporal_array[index];

                    key = get_key(*itr, *inner_itr);
                    index = edgeToIndex[key];
                    stats[1] = temporal_array[index];

                    key = get_key(v, *inner_itr);
                    index = edgeToIndex[key];
                    stats[2] = temporal_array[index];

                    std::sort(stats, stats + 3);
                    temporal_stats[0] += stats[0];
                    temporal_stats[1] += stats[1];
                    temporal_stats[2] += stats[2];
                }                
            }
        }

        return butterfly_count;        
    }


};


#endif