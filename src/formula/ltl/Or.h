/*
 * Or.h
 *
 *  Created on: 22.04.2013
 *      Author: thomas
 */

#ifndef STORM_FORMULA_LTL_OR_H_
#define STORM_FORMULA_LTL_OR_H_

#include "AbstractLtlFormula.h"

namespace storm {
namespace property {
namespace ltl {

// Forward declaration for the interface class.
template <class T> class Or;

/*!
 * Interface class for model checkers that support Or.
 *
 * All model checkers that support the formula class Or must inherit
 * this pure virtual class.
 */
template <class T>
class IOrModelChecker {
	public:

		/*!
		 * Empty virtual destructor.
		 */
		virtual ~IOrModelChecker() {
			// Intentionally left empty
		}

		/*!
		 * Evaluates Or formula within a model checker.
		 *
		 * @param obj Formula object with subformulas.
		 * @return Result of the formula for every node.
		 */
		virtual std::vector<T> checkOr(const Or<T>& obj) const = 0;
};

/*!
 * Class for an abstract formula tree with an Or node as root.
 *
 * Has two Ltl formulas as sub formulas/trees.
 *
 * As Or is commutative, the order is \e theoretically not important, but will influence the order in which
 * the model checker works.
 *
 * The object has shared ownership of its subtrees. If this object is deleted and no other object has a shared
 * ownership of the subtrees they will be deleted as well.
 *
 * @see AbstractLtlFormula
 */
template <class T>
class Or: public storm::property::ltl::AbstractLtlFormula<T> {

public:

	/*!
	 * Creates an Or node without subnodes.
	 * The resulting object will not represent a complete formula!
	 */
	Or() : left(nullptr), right(nullptr) {
		// Intentionally left empty.
	}

	/*!
	 * Creates an Or node with the parameters as subtrees.
	 *
	 * @param left The left sub formula.
	 * @param right The right sub formula.
	 */
	Or(std::shared_ptr<AbstractLtlFormula<T>> const & left, std::shared_ptr<AbstractLtlFormula<T>> const & right) : left(left), right(right) {
		// Intentionally left empty.
	}

	/*!
	 * Empty virtual destructor.
	 */
	virtual ~Or() {
		// Intentionally left empty.
	}

	/*!
	 * Clones the called object.
	 *
	 * Performs a "deep copy", i.e. the subtrees of the new object are clones of the original ones.
	 *
	 * @returns A new Or object that is a deep copy of the called object.
	 */
	virtual std::shared_ptr<AbstractLtlFormula<T>> clone() const override {
		std::shared_ptr<Or<T>> result(new Or<T>());
		if (this->isLeftSet()) {
		  result->setLeft(left->clone());
		}
		if (this->isRightSet()) {
		  result->setRight(right->clone());
		}
		return result;
	}

	/*!
	 * Calls the model checker to check this formula.
	 * Needed to infer the correct type of formula class.
	 *
	 * @note This function should only be called in a generic check function of a model checker class. For other uses,
	 *       the methods of the model checker should be used.
	 *
	 * @returns A bit vector indicating all states that satisfy the formula represented by the called object.
	 */
	virtual std::vector<T> check(storm::modelchecker::ltl::AbstractModelChecker<T> const & modelChecker) const override {
		return modelChecker.template as<IOrModelChecker>()->checkOr(*this);
	}

	/*!
	 * Returns a textual representation of the formula tree with this node as root.
	 *
	 * @returns A string representing the formula tree.
	 */
	virtual std::string toString() const override {
		std::string result = "(";
		result += left->toString();
		result += " | ";
		result += right->toString();
		result += ")";
		return result;
	}

	/*!
	 * Returns whether the formula is a propositional logic formula.
	 * That is, this formula and all its subformulas consist only of And, Or, Not and AP.
	 *
	 * @return True iff this is a propositional logic formula.
	 */
	virtual bool isPropositional() const override {
		return left->isPropositional() && right->isPropositional();
	}

	/*!
	 * Gets the left child node.
	 *
	 * @returns The left child node.
	 */
	std::shared_ptr<AbstractLtlFormula<T>> const & getLeft() const {
		return left;
	}

	/*!
	 * Gets the right child node.
	 *
	 * @returns The right child node.
	 */
	std::shared_ptr<AbstractLtlFormula<T>> const & getRight() const {
		return right;
	}

	/*!
	 * Sets the left child node.
	 *
	 * @param newLeft The new left child.
	 */
	void setLeft(std::shared_ptr<AbstractLtlFormula<T>> const & newLeft) {
		left = newLeft;
	}

	/*!
	 * Sets the right child node.
	 *
	 * @param newRight The new right child.
	 */
	void setRight(std::shared_ptr<AbstractLtlFormula<T>> const & newRight) {
		right = newRight;
	}

	/*!
	 * Checks if the left child is set, i.e. it does not point to null.
	 *
	 * @return True iff the left child is set.
	 */
	bool isLeftSet() const {
		return left.get() != nullptr;
	}

	/*!
	 * Checks if the right child is set, i.e. it does not point to null.
	 *
	 * @return True iff the left right is set.
	 */
	bool isRightSet() const {
		return right.get() != nullptr;
	}

private:

	// The left child node.
	std::shared_ptr<AbstractLtlFormula<T>> left;

	// The right child node.
	std::shared_ptr<AbstractLtlFormula<T>> right;
};

} /* namespace ltl */
} /* namespace property */
} /* namespace storm */
#endif /* OR_H_ */
