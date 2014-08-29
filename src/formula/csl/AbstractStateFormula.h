/*
 * AbstractStateFormula.h
 *
 *  Created on: 19.10.2012
 *      Author: Thomas Heinemann
 */

#ifndef STORM_FORMULA_CSL_ABSTRACTSTATEFORMULA_H_
#define STORM_FORMULA_CSL_ABSTRACTSTATEFORMULA_H_

#include "src/formula/csl/AbstractCslFormula.h"
#include "src/storage/BitVector.h"
#include "src/modelchecker/csl/ForwardDeclarations.h"

namespace storm {
namespace property {
namespace csl {

/*!
 * Abstract base class for Csl state formulas.
 */
template <class T>
class AbstractStateFormula : public storm::property::csl::AbstractCslFormula<T> {

public:

	/*!
	 * Empty virtual destructor.
	 */
	virtual ~AbstractStateFormula() {
		// Intentionally left empty
	}

	/*!
	 * Clones the called object.
	 *
	 * Performs a "deep copy", i.e. the subtrees of the new object are clones of the original ones
	 *
	 * @note This function is not implemented in this class.
	 *
	 * @returns A deep copy of the called object.
	 */
	virtual std::shared_ptr<AbstractStateFormula<T>> clone() const = 0;

	/*!
	 * Calls the model checker to check this formula.
	 * Needed to infer the correct type of formula class.
	 *
	 * @note This function should only be called in a generic check function of a model checker class. For other uses,
	 *       the methods of the model checker should be used.
	 *
	 * @note This function is not implemented in this class.
	 *
	 * @returns A bit vector indicating all states that satisfy the formula represented by the called object.
	 */
	virtual storm::storage::BitVector check(storm::modelchecker::csl::AbstractModelChecker<T> const & modelChecker) const = 0;
};

} //namespace csl
} //namespace property
} //namespace storm


#endif /* STORM_FORMULA_CSL_AbstractSTATEFORMULA_H_ */
