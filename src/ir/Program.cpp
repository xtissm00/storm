/*
 * Program.cpp
 *
 *  Created on: 12.01.2013
 *      Author: Christian Dehnert
 */

#include "Program.h"
#include "exceptions/InvalidArgumentException.h"

#include <sstream>
#include <iostream>

#include "log4cplus/logger.h"
#include "log4cplus/loggingmacros.h"
extern log4cplus::Logger logger;

namespace storm {

namespace ir {

// Initializes all members with their default constructors.
Program::Program() : modelType(UNDEFINED), booleanUndefinedConstantExpressions(), integerUndefinedConstantExpressions(), doubleUndefinedConstantExpressions(), modules(), rewards(), actions(), actionsToModuleIndexMap() {
	// Nothing to do here.
}

// Initializes all members according to the given values.
Program::Program(ModelType modelType, std::map<std::string, std::shared_ptr<storm::ir::expressions::BooleanConstantExpression>> booleanUndefinedConstantExpressions, std::map<std::string, std::shared_ptr<storm::ir::expressions::IntegerConstantExpression>> integerUndefinedConstantExpressions, std::map<std::string, std::shared_ptr<storm::ir::expressions::DoubleConstantExpression>> doubleUndefinedConstantExpressions, std::vector<storm::ir::Module> modules, std::map<std::string, storm::ir::RewardModel> rewards, std::map<std::string, std::shared_ptr<storm::ir::expressions::BaseExpression>> labels)
	: modelType(modelType), booleanUndefinedConstantExpressions(booleanUndefinedConstantExpressions), integerUndefinedConstantExpressions(integerUndefinedConstantExpressions), doubleUndefinedConstantExpressions(doubleUndefinedConstantExpressions), modules(modules), rewards(rewards), labels(labels), actionsToModuleIndexMap() {
	// Build actionsToModuleIndexMap
    for (unsigned int id = 0; id < this->modules.size(); id++) {
		for (auto action : this->modules[id].getActions()) {
			if (this->actionsToModuleIndexMap.count(action) == 0) {
				this->actionsToModuleIndexMap[action] = std::set<uint_fast64_t>();
			}
            this->actionsToModuleIndexMap[action].insert(id);
            this->actions.insert(action);
        }
    }
}

Program::ModelType Program::getModelType() const {
	return modelType;
}

// Build a string representation of the program.
std::string Program::toString() const {
	std::stringstream result;
	switch (modelType) {
	case UNDEFINED: result << "undefined"; break;
	case DTMC: result << "dtmc"; break;
	case CTMC: result << "ctmc"; break;
	case MDP: result << "mdp"; break;
	case CTMDP: result << "ctmdp"; break;
	}
	result << std::endl;

	for (auto element : booleanUndefinedConstantExpressions) {
		result << "const bool " << element.first << " [" << element.second->toString() << "]" << ";" << std::endl;
	}
	for (auto element : integerUndefinedConstantExpressions) {
		result << "const int " << element.first << " [" << element.second->toString() << "]" << ";" << std::endl;
	}
	for (auto element : doubleUndefinedConstantExpressions) {
		result << "const double " << element.first << " [" << element.second->toString() << "]" << ";" << std::endl;
	}
	result << std::endl;

	for (auto module : modules) {
		result << module.toString() << std::endl;
	}

	for (auto rewardModel : rewards) {
		result << rewardModel.first << ": " << rewardModel.second.toString() << std::endl;
	}

	for (auto label : labels) {
		result << "label " << label.first << " = " << label.second->toString() <<";" << std::endl;
	}

	return result.str();
}

uint_fast64_t Program::getNumberOfModules() const {
	return this->modules.size();
}

storm::ir::Module const& Program::getModule(uint_fast64_t index) const {
	return this->modules[index];
}

// Return set of actions.
std::set<std::string> const& Program::getActions() const {
	return this->actions;
}

// Return modules with given action.
std::set<uint_fast64_t> const Program::getModulesByAction(std::string const& action) const {
	auto res = this->actionsToModuleIndexMap.find(action);
	if (res == this->actionsToModuleIndexMap.end()) {
		return std::set<uint_fast64_t>();
	} else {
		return res->second;
	}
}

storm::ir::RewardModel Program::getRewardModel(std::string const & name) const {
	auto it = this->rewards.find(name);
	if (it == this->rewards.end()) {
		LOG4CPLUS_ERROR(logger, "The given reward model \"" << name << "\" does not exist. We will proceed without rewards.");
		throw "Rewardmodel does not exist.";
	} else {
		return it->second;
	}
}

std::map<std::string, std::shared_ptr<storm::ir::expressions::BaseExpression>> Program::getLabels() const {
	return this->labels;
}

std::string Program::getVariableString() const {
	std::map<uint_fast64_t, std::string> bools;
	std::map<uint_fast64_t, std::string> ints;
	uint_fast64_t maxInt = 0, maxBool = 0;
	for (Module module: this->modules) {
		for (uint_fast64_t i = 0; i < module.getNumberOfBooleanVariables(); i++) {
			storm::ir::BooleanVariable var = module.getBooleanVariable(i);
			bools[var.getIndex()] = var.getName();
			if (var.getIndex() >= maxBool) maxBool = var.getIndex()+1;
		}
		for (uint_fast64_t i = 0; i < module.getNumberOfIntegerVariables(); i++) {
			storm::ir::IntegerVariable var = module.getIntegerVariable(i);
			ints[var.getIndex()] = var.getName();
			if (var.getIndex() >= maxInt) maxInt = var.getIndex()+1;
		}
	}
	std::stringstream ss;
	for (uint_fast64_t i = 0; i < maxBool; i++) ss << bools[i] << "\t";
	for (uint_fast64_t i = 0; i < maxInt; i++) ss << ints[i] << "\t";
	return ss.str();
}

} // namespace ir

} // namepsace storm