/*
 * AutoParser.cpp
 *
 *  Created on: Jan 20, 2014
 *      Author: Manuel S. Weiand
 */

#include "src/parser/AutoParser.h"

#include "src/parser/MappedFile.h"

#include "src/parser/DeterministicModelParser.h"
#include "src/parser/NondeterministicModelParser.h"
#include "src/parser/MarkovAutomatonParser.h"
#include "src/exceptions/WrongFormatException.h"

#include "src/utility/cstring.h"
#include "src/utility/OsDetection.h"

namespace storm {
	namespace parser {

		using namespace storm::utility::cstring;

		std::shared_ptr<storm::models::AbstractModel<double>> AutoParser::parseModel(std::string const & transitionsFilename,
																					 std::string const & labelingFilename,
																					 std::string const & stateRewardFilename,
																					 std::string const & transitionRewardFilename) {

			// Find and parse the model type hint.
			storm::models::ModelType type = AutoParser::analyzeHint(transitionsFilename);

			// In case the hint string is unknown or could not be found, throw an exception.
			if (type == storm::models::Unknown) {
				LOG4CPLUS_ERROR(logger, "Could not determine file type of " << transitionsFilename << ".");
				LOG4CPLUS_ERROR(logger, "The first line of the file should contain a format hint. Please fix your file and try again.");
				throw storm::exceptions::WrongFormatException() << "Could not determine type of file " << transitionsFilename;
			} else {
				LOG4CPLUS_INFO(logger, "Model type seems to be " << type);
			}

			// Do the actual parsing.
			std::shared_ptr<storm::models::AbstractModel<double>> model;
			switch (type) {
				case storm::models::DTMC: {
					model.reset(new storm::models::Dtmc<double>(std::move(DeterministicModelParser::parseDtmc(transitionsFilename, labelingFilename, stateRewardFilename, transitionRewardFilename))));
					break;
				}
				case storm::models::CTMC: {
					model.reset(new storm::models::Ctmc<double>(std::move(DeterministicModelParser::parseCtmc(transitionsFilename, labelingFilename, stateRewardFilename, transitionRewardFilename))));
					break;
				}
				case storm::models::MDP: {
					model.reset(new storm::models::Mdp<double>(std::move(NondeterministicModelParser::parseMdp(transitionsFilename, labelingFilename, stateRewardFilename, transitionRewardFilename))));
					break;
				}
				case storm::models::CTMDP: {
					model.reset(new storm::models::Ctmdp<double>(std::move(NondeterministicModelParser::parseCtmdp(transitionsFilename, labelingFilename, stateRewardFilename, transitionRewardFilename))));
					break;
				}
				case storm::models::MA: {
					model.reset(new storm::models::MarkovAutomaton<double>(storm::parser::MarkovAutomatonParser::parseMarkovAutomaton(transitionsFilename, labelingFilename, stateRewardFilename, transitionRewardFilename)));
					break;
				}
				default:
					LOG4CPLUS_WARN(logger, "Unknown/Unhandled Model Type which cannot be parsed.");  // Unknown
			}

			return model;
		}

		storm::models::ModelType AutoParser::analyzeHint(std::string const & filename) {
			storm::models::ModelType hintType = storm::models::Unknown;

			// Open the file.
			MappedFile file(filename.c_str());
			char* buf = file.getData();

			// Find and read in the hint.
			char hint[65];
			// %60s => The input hint can be AT MOST 60 chars long.
		#ifdef WINDOWS
			sscanf_s(buf, "%60s", hint, sizeof(hint));
		#else
			sscanf(buf, "%60s", hint);
		#endif

			for (char* c = hint; *c != '\0'; c++) *c = toupper(*c);

			// Check if the hint value is known and store the appropriate enum value.
			if (strncmp(hint, "DTMC", sizeof(hint)) == 0) hintType = storm::models::DTMC;
			else if (strncmp(hint, "CTMC", sizeof(hint)) == 0) hintType = storm::models::CTMC;
			else if (strncmp(hint, "MDP", sizeof(hint)) == 0) hintType = storm::models::MDP;
			else if (strncmp(hint, "CTMDP", sizeof(hint)) == 0) hintType = storm::models::CTMDP;
			else if (strncmp(hint, "MA", sizeof(hint)) == 0) hintType = storm::models::MA;

			return hintType;
		}

	} // namespace parser
} // namespace storm
