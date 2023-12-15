#include "gtest/gtest.h"
#include <cstddef>
#include <cstdint>
#include <iostream>

#include "Automaton.h"

TEST(AutomatonExampleTest, Default) {
  fa::Automaton fa;
  EXPECT_FALSE(fa.isValid());
}

// IsValid()

TEST(isValidTest,NoSymbol) {
  fa::Automaton fa = fa::Automaton();
  EXPECT_TRUE(fa.addState(1));
  EXPECT_FALSE(fa.isValid());
}

TEST(isValidTest,NoState) {
  fa::Automaton fa = fa::Automaton();
  EXPECT_TRUE(fa.addSymbol('a'));
  EXPECT_FALSE(fa.isValid());
}

TEST(isValidTest,Valid) {
  fa::Automaton fa = fa::Automaton();
  EXPECT_TRUE(fa.addSymbol('a'));
  EXPECT_TRUE(fa.addState(1));
  EXPECT_TRUE(fa.isValid());
}

// AddSymbol()

TEST(addSymbolTest, OneSymbol) {
  fa::Automaton fa = fa::Automaton();
  EXPECT_TRUE(fa.addSymbol('a'));
}

TEST(addSymbolTest, TwoSymbols) {
  fa::Automaton fa = fa::Automaton();
  EXPECT_TRUE(fa.addSymbol('a'));
  EXPECT_TRUE(fa.addSymbol('b'));
}

TEST(addSymbolTest, TwoIdenticalSymbols) {
  fa::Automaton fa = fa::Automaton();
  EXPECT_TRUE(fa.addSymbol('a'));
  EXPECT_FALSE(fa.addSymbol('a'));
}

TEST(addSymbolTest, SymbolWithoutGraphicalRepresentation) {
  fa::Automaton fa = fa::Automaton();
  EXPECT_FALSE(fa.addSymbol(' '));
  EXPECT_FALSE(fa.addSymbol('\n'));
}

TEST(addSymbolTest, EmptySymbol) {
  fa::Automaton fa = fa::Automaton();
  EXPECT_FALSE(fa.addSymbol('\0'));
}

TEST(addSymbolTest, AddEpsilon) {
  fa::Automaton fa = fa::Automaton();
  EXPECT_FALSE(fa.addSymbol(fa::Epsilon));
}

// RemoveSymbol()

TEST(removeSymbolTest, OneSymbol) {
  fa::Automaton fa = fa::Automaton();
  fa.addSymbol('a');
  EXPECT_TRUE(fa.removeSymbol('a'));
}

TEST(removeSymbolTest, UnknownSymbol) {
  fa::Automaton fa = fa::Automaton();
  EXPECT_FALSE(fa.removeSymbol('b'));
}

TEST(removeSymbolTest, EmptySymbol) {
  fa::Automaton fa = fa::Automaton();
  EXPECT_FALSE(fa.removeSymbol('\0'));
}

TEST(removeSymbolTest, SymbolInTransition) {
  fa::Automaton fa = fa::Automaton();
  fa.addSymbol('a');
  fa.addState(0);
  fa.addState(1);
  fa.addTransition(0, 'a', 1);
  fa.removeSymbol('a');
  EXPECT_FALSE(fa.hasTransition(0, 'a', 1));
}

// HasSymbol()

TEST(hasSymbolTest, PresentSymbol) {
  fa::Automaton fa = fa::Automaton();
  fa.addSymbol('a');
  EXPECT_TRUE(fa.hasSymbol('a')); 
}

TEST(hasSymbolTest, NoSymbol) {
  fa::Automaton fa = fa::Automaton();
  EXPECT_FALSE(fa.hasSymbol('a')); 
}

// CountSymbol()

TEST(countSymbolsTest, NoSymbol) {
  fa::Automaton fa = fa::Automaton();
  EXPECT_TRUE(fa.countSymbols() == 0); 
}

TEST(countSymbolsTest, TwoSymbols) {
  fa::Automaton fa = fa::Automaton();
  fa.addSymbol('a');
  fa.addSymbol('b');
  EXPECT_TRUE(fa.countSymbols() == 2); 
}

// AddState()

TEST(addStateTest, OneState) {
  fa::Automaton fa = fa::Automaton();
  EXPECT_TRUE(fa.addState(1)); 
  EXPECT_TRUE(fa.hasState(1));
}

TEST(addStateTest, TwoStates) {
  fa::Automaton fa = fa::Automaton();
  EXPECT_TRUE(fa.addState(1)); 
  EXPECT_TRUE(fa.addState(2)); 
}

TEST(addStateTest, NegativeState) {
  fa::Automaton fa = fa::Automaton();
  EXPECT_FALSE(fa.addState(-1)); 
  EXPECT_FALSE(fa.hasState(-1));
}

TEST(addStateTest, TwoIdenticalStates) {
  fa::Automaton fa = fa::Automaton();
  fa.addState(1); 
  EXPECT_FALSE(fa.addState(1));
}

// RemoveState()

TEST(removeStateTest, OneState) {
  fa::Automaton fa = fa::Automaton();
  fa.addState(1); 
  EXPECT_TRUE(fa.removeState(1));
  EXPECT_FALSE(fa.hasState(1));
}

TEST(removeStateTest, NoState) {
  fa::Automaton fa = fa::Automaton();
  EXPECT_FALSE(fa.removeState(1));
  EXPECT_FALSE(fa.hasState(1));
}

TEST(removeStateTest, DestinatioInTransitionn) {
  fa::Automaton fa = fa::Automaton();
  fa.addSymbol('a');
  fa.addState(0);
  fa.addState(1);
  fa.addTransition(0, 'a', 1);
  fa.removeState(1);
  EXPECT_FALSE(fa.hasTransition(0, 'a', 1));
}

TEST(removeStateTest, SourceInTransition) {
  fa::Automaton fa = fa::Automaton();
  fa.addSymbol('a');
  fa.addState(0);
  fa.addState(1);
  fa.addTransition(0, 'a', 1);
  fa.removeState(0);
  EXPECT_FALSE(fa.hasTransition(0, 'a', 1));
}

TEST(removeStateTest, SourceAndDestinationInTransition) {
  fa::Automaton fa = fa::Automaton();
  fa.addSymbol('a');
  fa.addState(0);
  fa.addTransition(0, 'a', 0);
  fa.removeState(0);
  EXPECT_FALSE(fa.hasTransition(0, 'a', 0));
}

// HasState()

TEST(hasStateTest, PresentState) {
  fa::Automaton fa = fa::Automaton();
  fa.addState(1); 
  EXPECT_TRUE(fa.hasState(1));
}

TEST(hasStateTest, NoState) {
  fa::Automaton fa = fa::Automaton();
  EXPECT_FALSE(fa.hasState(1));
}

TEST(hasStateTest, NegativeState) {
  fa::Automaton fa = fa::Automaton();
  EXPECT_FALSE(fa.hasState(-1));
}

// CountStates()

TEST(countStatesTest, NoState) {
  fa::Automaton fa = fa::Automaton();
  EXPECT_TRUE(fa.countStates() == 0);
}

TEST(countStatesTest, TwoStates) {
  fa::Automaton fa = fa::Automaton();
  fa.addState(0);
  fa.addState(1);
  EXPECT_TRUE(fa.countStates() == 2);
}

// SetInitialState() - IsStateInitial()

TEST(setStateInitialTest, OneInitialState) {
  fa::Automaton fa = fa::Automaton();
  fa.addState(0);
  fa.setStateInitial(0);
  EXPECT_TRUE(fa.isStateInitial(0));
}

TEST(setStateInitialTest, TwoInitialStates) {
  fa::Automaton fa = fa::Automaton();
  fa.addState(0);
  fa.setStateInitial(0);
  fa.addState(1);
  fa.setStateInitial(1);
  EXPECT_TRUE(fa.isStateInitial(0));
  EXPECT_TRUE(fa.isStateInitial(1));
}

TEST(setStateInitialTest, NoState) {
  fa::Automaton fa = fa::Automaton();
  fa.setStateInitial(0);
  EXPECT_FALSE(fa.isStateInitial(0));
}

TEST(setStateInitialTest, UnknownState) {
  fa::Automaton fa = fa::Automaton();
  EXPECT_FALSE(fa.isStateInitial(0));
}

TEST(setStateInitialTest, ReadEmptyString) {
  fa::Automaton fa = fa::Automaton();
  // TODO Comprendre ce test 
}

// SetFinalState() - IsStateFinal()

TEST(setStateFinalTest, OneState) {
  fa::Automaton fa = fa::Automaton();
  fa.addState(0);
  fa.setStateFinal(0);
  EXPECT_TRUE(fa.isStateFinal(0));
}

TEST(setStateFinalTest, TwoStates) {
  fa::Automaton fa = fa::Automaton();
  fa.addState(0);
  fa.setStateFinal(0);
  fa.addState(1);
  fa.setStateFinal(1);
  EXPECT_TRUE(fa.isStateFinal(0));
  EXPECT_TRUE(fa.isStateFinal(1));
}

TEST(setStateFinalTest, NoState) {
  fa::Automaton fa = fa::Automaton();
  fa.setStateFinal(0);
  EXPECT_FALSE(fa.isStateFinal(0));
}

// AddTransition()

TEST(addTransitionTest, OneTransition) {
  fa::Automaton fa = fa::Automaton();
  fa.addSymbol('a');
  fa.addState(0);
  fa.addState(1);
  EXPECT_TRUE(fa.addTransition(0, 'a', 1));
}

TEST(addTransitionTest, OneTransitionSameSourceAndDestination) {
  fa::Automaton fa = fa::Automaton();
  fa.addSymbol('a');
  fa.addState(0);
  EXPECT_TRUE(fa.addTransition(0, 'a', 0));
}

TEST(addTransitionTest, NoSymbol) {
  fa::Automaton fa = fa::Automaton();
  fa.addState(0);
  fa.addState(1);
  EXPECT_FALSE(fa.addTransition(0, 'a', 1));
}

TEST(addTransitionTest, NoSourceState) {
  fa::Automaton fa = fa::Automaton();
  fa.addSymbol('a');
  fa.addState(1);
  EXPECT_FALSE(fa.addTransition(0, 'a', 1));
}

TEST(addTransitionTest, NoDestinationState) {
  fa::Automaton fa = fa::Automaton();
  fa.addSymbol('a');
  fa.addState(0);
  EXPECT_FALSE(fa.addTransition(0, 'a', 1));
}

TEST(addTransitionTest, TwoIdenticalTransitions) {
  fa::Automaton fa = fa::Automaton();
  fa.addSymbol('a');
  fa.addState(0);
  fa.addState(1);
  fa.addTransition(0, 'a', 1);
  EXPECT_FALSE(fa.addTransition(0, 'a', 1));
}

TEST(addTransitionTest, SameOriginAndLetter) {
  fa::Automaton fa = fa::Automaton();
  fa.addSymbol('a');
  fa.addState(0);
  fa.addState(1);
  fa.addState(2);
  EXPECT_TRUE(fa.addTransition(0, 'a', 1));
  EXPECT_TRUE(fa.addTransition(0, 'a', 2));
  EXPECT_TRUE(fa.hasTransition(0, 'a', 2));
  EXPECT_TRUE(fa.hasTransition(0, 'a', 1));
}

TEST(addTransitionTest, SameOriginAndDestination) {
  fa::Automaton fa = fa::Automaton();
  fa.addSymbol('a');
  fa.addSymbol('b');
  fa.addState(0);
  fa.addState(1);
  EXPECT_TRUE(fa.addTransition(0, 'a', 1));
  EXPECT_TRUE(fa.addTransition(0, 'b', 1));
  EXPECT_TRUE(fa.hasTransition(0, 'a', 1));
  EXPECT_TRUE(fa.hasTransition(0, 'b', 1));
}

TEST(addTransitionTest, SameLetterAndDestination) {
  fa::Automaton fa = fa::Automaton();
  fa.addSymbol('a');
  fa.addState(0);
  fa.addState(1);
  fa.addState(2);
  EXPECT_TRUE(fa.addTransition(0, 'a', 1));
  EXPECT_TRUE(fa.addTransition(2, 'a', 1));
  EXPECT_TRUE(fa.hasTransition(0, 'a', 1));
  EXPECT_TRUE(fa.hasTransition(2, 'a', 1));
}

TEST(addTransitionTest, EpsilonTransition) {
  fa::Automaton fa = fa::Automaton();
  fa.addState(0);
  fa.addState(1);
  EXPECT_TRUE(fa.addTransition(0, fa::Epsilon, 1));
}

// RemoveTransition()

TEST(removeTransitionTest, OneTransition) {
  fa::Automaton fa = fa::Automaton();
  fa.addSymbol('a');
  fa.addState(0);
  fa.addState(1);
  fa.addTransition(0, 'a', 1);
  EXPECT_TRUE(fa.removeTransition(0, 'a', 1));
}

TEST(removeTransitionTest, NoTransition) {
  fa::Automaton fa = fa::Automaton();
  EXPECT_FALSE(fa.removeTransition(0, 'a', 1));
}

TEST(removeTransitionTest, NoOriginInTransition) {
  fa::Automaton fa = fa::Automaton();
  fa.addSymbol('a');
  fa.addState(1);
  EXPECT_FALSE(fa.removeTransition(0, 'a', 1));
}

TEST(removeTransitionTest, NoDestinationInTransition) {
  fa::Automaton fa = fa::Automaton();
  fa.addSymbol('a');
  fa.addState(0);
  EXPECT_FALSE(fa.removeTransition(0, 'a', 1));
}

TEST(removeTransitionTest, NoSymbolInTransition) {
  fa::Automaton fa = fa::Automaton();
  fa.addState(0);
  fa.addState(1);
  EXPECT_FALSE(fa.removeTransition(0, 'a', 1));
}

// HasTransition()

TEST(hasTransitionTest, AddTransition) {
  fa::Automaton fa = fa::Automaton();
  fa.addSymbol('a');
  fa.addState(0);
  fa.addState(1);
  fa.addTransition(0, 'a', 1);
  EXPECT_TRUE(fa.hasTransition(0, 'a', 1));
}

TEST(hasTransitionTest, NoTransition) {
  fa::Automaton fa = fa::Automaton();
  EXPECT_FALSE(fa.hasTransition(0, 'a', 1));
}

// PrettyPrint

TEST(prettyPrintTest, AnyValidAutomaton) {
  fa::Automaton fa = fa::Automaton();
  fa.addSymbol('a');
  fa.addState(0);
  fa.setStateInitial(0);
  fa.addState(1);
  fa.setStateFinal(1);
  fa.addState(2);
  fa.setStateFinal(2);
  fa.addTransition(0, 'a', 1);
  fa.addTransition(0, 'a', 2);
  fa.addTransition(1, 'a', 0);
  fa.prettyPrint(std::cout);
}

// HasEpsilonTransitions()

TEST(hasEpsilonTransitionTest, HasEpsilonTransition) {
  fa::Automaton fa = fa::Automaton();
  fa.addSymbol('a');
  fa.addState(0);
  fa.addState(1);
  fa.addTransition(0, fa::Epsilon, 1);
  EXPECT_TRUE(fa.hasEpsilonTransition());
}

TEST(hasEpsilonTransitionTest, NoEpsilonTransition) {
  fa::Automaton fa = fa::Automaton();
  fa.addSymbol('a');
  fa.addState(0);
  fa.addState(1);
  fa.addTransition(0, 'a', 1);
  EXPECT_FALSE(fa.hasEpsilonTransition());
}

TEST(hasEpsilonTransitionTest, AddAndRemoveTransition) {
  fa::Automaton fa = fa::Automaton();
  fa.addSymbol('a');
  fa.addState(0);
  fa.addState(1);
  fa.addTransition(0, 'a', 1);
  EXPECT_FALSE(fa.hasEpsilonTransition());
  fa.addTransition(1, fa::Epsilon, 1);
  EXPECT_TRUE(fa.hasEpsilonTransition());
  fa.removeTransition(1, fa::Epsilon, 1);
  EXPECT_FALSE(fa.hasEpsilonTransition());
}

// isDeterministic()

TEST(isDeterministicTest, Valid) {
  fa::Automaton fa = fa::Automaton();
  fa.addSymbol('a');
  fa.addState(0);
  fa.setStateInitial(0);
  fa.addState(1);
  fa.addTransition(0, 'a', 1);
  EXPECT_TRUE(fa.isDeterministic());
}

TEST(isDeterministicTest, NoInitialState) {
  fa::Automaton fa = fa::Automaton();
  fa.addSymbol('a');
  fa.addState(0);
  fa.addState(1);
  fa.addTransition(0, 'a', 1);
  EXPECT_FALSE(fa.isDeterministic());
}

TEST(isDeterministicTest, MultipleInitialState) {
  fa::Automaton fa = fa::Automaton();
  fa.addSymbol('a');
  fa.addState(0);
  fa.addState(1);
  fa.setStateInitial(0);
  fa.setStateInitial(1);
  fa.addTransition(0, 'a', 1);
  EXPECT_FALSE(fa.isDeterministic());
}

TEST(isDeterministicTest, TwoTransitions) {
  fa::Automaton fa = fa::Automaton();
  fa.addSymbol('a');
  fa.addState(0);
  fa.setStateInitial(0);
  fa.addState(1);
  fa.addTransition(0, 'a', 1);
  fa.addTransition(1, 'a', 1);
  EXPECT_TRUE(fa.isDeterministic());
}

TEST(isDeterministicTest, EpsilonTransition) {
  fa::Automaton fa = fa::Automaton();
  fa.addSymbol('a');
  fa.addState(0);
  fa.setStateInitial(0);
  fa.addState(1);
  fa.addTransition(0, 'a', 1);
  fa.addTransition(0, fa::Epsilon, 1);
  EXPECT_FALSE(fa.isDeterministic());
}

// isComplete()

TEST(isCompleteTest, Valid) {
  fa::Automaton fa = fa::Automaton();
  fa.addSymbol('a');
  fa.addState(0);
  fa.addTransition(0, 'a', 0);
  EXPECT_TRUE(fa.isComplete());
}

TEST(isCompleteTest, MissingOneTransition) {
  fa::Automaton fa = fa::Automaton();
  fa.addSymbol('a');
  fa.addSymbol('b');
  fa.addState(0);
  fa.addTransition(0, 'a', 0);
  EXPECT_FALSE(fa.isComplete());
}

TEST(isCompleteTest, TwoStatesValid) {
  fa::Automaton fa = fa::Automaton();
  fa.addSymbol('a');
  fa.addState(0);
  fa.addState(1);
  fa.addTransition(0, 'a', 0);
  fa.addTransition(1, 'a', 0);
  EXPECT_TRUE(fa.isComplete());
}

TEST(isCompleteTest, TwoStatesInvalid) {
  fa::Automaton fa = fa::Automaton();
  fa.addSymbol('a');
  fa.addSymbol('b');
  fa.addState(0);
  fa.addState(1);
  fa.addTransition(0, 'a', 0);
  fa.addTransition(0, 'b', 1);
  fa.addTransition(1, 'a', 1);
  EXPECT_FALSE(fa.isComplete());
}

// createComplete

TEST(createCompleteTest, TwoStatesInvalid) {
  fa::Automaton fa = fa::Automaton();
  fa.addSymbol('a');
  fa.addSymbol('b');
  fa.addState(0);
  fa.addState(1);
  fa.addTransition(0, 'a', 0);
  fa.addTransition(0, 'b', 1);
  fa.addTransition(1, 'a', 1);
  EXPECT_FALSE(fa.isComplete());
  auto fa_complete = fa.createComplete(fa);
  EXPECT_TRUE(fa_complete.isComplete());
}

TEST(createCompleteTest, alreadyComplete) {
  fa::Automaton fa = fa::Automaton();
  fa.addSymbol('a');
  fa.addState(0);
  fa.addState(1);
  fa.addTransition(0, 'a', 0);
  fa.addTransition(1, 'a', 0);
  EXPECT_TRUE(fa.isComplete());
  auto fa_complete = fa.createComplete(fa);
  EXPECT_TRUE(fa_complete.isComplete());
}

TEST(createCompleteTest, MissingOneTransition) {
  fa::Automaton fa = fa::Automaton();
  fa.addSymbol('a');
  fa.addSymbol('b');
  fa.addState(2);
  fa.addTransition(2, 'a', 2);
  EXPECT_FALSE(fa.isComplete());
}

//TEST(Automaton)

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}