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

#ifdef UNIT_TEST
State::State( string stateName, States estate, void (*updateFunction)(FiniteStateMachine*) )
#else
State::State( String stateName, States estate, void (*updateFunction)(FiniteStateMachine*) )
#endif
{
	this->stateName = stateName;
	this->userEnter = 0;
	this->userUpdate = updateFunction;
	this->userExit = 0;
}

#ifdef UNIT_TEST
State::State( string stateName, States estate, void (*enterFunction)(FiniteStateMachine*), void (*updateFunction)(FiniteStateMachine*), void (*exitFunction)(FiniteStateMachine*) )
#else
State::State( String stateName, States estate, void (*enterFunction)(FiniteStateMachine*), void (*updateFunction)(FiniteStateMachine*), void (*exitFunction)(FiniteStateMachine*) )
#endif
{
	this->estate = estate;
	this->stateName = stateName;
	this->userEnter = enterFunction;
	this->userUpdate = updateFunction;
	this->userExit = exitFunction;
}

//what to do when entering this state
void State::enter(FiniteStateMachine *fsm){
	if (this->userEnter){
		this->userEnter(fsm);
	}
}

//what to do when this state updates
void State::update(FiniteStateMachine *fsm){
	if (this->userUpdate){
		this->userUpdate(fsm);
	}
}

//what to do when exiting this state
void State::exit(FiniteStateMachine *fsm){
	if (this->userExit){
		this->userExit(fsm);
	}
}

States State::getState() {
	return this->estate;
}

#ifdef UNIT_TEST
string State::getStateName()
#else
String State::getStateName()
#endif
{
	return this->stateName;
}
//END FINITE STATE


//FINITE STATE MACHINE
FiniteStateMachine::FiniteStateMachine(State& current, Deps *deps){
	this->deps = deps;
	this->needToTriggerEnter = true;
	this->currentState = this->nextState = &current;
	this->stateChangeTime = 0;
}

FiniteStateMachine& FiniteStateMachine::update() {
	//simulate a transition to the first state
	//this only happens the first time update is called
	if (this->needToTriggerEnter) { 
		this->currentState->enter(this);
		this->needToTriggerEnter = false;
	} else {
		if (this->currentState != this->nextState){
			immediateTransitionTo(*this->nextState);
		}
		this->currentState->update(this);
	}
	return *this;
}

FiniteStateMachine& FiniteStateMachine::transitionTo(State& state){
	this->nextState = &state;
	this->stateChangeTime = this->deps->hardware->time();
	return *this;
}

FiniteStateMachine& FiniteStateMachine::immediateTransitionTo(State& state){
	this->currentState->exit(this);
	this->currentState = this->nextState = &state;
	this->currentState->enter(this);
	this->stateChangeTime = this->deps->hardware->time();
	return *this;
}

Deps* FiniteStateMachine::getDeps() {
	return this->deps;
}

//return the current state
State& FiniteStateMachine::getCurrentState() {
	return *this->currentState;
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
	return this->deps->hardware->time() - this->stateChangeTime;
}
//END FINITE STATE MACHINE