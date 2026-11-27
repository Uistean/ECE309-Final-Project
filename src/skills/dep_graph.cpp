// src/skills/dep_graph.cpp — YOU IMPLEMENT. Spec §6.2.

#include "skills/dep_graph.h"
#include <algorithm>
#include <unordered_set>

bool DepGraph::add_node(const std::string& name) {
    if(requires_.find(name) != requires_.end()){
        return 0;
    }
    requires_[name] = std::vector<std::string>();
    // TODO
    return 1;
}

bool DepGraph::add_edge(const std::string& from, const std::string& to) {
    if(requires_.find(from) == requires_.end()){
        return 0;
    }
    for(auto i : requires_[from]){
        if(i == to){return 0;}
    }
    requires_[from].push_back(to);
    required_by_[to].push_back(from);
    return 1;

    
    // TODO
    
    
}

bool DepGraph::has_node(const std::string& name) const {
    if(requires_.find(name) == requires_.end()){
        return 0;
    }
    // TODO
   return 1;
}

std::size_t DepGraph::node_count() const noexcept {
    size_t i =0;
    for(auto& x: requires_){
        i++;
        (void)x;
    }
    // TODO
    return i;
}

const std::vector<std::string>& DepGraph::requirements(const std::string& name) const {
    // TODO: return the stored list; for an unknown name return a reference
    // to an empty vector that outlives the call (e.g. a function-local
    // static const).
    static const std::vector<std::string> kNone;
    auto look =  requires_.find(name);
    if(look == requires_.end()){
        return kNone ;
    }
    return look->second;
}

bool DepGraph::order_from(const std::string& root, std::vector<std::string>* out,
                          std::string* error) const {
    // TODO: DFS with three colors (white / gray = on the current path /
    // black = finished). Spec §6.2 gives the exact error strings, and
    // Appendix D traces the diamond.
    // Clues:
    //  * Keep a vector<string> path of the gray nodes: push_back when a node
    //    turns gray, pop_back when it turns black. When an edge reaches a
    //    gray node x, the cycle is the part of path from x to the end, then
    //    x again. Check yourself with a>b, a>c, c>a: the answer is
    //    "cycle: a -> c -> a" (b is already finished and must not appear).
    //  * Append a node to *out when it turns BLACK (after its requirements),
    //    never when it turns gray.
    //  * Order of checks for each edge: missing node, then gray (cycle),
    //    then white (visit). Black: skip, that is not a cycle.
    //  * Separators are exactly " -> " (space, dash, greater-than, space)
    if(!has_node(root)){
        *error = "unknown: " + root;
        return 0;
    }
    out->clear();
    std::unordered_map<std::string, int> colors;
    std::vector<std::string> path;
    return visitnde(root, out, colors, path, error);
    
}

bool DepGraph::visitnde(const std::string& current, std::vector<std::string>* out,std::unordered_map<std::string, int>& colors,std::vector<std::string>& path, std::string* error) const {   
    colors[current]=1;
    path.push_back(current);


    for(auto& i : requires_.find(current)->second){
        if(!has_node(i)){
            *error = "missing: " +i+ " (required by " +current+")";
            return 0;}

        if(colors[i]==1){
            size_t start = 0;
            while(path[start] != i){start++;}

            std::string erormessage = "cycle: ";
            for(auto x = start; x <path.size();x++){
                erormessage += path[x] + " -> ";  
            }
            erormessage += i;
            *error = erormessage;
            return 0;
            
        }
        if(colors[i] == 0){

          if(!visitnde(i,out,colors, path,error)){
            return 0;
        }
            
        }
    }
    colors[current] =2;
    path.pop_back();
    out->push_back(current);
    return 1;
    }

std::vector<std::string> DepGraph::dependents(const std::string& name) const {
    // TODO: BFS over the reversed edges.
    // Clues:
    //  * Mark `name` itself as seen before you start, so a cycle can't add
    //    it to the result.
    //  * The order is by (distance, name). Easiest: sort each BFS level
    //    alphabetically before appending it. Sorting the whole result by
    //    name alone loses the nearest-first order.
    //  * std::queue<std::pair<std::string, int>> (node, distance) works, or
    //    process one level (a vector) at a time.
    std::vector<std::string> dependentlist;

    std::unordered_set<std::string> seen;
    seen.insert(name);

    std::vector<std::string> curr_level; 
    curr_level.push_back(name);

    while(!curr_level.empty()){
        std::vector<std::string> next_level;
        
        for(size_t i = 0; i < curr_level.size(); i++){
            auto look = required_by_.find(curr_level[i]);
            if(look == required_by_.end()){ continue;}

            for(auto& user: look->second){
                if(seen.count(user) == 0){
                    seen.insert(user);
                    next_level.push_back(user);
                }
            }

        }
        std::sort(next_level.begin(), next_level.end());
        for(size_t i = 0; i < next_level.size(); i++){
            dependentlist.push_back(next_level[i]);
        }
        curr_level = next_level;






    }


    
    return dependentlist;
}

