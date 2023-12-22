#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <vector>

#include "Automaton.h"
#include "gtest/gtest.h"
#include "googletest/googletest/include/gtest/gtest.h"

/***************************** */
/*       My Functions          */
/***************************** */

fa::Automaton createAutomaton(int nbState, std::vector<char> symbols){
  fa::Automaton fa;
  for(int i = 0 ; i < nbState ; ++i){
    EXPECT_TRUE(fa.addState(i));
    EXPECT_TRUE(fa.hasState(i));
  }
  for(auto s : symbols){
    EXPECT_TRUE(fa.addSymbol(s));
    EXPECT_TRUE(fa.hasSymbol(s));
  }
  return fa;
}

/***************************** */
/*           TESTS             */
/***************************** */

TEST(AutomatonExampleTest, Default) {
  fa::Automaton fa;
  EXPECT_FALSE(fa.isValid());
}

/***************************** */
/*          isValid            */
/***************************** */

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

/***************************** */
/*          addSymbol          */
/***************************** */

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

/***************************** */
/*       RemoveSymbol          */
/***************************** */ 

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

/***************************** */
/*          HasSymbol          */
/***************************** */ 

TEST(hasSymbolTest, PresentSymbol) {
  fa::Automaton fa = fa::Automaton();
  fa.addSymbol('a');
  EXPECT_TRUE(fa.hasSymbol('a')); 
}

TEST(hasSymbolTest, NoSymbol) {
  fa::Automaton fa = fa::Automaton();
  EXPECT_FALSE(fa.hasSymbol('a')); 
}

/***************************** */
/*        CountSymbol          */
/***************************** */ 

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

/***************************** */
/*          AddState           */
/***************************** */ 

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

/***************************** */
/*        RemoveState          */
/***************************** */ 

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
  EXPECT_TRUE(fa.hasTransition(0, 'a', 1));
  EXPECT_EQ(fa.countTransitions(), 1u);
  fa.removeState(1);
  EXPECT_FALSE(fa.hasTransition(0, 'a', 1));
  EXPECT_EQ(fa.countTransitions(), 0u);
}

TEST(removeStateTest, SourceInTransition) {
  fa::Automaton fa = fa::Automaton();
  fa.addSymbol('a');
  fa.addState(0);
  fa.addState(1);
  fa.addTransition(0, 'a', 1);
  EXPECT_TRUE(fa.hasTransition(0, 'a', 1));
  EXPECT_EQ(fa.countTransitions(), 1u);
  fa.removeState(0);
  EXPECT_FALSE(fa.hasTransition(0, 'a', 1));
  EXPECT_EQ(fa.countTransitions(), 0u);
}

TEST(removeStateTest, SourceAndDestinationInTransition) {
  fa::Automaton fa = fa::Automaton();
  fa.addSymbol('a');
  fa.addState(0);
  fa.addTransition(0, 'a', 0);
  EXPECT_TRUE(fa.hasTransition(0, 'a', 0));
  EXPECT_EQ(fa.countTransitions(), 1u);
  fa.removeState(0);
  EXPECT_FALSE(fa.hasTransition(0, 'a', 0));
  EXPECT_EQ(fa.countTransitions(), 0u);
}

/***************************** */
/*          HasState           */
/***************************** */ 

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

/***************************** */
/*         CountStates         */
/***************************** */ 

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

/***************************** */
/* SetInitialState- IsStateInitial  */
/***************************** */ 

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

/***************************** */
/* SetFinalState - IsStateFinal */
/***************************** */ 

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

/***************************** */
/*       AddTransition         */
/***************************** */ 

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
  EXPECT_EQ(fa.countTransitions(), 1u);
}

/***************************** */
/*      RemoveTransition       */
/***************************** */ 

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

/***************************** */
/*        HasTransition        */
/***************************** */ 

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

/***************************** */
/*         PrettyPrint         */
/***************************** */ 

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

/***************************** */
/*    HasEpsilonTransitions    */
/***************************** */ 

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

/***************************** */
/*      isDeterministic        */
/***************************** */ 

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

/***************************** */
/*         isComplete          */
/***************************** */ 

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

/***************************** */
/*       createComplete        */
/***************************** */ 

TEST(createCompleteTest, TwoStatesInvalid) {
  static const std::vector<char> symbols = {'a','b'};
	fa::Automaton fa = createAutomaton(2,symbols);
  fa.addTransition(0, 'a', 0);
  fa.addTransition(0, 'b', 1);
  fa.addTransition(1, 'a', 1);
  EXPECT_FALSE(fa.isComplete());
  auto fa_complete = fa.createComplete(fa);
  EXPECT_TRUE(fa_complete.isComplete());
}

TEST(createCompleteTest, alreadyComplete) {
  static const std::vector<char> symbols = {'a'};
	fa::Automaton fa = createAutomaton(2,symbols);
  fa.addTransition(0, 'a', 0);
  fa.addTransition(1, 'a', 0);
  EXPECT_TRUE(fa.isComplete());
  auto fa_complete = fa.createComplete(fa);
  EXPECT_TRUE(fa_complete.isComplete());
}

TEST(createCompleteTest, MissingOneTransition) {
  static const std::vector<char> symbols = {'a','b'};
	fa::Automaton fa = createAutomaton(1,symbols);
  fa.addTransition(2, 'a', 2);
  EXPECT_FALSE(fa.isComplete());
}

TEST(createCompleteTest, Many) {
  static const std::vector<char> symbols = {'a'};
    fa::Automaton fa = createAutomaton(999,symbols);
  fa.addTransition(0, 'a', 1);
  fa.addTransition(1, 'a', 0);
  EXPECT_FALSE(fa.isComplete());
  auto fa_complete = fa.createComplete(fa);
  EXPECT_TRUE(fa_complete.isComplete());
}

/***************************** */
/*        createMirror         */
/***************************** */ 

TEST(createMirrorTest, Valid) {
	static const std::vector<char> symbols = {'a','b'};
	fa::Automaton fa = createAutomaton(3,symbols);

	fa.setStateInitial(0);
	fa.setStateFinal(2);

	fa.addTransition(0,'a',1);
	fa.addTransition(0,'a',2);
	fa.addTransition(1,'b',2);
	fa.addTransition(2,'b',2);
	fa.addTransition(2,'b',0);
	
	
	EXPECT_TRUE(fa.isValid());
	EXPECT_EQ(5u,fa.countTransitions());
	EXPECT_EQ(2u,fa.countSymbols());
	EXPECT_FALSE(fa.match("bab"));
	EXPECT_TRUE(fa.match("aba"));
	EXPECT_TRUE(fa.match("a"));
	EXPECT_FALSE(fa.match("ba"));
	
	fa = fa.createMirror(fa);
	
	
	EXPECT_TRUE(fa.isValid());
	EXPECT_FALSE(fa.match("bab"));
	EXPECT_TRUE(fa.match("aba"));
	EXPECT_TRUE(fa.match("ba"));
	EXPECT_TRUE(fa.match("a"));
	EXPECT_EQ(5u,fa.countTransitions());
	EXPECT_EQ(2u,fa.countSymbols());

}

TEST(createMirrorTest, NoTransition){
	static const std::vector<char> symbols = {'a','b'};
	fa::Automaton fa = createAutomaton(3,symbols);

	fa.setStateInitial(0);
	fa.setStateFinal(2);
	
	EXPECT_TRUE(fa.isValid());
	EXPECT_EQ(0u,fa.countTransitions());
	EXPECT_EQ(2u,fa.countSymbols());
	EXPECT_TRUE(fa.isLanguageEmpty());

	fa = fa.createMirror(fa);

	EXPECT_TRUE(fa.isValid());
	EXPECT_EQ(0u,fa.countTransitions());
	EXPECT_EQ(2u,fa.countSymbols());
	EXPECT_TRUE(fa.isLanguageEmpty());
}

TEST(createMirrorTest, OneStateInitialAndFinal){
	static const std::vector<char> symbols = {'a'};
	fa::Automaton fa = createAutomaton(1,symbols);

	fa.setStateInitial(0);
	fa.setStateFinal(0);

	EXPECT_TRUE(fa.addTransition(0,'a',0));
	
	EXPECT_TRUE(fa.match("aaa"));
	EXPECT_TRUE(fa.isValid());
	EXPECT_EQ(1u,fa.countStates());
	EXPECT_EQ(1u,fa.countTransitions());
	
	fa = fa.createMirror(fa);

	EXPECT_TRUE(fa.isValid());
	EXPECT_EQ(1u,fa.countStates());
	EXPECT_EQ(1u,fa.countTransitions());
	EXPECT_TRUE(fa.match("aaa"));
	
}

/***************************** */
/*  RemoveNonAccessibleStates  */
/***************************** */ 

TEST(removeNonAccessibleStates, OneNonAccessibleState){
  static const std::vector<char> symbols = {'a'};
  fa::Automaton fa = createAutomaton(3, symbols);
	
  fa.setStateInitial(0);
  EXPECT_TRUE(fa.addTransition(0,'a',1));
  EXPECT_TRUE(fa.addTransition(1,'a',0));
  EXPECT_TRUE(fa.addTransition(1,'a',1));

  EXPECT_TRUE(fa.isValid());
	EXPECT_EQ(fa.countStates(), 3u);

	fa.removeNonAccessibleStates();

	EXPECT_EQ(fa.countStates(), 2u);
	EXPECT_TRUE(fa.isValid());
}

TEST(removeNonAccessibleStates,NoInitialState){
	static const std::vector<char> symbols = {'a','b'};
  fa::Automaton fa = createAutomaton(3, symbols);
	
	EXPECT_TRUE(fa.isValid());
	EXPECT_TRUE(fa.isLanguageEmpty());
	EXPECT_EQ(fa.countStates(), 3u);

	fa.removeNonAccessibleStates();

	EXPECT_EQ(fa.countStates(), 1u);
	EXPECT_TRUE(fa.isLanguageEmpty());
	EXPECT_TRUE(fa.isValid());
}

TEST(removeNonAccessibleStates, noNonAccessiblesStates){
  static const std::vector<char> symbols = {'a','b','c'};
	fa::Automaton fa = createAutomaton(4,symbols);	

	fa.setStateInitial(0);
	EXPECT_TRUE(fa.addTransition(0,'a',1));
	EXPECT_TRUE(fa.addTransition(1,'a',2));
	EXPECT_TRUE(fa.addTransition(2,'a',3));

	EXPECT_TRUE(fa.isValid());
	EXPECT_EQ(4u,fa.countStates());

	fa.removeNonAccessibleStates();

	EXPECT_TRUE(fa.isValid());
	EXPECT_EQ(4u,fa.countStates());
}

TEST(removeNonAccessibleStates, lastNoAccessible){
	static const std::vector<char> symbols = {'a','b','c'};
	fa::Automaton fa = createAutomaton(4,symbols);	

	fa.setStateInitial(0);
	EXPECT_TRUE(fa.addTransition(0,'a',1));
	EXPECT_TRUE(fa.addTransition(1,'a',2));
	EXPECT_TRUE(fa.addTransition(3,'a',3));

	EXPECT_TRUE(fa.isValid());
	EXPECT_EQ(4u,fa.countStates());
	EXPECT_EQ(3u,fa.countTransitions());

	fa.removeNonAccessibleStates();

	EXPECT_TRUE(fa.isValid());
	EXPECT_FALSE(fa.hasState(3));
	EXPECT_EQ(3u,fa.countStates());
	EXPECT_EQ(2u,fa.countTransitions());
}

TEST(removeNonAccessibleStates, ChainedAutomaton){
	static const std::vector<char> symbols = {'a','b','c'};
	fa::Automaton fa = createAutomaton(4,symbols);	
	
	fa.setStateInitial(0);
	
	EXPECT_TRUE(fa.addTransition(1,'a',1));
	EXPECT_TRUE(fa.addTransition(1,'a',2));
	EXPECT_TRUE(fa.addTransition(2,'a',3));
	EXPECT_TRUE(fa.addTransition(3,'a',3));

	EXPECT_TRUE(fa.isValid());
	EXPECT_EQ(4u,fa.countStates());
	EXPECT_EQ(4u,fa.countTransitions());

	fa.removeNonAccessibleStates();


	EXPECT_TRUE(fa.isValid());
	EXPECT_EQ(1u,fa.countStates());
	EXPECT_EQ(0u,fa.countTransitions());
}

TEST(removeNonAccessibleStates, Many) {
  static const std::vector<char> symbols = {'a'};
    fa::Automaton fa = createAutomaton(999,symbols);

    fa.setStateInitial(0);
  fa.addTransition(0, 'a', 1);
  fa.addTransition(1, 'a', 0);
  
  EXPECT_TRUE(fa.isValid());
  EXPECT_EQ(999u,fa.countStates());
  EXPECT_EQ(2u,fa.countTransitions());
  
  fa.removeNonAccessibleStates();
  
  EXPECT_TRUE(fa.isValid());
  EXPECT_EQ(2u,fa.countStates());
  EXPECT_EQ(2u,fa.countTransitions());
}

/***************************** */
/*     CreateDeterministic     */
/***************************** */ 

TEST(createDeterministicTest, Deterministic){
  std::vector<char> symbols = {'a', 'b'};
  fa::Automaton fa = createAutomaton(3, symbols);

  fa.setStateInitial(0);

	EXPECT_TRUE(fa.addTransition(0,'a',1));
	EXPECT_TRUE(fa.addTransition(0,'b',2));
	EXPECT_TRUE(fa.addTransition(1,'a',0));

  EXPECT_TRUE(fa.isDeterministic());

  fa = fa.createDeterministic(fa);

  EXPECT_TRUE(fa.isValid());
  EXPECT_TRUE(fa.isDeterministic());
}

TEST(createDeterministicTest, NoInitialState){
  std::vector<char> symbols = {'a', 'b'};
  fa::Automaton fa = createAutomaton(3,symbols);

  EXPECT_TRUE(fa.getInitialSt().empty());

  EXPECT_TRUE(fa.addTransition(0,'a',0));
  EXPECT_TRUE(fa.addTransition(0,'a',1));
  EXPECT_TRUE(fa.addTransition(0,'b',2));
  EXPECT_TRUE(fa.addTransition(1,'a',0));

  EXPECT_FALSE(fa.isDeterministic());

  fa = fa.createDeterministic(fa);

  EXPECT_TRUE(fa.isValid());
  EXPECT_TRUE(fa.isDeterministic());
}

TEST(createDeterministicTest,TwoInitialStates){
  std::vector<char> symbols = {'a', 'b'};
  fa::Automaton fa = createAutomaton(3, symbols);

  fa.setStateInitial(0);
  fa.setStateInitial(1);

  EXPECT_TRUE(fa.addTransition(0,'a',0));
	EXPECT_TRUE(fa.addTransition(0,'a',1));
	EXPECT_TRUE(fa.addTransition(0,'b',2));
	EXPECT_TRUE(fa.addTransition(1,'a',0));

  EXPECT_FALSE(fa.isDeterministic());

  fa = fa.createDeterministic(fa);

  EXPECT_TRUE(fa.isValid());
  EXPECT_TRUE(fa.isDeterministic());
}

TEST(createDeterministicTest, OneState){
  std::vector<char> symbols = {'a'};
  fa::Automaton fa = createAutomaton(1,symbols);
  fa.setStateInitial(0);

  EXPECT_TRUE(fa.isDeterministic());
  fa = fa.createDeterministic(fa);
  EXPECT_TRUE(fa.isValid());
  EXPECT_TRUE(fa.isDeterministic());
}

TEST(createDeterministicTest, Many) {
  static const std::vector<char> symbols = {'a', 'b'};
    fa::Automaton fa = createAutomaton(999,symbols);
  fa.setStateInitial(0);
  fa.setStateInitial(1);

  EXPECT_TRUE(fa.addTransition(0,'a',0));
	EXPECT_TRUE(fa.addTransition(0,'a',1));
	EXPECT_TRUE(fa.addTransition(0,'b',2));
	EXPECT_TRUE(fa.addTransition(1,'a',0));
  
  EXPECT_FALSE(fa.isDeterministic());
  
  fa = fa.createDeterministic(fa);
  
  EXPECT_TRUE(fa.isDeterministic());
}

/***************************** */
/*         MinimalMoore        */
/***************************** */ 

TEST(minimalMooreTest, CourseExample) {
  static const std::vector<char> symbols = {'a','b'};
	fa::Automaton fa = createAutomaton(6,symbols);	
  fa.setStateInitial(0);
  fa.setStateFinal(3);
  fa.setStateFinal(4);
  fa.addTransition(0, 'a', 1);
  fa.addTransition(0, 'b', 2);
  fa.addTransition(1, 'a', 2);
  fa.addTransition(1, 'b', 3);
  fa.addTransition(2, 'a', 1);
  fa.addTransition(2, 'b', 4);
  fa.addTransition(3, 'a', 4);
  fa.addTransition(3, 'b', 5);
  fa.addTransition(4, 'a', 3);
  fa.addTransition(4, 'b', 5);
  fa.addTransition(5, 'a', 5);
  fa.addTransition(5, 'b', 5);

  EXPECT_EQ(fa.countStates(), 6u);

  EXPECT_TRUE(fa.isValid());
  EXPECT_TRUE(fa.isDeterministic());
  EXPECT_TRUE(fa.isComplete());

  fa::Automaton minimal_fa = fa.createMinimalMoore(fa);

  EXPECT_EQ(minimal_fa.countTransitions(), 8u);
  EXPECT_EQ(minimal_fa.countStates(), 4u);
  EXPECT_EQ(minimal_fa.countSymbols(), 2u);

  EXPECT_TRUE(minimal_fa.isValid());
  EXPECT_TRUE(minimal_fa.isDeterministic());
  EXPECT_TRUE(minimal_fa.isComplete());
}

TEST(minimalMooreTest, MinimalCourseExample) {
  static const std::vector<char> symbols = {'a','b'};
	fa::Automaton fa = createAutomaton(4,symbols);	
  fa.setStateInitial(0);

  fa.setStateFinal(2);
  
  fa.addTransition(0, 'a', 1);
  fa.addTransition(0, 'b', 1);
  fa.addTransition(1, 'a', 1);
  fa.addTransition(1, 'b', 2);
  fa.addTransition(2, 'a', 2);
  fa.addTransition(2, 'b', 3);
  fa.addTransition(3, 'a', 3);
  fa.addTransition(3, 'b', 3);

  EXPECT_TRUE(fa.isValid());
  EXPECT_TRUE(fa.isDeterministic());
  EXPECT_TRUE(fa.isComplete());

  fa::Automaton minimal_fa = fa.createMinimalMoore(fa);

  EXPECT_EQ(minimal_fa.countTransitions(), 8u);
  EXPECT_EQ(minimal_fa.countStates(), 4u);
  EXPECT_EQ(minimal_fa.countSymbols(), 2u);

  EXPECT_TRUE(minimal_fa.isValid());
  EXPECT_TRUE(minimal_fa.isDeterministic());
  EXPECT_TRUE(minimal_fa.isComplete());
}

TEST(minimalMooreTest, NoInitialState) {
  static const std::vector<char> symbols = {'a','b'};
	fa::Automaton fa = createAutomaton(6,symbols);
  fa.setStateFinal(3);
  fa.setStateFinal(4);
  fa.addTransition(0, 'a', 1);
  fa.addTransition(0, 'b', 2);
  fa.addTransition(1, 'a', 2);
  fa.addTransition(1, 'b', 3);
  fa.addTransition(2, 'a', 1);
  fa.addTransition(2, 'b', 4);
  fa.addTransition(3, 'a', 4);
  fa.addTransition(3, 'b', 5);
  fa.addTransition(4, 'a', 3);
  fa.addTransition(4, 'b', 5);
  fa.addTransition(5, 'a', 5);
  fa.addTransition(5, 'b', 5);

  EXPECT_TRUE(fa.getInitialSt().empty());

  EXPECT_TRUE(fa.isValid());
  EXPECT_FALSE(fa.isDeterministic());
  EXPECT_TRUE(fa.isComplete());

  fa::Automaton minimal_fa = fa.createMinimalMoore(fa);

  EXPECT_TRUE(minimal_fa.isValid());
  EXPECT_TRUE(minimal_fa.isDeterministic());
  EXPECT_TRUE(minimal_fa.isComplete());
}

TEST(minimalMooreTest,CompleteAndDeterministic){
	static const std::vector<char> symbols = {'a','b'};
	fa::Automaton fa = createAutomaton(6,symbols);
	
	fa.setStateInitial(0);

	fa.setStateFinal(4);
	fa.setStateFinal(5);

	EXPECT_TRUE(fa.addTransition(0,'a',1));
	EXPECT_TRUE(fa.addTransition(1,'a',0));
	EXPECT_TRUE(fa.addTransition(1,'b',2));
	EXPECT_TRUE(fa.addTransition(2,'a',1));
	EXPECT_TRUE(fa.addTransition(0,'b',3));
	EXPECT_TRUE(fa.addTransition(3,'a',1));
	EXPECT_TRUE(fa.addTransition(3,'b',4));
	EXPECT_TRUE(fa.addTransition(4,'b',4));
	EXPECT_TRUE(fa.addTransition(4,'a',0));
	EXPECT_TRUE(fa.addTransition(2,'b',5));
	EXPECT_TRUE(fa.addTransition(5,'a',1));
	EXPECT_TRUE(fa.addTransition(5,'b',4));

	EXPECT_TRUE(fa.isValid());
	EXPECT_TRUE(fa.isComplete());
	EXPECT_TRUE(fa.isDeterministic());
	EXPECT_EQ(fa.countStates(), 6u);

	fa::Automaton minimal_fa = fa.createMinimalMoore(fa);

	EXPECT_TRUE(minimal_fa.isValid());
	EXPECT_TRUE(minimal_fa.isComplete());
	EXPECT_TRUE(minimal_fa.isDeterministic());
	EXPECT_EQ(minimal_fa.countStates(), 3u);

	EXPECT_TRUE(fa.isIncludedIn(minimal_fa));
	EXPECT_TRUE(minimal_fa.isIncludedIn(fa));
}

TEST(minimalMooreTest, Many) {
  static const std::vector<char> symbols = {'a','b'};
	fa::Automaton fa = createAutomaton(999,symbols);
	
	fa.setStateInitial(0);

	fa.setStateFinal(4);
	fa.setStateFinal(5);

	EXPECT_TRUE(fa.addTransition(0,'a',1));
	EXPECT_TRUE(fa.addTransition(1,'a',0));
	EXPECT_TRUE(fa.addTransition(1,'b',2));
	EXPECT_TRUE(fa.addTransition(2,'a',1));
	EXPECT_TRUE(fa.addTransition(0,'b',3));
	EXPECT_TRUE(fa.addTransition(3,'a',1));
	EXPECT_TRUE(fa.addTransition(3,'b',4));
	EXPECT_TRUE(fa.addTransition(4,'b',4));
	EXPECT_TRUE(fa.addTransition(4,'a',0));
	EXPECT_TRUE(fa.addTransition(2,'b',5));
	EXPECT_TRUE(fa.addTransition(5,'a',1));
	EXPECT_TRUE(fa.addTransition(5,'b',4));

	EXPECT_TRUE(fa.isValid());
	EXPECT_FALSE(fa.isComplete());
	EXPECT_TRUE(fa.isDeterministic());
	EXPECT_EQ(fa.countStates(), 999u);
  
    fa::Automaton minimal_fa = fa.createMinimalMoore(fa);

	EXPECT_TRUE(minimal_fa.isValid());
	EXPECT_TRUE(minimal_fa.isComplete());
	EXPECT_TRUE(minimal_fa.isDeterministic());
	EXPECT_EQ(minimal_fa.countStates(), 3u);

	EXPECT_TRUE(fa.isIncludedIn(minimal_fa));
	EXPECT_TRUE(minimal_fa.isIncludedIn(fa));
}

/***************************** */
/*     MinimalBororxkcvfuihr   */
/***************************** */ 

TEST(MinimalBororxkcvfuihrTest, CourseExample) {
  static const std::vector<char> symbols = {'a','b'};
	fa::Automaton fa = createAutomaton(6,symbols);	
  fa.setStateInitial(0);
  fa.setStateFinal(3);
  fa.setStateFinal(4);
  fa.addTransition(0, 'a', 1);
  fa.addTransition(0, 'b', 2);
  fa.addTransition(1, 'a', 2);
  fa.addTransition(1, 'b', 3);
  fa.addTransition(2, 'a', 1);
  fa.addTransition(2, 'b', 4);
  fa.addTransition(3, 'a', 4);
  fa.addTransition(3, 'b', 5);
  fa.addTransition(4, 'a', 3);
  fa.addTransition(4, 'b', 5);
  fa.addTransition(5, 'a', 5);
  fa.addTransition(5, 'b', 5);

  EXPECT_EQ(fa.countStates(), 6u);

  EXPECT_TRUE(fa.isValid());
  EXPECT_TRUE(fa.isDeterministic());
  EXPECT_TRUE(fa.isComplete());

  fa::Automaton minimal_fa = fa.createMinimalBrzozowski(fa);

  EXPECT_EQ(minimal_fa.countTransitions(), 8u);
  EXPECT_EQ(minimal_fa.countStates(), 4u);
  EXPECT_EQ(minimal_fa.countSymbols(), 2u);

  EXPECT_TRUE(minimal_fa.isValid());
  EXPECT_TRUE(minimal_fa.isDeterministic());
  EXPECT_TRUE(minimal_fa.isComplete());
}

TEST(MinimalBororxkcvfuihrTest, MinimalCourseExample) {
  static const std::vector<char> symbols = {'a','b'};
	fa::Automaton fa = createAutomaton(4,symbols);	
  fa.setStateInitial(0);

  fa.setStateFinal(2);
  
  fa.addTransition(0, 'a', 1);
  fa.addTransition(0, 'b', 1);
  fa.addTransition(1, 'a', 1);
  fa.addTransition(1, 'b', 2);
  fa.addTransition(2, 'a', 2);
  fa.addTransition(2, 'b', 3);
  fa.addTransition(3, 'a', 3);
  fa.addTransition(3, 'b', 3);

  EXPECT_TRUE(fa.isValid());
  EXPECT_TRUE(fa.isDeterministic());
  EXPECT_TRUE(fa.isComplete());

  fa::Automaton minimal_fa = fa.createMinimalBrzozowski(fa);

  EXPECT_EQ(minimal_fa.countTransitions(), 8u);
  EXPECT_EQ(minimal_fa.countStates(), 4u);
  EXPECT_EQ(minimal_fa.countSymbols(), 2u);

  EXPECT_TRUE(minimal_fa.isValid());
  EXPECT_TRUE(minimal_fa.isDeterministic());
  EXPECT_TRUE(minimal_fa.isComplete());
}

TEST(MinimalBororxkcvfuihrTest, NoInitialState) {
  static const std::vector<char> symbols = {'a','b'};
	fa::Automaton fa = createAutomaton(6,symbols);	
  fa.setStateFinal(3);
  fa.setStateFinal(4);
  fa.addTransition(0, 'a', 1);
  fa.addTransition(0, 'b', 2);
  fa.addTransition(1, 'a', 2);
  fa.addTransition(1, 'b', 3);
  fa.addTransition(2, 'a', 1);
  fa.addTransition(2, 'b', 4);
  fa.addTransition(3, 'a', 4);
  fa.addTransition(3, 'b', 5);
  fa.addTransition(4, 'a', 3);
  fa.addTransition(4, 'b', 5);
  fa.addTransition(5, 'a', 5);
  fa.addTransition(5, 'b', 5);

  EXPECT_TRUE(fa.isValid());
  EXPECT_FALSE(fa.isDeterministic());
  EXPECT_TRUE(fa.isComplete());

  fa::Automaton minimal_fa = fa.createMinimalBrzozowski(fa);

  EXPECT_TRUE(minimal_fa.isValid());
  EXPECT_TRUE(minimal_fa.isDeterministic());
  EXPECT_TRUE(minimal_fa.isComplete());
}

TEST(MinimalBororxkcvfuihrTest,CompleteAndDeterministic){
	static const std::vector<char> symbols = {'a','b'};
	fa::Automaton fa = createAutomaton(6,symbols);
	
	fa.setStateInitial(0);

	fa.setStateFinal(4);
	fa.setStateFinal(5);

	EXPECT_TRUE(fa.addTransition(0,'a',1));
	EXPECT_TRUE(fa.addTransition(1,'a',0));
	EXPECT_TRUE(fa.addTransition(1,'b',2));
	EXPECT_TRUE(fa.addTransition(2,'a',1));
	EXPECT_TRUE(fa.addTransition(0,'b',3));
	EXPECT_TRUE(fa.addTransition(3,'a',1));
	EXPECT_TRUE(fa.addTransition(3,'b',4));
	EXPECT_TRUE(fa.addTransition(4,'b',4));
	EXPECT_TRUE(fa.addTransition(4,'a',0));
	EXPECT_TRUE(fa.addTransition(2,'b',5));
	EXPECT_TRUE(fa.addTransition(5,'a',1));
	EXPECT_TRUE(fa.addTransition(5,'b',4));

	EXPECT_TRUE(fa.isValid());
	EXPECT_TRUE(fa.isComplete());
	EXPECT_TRUE(fa.isDeterministic());
	EXPECT_EQ(fa.countStates(), 6u);

	fa::Automaton minimal_fa = fa.createMinimalBrzozowski(fa);

	EXPECT_TRUE(minimal_fa.isValid());
	EXPECT_TRUE(minimal_fa.isComplete());
	EXPECT_TRUE(minimal_fa.isDeterministic());
	EXPECT_EQ(minimal_fa.countStates(), 3u);

	EXPECT_TRUE(fa.isIncludedIn(minimal_fa));
	EXPECT_TRUE(minimal_fa.isIncludedIn(fa));
}

/***************************** */
/*       TEST(Automaton)       */
/***************************** */

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}