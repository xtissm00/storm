#include "gtest/gtest.h"
#include "storm-config.h"

#include "src/parser/PrismParser.h"
#include "src/models/symbolic/Dtmc.h"
#include "src/models/symbolic/Mdp.h"
#include "src/models/symbolic/StandardRewardModel.h"
#include "src/models/sparse/Dtmc.h"
#include "src/models/sparse/Mdp.h"
#include "src/models/sparse/StandardRewardModel.h"
#include "src/builder/DdPrismModelBuilder.h"
#include "src/builder/ExplicitModelBuilder.h"
#include "src/utility/graph.h"
#include "src/storage/dd/Add.h"
#include "src/storage/dd/Bdd.h"
#include "src/storage/dd/DdManager.h"

TEST(GraphTest, SymbolicProb01_Cudd) {
    storm::prism::Program program = storm::parser::PrismParser::parse(STORM_CPP_TESTS_BASE_PATH "/functional/builder/crowds-5-5.pm");
    std::shared_ptr<storm::models::symbolic::Model<storm::dd::DdType::CUDD>> model = storm::builder::DdPrismModelBuilder<storm::dd::DdType::CUDD>().build(program);
    
    ASSERT_TRUE(model->getType() == storm::models::ModelType::Dtmc);
    
    {
        // This block is necessary, so the BDDs get disposed before the manager (contained in the model).
        std::pair<storm::dd::Bdd<storm::dd::DdType::CUDD>, storm::dd::Bdd<storm::dd::DdType::CUDD>> statesWithProbability01;
        
        ASSERT_NO_THROW(statesWithProbability01 = storm::utility::graph::performProb01(*model->as<storm::models::symbolic::Dtmc<storm::dd::DdType::CUDD>>(), model->getReachableStates(), model->getStates("observe0Greater1")));
        EXPECT_EQ(4409ul, statesWithProbability01.first.getNonZeroCount());
        EXPECT_EQ(1316ul, statesWithProbability01.second.getNonZeroCount());
        
        ASSERT_NO_THROW(statesWithProbability01 = storm::utility::graph::performProb01(*model->as<storm::models::symbolic::Dtmc<storm::dd::DdType::CUDD>>(), model->getReachableStates(), model->getStates("observeIGreater1")));
        EXPECT_EQ(1091ul, statesWithProbability01.first.getNonZeroCount());
        EXPECT_EQ(4802ul, statesWithProbability01.second.getNonZeroCount());
        
        ASSERT_NO_THROW(statesWithProbability01 = storm::utility::graph::performProb01(*model->as<storm::models::symbolic::Dtmc<storm::dd::DdType::CUDD>>(), model->getReachableStates(), model->getStates("observeOnlyTrueSender")));
        EXPECT_EQ(5829ul, statesWithProbability01.first.getNonZeroCount());
        EXPECT_EQ(1032ul, statesWithProbability01.second.getNonZeroCount());
    }
}

TEST(GraphTest, SymbolicProb01_Sylvan) {
    storm::prism::Program program = storm::parser::PrismParser::parse(STORM_CPP_TESTS_BASE_PATH "/functional/builder/crowds-5-5.pm");
    std::shared_ptr<storm::models::symbolic::Model<storm::dd::DdType::Sylvan>> model = storm::builder::DdPrismModelBuilder<storm::dd::DdType::Sylvan>().build(program);
    
    ASSERT_TRUE(model->getType() == storm::models::ModelType::Dtmc);
    
    {
        // This block is necessary, so the BDDs get disposed before the manager (contained in the model).
        std::pair<storm::dd::Bdd<storm::dd::DdType::Sylvan>, storm::dd::Bdd<storm::dd::DdType::Sylvan>> statesWithProbability01;
        
        ASSERT_NO_THROW(statesWithProbability01 = storm::utility::graph::performProb01(*model->as<storm::models::symbolic::Dtmc<storm::dd::DdType::Sylvan>>(), model->getReachableStates(), model->getStates("observe0Greater1")));
        EXPECT_EQ(4409ul, statesWithProbability01.first.getNonZeroCount());
        EXPECT_EQ(1316ul, statesWithProbability01.second.getNonZeroCount());
        
        ASSERT_NO_THROW(statesWithProbability01 = storm::utility::graph::performProb01(*model->as<storm::models::symbolic::Dtmc<storm::dd::DdType::Sylvan>>(), model->getReachableStates(), model->getStates("observeIGreater1")));
        EXPECT_EQ(1091ul, statesWithProbability01.first.getNonZeroCount());
        EXPECT_EQ(4802ul, statesWithProbability01.second.getNonZeroCount());
        
        ASSERT_NO_THROW(statesWithProbability01 = storm::utility::graph::performProb01(*model->as<storm::models::symbolic::Dtmc<storm::dd::DdType::Sylvan>>(), model->getReachableStates(), model->getStates("observeOnlyTrueSender")));
        EXPECT_EQ(5829ul, statesWithProbability01.first.getNonZeroCount());
        EXPECT_EQ(1032ul, statesWithProbability01.second.getNonZeroCount());
    }
}

TEST(GraphTest, SymbolicProb01MinMax_Cudd) {
    storm::prism::Program program = storm::parser::PrismParser::parse(STORM_CPP_TESTS_BASE_PATH "/functional/builder/leader3.nm");
    std::shared_ptr<storm::models::symbolic::Model<storm::dd::DdType::CUDD>> model = storm::builder::DdPrismModelBuilder<storm::dd::DdType::CUDD>().build(program);
    
    ASSERT_TRUE(model->getType() == storm::models::ModelType::Mdp);

    {
        // This block is necessary, so the BDDs get disposed before the manager (contained in the model).
        std::pair<storm::dd::Bdd<storm::dd::DdType::CUDD>, storm::dd::Bdd<storm::dd::DdType::CUDD>> statesWithProbability01;
        
        ASSERT_NO_THROW(statesWithProbability01 = storm::utility::graph::performProb01Min(*model->as<storm::models::symbolic::Mdp<storm::dd::DdType::CUDD>>(), model->getReachableStates(), model->getStates("elected")));
        EXPECT_EQ(0ul, statesWithProbability01.first.getNonZeroCount());
        EXPECT_EQ(364ul, statesWithProbability01.second.getNonZeroCount());
        
        ASSERT_NO_THROW(statesWithProbability01 = storm::utility::graph::performProb01Max(*model->as<storm::models::symbolic::Mdp<storm::dd::DdType::CUDD>>(), model->getReachableStates(), model->getStates("elected")));
        EXPECT_EQ(0ul, statesWithProbability01.first.getNonZeroCount());
        EXPECT_EQ(364ul, statesWithProbability01.second.getNonZeroCount());
    }
    
    program = storm::parser::PrismParser::parse(STORM_CPP_TESTS_BASE_PATH "/functional/builder/coin2-2.nm");
    model = storm::builder::DdPrismModelBuilder<storm::dd::DdType::CUDD>().build(program);
    
    ASSERT_TRUE(model->getType() == storm::models::ModelType::Mdp);

    {
        // This block is necessary, so the BDDs get disposed before the manager (contained in the model).
        std::pair<storm::dd::Bdd<storm::dd::DdType::CUDD>, storm::dd::Bdd<storm::dd::DdType::CUDD>> statesWithProbability01;
        
        ASSERT_NO_THROW(statesWithProbability01 = storm::utility::graph::performProb01Min(*model->as<storm::models::symbolic::Mdp<storm::dd::DdType::CUDD>>(), model->getReachableStates(), model->getStates("all_coins_equal_0")));
        EXPECT_EQ(77ul, statesWithProbability01.first.getNonZeroCount());
        EXPECT_EQ(149ul, statesWithProbability01.second.getNonZeroCount());
        
        ASSERT_NO_THROW(statesWithProbability01 = storm::utility::graph::performProb01Max(*model->as<storm::models::symbolic::Mdp<storm::dd::DdType::CUDD>>(), model->getReachableStates(), model->getStates("all_coins_equal_0")));
        EXPECT_EQ(74ul, statesWithProbability01.first.getNonZeroCount());
        EXPECT_EQ(198ul, statesWithProbability01.second.getNonZeroCount());
        
        ASSERT_NO_THROW(statesWithProbability01 = storm::utility::graph::performProb01Min(*model->as<storm::models::symbolic::Mdp<storm::dd::DdType::CUDD>>(), model->getReachableStates(), model->getStates("all_coins_equal_1")));
        EXPECT_EQ(94ul, statesWithProbability01.first.getNonZeroCount());
        EXPECT_EQ(33ul, statesWithProbability01.second.getNonZeroCount());
        
        ASSERT_NO_THROW(statesWithProbability01 = storm::utility::graph::performProb01Max(*model->as<storm::models::symbolic::Mdp<storm::dd::DdType::CUDD>>(), model->getReachableStates(), model->getStates("all_coins_equal_1")));
        EXPECT_EQ(83ul, statesWithProbability01.first.getNonZeroCount());
        EXPECT_EQ(35ul, statesWithProbability01.second.getNonZeroCount());
    }
        
    program = storm::parser::PrismParser::parse(STORM_CPP_TESTS_BASE_PATH "/functional/builder/csma2-2.nm");
    model = storm::builder::DdPrismModelBuilder<storm::dd::DdType::CUDD>().build(program);
    
    ASSERT_TRUE(model->getType() == storm::models::ModelType::Mdp);
    
    {
        // This block is necessary, so the BDDs get disposed before the manager (contained in the model).
        std::pair<storm::dd::Bdd<storm::dd::DdType::CUDD>, storm::dd::Bdd<storm::dd::DdType::CUDD>> statesWithProbability01;
        
        ASSERT_NO_THROW(statesWithProbability01 = storm::utility::graph::performProb01Min(*model->as<storm::models::symbolic::Mdp<storm::dd::DdType::CUDD>>(), model->getReachableStates(), model->getStates("collision_max_backoff")));
        EXPECT_EQ(993ul, statesWithProbability01.first.getNonZeroCount());
        EXPECT_EQ(16ul, statesWithProbability01.second.getNonZeroCount());
        
        ASSERT_NO_THROW(statesWithProbability01 = storm::utility::graph::performProb01Max(*model->as<storm::models::symbolic::Mdp<storm::dd::DdType::CUDD>>(), model->getReachableStates(), model->getStates("collision_max_backoff")));
        EXPECT_EQ(993ul, statesWithProbability01.first.getNonZeroCount());
        EXPECT_EQ(16ul, statesWithProbability01.second.getNonZeroCount());
    }
}

TEST(GraphTest, SymbolicProb01MinMax_Sylvan) {
    storm::prism::Program program = storm::parser::PrismParser::parse(STORM_CPP_TESTS_BASE_PATH "/functional/builder/leader3.nm");
    std::shared_ptr<storm::models::symbolic::Model<storm::dd::DdType::Sylvan>> model = storm::builder::DdPrismModelBuilder<storm::dd::DdType::Sylvan>().build(program);
    
    ASSERT_TRUE(model->getType() == storm::models::ModelType::Mdp);

    {
        // This block is necessary, so the BDDs get disposed before the manager (contained in the model).
        std::pair<storm::dd::Bdd<storm::dd::DdType::Sylvan>, storm::dd::Bdd<storm::dd::DdType::Sylvan>> statesWithProbability01;
        
        ASSERT_NO_THROW(statesWithProbability01 = storm::utility::graph::performProb01Min(*model->as<storm::models::symbolic::Mdp<storm::dd::DdType::Sylvan>>(), model->getReachableStates(), model->getStates("elected")));
        EXPECT_EQ(0ul, statesWithProbability01.first.getNonZeroCount());
        EXPECT_EQ(364ul, statesWithProbability01.second.getNonZeroCount());
        
        ASSERT_NO_THROW(statesWithProbability01 = storm::utility::graph::performProb01Max(*model->as<storm::models::symbolic::Mdp<storm::dd::DdType::Sylvan>>(), model->getReachableStates(), model->getStates("elected")));
        EXPECT_EQ(0ul, statesWithProbability01.first.getNonZeroCount());
        EXPECT_EQ(364ul, statesWithProbability01.second.getNonZeroCount());
    }
    
    program = storm::parser::PrismParser::parse(STORM_CPP_TESTS_BASE_PATH "/functional/builder/coin2-2.nm");
    model = storm::builder::DdPrismModelBuilder<storm::dd::DdType::Sylvan>().build(program);
    
    ASSERT_TRUE(model->getType() == storm::models::ModelType::Mdp);
    
    {
        // This block is necessary, so the BDDs get disposed before the manager (contained in the model).
        std::pair<storm::dd::Bdd<storm::dd::DdType::Sylvan>, storm::dd::Bdd<storm::dd::DdType::Sylvan>> statesWithProbability01;

        ASSERT_NO_THROW(statesWithProbability01 = storm::utility::graph::performProb01Min(*model->as<storm::models::symbolic::Mdp<storm::dd::DdType::Sylvan>>(), model->getReachableStates(), model->getStates("all_coins_equal_0")));
        EXPECT_EQ(77ul, statesWithProbability01.first.getNonZeroCount());
        EXPECT_EQ(149ul, statesWithProbability01.second.getNonZeroCount());
        
        ASSERT_NO_THROW(statesWithProbability01 = storm::utility::graph::performProb01Max(*model->as<storm::models::symbolic::Mdp<storm::dd::DdType::Sylvan>>(), model->getReachableStates(), model->getStates("all_coins_equal_0")));
        EXPECT_EQ(74ul, statesWithProbability01.first.getNonZeroCount());
        EXPECT_EQ(198ul, statesWithProbability01.second.getNonZeroCount());
        
        ASSERT_NO_THROW(statesWithProbability01 = storm::utility::graph::performProb01Min(*model->as<storm::models::symbolic::Mdp<storm::dd::DdType::Sylvan>>(), model->getReachableStates(), model->getStates("all_coins_equal_1")));
        EXPECT_EQ(94ul, statesWithProbability01.first.getNonZeroCount());
        EXPECT_EQ(33ul, statesWithProbability01.second.getNonZeroCount());
        
        ASSERT_NO_THROW(statesWithProbability01 = storm::utility::graph::performProb01Max(*model->as<storm::models::symbolic::Mdp<storm::dd::DdType::Sylvan>>(), model->getReachableStates(), model->getStates("all_coins_equal_1")));
        EXPECT_EQ(83ul, statesWithProbability01.first.getNonZeroCount());
        EXPECT_EQ(35ul, statesWithProbability01.second.getNonZeroCount());
    }
    
    program = storm::parser::PrismParser::parse(STORM_CPP_TESTS_BASE_PATH "/functional/builder/csma2-2.nm");
    model = storm::builder::DdPrismModelBuilder<storm::dd::DdType::Sylvan>().build(program);
    
    ASSERT_TRUE(model->getType() == storm::models::ModelType::Mdp);
    
    {
        // This block is necessary, so the BDDs get disposed before the manager (contained in the model).
        std::pair<storm::dd::Bdd<storm::dd::DdType::Sylvan>, storm::dd::Bdd<storm::dd::DdType::Sylvan>> statesWithProbability01;
        
        ASSERT_NO_THROW(statesWithProbability01 = storm::utility::graph::performProb01Min(*model->as<storm::models::symbolic::Mdp<storm::dd::DdType::Sylvan>>(), model->getReachableStates(), model->getStates("collision_max_backoff")));
        EXPECT_EQ(993ul, statesWithProbability01.first.getNonZeroCount());
        EXPECT_EQ(16ul, statesWithProbability01.second.getNonZeroCount());
        
        ASSERT_NO_THROW(statesWithProbability01 = storm::utility::graph::performProb01Max(*model->as<storm::models::symbolic::Mdp<storm::dd::DdType::Sylvan>>(), model->getReachableStates(), model->getStates("collision_max_backoff")));
        EXPECT_EQ(993ul, statesWithProbability01.first.getNonZeroCount());
        EXPECT_EQ(16ul, statesWithProbability01.second.getNonZeroCount());
    }
}

#ifdef STORM_HAVE_MSAT

#include "src/abstraction/prism/AbstractProgram.h"

#include "src/storage/expressions/Expression.h"

#include "src/utility/solver.h"

TEST(GraphTest, SymbolicProb01StochasticGameDieSmall) {
    storm::prism::Program program = storm::parser::PrismParser::parse(STORM_CPP_TESTS_BASE_PATH "/functional/builder/die.pm");
    
    std::vector<storm::expressions::Expression> initialPredicates;
    storm::expressions::ExpressionManager& manager = program.getManager();
    
    initialPredicates.push_back(manager.getVariableExpression("s") < manager.integer(3));
    
    storm::abstraction::prism::AbstractProgram<storm::dd::DdType::CUDD, double> abstractProgram(program, initialPredicates, std::make_shared<storm::utility::solver::MathsatSmtSolverFactory>(), false);
    
    storm::abstraction::MenuGame<storm::dd::DdType::CUDD, double> game = abstractProgram.getAbstractGame();
    
    // The target states are those states where !(s < 3).
    storm::dd::Bdd<storm::dd::DdType::CUDD> targetStates = game.getStates(initialPredicates[0], true);
    
    storm::utility::graph::GameProb01Result<storm::dd::DdType::CUDD> result = storm::utility::graph::performProb0(game, game.getQualitativeTransitionMatrix(), game.getReachableStates(), targetStates, storm::OptimizationDirection::Minimize, storm::OptimizationDirection::Minimize, true);
    EXPECT_EQ(1, result.states.getNonZeroCount());
    EXPECT_TRUE(static_cast<bool>(result.player1Strategy));
    EXPECT_TRUE(static_cast<bool>(result.player2Strategy));
    
    result = storm::utility::graph::performProb1(game, game.getQualitativeTransitionMatrix(), game.getReachableStates(), targetStates, storm::OptimizationDirection::Minimize, storm::OptimizationDirection::Minimize, true);
    EXPECT_EQ(1, result.states.getNonZeroCount());
    
    result = storm::utility::graph::performProb0(game, game.getQualitativeTransitionMatrix(), game.getReachableStates(), targetStates, storm::OptimizationDirection::Minimize, storm::OptimizationDirection::Maximize, true);
    EXPECT_EQ(1, result.states.getNonZeroCount());
    
    result = storm::utility::graph::performProb0(game, game.getQualitativeTransitionMatrix(), game.getReachableStates(), targetStates, storm::OptimizationDirection::Minimize, storm::OptimizationDirection::Maximize, true);
    EXPECT_EQ(1, result.states.getNonZeroCount());
    
    result = storm::utility::graph::performProb0(game, game.getQualitativeTransitionMatrix(), game.getReachableStates(), targetStates, storm::OptimizationDirection::Maximize, storm::OptimizationDirection::Minimize, true);
    EXPECT_EQ(0, result.states.getNonZeroCount());
    
    result = storm::utility::graph::performProb1(game, game.getQualitativeTransitionMatrix(), game.getReachableStates(), targetStates, storm::OptimizationDirection::Maximize, storm::OptimizationDirection::Minimize, true);
    EXPECT_EQ(2, result.states.getNonZeroCount());
    
    result = storm::utility::graph::performProb0(game, game.getQualitativeTransitionMatrix(), game.getReachableStates(), targetStates, storm::OptimizationDirection::Maximize, storm::OptimizationDirection::Maximize, true);
    EXPECT_EQ(0, result.states.getNonZeroCount());
    
    result = storm::utility::graph::performProb1(game, game.getQualitativeTransitionMatrix(), game.getReachableStates(), targetStates, storm::OptimizationDirection::Maximize, storm::OptimizationDirection::Maximize, true);
    EXPECT_EQ(2, result.states.getNonZeroCount());
    EXPECT_TRUE(static_cast<bool>(result.player1Strategy));
    EXPECT_TRUE(static_cast<bool>(result.player2Strategy));
    
    abstractProgram.refine({manager.getVariableExpression("s") < manager.integer(2)});
    game = abstractProgram.getAbstractGame();
    
    // We need to create a new BDD for the target states since the reachable states might have changed.
    targetStates = game.getStates(initialPredicates[0], true);
    
    result = storm::utility::graph::performProb0(game, game.getQualitativeTransitionMatrix(), game.getReachableStates(), targetStates, storm::OptimizationDirection::Minimize, storm::OptimizationDirection::Minimize, true);
    EXPECT_EQ(0, result.states.getNonZeroCount());
    EXPECT_TRUE(static_cast<bool>(result.player1Strategy));
    EXPECT_TRUE(static_cast<bool>(result.player2Strategy));
    
    // Check the validity of the strategies. Start by checking whether only prob0 states have a strategy.
    storm::dd::Bdd<storm::dd::DdType::CUDD> nonProb0StatesWithStrategy = !result.states && result.player1Strategy.get();
    EXPECT_TRUE(nonProb0StatesWithStrategy.isZero());
    
    // Proceed by checking whether they select exactly one action in each state.
    storm::dd::Add<storm::dd::DdType::CUDD, double> stateDistributionsUnderStrategies = (game.getTransitionMatrix() * result.player1Strategy.get().template toAdd<double>() * result.player2Strategy.get().template toAdd<double>()).sumAbstract(game.getColumnVariables());;
    EXPECT_EQ(0, stateDistributionsUnderStrategies.getNonZeroCount());
    
    // Check that the number of distributions per state is one (or zero in the case where there are no prob0 states).
    storm::dd::Add<storm::dd::DdType::CUDD> stateDistributionCount = stateDistributionsUnderStrategies.sumAbstract(game.getNondeterminismVariables());
    EXPECT_EQ(0, stateDistributionCount.getMax());
    
    result = storm::utility::graph::performProb1(game, game.getQualitativeTransitionMatrix(), game.getReachableStates(), targetStates, storm::OptimizationDirection::Minimize, storm::OptimizationDirection::Minimize, true);
    EXPECT_EQ(3, result.states.getNonZeroCount());
    
    result = storm::utility::graph::performProb0(game, game.getQualitativeTransitionMatrix(), game.getReachableStates(), targetStates, storm::OptimizationDirection::Minimize, storm::OptimizationDirection::Maximize, true);
    EXPECT_EQ(0, result.states.getNonZeroCount());
    
    result = storm::utility::graph::performProb1(game, game.getQualitativeTransitionMatrix(), game.getReachableStates(), targetStates, storm::OptimizationDirection::Minimize, storm::OptimizationDirection::Maximize, true);
    EXPECT_EQ(3, result.states.getNonZeroCount());
    
    result = storm::utility::graph::performProb0(game, game.getQualitativeTransitionMatrix(), game.getReachableStates(), targetStates, storm::OptimizationDirection::Maximize, storm::OptimizationDirection::Minimize, true);
    EXPECT_EQ(0, result.states.getNonZeroCount());
    
    result = storm::utility::graph::performProb1(game, game.getQualitativeTransitionMatrix(), game.getReachableStates(), targetStates, storm::OptimizationDirection::Maximize, storm::OptimizationDirection::Minimize, true);
    EXPECT_EQ(3, result.states.getNonZeroCount());
    
    result = storm::utility::graph::performProb0(game, game.getQualitativeTransitionMatrix(), game.getReachableStates(), targetStates, storm::OptimizationDirection::Maximize, storm::OptimizationDirection::Maximize, true);
    EXPECT_EQ(0, result.states.getNonZeroCount());
    
    result = storm::utility::graph::performProb1(game, game.getQualitativeTransitionMatrix(), game.getReachableStates(), targetStates, storm::OptimizationDirection::Maximize, storm::OptimizationDirection::Maximize, true);
    EXPECT_EQ(3, result.states.getNonZeroCount());
    EXPECT_TRUE(static_cast<bool>(result.player1Strategy));
    EXPECT_TRUE(static_cast<bool>(result.player2Strategy));
    
    // Check the validity of the strategies. Start by checking whether only prob1 states have a strategy.
    storm::dd::Bdd<storm::dd::DdType::CUDD> nonProb1StatesWithStrategy = !result.states && result.player1Strategy.get();
    EXPECT_TRUE(nonProb1StatesWithStrategy.isZero());
    
    // Proceed by checking whether they select exactly one action in each state.
    stateDistributionsUnderStrategies = (game.getTransitionMatrix() * result.player1Strategy.get().template toAdd<double>() * result.player2Strategy.get().template toAdd<double>()).sumAbstract(game.getColumnVariables());
    EXPECT_EQ(3, stateDistributionsUnderStrategies.getNonZeroCount());
    
    // Check that the number of distributions per state is one (or zero in the case where there are no prob1 states).
    stateDistributionCount = stateDistributionsUnderStrategies.sumAbstract(game.getNondeterminismVariables());
    EXPECT_EQ(1, stateDistributionCount.getMax());
}

TEST(GraphTest, SymbolicProb01StochasticGameTwoDice) {
    storm::prism::Program program = storm::parser::PrismParser::parse(STORM_CPP_TESTS_BASE_PATH "/functional/builder/two_dice.nm");
    program = program.substituteConstants();
    program = program.flattenModules(std::make_unique<storm::utility::solver::MathsatSmtSolverFactory>());
    
    std::vector<storm::expressions::Expression> initialPredicates;
    storm::expressions::ExpressionManager& manager = program.getManager();
    
    initialPredicates.push_back(manager.getVariableExpression("s1") == manager.integer(0));
    initialPredicates.push_back(manager.getVariableExpression("s1") == manager.integer(1));
    initialPredicates.push_back(manager.getVariableExpression("s1") == manager.integer(2));
    initialPredicates.push_back(manager.getVariableExpression("s1") == manager.integer(3));
    initialPredicates.push_back(manager.getVariableExpression("s1") == manager.integer(4));
    initialPredicates.push_back(manager.getVariableExpression("s1") == manager.integer(5));
    initialPredicates.push_back(manager.getVariableExpression("s1") == manager.integer(6));
    initialPredicates.push_back(manager.getVariableExpression("s1") == manager.integer(7));
    
    initialPredicates.push_back(manager.getVariableExpression("d1") == manager.integer(0));
    initialPredicates.push_back(manager.getVariableExpression("d1") == manager.integer(1));
    initialPredicates.push_back(manager.getVariableExpression("d1") == manager.integer(2));
    initialPredicates.push_back(manager.getVariableExpression("d1") == manager.integer(3));
    initialPredicates.push_back(manager.getVariableExpression("d1") == manager.integer(4));
    initialPredicates.push_back(manager.getVariableExpression("d1") == manager.integer(5));
    initialPredicates.push_back(manager.getVariableExpression("d1") == manager.integer(6));
    
    initialPredicates.push_back(manager.getVariableExpression("s2") == manager.integer(0));
    initialPredicates.push_back(manager.getVariableExpression("s2") == manager.integer(1));
    initialPredicates.push_back(manager.getVariableExpression("s2") == manager.integer(2));
    initialPredicates.push_back(manager.getVariableExpression("s2") == manager.integer(3));
    initialPredicates.push_back(manager.getVariableExpression("s2") == manager.integer(4));
    initialPredicates.push_back(manager.getVariableExpression("s2") == manager.integer(5));
    initialPredicates.push_back(manager.getVariableExpression("s2") == manager.integer(6));
    initialPredicates.push_back(manager.getVariableExpression("s2") == manager.integer(7));
    
    initialPredicates.push_back(manager.getVariableExpression("d2") == manager.integer(0));
    initialPredicates.push_back(manager.getVariableExpression("d2") == manager.integer(1));
    initialPredicates.push_back(manager.getVariableExpression("d2") == manager.integer(2));
    initialPredicates.push_back(manager.getVariableExpression("d2") == manager.integer(3));
    initialPredicates.push_back(manager.getVariableExpression("d2") == manager.integer(4));
    initialPredicates.push_back(manager.getVariableExpression("d2") == manager.integer(5));
    initialPredicates.push_back(manager.getVariableExpression("d2") == manager.integer(6));
    
    storm::abstraction::prism::AbstractProgram<storm::dd::DdType::CUDD, double> abstractProgram(program, initialPredicates, std::make_shared<storm::utility::solver::MathsatSmtSolverFactory>(), false);
    
    storm::abstraction::MenuGame<storm::dd::DdType::CUDD, double> game = abstractProgram.getAbstractGame();
    
    // The target states are those states where s1 == 7 & s2 == 7 & d1 + d2 == 1.
    storm::dd::Bdd<storm::dd::DdType::CUDD> targetStates = game.getStates(initialPredicates[7], false) && game.getStates(initialPredicates[22], false) && game.getStates(initialPredicates[9], false) && game.getStates(initialPredicates[24], false);
    
    storm::utility::graph::GameProb01Result<storm::dd::DdType::CUDD> result = storm::utility::graph::performProb0(game, game.getQualitativeTransitionMatrix(), game.getReachableStates(), targetStates, storm::OptimizationDirection::Minimize, storm::OptimizationDirection::Minimize, true);
    EXPECT_EQ(153, result.states.getNonZeroCount());
    EXPECT_TRUE(static_cast<bool>(result.player1Strategy));
    EXPECT_TRUE(static_cast<bool>(result.player2Strategy));
    
    // Check the validity of the strategies. Start by checking whether only prob0 states have a strategy.
    storm::dd::Bdd<storm::dd::DdType::CUDD> nonProb0StatesWithStrategy = !result.states && result.player1Strategy.get();
    EXPECT_TRUE(nonProb0StatesWithStrategy.isZero());
    
    // Proceed by checking whether they select exactly one exaction in each state.
    storm::dd::Add<storm::dd::DdType::CUDD, double> stateDistributionsUnderStrategies = (game.getTransitionMatrix() * result.player1Strategy.get().template toAdd<double>() * result.player2Strategy.get().template toAdd<double>()).sumAbstract(game.getColumnVariables());
    EXPECT_EQ(153, stateDistributionsUnderStrategies.getNonZeroCount());
    
    storm::dd::Add<storm::dd::DdType::CUDD> stateDistributionCount = stateDistributionsUnderStrategies.sumAbstract(game.getNondeterminismVariables());
    EXPECT_EQ(1, stateDistributionCount.getMax());
    
    result = storm::utility::graph::performProb1(game, game.getQualitativeTransitionMatrix(), game.getReachableStates(), targetStates, storm::OptimizationDirection::Minimize, storm::OptimizationDirection::Minimize, true);
    EXPECT_EQ(1, result.states.getNonZeroCount());
    
    result = storm::utility::graph::performProb0(game, game.getQualitativeTransitionMatrix(), game.getReachableStates(), targetStates, storm::OptimizationDirection::Minimize, storm::OptimizationDirection::Maximize, true);
    EXPECT_EQ(153, result.states.getNonZeroCount());
    
    result = storm::utility::graph::performProb1(game, game.getQualitativeTransitionMatrix(), game.getReachableStates(), targetStates, storm::OptimizationDirection::Minimize, storm::OptimizationDirection::Maximize, true);
    EXPECT_EQ(1, result.states.getNonZeroCount());
    
    result = storm::utility::graph::performProb0(game, game.getQualitativeTransitionMatrix(), game.getReachableStates(), targetStates, storm::OptimizationDirection::Maximize, storm::OptimizationDirection::Minimize, true);
    EXPECT_EQ(153, result.states.getNonZeroCount());
    
    result = storm::utility::graph::performProb1(game, game.getQualitativeTransitionMatrix(), game.getReachableStates(), targetStates, storm::OptimizationDirection::Maximize, storm::OptimizationDirection::Minimize, true);
    EXPECT_EQ(1, result.states.getNonZeroCount());
    
    result = storm::utility::graph::performProb0(game, game.getQualitativeTransitionMatrix(), game.getReachableStates(), targetStates, storm::OptimizationDirection::Maximize, storm::OptimizationDirection::Maximize, true);
    EXPECT_EQ(153, result.states.getNonZeroCount());
    
    result = storm::utility::graph::performProb1(game, game.getQualitativeTransitionMatrix(), game.getReachableStates(), targetStates, storm::OptimizationDirection::Maximize, storm::OptimizationDirection::Maximize, true);
    EXPECT_EQ(1, result.states.getNonZeroCount());
    EXPECT_TRUE(static_cast<bool>(result.player1Strategy));
    EXPECT_TRUE(static_cast<bool>(result.player2Strategy));
    
    // Check the validity of the strategies. Start by checking whether only prob1 states have a strategy.
    storm::dd::Bdd<storm::dd::DdType::CUDD> nonProb1StatesWithStrategy = !result.states && result.player1Strategy.get();
    EXPECT_TRUE(nonProb1StatesWithStrategy.isZero());
    
    // Proceed by checking whether they select exactly one action in each state.
    stateDistributionsUnderStrategies = (game.getTransitionMatrix() * result.player1Strategy.get().template toAdd<double>() * result.player2Strategy.get().template toAdd<double>()).sumAbstract(game.getColumnVariables());
    EXPECT_EQ(1, stateDistributionsUnderStrategies.getNonZeroCount());
    
    // Check that the number of distributions per state is one (or zero in the case where there are no prob1 states).
    stateDistributionCount = stateDistributionsUnderStrategies.sumAbstract(game.getNondeterminismVariables());
    EXPECT_EQ(1, stateDistributionCount.getMax());
}

TEST(GraphTest, SymbolicProb01StochasticGameWlan) {
    storm::prism::Program program = storm::parser::PrismParser::parse(STORM_CPP_TESTS_BASE_PATH "/functional/builder/wlan0-2-4.nm");
    program = program.substituteConstants();
    program = program.flattenModules(std::make_unique<storm::utility::solver::MathsatSmtSolverFactory>());
    
    std::vector<storm::expressions::Expression> initialPredicates;
    storm::expressions::ExpressionManager& manager = program.getManager();
    
    initialPredicates.push_back(manager.getVariableExpression("col") == manager.integer(0));
    initialPredicates.push_back(manager.getVariableExpression("col") == manager.integer(1));
    initialPredicates.push_back(manager.getVariableExpression("col") == manager.integer(2));
    
    initialPredicates.push_back(manager.getVariableExpression("c1") == manager.integer(0));
    initialPredicates.push_back(manager.getVariableExpression("c1") == manager.integer(1));
    initialPredicates.push_back(manager.getVariableExpression("c1") == manager.integer(2));
    
    initialPredicates.push_back(manager.getVariableExpression("c2") == manager.integer(0));
    initialPredicates.push_back(manager.getVariableExpression("c2") == manager.integer(1));
    initialPredicates.push_back(manager.getVariableExpression("c2") == manager.integer(2));
    
    initialPredicates.push_back(manager.getVariableExpression("x1") == manager.integer(0));
    initialPredicates.push_back(manager.getVariableExpression("x1") == manager.integer(1));
    initialPredicates.push_back(manager.getVariableExpression("x1") == manager.integer(2));
    initialPredicates.push_back(manager.getVariableExpression("x1") == manager.integer(3));
    initialPredicates.push_back(manager.getVariableExpression("x1") == manager.integer(4));
    initialPredicates.push_back(manager.getVariableExpression("x1") == manager.integer(5));
    initialPredicates.push_back(manager.getVariableExpression("x1") == manager.integer(6));
    initialPredicates.push_back(manager.getVariableExpression("x1") == manager.integer(7));
    
    initialPredicates.push_back(manager.getVariableExpression("s1") == manager.integer(1));
    initialPredicates.push_back(manager.getVariableExpression("s1") == manager.integer(2));
    initialPredicates.push_back(manager.getVariableExpression("s1") == manager.integer(3));
    initialPredicates.push_back(manager.getVariableExpression("s1") == manager.integer(4));
    initialPredicates.push_back(manager.getVariableExpression("s1") == manager.integer(5));
    initialPredicates.push_back(manager.getVariableExpression("s1") == manager.integer(6));
    initialPredicates.push_back(manager.getVariableExpression("s1") == manager.integer(7));
    initialPredicates.push_back(manager.getVariableExpression("s1") == manager.integer(8));
    initialPredicates.push_back(manager.getVariableExpression("s1") == manager.integer(9));
    initialPredicates.push_back(manager.getVariableExpression("s1") == manager.integer(10));
    initialPredicates.push_back(manager.getVariableExpression("s1") == manager.integer(11));
    initialPredicates.push_back(manager.getVariableExpression("s1") == manager.integer(12));
    
    initialPredicates.push_back(manager.getVariableExpression("slot1") == manager.integer(0));
    initialPredicates.push_back(manager.getVariableExpression("slot1") == manager.integer(1));
    
    initialPredicates.push_back(manager.getVariableExpression("backoff1") == manager.integer(0));
    initialPredicates.push_back(manager.getVariableExpression("backoff1") == manager.integer(1));
    initialPredicates.push_back(manager.getVariableExpression("backoff1") == manager.integer(2));
    initialPredicates.push_back(manager.getVariableExpression("backoff1") == manager.integer(3));
    initialPredicates.push_back(manager.getVariableExpression("backoff1") == manager.integer(4));
    initialPredicates.push_back(manager.getVariableExpression("backoff1") == manager.integer(5));
    initialPredicates.push_back(manager.getVariableExpression("backoff1") == manager.integer(6));
    initialPredicates.push_back(manager.getVariableExpression("backoff1") == manager.integer(7));
    initialPredicates.push_back(manager.getVariableExpression("backoff1") == manager.integer(8));
    initialPredicates.push_back(manager.getVariableExpression("backoff1") == manager.integer(9));
    initialPredicates.push_back(manager.getVariableExpression("backoff1") == manager.integer(10));
    initialPredicates.push_back(manager.getVariableExpression("backoff1") == manager.integer(11));
    initialPredicates.push_back(manager.getVariableExpression("backoff1") == manager.integer(12));
    initialPredicates.push_back(manager.getVariableExpression("backoff1") == manager.integer(13));
    initialPredicates.push_back(manager.getVariableExpression("backoff1") == manager.integer(14));
    initialPredicates.push_back(manager.getVariableExpression("backoff1") == manager.integer(15));
    
    initialPredicates.push_back(manager.getVariableExpression("bc1") == manager.integer(0));
    initialPredicates.push_back(manager.getVariableExpression("bc1") == manager.integer(1));
    
    initialPredicates.push_back(manager.getVariableExpression("x2") == manager.integer(0));
    initialPredicates.push_back(manager.getVariableExpression("x2") == manager.integer(1));
    initialPredicates.push_back(manager.getVariableExpression("x2") == manager.integer(2));
    initialPredicates.push_back(manager.getVariableExpression("x2") == manager.integer(3));
    initialPredicates.push_back(manager.getVariableExpression("x2") == manager.integer(4));
    initialPredicates.push_back(manager.getVariableExpression("x2") == manager.integer(5));
    initialPredicates.push_back(manager.getVariableExpression("x2") == manager.integer(6));
    initialPredicates.push_back(manager.getVariableExpression("x2") == manager.integer(7));
    
    initialPredicates.push_back(manager.getVariableExpression("s2") == manager.integer(1));
    initialPredicates.push_back(manager.getVariableExpression("s2") == manager.integer(2));
    initialPredicates.push_back(manager.getVariableExpression("s2") == manager.integer(3));
    initialPredicates.push_back(manager.getVariableExpression("s2") == manager.integer(4));
    initialPredicates.push_back(manager.getVariableExpression("s2") == manager.integer(5));
    initialPredicates.push_back(manager.getVariableExpression("s2") == manager.integer(6));
    initialPredicates.push_back(manager.getVariableExpression("s2") == manager.integer(7));
    initialPredicates.push_back(manager.getVariableExpression("s2") == manager.integer(8));
    initialPredicates.push_back(manager.getVariableExpression("s2") == manager.integer(9));
    initialPredicates.push_back(manager.getVariableExpression("s2") == manager.integer(10));
    initialPredicates.push_back(manager.getVariableExpression("s2") == manager.integer(11));
    initialPredicates.push_back(manager.getVariableExpression("s2") == manager.integer(12));
    
    initialPredicates.push_back(manager.getVariableExpression("slot2") == manager.integer(0));
    initialPredicates.push_back(manager.getVariableExpression("slot2") == manager.integer(1));
    
    initialPredicates.push_back(manager.getVariableExpression("backoff2") == manager.integer(0));
    initialPredicates.push_back(manager.getVariableExpression("backoff2") == manager.integer(1));
    initialPredicates.push_back(manager.getVariableExpression("backoff2") == manager.integer(2));
    initialPredicates.push_back(manager.getVariableExpression("backoff2") == manager.integer(3));
    initialPredicates.push_back(manager.getVariableExpression("backoff2") == manager.integer(4));
    initialPredicates.push_back(manager.getVariableExpression("backoff2") == manager.integer(5));
    initialPredicates.push_back(manager.getVariableExpression("backoff2") == manager.integer(6));
    initialPredicates.push_back(manager.getVariableExpression("backoff2") == manager.integer(7));
    initialPredicates.push_back(manager.getVariableExpression("backoff2") == manager.integer(8));
    initialPredicates.push_back(manager.getVariableExpression("backoff2") == manager.integer(9));
    initialPredicates.push_back(manager.getVariableExpression("backoff2") == manager.integer(10));
    initialPredicates.push_back(manager.getVariableExpression("backoff2") == manager.integer(11));
    initialPredicates.push_back(manager.getVariableExpression("backoff2") == manager.integer(12));
    initialPredicates.push_back(manager.getVariableExpression("backoff2") == manager.integer(13));
    initialPredicates.push_back(manager.getVariableExpression("backoff2") == manager.integer(14));
    initialPredicates.push_back(manager.getVariableExpression("backoff2") == manager.integer(15));
    
    initialPredicates.push_back(manager.getVariableExpression("bc2") == manager.integer(0));
    initialPredicates.push_back(manager.getVariableExpression("bc2") == manager.integer(1));
    
    storm::abstraction::prism::AbstractProgram<storm::dd::DdType::CUDD, double> abstractProgram(program, initialPredicates, std::make_shared<storm::utility::solver::MathsatSmtSolverFactory>(), false);
    
    storm::abstraction::MenuGame<storm::dd::DdType::CUDD, double> game = abstractProgram.getAbstractGame();
    
    // The target states are those states where col == 2.
    storm::dd::Bdd<storm::dd::DdType::CUDD> targetStates = game.getStates(initialPredicates[2], false);
    
    storm::utility::graph::GameProb01Result<storm::dd::DdType::CUDD> result = storm::utility::graph::performProb0(game, game.getQualitativeTransitionMatrix(), game.getReachableStates(), targetStates, storm::OptimizationDirection::Minimize, storm::OptimizationDirection::Minimize, true);
    EXPECT_EQ(2831, result.states.getNonZeroCount());
    ASSERT_TRUE(static_cast<bool>(result.player1Strategy));
    ASSERT_TRUE(static_cast<bool>(result.player2Strategy));
    
    // Check the validity of the strategies. Start by checking whether only prob0 states have a strategy.
    storm::dd::Bdd<storm::dd::DdType::CUDD> nonProb0StatesWithStrategy = !result.states && result.player1Strategy.get();
    EXPECT_TRUE(nonProb0StatesWithStrategy.isZero());
    
    // Proceed by checking whether they select exactly one action in each state.
    storm::dd::Add<storm::dd::DdType::CUDD, double> stateDistributionsUnderStrategies = (game.getTransitionMatrix() * result.player1Strategy.get().template toAdd<double>() * result.player2Strategy.get().template toAdd<double>()).sumAbstract(game.getColumnVariables());;
    EXPECT_EQ(2831, stateDistributionsUnderStrategies.getNonZeroCount());
    
    // Check that the number of distributions per state is one (or zero in the case where there are no prob0 states).
    storm::dd::Add<storm::dd::DdType::CUDD> stateDistributionCount = stateDistributionsUnderStrategies.sumAbstract(game.getNondeterminismVariables());
    EXPECT_EQ(1, stateDistributionCount.getMax());
    
    result = storm::utility::graph::performProb1(game, game.getQualitativeTransitionMatrix(), game.getReachableStates(), targetStates, storm::OptimizationDirection::Minimize, storm::OptimizationDirection::Minimize, true);
    EXPECT_EQ(2692, result.states.getNonZeroCount());
    
    result = storm::utility::graph::performProb0(game, game.getQualitativeTransitionMatrix(), game.getReachableStates(), targetStates, storm::OptimizationDirection::Minimize, storm::OptimizationDirection::Maximize, true);
    EXPECT_EQ(2831, result.states.getNonZeroCount());
    
    result = storm::utility::graph::performProb1(game, game.getQualitativeTransitionMatrix(), game.getReachableStates(), targetStates, storm::OptimizationDirection::Minimize, storm::OptimizationDirection::Maximize, true);
    EXPECT_EQ(2692, result.states.getNonZeroCount());
    
    result = storm::utility::graph::performProb0(game, game.getQualitativeTransitionMatrix(), game.getReachableStates(), targetStates, storm::OptimizationDirection::Maximize, storm::OptimizationDirection::Minimize, true);
    EXPECT_EQ(2064, result.states.getNonZeroCount());
    
    result = storm::utility::graph::performProb1(game, game.getQualitativeTransitionMatrix(), game.getReachableStates(), targetStates, storm::OptimizationDirection::Maximize, storm::OptimizationDirection::Minimize, true);
    EXPECT_EQ(2884, result.states.getNonZeroCount());
    
    result = storm::utility::graph::performProb0(game, game.getQualitativeTransitionMatrix(), game.getReachableStates(), targetStates, storm::OptimizationDirection::Maximize, storm::OptimizationDirection::Maximize, true);
    EXPECT_EQ(2064, result.states.getNonZeroCount());
    
    result = storm::utility::graph::performProb1(game, game.getQualitativeTransitionMatrix(), game.getReachableStates(), targetStates, storm::OptimizationDirection::Maximize, storm::OptimizationDirection::Maximize, true);
    EXPECT_EQ(2884, result.states.getNonZeroCount());
    EXPECT_TRUE(static_cast<bool>(result.player1Strategy));
    EXPECT_TRUE(static_cast<bool>(result.player2Strategy));
    
    // Check the validity of the strategies. Start by checking whether only prob1 states have a strategy.
    storm::dd::Bdd<storm::dd::DdType::CUDD> nonProb1StatesWithStrategy = !result.states && result.player1Strategy.get();
    EXPECT_TRUE(nonProb1StatesWithStrategy.isZero());
    
    // Proceed by checking whether they select exactly one action in each state.
    stateDistributionsUnderStrategies = (game.getTransitionMatrix() * result.player1Strategy.get().template toAdd<double>() * result.player2Strategy.get().template toAdd<double>()).sumAbstract(game.getColumnVariables());
    EXPECT_EQ(2884, stateDistributionsUnderStrategies.getNonZeroCount());
    
    // Check that the number of distributions per state is one (or zero in the case where there are no prob1 states).
    stateDistributionCount = stateDistributionsUnderStrategies.sumAbstract(game.getNondeterminismVariables());
    EXPECT_EQ(1, stateDistributionCount.getMax());
}

#endif

TEST(GraphTest, ExplicitProb01) {
    storm::prism::Program program = storm::parser::PrismParser::parse(STORM_CPP_TESTS_BASE_PATH "/functional/builder/crowds-5-5.pm");
    std::shared_ptr<storm::models::sparse::Model<double>> model = storm::builder::ExplicitModelBuilder<double>(program, storm::generator::NextStateGeneratorOptions(false, true)).build();
    
    ASSERT_TRUE(model->getType() == storm::models::ModelType::Dtmc);
    
    std::pair<storm::storage::BitVector, storm::storage::BitVector> statesWithProbability01;
    
    ASSERT_NO_THROW(statesWithProbability01 = storm::utility::graph::performProb01(*model->as<storm::models::sparse::Dtmc<double>>(), storm::storage::BitVector(model->getNumberOfStates(), true), model->getStates("observe0Greater1")));
    EXPECT_EQ(4409ul, statesWithProbability01.first.getNumberOfSetBits());
    EXPECT_EQ(1316ul, statesWithProbability01.second.getNumberOfSetBits());
    
    ASSERT_NO_THROW(statesWithProbability01 = storm::utility::graph::performProb01(*model->as<storm::models::sparse::Dtmc<double>>(), storm::storage::BitVector(model->getNumberOfStates(), true), model->getStates("observeIGreater1")));
    EXPECT_EQ(1091ul, statesWithProbability01.first.getNumberOfSetBits());
    EXPECT_EQ(4802ul, statesWithProbability01.second.getNumberOfSetBits());
    
    ASSERT_NO_THROW(statesWithProbability01 = storm::utility::graph::performProb01(*model->as<storm::models::sparse::Dtmc<double>>(), storm::storage::BitVector(model->getNumberOfStates(), true), model->getStates("observeOnlyTrueSender")));
    EXPECT_EQ(5829ul, statesWithProbability01.first.getNumberOfSetBits());
    EXPECT_EQ(1032ul, statesWithProbability01.second.getNumberOfSetBits());
}

TEST(GraphTest, ExplicitProb01MinMax) {
    storm::prism::Program program = storm::parser::PrismParser::parse(STORM_CPP_TESTS_BASE_PATH "/functional/builder/leader3.nm");
    std::shared_ptr<storm::models::sparse::Model<double>> model = storm::builder::ExplicitModelBuilder<double>(program, storm::generator::NextStateGeneratorOptions(false, true)).build();
    
    ASSERT_TRUE(model->getType() == storm::models::ModelType::Mdp);
    
    std::pair<storm::storage::BitVector, storm::storage::BitVector> statesWithProbability01;
    
    ASSERT_NO_THROW(statesWithProbability01 = storm::utility::graph::performProb01Min(*model->as<storm::models::sparse::Mdp<double>>(), storm::storage::BitVector(model->getNumberOfStates(), true), model->getStates("elected")));
    EXPECT_EQ(0ul, statesWithProbability01.first.getNumberOfSetBits());
    EXPECT_EQ(364ul, statesWithProbability01.second.getNumberOfSetBits());
    
    ASSERT_NO_THROW(statesWithProbability01 = storm::utility::graph::performProb01Max(*model->as<storm::models::sparse::Mdp<double>>(), storm::storage::BitVector(model->getNumberOfStates(), true), model->getStates("elected")));
    EXPECT_EQ(0ul, statesWithProbability01.first.getNumberOfSetBits());
    EXPECT_EQ(364ul, statesWithProbability01.second.getNumberOfSetBits());
    
    program = storm::parser::PrismParser::parse(STORM_CPP_TESTS_BASE_PATH "/functional/builder/coin2-2.nm");
    model = storm::builder::ExplicitModelBuilder<double>(program, storm::generator::NextStateGeneratorOptions(false, true)).build();
    
    ASSERT_TRUE(model->getType() == storm::models::ModelType::Mdp);
    
    ASSERT_NO_THROW(statesWithProbability01 = storm::utility::graph::performProb01Min(*model->as<storm::models::sparse::Mdp<double>>(), storm::storage::BitVector(model->getNumberOfStates(), true), model->getStates("all_coins_equal_0")));
    EXPECT_EQ(77ul, statesWithProbability01.first.getNumberOfSetBits());
    EXPECT_EQ(149ul, statesWithProbability01.second.getNumberOfSetBits());
    
    ASSERT_NO_THROW(statesWithProbability01 = storm::utility::graph::performProb01Max(*model->as<storm::models::sparse::Mdp<double>>(), storm::storage::BitVector(model->getNumberOfStates(), true), model->getStates("all_coins_equal_0")));
    EXPECT_EQ(74ul, statesWithProbability01.first.getNumberOfSetBits());
    EXPECT_EQ(198ul, statesWithProbability01.second.getNumberOfSetBits());
    
    ASSERT_NO_THROW(statesWithProbability01 = storm::utility::graph::performProb01Min(*model->as<storm::models::sparse::Mdp<double>>(), storm::storage::BitVector(model->getNumberOfStates(), true), model->getStates("all_coins_equal_1")));
    EXPECT_EQ(94ul, statesWithProbability01.first.getNumberOfSetBits());
    EXPECT_EQ(33ul, statesWithProbability01.second.getNumberOfSetBits());
    
    ASSERT_NO_THROW(statesWithProbability01 = storm::utility::graph::performProb01Max(*model->as<storm::models::sparse::Mdp<double>>(), storm::storage::BitVector(model->getNumberOfStates(), true), model->getStates("all_coins_equal_1")));
    EXPECT_EQ(83ul, statesWithProbability01.first.getNumberOfSetBits());
    EXPECT_EQ(35ul, statesWithProbability01.second.getNumberOfSetBits());
    
    program = storm::parser::PrismParser::parse(STORM_CPP_TESTS_BASE_PATH "/functional/builder/csma2-2.nm");
    model = storm::builder::ExplicitModelBuilder<double>(program, storm::generator::NextStateGeneratorOptions(false, true)).build();
    
    ASSERT_TRUE(model->getType() == storm::models::ModelType::Mdp);
    
    ASSERT_NO_THROW(statesWithProbability01 = storm::utility::graph::performProb01Min(*model->as<storm::models::sparse::Mdp<double>>(), storm::storage::BitVector(model->getNumberOfStates(), true), model->getStates("collision_max_backoff")));
    EXPECT_EQ(993ul, statesWithProbability01.first.getNumberOfSetBits());
    EXPECT_EQ(16ul, statesWithProbability01.second.getNumberOfSetBits());
    
    ASSERT_NO_THROW(statesWithProbability01 = storm::utility::graph::performProb01Max(*model->as<storm::models::sparse::Mdp<double>>(), storm::storage::BitVector(model->getNumberOfStates(), true), model->getStates("collision_max_backoff")));
    EXPECT_EQ(993ul, statesWithProbability01.first.getNumberOfSetBits());
    EXPECT_EQ(16ul, statesWithProbability01.second.getNumberOfSetBits());
}
