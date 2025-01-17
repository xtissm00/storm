#include "DftInstantiator.h"
#include "storm-dft/builder/DFTBuilder.h"
#include "storm/exceptions/NotImplementedException.h"

namespace storm::dft {
namespace transformations {

template<typename ParametricType, typename ConstantType>
DftInstantiator<ParametricType, ConstantType>::DftInstantiator(storm::dft::storage::DFT<ParametricType> const &dft) : dft(dft) {}

template<typename ParametricType, typename ConstantType>
std::shared_ptr<storm::dft::storage::DFT<ConstantType>> DftInstantiator<ParametricType, ConstantType>::instantiate(
    storm::utility::parametric::Valuation<ParametricType> const &valuation) {
    storm::dft::builder::DFTBuilder<ConstantType> builder = storm::dft::builder::DFTBuilder<ConstantType>(true);
    for (size_t i = 0; i < dft.nrElements(); ++i) {
        std::shared_ptr<storm::dft::storage::elements::DFTElement<ParametricType> const> element = dft.getElement(i);
        switch (element->type()) {
            case storm::dft::storage::elements::DFTElementType::BE: {
                auto be = std::static_pointer_cast<storm::dft::storage::elements::DFTBE<ParametricType> const>(element);
                switch (be->beType()) {
                    case storm::dft::storage::elements::BEType::CONSTANT: {
                        auto beConst = std::static_pointer_cast<storm::dft::storage::elements::BEConst<ParametricType> const>(element);
                        builder.addBasicElementConst(beConst->name(), beConst->canFail());
                        break;
                    }
                    case storm::dft::storage::elements::BEType::EXPONENTIAL: {
                        auto beExp = std::static_pointer_cast<storm::dft::storage::elements::BEExponential<ParametricType> const>(element);
                        ConstantType activeFailureRate = storm::utility::convertNumber<ConstantType>(beExp->activeFailureRate().evaluate(valuation));
                        ConstantType dormancyFactor = storm::utility::convertNumber<ConstantType>(beExp->dormancyFactor().evaluate(valuation));
                        builder.addBasicElementExponential(beExp->name(), activeFailureRate, dormancyFactor, beExp->isTransient());
                        break;
                    }
                    default:
                        STORM_LOG_THROW(false, storm::exceptions::NotImplementedException, "BE type '" << be->beType() << "' not known.");
                        break;
                }
                break;
            }
            case storm::dft::storage::elements::DFTElementType::AND:
                builder.addAndElement(element->name(), getChildrenVector(element));
                break;
            case storm::dft::storage::elements::DFTElementType::OR:
                builder.addOrElement(element->name(), getChildrenVector(element));
                break;
            case storm::dft::storage::elements::DFTElementType::VOT: {
                auto vot = std::static_pointer_cast<storm::dft::storage::elements::DFTVot<ParametricType> const>(element);
                builder.addVotElement(vot->name(), vot->threshold(), getChildrenVector(vot));
                break;
            }
            case storm::dft::storage::elements::DFTElementType::PAND: {
                auto pand = std::static_pointer_cast<storm::dft::storage::elements::DFTPand<ParametricType> const>(element);
                builder.addPandElement(pand->name(), getChildrenVector(pand), pand->isInclusive());
                break;
            }
            case storm::dft::storage::elements::DFTElementType::POR: {
                auto por = std::static_pointer_cast<storm::dft::storage::elements::DFTPor<ParametricType> const>(element);
                builder.addPorElement(por->name(), getChildrenVector(por), por->isInclusive());
                break;
            }
            case storm::dft::storage::elements::DFTElementType::SPARE:
                builder.addSpareElement(element->name(), getChildrenVector(element));
                break;
            case storm::dft::storage::elements::DFTElementType::PDEP: {
                auto dep = std::static_pointer_cast<storm::dft::storage::elements::DFTDependency<ParametricType> const>(element);
                ConstantType probability = storm::utility::convertNumber<ConstantType>(dep->probability().evaluate(valuation));
                builder.addDepElement(dep->name(), getChildrenVector(dep), probability);
                break;
            }
            case storm::dft::storage::elements::DFTElementType::SEQ:
                builder.addSequenceEnforcer(element->name(), getChildrenVector(element));
                break;
            case storm::dft::storage::elements::DFTElementType::MUTEX:
                builder.addMutex(element->name(), getChildrenVector(element));
                break;
            default:
                STORM_LOG_THROW(false, storm::exceptions::NotImplementedException, "DFT type '" << element->type() << "' not known.");
                break;
        }
    }

    builder.setTopLevel(dft.getTopLevelElement()->name());
    return std::make_shared<storm::dft::storage::DFT<ConstantType>>(builder.build());
}

template<typename ParametricType, typename ConstantType>
std::vector<std::string> DftInstantiator<ParametricType, ConstantType>::getChildrenVector(
    std::shared_ptr<storm::dft::storage::elements::DFTElement<ParametricType> const> element) {
    std::vector<std::string> res;
    if (element->isDependency()) {
        // Dependencies have to be handled separately
        auto dependency = std::static_pointer_cast<storm::dft::storage::elements::DFTDependency<ParametricType> const>(element);
        res.push_back(dependency->triggerEvent()->name());
        for (auto const &depEvent : dependency->dependentEvents()) {
            res.push_back(depEvent->name());
        }
    } else {
        auto elementWithChildren = std::static_pointer_cast<storm::dft::storage::elements::DFTChildren<ParametricType> const>(element);
        for (auto const &child : elementWithChildren->children()) {
            res.push_back(child->name());
        }
    }
    return res;
}

template<typename ParametricType, typename ConstantType>
void DftInstantiator<ParametricType, ConstantType>::checkValid() const {
    // TODO write some checks
}

#ifdef STORM_HAVE_CARL

// Explicitly instantiate the class.
template class DftInstantiator<storm::RationalFunction, double>;

#endif
}  // namespace transformations
}  // namespace storm::dft
