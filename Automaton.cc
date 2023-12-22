#include "Automaton.h"
#include <algorithm>
#include <array>
#include <assert.h>
#include <cstddef>
#include <cstdlib>
#include <vector>
#include <ctype.h>
#include <map>
#include <ostream>
#include <set>
#include <string>
#include <utility>
#include <vector>
#include <cctype>
#include <clocale>
#include <iostream>
#include <algorithm>


namespace fa {

  Automaton::Automaton() {}

  /***************************** */
  /*            MISC             */
  /***************************** */

  std::set<char> Automaton::getAl() const {
    return al;
  }

  std::set<int> Automaton::getSt() const {
    return states;
  }

  std::set<int> Automaton::getInitialSt() const {
    return initial_states;
  }

  std::set<int> Automaton::getFinalSt() const {
    return final_states;
  }

  std::map<std::pair<int, char>, std::set<int>> Automaton::getTr() const {
    return tr;
  }

  void Automaton::setAl(std::set<char> _al){
    al = _al;
  }

  void Automaton::setSt(std::set<int> _st){
    states = _st;
  }

  void Automaton::setInitSt(std::set<int> _init_st){
    initial_states = _init_st;
  }

  void Automaton::setFinalSt(std::set<int> _final_st){
    final_states = _final_st;
  }

  void Automaton::setTr(std::map<std::pair<int, char>, std::set<int>> _tr){
    tr = _tr;
  }

  void Automaton::removeFinalState(int state){
    assert(&state != NULL);
    if(isStateFinal(state)){
      final_states.erase(state);
    }
  }

  void Automaton::removeInitialState(int state){
    if(isStateInitial(state)){
      initial_states.erase(state);
    }
  }

  void Automaton::copy(const Automaton& other){
    setAl(other.getAl());
    setSt(other.getSt());
    setFinalSt(other.getFinalSt());
    setInitSt(other.getInitialSt());
    setTr(other.getTr());
  }

  /***************************** */
  /*            MAIN             */
  /***************************** */

  bool Automaton::isValid() const {
    if(al.empty() || states.empty()){
      return false;
    }
    return true;
  }

  bool Automaton::addSymbol(char symbol) {
    assert(&symbol != NULL);
    if(std::isgraph(symbol) == 0 || symbol == fa::Epsilon){
      return false;
    }
    if(hasSymbol(symbol)){
      return false;
    }
    al.insert(symbol);
    if(hasSymbol(symbol)){
      return true;
    }
    return false;
  }

  bool Automaton::removeSymbol(char symbol) {
    assert(&symbol != NULL);
    if(hasSymbol(symbol)){
      al.erase(symbol);
      for (auto it = tr.begin(); it != tr.end();){
        if(it->first.second == symbol) it = tr.erase(it);
        else ++it;
      }
      return true;
    }
    return false;
  }

  bool Automaton::hasSymbol(char symbol) const {
    assert(&symbol != NULL);
    if(al.find(symbol) != al.end()){
      return true;
    }
    return false;
  }

  std::size_t Automaton::countSymbols() const {
    return al.size();
  }

  bool Automaton::addState(int state) {
    assert(&state != NULL);
    if(hasState(state) || state < 0){
      return false;
    }
    states.insert(state);
    if(hasState(state)){
      return true;
    }
    return false;
  }

  bool Automaton::removeState(int state) {
    assert(&state != NULL);
    if(hasState(state)){
      if(states.erase(state) != 1){
        return false;
      }

      // Remove transitions in which the state is implied
      for(auto symbol : getAl()){
        removeTransition(state, symbol, state);
        for(auto s : getSt()){
          removeTransition(s, symbol, state);
          removeTransition(state, symbol, s);
        }
      }

      // Delete the state from initial and final states if it is in them
      removeInitialState(state);
      removeFinalState(state);

      return true;
    }
    return false;
  }

  bool Automaton::hasState(int state) const {
    assert(&state != NULL);
    if(states.count(state) != 0){
      return true;
    }
    return false;
  }

  std::size_t Automaton::countStates() const {
    return states.size();
  }

  void Automaton::setStateInitial(int state) {
    assert(&state != NULL);
    // Test error "ReadEmptyString"
    if(hasState(state)){
      initial_states.insert(state);
    }
  }

  bool Automaton::isStateInitial(int state) const{
    assert(&state != NULL);
    return (initial_states.count(state) != 0);
  }

  void Automaton::setStateFinal(int state) {
    assert(&state != NULL);
    if(hasState(state)){
      final_states.insert(state);
    }
  }

  bool Automaton::isStateFinal(int state) const{
    assert(&state != NULL);
    return (final_states.count(state) != 0);
  }

  bool Automaton::addTransition(int from, char alpha, int to) {
    assert(&from != NULL);
    assert(&to != NULL);
    assert(&alpha != NULL);

    if(hasTransition(from, alpha, to) || 
    (alpha != fa::Epsilon && !hasSymbol(alpha)) || 
    !hasState(from) || 
    !hasState(to)){
      return false;
    }

    return tr[{from, alpha}].insert(to).second;
  }

  bool Automaton::removeTransition(int from, char alpha, int to) {
    assert(&from != NULL);
    assert(&to != NULL);
    assert(&alpha != NULL);
    if(!hasTransition(from, alpha, to)){
      // transition does not exist
      return false;
    }
    auto key = std::pair(from, alpha);
    if(tr.count(key) != 0){
      // transition found
      tr.erase(key);
    }
    //  
    return !hasTransition(from, alpha, to);
  }

  bool Automaton::hasTransition(int from, char alpha, int to) const {
    assert(&from != NULL);
    assert(&to != NULL);
    assert(&alpha != NULL);
    auto find = tr.find({from, alpha});
    if(find != tr.end() && find->second.count(to) != 0) return true;
    return false;
  }

  std::size_t Automaton::countTransitions() const {
    std::size_t res = 0;
    for(auto from : getSt()){
      for(auto symbol : getAl()){
        auto findKey = tr.find(std::make_pair(from, symbol)); 
        if(findKey != tr.end()) res += findKey->second.size();
      }
      auto findKey = tr.find(std::make_pair(from, fa::Epsilon)); 
        if(findKey != tr.end()) res += findKey->second.size();    
    }
    return res;
  }

  void Automaton::prettyPrint(std::ostream& os) const {
    os << "\nInitial states :\n\t";
    std::for_each(initial_states.begin(), initial_states.end(), [&os](int x){
      os << x << " ";
    });
    os << "\nFinal states :\n\t";
    std::for_each(final_states.begin(), final_states.end(), [&os](int x){
      os << x << " ";
    });
    os << "\nTransitions :";
    int last_state = -1;
    for (auto it = tr.begin(); it != tr.end(); ++it){
      int curr_state = it->first.first;
      if(curr_state != last_state){
        os << "\n\tFor state " << curr_state << " :";
      }
      os << "\n\t\tFor letter " << it->first.second << " : ";
      std::for_each(it->second.begin(), it->second.end(), [&os](int x){
        os << x << " ";
      });
      last_state = curr_state;
    }
    os << "\n\n";
  }

  //void Automaton::dotPrint(std::ostream& os) const {}

  bool Automaton::hasEpsilonTransition() const {
    assert(isValid());

    for(auto it : tr){
      if(it.first.second == fa::Epsilon){
        return true;
      }
    }

    return false;
  }

  bool Automaton::isDeterministic() const{
    assert(isValid());

    if(initial_states.size() != 1 || hasEpsilonTransition()){
      return false;
    }

    for(auto t : getTr()){
      if(t.second.size() > 1){
        return false;
      }
    }

    return true;
  }

  bool Automaton::isComplete() const {
    assert(isValid());
    for(auto it_st : getSt()){
      for(auto it_al : getAl()){
        if(tr.count({it_st, it_al}) == 0) return false;
      }
    }
    return true;
  }

  Automaton Automaton::createComplete(const Automaton& automaton) {
    fa::Automaton completeAutomaton = automaton;
    
    if(automaton.isComplete()){
      return completeAutomaton;
    }

    //if(completeAutomaton.countStates() > 100) return completeAutomaton;

    // Dump State creation to complete the automaton
    int dump_state = 0;
    while(completeAutomaton.hasState(dump_state)) ++ dump_state;
    completeAutomaton.addState(dump_state);

    for(auto symbol : completeAutomaton.getAl()){
      completeAutomaton.addTransition(dump_state, symbol, dump_state);
      for(auto state : completeAutomaton.getSt()){
        if(completeAutomaton.tr.count({state, symbol}) == 0){
          completeAutomaton.addTransition(state, symbol, dump_state);
        }
      }
    }

    return completeAutomaton;
  }

  std::set<int> Automaton::makeTransition(const std::set<int>& origin, char alpha) const {
    auto set = std::set<int>();
    
    for(auto o : origin){
      for(auto s : states){
        if(hasTransition(o, alpha, s)) set.insert(s);
      }
    }

    return set;
  }

  std::set<int> Automaton::readString(const std::string& word) const {
    auto set = getInitialSt();

    for(auto letter : word){
      set = makeTransition(set, letter);
    }

    return set;
  }

  bool Automaton::match(const std::string& word) const {
    auto read = readString(word);

    for(auto s : states){
      for(auto r : read){
        if(s == r && isStateFinal(s)) return true;
      }
    }

    return false;
  }

  void Automaton::removeNonAccessibleStates() {
    assert(isValid());

    if(getInitialSt().empty()){
      for(auto s : getSt()){
        removeState(s);
      }
      addState(0);
      setStateInitial(0);
      addSymbol('a');
      return;
    }

    std::set<int> visited;

    for(auto s : getInitialSt()){
      DFS(visited, s, false);
    }

    for(auto s : getSt()){
      if(visited.find(s) == visited.end()){
        removeState(s);
      }
    }
  }

  void Automaton::removeNonCoAccessibleStates() {
    *this = createMirror(*this);
    this->removeNonAccessibleStates();
    *this = createMirror(*this);
  }

  bool Automaton::DFS(std::set<int>& visited, int s, bool return_on_final) const{
    assert(isValid());

    bool res = true;

    // VISITED(s) -> true
    visited.insert(s);

    for(auto t : tr){
      if(t.first.first != s) continue;
      // for u in adjacent(G,s)
      for(auto u : t.second){
        if(return_on_final && isStateFinal(u)) return false;
        // if not VISITED(u)
        if(visited.find(u) == visited.end()){
          // DFS(G,u)
          res = DFS(visited, u, return_on_final);
        }
      }
    }

    return res;
  }

  bool Automaton::isLanguageEmpty() const {
    assert(isValid());

    for(auto s : initial_states){
      if(isStateFinal(s)){
        return false;
      }
    }

    if(getInitialSt().size() == 0 || getFinalSt().size() == 0) return true;

    for(auto init_st : initial_states){
      auto visited = std::set<int>();
      if(!DFS(visited, init_st, true)){
        return false;
      }
    }

    return true;
  }

  bool Automaton::hasEmptyIntersectionWith(const Automaton& other) const {
    return createIntersection(*this, other).isLanguageEmpty();
  }

  bool Automaton::isIncludedIn(const Automaton& other) const {
    assert(other.isValid());
    assert(isValid());

    fa::Automaton _other = other;

    for(auto symbol : getAl()) {
        if(!_other.hasSymbol(symbol)) _other.addSymbol(symbol);
    }

    fa::Automaton complement = createComplement(_other);

    return hasEmptyIntersectionWith(complement);
  }

  Automaton Automaton::createMirror(const Automaton& automaton) {
    assert(automaton.isValid());

    fa::Automaton mirror_automaton;
    mirror_automaton.setAl(automaton.getAl());
    mirror_automaton.setSt(automaton.getSt());
    mirror_automaton.setInitSt(automaton.getInitialSt());
    mirror_automaton.setFinalSt(automaton.getFinalSt());

    auto init = mirror_automaton.getInitialSt();
    auto final = mirror_automaton.getFinalSt();

    for(auto it : final){
      if(!mirror_automaton.isStateInitial(it)){
        mirror_automaton.setStateInitial(it);
        mirror_automaton.removeFinalState(it);
      }
    }

    for(auto it : init){
      if(!mirror_automaton.isStateFinal(it)){
        mirror_automaton.setStateFinal(it);
        mirror_automaton.removeInitialState(it);
      }
    }

    for(auto t : automaton.getTr()){
      for(auto t_to : t.second){
        mirror_automaton.addTransition(t_to, 
                                       t.first.second,
                                       t.first.first);
      }
    }

    return mirror_automaton;
  }

  Automaton Automaton::createComplement(const Automaton& automaton) {
    fa::Automaton complementAutomaton = automaton;

    if(!complementAutomaton.isDeterministic()) complementAutomaton = createDeterministic(complementAutomaton);

    if(!complementAutomaton.isComplete()) complementAutomaton = createComplete(complementAutomaton);

    auto st = complementAutomaton.getSt();
    for(auto it_st : st){
      (complementAutomaton.isStateFinal(it_st)) ? 
      complementAutomaton.removeFinalState(it_st) : 
      complementAutomaton.setStateFinal(it_st);
    } 

    return complementAutomaton;
  }

  Automaton Automaton::createIntersection(const Automaton& lhs, const Automaton& rhs) {
    // For this function, refer to https://moodle.univ-fcomte.fr/pluginfile.php/644679/mod_resource/content/16/thlang.pdf
    // Page 158, this is the process used
    
    assert(lhs.isValid());
    assert(rhs.isValid());

    fa::Automaton intersection;

    // Variables
    std::map<std::pair<int, int>, int> to_process; // {(lhs_st, rhs_st), intersection_st}
    int curr_st = 0; // count the states of the intersection
    
    // First we make the instersection of both alphabets
    auto al = std::set<char>(); 
    for(auto a : lhs.getAl()){
      if(rhs.hasSymbol(a)){
        al.insert(a);
      }
    }
    intersection.setAl(al);

    // Then we get every pair of initial states
    for(auto lhs_ptr : lhs.getInitialSt()){
      for(auto rhs_ptr : rhs.getInitialSt()){
        to_process.insert({std::make_pair(lhs_ptr, rhs_ptr), curr_st});
        intersection.addState(curr_st);
        intersection.setStateInitial(curr_st);
        ++curr_st;
      }
    }

    // Visit both automaton and create new states / intersections
    for(auto to_process_curr : to_process){
      // Get every pair of states for every symbols in the alphabet 
      for(auto symbol : intersection.getAl()){
        std::set<int> lhs_symbol_state;
        std::set<int> rhs_symbol_state;
        
        for(auto lhs_st_to : lhs.getSt()){
          // looking for every transitions starting by the current to process lhs state and the using current symbol 
          if(lhs.hasTransition(to_process_curr.first.first, symbol, lhs_st_to)){
            lhs_symbol_state.insert(lhs_st_to);
          }
        }

        for(auto rhs_st_to : rhs.getSt()){
          // looking for every transitions starting by the current rhs state and the using current symbol 
          if(rhs.hasTransition(to_process_curr.first.second, symbol, rhs_st_to)){
            rhs_symbol_state.insert(rhs_st_to);
          }
        }

        // Adding every pair of state in the intersection
        for(auto lhs_ptr : lhs_symbol_state){
          for(auto rhs_ptr : rhs_symbol_state){
            auto pair = std::make_pair(lhs_ptr, rhs_ptr);
            auto findState = to_process.find(pair);
            if(findState != to_process.end()){
              // The pair is already known, we add the transition from the current state
              // to the state of the said pair
              intersection.addTransition(to_process_curr.second, symbol, findState->second);
            }else{
              to_process.insert({std::make_pair(lhs_ptr, rhs_ptr), curr_st});
              intersection.addState(curr_st);
              intersection.addTransition(to_process_curr.second, symbol, curr_st);
              ++curr_st;
            }
          }
        }
      }

      // If both left and right states were final, then the current state is final
      if(lhs.isStateFinal(to_process_curr.first.first) && rhs.isStateFinal(to_process_curr.first.second)){
        intersection.setStateFinal(to_process_curr.second);
      }
    }

    // Make intersection valid if it was not
    if(!intersection.countStates()) intersection.addState(0);
    if(!intersection.countSymbols()) intersection.addSymbol('a');

    return intersection;
  }

  Automaton Automaton::createDeterministic(const Automaton& other) {
    assert(other.isValid());

    if(other.isDeterministic()){
      fa::Automaton copy = other;
      return other;
    } 

    if(other.getInitialSt().empty()){
      fa::Automaton a;
      a.addState(0);
      a.setStateInitial(0);
      a.addSymbol('a');
      return a;
    }

    fa::Automaton deterministic;

    std::map<std::set<int>, int> visited; // {[other_st_1, ...], deterministic_st}
    int curr_st = 1;

    // Alphabet
    deterministic.setAl(other.getAl());

    // Initial states
    visited.insert({other.getInitialSt(), 0});
    deterministic.addState(0);
    deterministic.setStateInitial(0);

    // Transitions
    for(auto visited_curr : visited){
      for(auto symbol : deterministic.getAl()){
        std::set<int> arrival_states;

        for(auto st_to : other.getSt()){
          for(auto st_from : visited_curr.first){
            if(other.hasTransition(st_from, symbol, st_to)){
              arrival_states.insert(st_to);
            } 
          }
        }

        auto findKey = visited.find(arrival_states);
        if(findKey != visited.end()){
          deterministic.addTransition(visited_curr.second, symbol, findKey->second);
        }else{
          visited.insert({arrival_states, curr_st});
          deterministic.addState(curr_st);
          deterministic.addTransition(visited_curr.second, symbol, curr_st);
          ++curr_st;
        }

        for(auto st : arrival_states){
          if(other.isStateFinal(st)) deterministic.setStateFinal(visited_curr.second);
        }

      }
    }

    return deterministic;
  }

  Automaton Automaton::createMinimalMoore(const Automaton& other) {
    assert(other.isValid());
    //
    fa::Automaton _other =  other;

    _other.removeNonAccessibleStates();
    _other = createComplete(_other);
    _other = createDeterministic(_other);
    //

    if(_other.countStates() == 1) return _other;

    std::vector<int> state_vector;
    for(auto st : _other.getSt()){
      // Build a vector of every states
      state_vector.push_back(st);
    }
    //
    std::vector<char> al_vector;
    for(auto a : _other.getAl()){
      // Build a vector of every symbols
      al_vector.push_back(a);
    }
    //
    std::vector<int> n0;
    std::map<char, std::vector<int>> nX;
    do {
      if(nX.empty()){ // First iteration
        std::vector<int> res;
        for(auto st : state_vector){
          // Final states are marked with a 2 and non-final with a 1
          (_other.isStateFinal(st)) ? res.push_back(2) : res.push_back(1) ;
        }
        nX.insert({' ', res});
      }else{ // Count the different states
        n0 = nX.at(' ');
        //
        std::vector<int> res;
        std::map<std::vector<int>, int> tuple_map;
        int count = 1;
        //
        for(auto st : state_vector){
          std::vector<int> tuple;
          tuple.push_back(n0[st]);
          for(auto symbol : al_vector){
            tuple.push_back(nX.at(symbol)[st]);
          }
          //
          if(tuple_map.count(tuple) == 0){
            tuple_map.insert({tuple, count});
            res.push_back(count);
            ++count;
          }else{
            res.push_back(tuple_map.at(tuple));
          }
        }
        //
        nX = std::map<char, std::vector<int>>(); // reset
        nX.insert({' ', res});
      }
      //
      for(auto symbol : al_vector){
        std::vector<int> symbol_res;
        //
        for(auto st_from : state_vector){
          for(auto st_to : _other.getSt()){
            if(_other.hasTransition(st_from, symbol, st_to)){
              symbol_res.push_back(nX.at(' ')[st_to]);
              // can break here
              break;
            }
          }
        }
        //
        nX.insert({symbol, symbol_res});
      }
      //
    }while (n0 != nX.find(' ')->second);
    
    // Creation of the minimal automaton
    fa::Automaton minimal_moore;
    // Same Symbols
    minimal_moore.setAl(_other.getAl());
    // States
    for(auto st : n0){
      minimal_moore.addState(st);
    }
    for(auto st : state_vector){
      if(_other.isStateInitial(st)){
        minimal_moore.setStateInitial(nX.find(' ')->second[st]);
      }
      if(_other.isStateFinal(st)){
        minimal_moore.setStateFinal(nX.find(' ')->second[st]);
      }
    }
    // Transitions
    for(auto n : nX){
      if(n.first == ' ') continue;
      for(auto st : state_vector){
        minimal_moore.addTransition(n0[st], n.first, n.second[st]);
      }
    }
    // Return
    return minimal_moore;
  }

  Automaton Automaton::createMinimalBrzozowski(const Automaton& other) {
    assert(other.isValid());

    fa::Automaton minimal_Brzozozzzozzozozzwwkswski = other;

    minimal_Brzozozzzozzozozzwwkswski = createMirror(minimal_Brzozozzzozzozozzwwkswski);
    minimal_Brzozozzzozzozozzwwkswski = createDeterministic(minimal_Brzozozzzozzozozzwwkswski);
    minimal_Brzozozzzozzozozzwwkswski = createMirror(minimal_Brzozozzzozzozozzwwkswski);
    minimal_Brzozozzzozzozozzwwkswski = createDeterministic(minimal_Brzozozzzozzozozzwwkswski);
    minimal_Brzozozzzozzozozzwwkswski = createComplete(minimal_Brzozozzzozzozozzwwkswski);

    return minimal_Brzozozzzozzozozzwwkswski;
  }

}

