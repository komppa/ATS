#ifndef ESTATES_H
#define ESTATES_H

 
enum States {

	// ATS FSM states
	UNKNOWNSTART,
	NORMAL,
	NORMAL2,
	STABILITY,
	WAITGEN,
	WARMUP,
	SWITCHDELAYTOGEN,
	SWITCHTOGEN,
	SWITCHDELAYTOGRID,
	DETACHGEN,

	// Display FSM states
	DISPLAYUNKNOWNSTART,
	DISPLAYSTART,
	SETTINGSSTART,
	SETTINGSMANUALDRIVE,
	SETTINGSSTABILITYTIME,
	SETTINGSSWITCHINGDELAY,
	SETTINGSWARMUPTIME,
	SETTINGSINPUT

	// TODO Writer states
	// ....

};

#endif // ESTATES_H