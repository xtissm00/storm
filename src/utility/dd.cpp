#include "src/utility/dd.h"

#include "src/storage/dd/Add.h"
#include "src/storage/dd/Bdd.h"

#include "src/utility/macros.h"

namespace storm {
    namespace utility {
        namespace dd {
            
            template <storm::dd::DdType Type>
            storm::dd::Bdd<Type> computeReachableStates(storm::dd::Bdd<Type> const& initialStates, storm::dd::Bdd<Type> const& transitions, std::set<storm::expressions::Variable> const& rowMetaVariables, std::set<storm::expressions::Variable> const& columnMetaVariables) {
                storm::dd::Bdd<Type> reachableStates = initialStates;
                
                // Perform the BFS to discover all reachable states.
                bool changed = true;
                uint_fast64_t iteration = 0;
                do {
                    STORM_LOG_TRACE("Iteration " << iteration << " of reachability analysis.");
                    changed = false;
                    storm::dd::Bdd<Type> tmp = reachableStates.relationalProduct(transitions, rowMetaVariables, columnMetaVariables);
                    storm::dd::Bdd<Type> newReachableStates = tmp && (!reachableStates);
                    
                    // Check whether new states were indeed discovered.
                    if (!newReachableStates.isZero()) {
                        changed = true;
                    }
                    
                    
                    reachableStates |= newReachableStates;

                    ++iteration;
                } while (changed);
                
                return reachableStates;
            }
            
            template storm::dd::Bdd<storm::dd::DdType::CUDD> computeReachableStates(storm::dd::Bdd<storm::dd::DdType::CUDD> const& initialStates, storm::dd::Bdd<storm::dd::DdType::CUDD> const& transitions, std::set<storm::expressions::Variable> const& rowMetaVariables, std::set<storm::expressions::Variable> const& columnMetaVariables);
            template storm::dd::Bdd<storm::dd::DdType::Sylvan> computeReachableStates(storm::dd::Bdd<storm::dd::DdType::Sylvan> const& initialStates, storm::dd::Bdd<storm::dd::DdType::Sylvan> const& transitions, std::set<storm::expressions::Variable> const& rowMetaVariables, std::set<storm::expressions::Variable> const& columnMetaVariables);
            
        }
    }
}