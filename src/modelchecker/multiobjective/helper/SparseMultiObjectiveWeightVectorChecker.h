#ifndef STORM_MODELCHECKER_MULTIOBJECTIVE_HELPER_SPARSEMULTIOBJECTIVEWEIGHTVECTORCHECKER_H_
#define STORM_MODELCHECKER_MULTIOBJECTIVE_HELPER_SPARSEMULTIOBJECTIVEWEIGHTVECTORCHECKER_H_

#include <vector>

#include "src/modelchecker/multiobjective/helper/SparseMultiObjectivePreprocessorData.h"
#include "src/storage/TotalScheduler.h"

namespace storm {
    namespace modelchecker {
        namespace helper {
            
            /*!
             * Helper Class that takes preprocessed multi objective data and a weight vector and ...
             * - computes the maximal expected reward w.r.t. the weighted sum of the rewards of the individual objectives
             * - extracts the scheduler that induces this maximum
             * - computes for each objective the value induced by this scheduler
             */
            template <class SparseModelType>
            class SparseMultiObjectiveWeightVectorChecker {
            public:
                typedef typename SparseModelType::ValueType ValueType;
                typedef typename SparseModelType::RewardModelType RewardModelType;
                typedef SparseMultiObjectivePreprocessorData<SparseModelType> PreprocessorData;
                
                SparseMultiObjectiveWeightVectorChecker(PreprocessorData const& data);
                
                /*!
                 * - computes the maximal expected reward w.r.t. the weighted sum of the rewards of the individual objectives
                 * - extracts the scheduler that induces this maximum
                 * - computes for each objective the value induced by this scheduler
                 */
                void check(std::vector<ValueType> const& weightVector);
                
                /*!
                 * Sets the maximum gap that is allowed between the lower and upper bound of the result of some objective.
                 */
                void setMaximumLowerUpperBoundGap(ValueType const& value);
                
                /*!
                 * Retrieves the maximum gap that is allowed between the lower and upper bound of the result of some objective.
                 */
                ValueType const& getMaximumLowerUpperBoundGap() const;
                
                /*!
                 * Retrieves the results of the individual objectives at the initial state of the given model.
                 * Note that check(..) has to be called before retrieving results. Otherwise, an exception is thrown.
                 * Also note that there is no guarantee that the lower/upper bounds are sound
                 * as long as the underlying solution methods are unsound (e.g., standard value iteration).
                 */
                std::vector<ValueType> getLowerBoundsOfInitialStateResults() const;
                std::vector<ValueType> getUpperBoundsOfInitialStateResults() const;
                
                /*!
                 * Retrieves a scheduler that induces the current values
                 * Note that check(..) has to be called before retrieving the scheduler. Otherwise, an exception is thrown.
                 */
                storm::storage::TotalScheduler const& getScheduler() const;
                
                
            protected:
                
                /*!
                 * Determines the scheduler that maximizes the weighted reward vector of the unbounded objectives
                 *
                 * @param weightedRewardVector the weighted rewards (only considering the unbounded objectives)
                 */
                void unboundedWeightedPhase(std::vector<ValueType> const& weightedRewardVector);
                
                /*!
                 * Computes the values of the objectives that do not have a stepBound w.r.t. the scheduler computed in the unboundedWeightedPhase
                 *
                 * @param weightVector the weight vector of the current check
                 */
                void unboundedIndividualPhase(std::vector<ValueType> const& weightVector);
                
                /*!
                 * For each time epoch (starting with the maximal stepBound occurring in the objectives), this method
                 * - determines the objectives that are relevant in the current time epoch
                 * - determines the maximizing scheduler for the weighted reward vector of these objectives
                 * - computes the values of these objectives w.r.t. this scheduler
                 *
                 * @param weightVector the weight vector of the current check
                 * @param weightedRewardVector the weighted rewards considering the unbounded objectives. Will be invalidated after calling this.
                 */
                virtual void boundedPhase(std::vector<ValueType> const& weightVector, std::vector<ValueType>& weightedRewardVector) = 0;
                
                /*!
                 * Transforms the results of a min-max-solver that considers a reduced model (without end components) to a result for the original (unreduced) model
                 */
                void transformReducedSolutionToOriginalModel(storm::storage::SparseMatrix<ValueType> const& reducedMatrix,
                                                             std::vector<ValueType> const& reducedSolution,
                                                             std::vector<uint_fast64_t> const& reducedOptimalChoices,
                                                             std::vector<uint_fast64_t> const& reducedToOriginalChoiceMapping,
                                                             std::vector<uint_fast64_t> const& originalToReducedStateMapping,
                                                             storm::storage::SparseMatrix<ValueType> const& originalMatrix,
                                                             std::vector<ValueType>& originalSolution,
                                                             std::vector<uint_fast64_t>& originalOptimalChoices) const;
                
                
                // stores the considered information of the multi-objective model checking problem
                PreprocessorData const& data;
                // stores the indices of the objectives for which there is no upper time bound
                storm::storage::BitVector objectivesWithNoUpperTimeBound;
                // stores the (discretized) state action rewards for each objective.
                std::vector<std::vector<ValueType>>discreteActionRewards;
                
                // stores the set of states for which it is allowed to visit them infinitely often
                // This means that, if one of the states is part of a neutral EC, it is allowed to
                // stay in this EC forever.
                storm::storage::BitVector statesThatAreAllowedToBeVisitedInfinitelyOften;
                
                // becomes true after the first call of check(..)
                bool checkHasBeenCalled;
                
                // stores the maximum gap that is allowed between the lower and upper bound of the result of some objective.
                ValueType maximumLowerUpperBoundGap;
                
                // The result for the weighted reward vector (for all states of the model)
                std::vector<ValueType> weightedResult;
                // The lower bounds of the results for the individual objectives (w.r.t. all states of the model)
                std::vector<std::vector<ValueType>> objectiveResults;
                // Stores for each objective the distance between the computed result (w.r.t. the initial state) and a lower/upper bound for the actual result.
                // The distances are stored as a (possibly negative) offset that has to be added to to the objectiveResults.
                // Note that there is no guarantee that the lower/upper bounds are sound as long as the underlying solution method is not sound (e.g. standard value iteration).
                std::vector<ValueType> offsetsToLowerBound;
                std::vector<ValueType> offsetsToUpperBound;
                
                // The scheduler that maximizes the weighted rewards
                storm::storage::TotalScheduler scheduler;
                
            };
            
        }
    }
}

#endif /* STORM_MODELCHECKER_MULTIOBJECTIVE_HELPER_SPARSEMULTIOBJECTIVEWEIGHTEDVECTORCHECKER_H_ */
