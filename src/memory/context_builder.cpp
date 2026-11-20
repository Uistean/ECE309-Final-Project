// src/memory/context_builder.cpp — YOU IMPLEMENT. Spec §4.4.

#include "memory/context_builder.h"
#include "util/min_heap.h"
#include <utility>

ContextPlan select_context(const std::vector<ContextItem>& items,
                           std::size_t budget, std::size_t keep_recent) {
    // TODO: total the tokens; if they fit, keep everything. Otherwise push
    // every unprotected item onto a MinHeap keyed on (drop_rank(role), index)
    // and pop/drop until the total fits or the heap is empty.
    //
    // Clues:
    //  * Heap entries:  MinHeap<std::pair<int, std::size_t>> heap;
    //    std::pair compares .first, then .second, which is exactly
    //    "lowest rank first, then oldest first".
    //  * "The last keep_recent items" start at index items.size() - keep_recent,
    //    but these are unsigned: if keep_recent > items.size() that
    //    subtraction wraps around to a huge number. Handle that case first.
    //  * A vector<bool> keep(items.size(), true) makes the final pass easy:
    //    collect the indices still marked true, in ascending order.
    //  * total_tokens is the sum of the KEPT items; over_budget is
    //    total_tokens > budget.
    MinHeap<std::pair<int, std::size_t>> heap;
    std::size_t total_tokens =0;
    ContextPlan final_result;
    for(std::size_t i=0 ; i<items.size(); i++){
        total_tokens += items[i].tokens;
    }

    if(total_tokens > budget){

       std::vector<bool> protect(items.size(),true);
       std::size_t recentindex = 0;
       if(keep_recent < items.size()){
            recentindex = items.size()-keep_recent;

       }
        for(std::size_t i =0; i <items.size(); i++){
            if((items[i].role== Role::System)||(items[i].pinned)||(i>=recentindex)){
                //cant be bothered to add !()
            }else{
                heap.push({drop_rank(items[i].role), i});

            }
        }
        while(total_tokens>budget && !heap.empty()){
            auto i = heap.pop();
            protect[i.second] = false;
            total_tokens -= items[i.second].tokens;

        }

        for(std::size_t i = 0; i<items.size();i++){
            if(protect[i]== true){
                final_result.keep.push_back(i);
            }
        }
        final_result.total_tokens = total_tokens;
        final_result.over_budget = (total_tokens>budget);
        return final_result;

        }else{
            final_result.total_tokens = total_tokens;
            final_result.over_budget = false;
            for(std::size_t i = 0; i < items.size(); i++){
               final_result.keep.push_back(i);
            }

        }
    
    return final_result;
}
