/*
 * AbstractPathFormula.h
 *
 *  Created on: 19.10.2012
 *      Author: Thomas Heinemann
 */

#ifndef STORM_FORMULA_PRCTL_ABSTRACTPATHFORMULA_H_
#define STORM_FORMULA_PRCTL_ABSTRACTPATHFORMULA_H_

#include "src/formula/prctl/AbstractPrctlFormula.h"
#include "src/modelchecker/prctl/ForwardDeclarations.h"

#include <vector>
#include <iostream>
#include <typeinfo>

namespace storm {
namespace property {
namespace prctl {

/*!
 * Abstract base class for Prctl path formulas.
 *
 * @note Differing from the formal definitions of PRCTL a path formula may be the root of a PRCTL formula.
 *       The result of a modelchecking process on such a formula is a vector representing the satisfaction probabilities for each state of the model.
 */
template <class T>
class AbstractPathFormula : public virtual storm::property::prctl::AbstractPrctlFormula<T> {

public:

	/*!
	 * The virtual destructor.
	 */
	virtual ~AbstractPathFormula() {
		// Intentionally left empty
	}

	/*!
	 * Clones the called object.
	 *
	 * Performs a "deep copy", i.e. the subtrees of the new object are clones of the original ones.
	 *
	 * @note This function is not implemented in this class.
	 *
	 * @returns A deep copy of the called object.
	 */
	virtual std::shared_ptr<AbstractPathFormula<T>> clone() const = 0;

	/*!
	 * Calls the model checker to check this formula.
	 * Needed to infer the correct type of formula class.
	 *
	 * @note This function should only be called in a generic check function of a model checker class. For other uses,
	 *       the methods of the model checker should be used.
	 *
	 * @note This function is not implemented in this class.
	 *
	 * @returns A vector indicating the probability that the formula holds for each state.
	 */
	virtual std::vector<T> check(const storm::modelchecker::prctl::AbstractModelChecker<T>& modelChecker, bool qualitative) const = 0;
};

} //namespace prctl
} //namespace property
} //namespace storm

#endif /* STORM_FORMULA_PRCTL_ABSTRACTPATHFORMULA_H_ */
