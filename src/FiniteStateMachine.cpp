/*
||
|| @file FiniteStateMachine.cpp
|| @version 1.7
|| @author Alexander Brevig
|| @contact alexanderbrevig@gmail.com
||
|| @description
|| | Provide an easy way of making finite state machines
|| #
||
|| @license
|| | This library is free software; you can redistribute it and/or
|| | modify it under the terms of the GNU Lesser General Public
|| | License as published by the Free Software Foundation; version
|| | 2.1 of the License.
|| |
|| | This library is distributed in the hope that it will be useful,
|| | but WITHOUT ANY WARRANTY; without even the implied warranty of
|| | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
|| | Lesser General Public License for more details.
|| |
|| | You should have received a copy of the GNU Lesser General Public
|| | License along with this library; if not, write to the Free Software
|| | Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
|| #
||
*/

#include "FiniteStateMachine.hpp"

State::State( string stateName, void (*enterFunction)(FiniteStateMachine*), void (*updateFunction)(FiniteStateMachine*), void (*exitFunction)(FiniteStateMachine*) ){
	this->stateName = stateName;
	userEnter = enterFunction;
	userUpdate = updateFunction;
	userExit = exitFunction;
}

//what to do when entering this state
void State::enter(FiniteStateMachine *fsm){
	if (userEnter){
		userEnter(fsm);
	}
}

//what to do when this state updates
void State::update(FiniteStateMachine *fsm){
	if (userUpdate){
		userUpdate(fsm);
	}
}

//what to do when exiting this state
void State::exit(FiniteStateMachine *fsm){
	if (userExit){
		userExit(fsm);
	}
}

string State::getStateName() {
	return this->stateName;
}
//END FINITE STATE


//FINITE STATE MACHINE
FiniteStateMachine::FiniteStateMachine(State& current, Deps *deps){
	this->deps = deps;
	needToTriggerEnter = true;
	currentState = nextState = &current;
	// stateChangeTime = 0;
}

FiniteStateMachine& FiniteStateMachine::update() {
	//simulate a transition to the first state
	//this only happens the first time update is called
	if (needToTriggerEnter) { 
		currentState->enter(this);
		needToTriggerEnter = false;
	} else {
		if (currentState != nextState){
			immediateTransitionTo(*nextState);
		}
		currentState->update(this);
	}
	return *this;
}

FiniteStateMachine& FiniteStateMachine::transitionTo(State& state){
	nextState = &state;
	// stateChangeTime = millis();
	stateChangeTime = this->deps->hardware.millis();
	return *this;
}

FiniteStateMachine& FiniteStateMachine::immediateTransitionTo(State& state){
	currentState->exit(this);
	currentState = nextState = &state;
	currentState->enter(this);
	// stateChangeTime = millis();
	stateChangeTime = this->deps->hardware.millis();
	return *this;
}

Deps* FiniteStateMachine::getDeps() {
	return this->deps;
}

//return the current state
State& FiniteStateMachine::getCurrentState() {
	return *currentState;
}

//check if state is equal to the currentState
/// boolean FiniteStateMachine::isInState( State &state ) const {
bool FiniteStateMachine::isInState( State &state ) const {
	if (&state == currentState) {
		return true;
	} else {
		return false;
	}
}

unsigned long FiniteStateMachine::timeInCurrentState() { 
	// millis() - stateChangeTime; 
	return this->deps->hardware.millis() - stateChangeTime;
}
//END FINITE STATE MACHINE