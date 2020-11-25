#include "hamsandwich.h"

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "amxxmodule.h"
#include "config_parser.h"
#include "ham_hook_callbacks.h"
#include "ham_hooklist.h"
#include "ham_hook.h"

#include <amtl/am-vector.h>

#define V(__KEYNAME, __STUFF__) 0, 0, __KEYNAME, false, RT_##__STUFF__, RB_##__STUFF__, PC_##__STUFF__
#define V_REMOVED(__KEYNAME) 0, 0, __KEYNAME, true, RT_Void_Void, RB_Void_Void, PC_Void_Void

ham_hook_t ham_hooklist[] =
{
	{ V("spawn",					Void_Void) },
	{ V("precache",					Void_Void) },
	{ V("keyvalue",					Void_Int) },
	{ V("objectcaps",				Int_Void) },
	{ V("activate",					Void_Void) },
	{ V("setobjectcollisionbox",	Void_Void) },
	{ V("classify",					Int_Void) },
	{ V("deathnotice",				Void_Entvar) },
	{ V("traceattack",				Void_Entvar_Float_Vector_Trace_Int) },
	{ V("takedamage",				Int_Entvar_Entvar_Float_Int) },
	{ V("takehealth",				Int_Float_Int) },
	{ V("killed",					Void_Entvar_Int) },
	{ V("bloodcolor",				Int_Void) },
	{ V("tracebleed",				Void_Float_Vector_Trace_Int) },
	{ V("istriggered",				Int_Cbase) },
	{ V("mymonsterpointer",			Cbase_Void) },
	{ V("mysquadmonsterpointer",	Cbase_Void) },
	{ V("gettogglestate",			Int_Void) },
	{ V("addpoints",				Void_Int_Int) },
	{ V("addpointstoteam",			Void_Int_Int) },
	{ V("addplayeritem",			Int_Cbase) },
	{ V("removeplayeritem",			Int_Cbase) },
	{ V("giveammo",					Int_Int_Str_Int) },
	{ V("getdelay",					Float_Void) },
	{ V("ismoving",					Int_Void) },
	{ V("overridereset",			Void_Void) },
	{ V("damagedecal",				Int_Int) },
	{ V("settogglestate",			Void_Int) },
	{ V("startsneaking",			Void_Void) },
	{ V("stopsneaking",				Void_Void) },
	{ V("oncontrols",				Int_Entvar) },
	{ V("issneaking",				Int_Void) },
	{ V("isalive",					Int_Void) },
	{ V("isbspmodel",				Int_Void) },
	{ V("reflectgauss",				Int_Void) },
	{ V("hastarget",				Int_Int) },
	{ V("isinworld",				Int_Void) },
	{ V("isplayer",					Int_Void) },
	{ V("isnetclient",				Int_Void) },
	{ V("teamid",					Str_Void) },
	{ V("getnexttarget",			Cbase_Void) },
	{ V("think",					Void_Void) },
	{ V("touch",					Void_Cbase) },
	{ V("use",						Void_Cbase_Cbase_Int_Float) },
	{ V("blocked",					Void_Cbase) },
	{ V("respawn",					Cbase_Void) },
	{ V("updateowner",				Void_Void) },
	{ V("fbecomeprone",				Int_Void) },
	{ V("center",					Vector_Void) },
	{ V("eyeposition",				Vector_Void) },
	{ V("earposition",				Vector_Void) },
	{ V("bodytarget",				Vector_pVector) },
	{ V("illumination",				Int_Void) },
	{ V("fvisible",					Int_Cbase) },
	{ V("fvecvisible",				Int_pVector) },

	/** Entity specific hooks **/

	/* CBasePlayer */
	{ V("player_jump",				Void_Void) },
	{ V("player_duck",				Void_Void) },
	{ V("player_prethink",			Void_Void) },
	{ V("player_postthink",			Void_Void) },
	{ V("player_getgunposition",	Vector_Void) },
	{ V("player_shouldfadeondeath",	Int_Void) },
	{ V("player_impulsecommands",	Void_Void) },
	{ V("player_updateclientdata",	Void_Void) },

	/* CBasePlayerItem */
	{ V("item_addtoplayer",			Int_Cbase) },
	{ V("item_addduplicate",		Int_Cbase) },
	{ V("item_candeploy",			Int_Void) },
	{ V("item_deploy",				Int_Void) },
	{ V("item_canholster",			Int_Void) },
	{ V("item_holster",				Void_Int) },
	{ V("item_updateiteminfo",		Void_Void) },
	{ V("item_preframe",			Void_Void) },
	{ V("item_postframe",			Void_Void) },
	{ V("item_drop",				Void_Void) },
	{ V("item_kill",				Void_Void) },
	{ V("item_attachtoplayer",		Void_Cbase) },
	{ V("item_primaryammoindex",	Int_Void) },
	{ V("item_secondaryammoindex",	Int_Void) },
	{ V("item_updateclientdata",	Int_Cbase) },
	{ V("item_getweaponptr",		Cbase_Void) },
	{ V("item_itemslot",			Int_Void) },

	/* CBasePlayerWeapon */
	{ V("weapon_extractammo",		Int_Cbase) },
	{ V("weapon_extractclipammo",	Int_Cbase) },
	{ V("weapon_addweapon",			Int_Void) },
	{ V("weapon_playemptysound",	Int_Void) },
	{ V("weapon_resetemptysound",	Void_Void) },
	{ V("weapon_sendweaponanim",	Void_Int_Int_Int) },
	{ V("weapon_isusable",			Int_Void) },
	{ V("weapon_primaryattack",		Void_Void) },
	{ V("weapon_secondaryattack",	Void_Void) },
	{ V("weapon_reload",			Void_Void) },
	{ V("weapon_weaponidle",		Void_Void) },
	{ V("weapon_retireweapon",		Void_Void) },
	{ V("weapon_shouldweaponidle",	Int_Void) },
	{ V("weapon_usedecrement",		Int_Void) },

	/** Mod specific hooks **/

	/* The Specialists */
	/*
	{ V("ts_breakablerespawn",		Int_Int) },
	{ V("ts_canusedthroughwalls",	Int_Void) },
	{ V("ts_respawnwait",			Deprecated) },
	*/

	/* Counter-Strike */
	{ V("cstrike_restart",			Void_Void) },
	{ V("cstrike_roundrespawn",		Void_Void) },
	{ V("cstrike_item_candrop",		Int_Void) },
	{ V("cstrike_item_getmaxspeed",	Float_Void) },
	
	/*
	// Day of Defeat
	{ V("dod_roundrespawn",			Void_Void) },
	{ V("dod_roundrespawnent",		Void_Void) },
	{ V("dod_roundstore",			Void_Void) },
	{ V("dod_areasetindex",			Void_Int) },
	{ V("dod_areasendstatus",		Void_Cbase) },
	{ V("dod_getstate",				Int_Void) },
	{ V("dod_getstateent",			Int_Cbase) },
	{ V("dod_item_candrop",			Int_Void) },

	// Team Fortress Classic
	{ V("tfc_engineeruse",			Int_Cbase) },
	{ V("tfc_finished",				Void_Void) },
	{ V("tfc_empexplode",			Void_Entvar_Float_Float) },
	{ V("tfc_calcempdmgrad",		Void_pFloat_pFloat) },
	{ V("tfc_takeempblast",			Void_Entvar) },
	{ V("tfc_empremove",			Void_Void) },
	{ V("tfc_takeconcussionblast",	Void_Entvar_Float) },
	{ V("tfc_concuss",				Void_Entvar) },

	// Earth's Special Forces
	/*
	{ V("esf_isenvmodel",			Int_Void) },
	{ V("esf_takedamage2",			Int_Entvar_Entvar_Float_Float_Int) },
	
	// Natural-Selection
	{ V("ns_getpointvalue",			Int_Void) },
	{ V("ns_awardkill",				Void_Entvar) },
	{ V("ns_resetentity",			Void_Void) },
	{ V("ns_updateonremove",		Void_Void) },

	{ V("ts_giveslowmul",			Void_Void) },
	{ V("ts_goslow",				Void_Float_Int) },
	{ V("ts_inslow",				Int_Void) },
	{ V("ts_isobjective",			Int_Void) },
	{ V("ts_enableobjective",		Void_Int) },
	{ V("ts_onfreeentprivatedata",	Void_Void) },
	{ V("ts_shouldcollide",			Int_Cbase) },
	*/

	/* New Additions (2011) */
	{ V("changeyaw",				Float_Int) },
	{ V("hashumangibs",				Int_Void) },
	{ V("hasaliengibs",				Int_Void) },
	{ V("fademonster",				Void_Void) },
	{ V("gibmonster",				Void_Void) },
	{ V("becomedead",				Void_Void) },
	{ V("irelationship",			Int_Cbase) },
	{ V("painsound",				Void_Void) },
	{ V("reportaistate",			Void_Void) },
	{ V("monsterinitdead",			Void_Void) },
	{ V("look",						Void_Int) },
	{ V("bestvisibleenemy",			Cbase_Void) },
	{ V("finviewcone",				Int_Cbase) },
	{ V("fvecinviewcone",			Int_pVector) },
	{ V("getdeathactivity",			Int_Void) },

	/* Not supported by Counter-Strike, The Specialists and Natural Selection mods. */
	{ V("runai",					Void_Void) },
	{ V("monsterthink",				Void_Void) },
	{ V("monsterinit",				Void_Void) },
	{ V("checklocalmove",			Int_pVector_pVector_Cbase_pFloat) },
	{ V("move",						Void_Float) },
	{ V("moveexecute",				Void_Cbase_pVector_Float) },
	{ V("shouldadvanceroute",		Int_Float) },
	{ V("getstoppedactivity",		Int_Void) },
	{ V("stop",						Void_Void) },
	{ V("checkrangeattack1",		Int_Float_Float) },
	{ V("checkrangeattack2",		Int_Float_Float) },
	{ V("checkmeleeattack1",		Int_Float_Float) },
	{ V("checkmeleeattack2",		Int_Float_Float) },
	{ V("schedulechange",			Void_Void) },
	{ V("canplaysequence",			Int_Int_Int) },
	{ V("canplaysentence2",			Int_Int) },
	{ V("playsentence",				Void_Str_Float_Float_Float) },
	{ V("playscriptedsentence",		Void_Str_Float_Float_Float_Int_Cbase) },
	{ V("sentencestop",				Void_Void) },
	{ V("getidealstate",			Int_Void) },
	{ V("setactivity",				Void_Int) },
	{ V("checkenemy",				Int_Cbase) },
	{ V("ftriangulate",				Int_pVector_pVector_Float_Cbase_pVector) },
	{ V("setyawspeed",				Void_Void) },
	{ V("buildnearestroute",		Int_Vector_Vector_Float_Float) },
	{ V("findcover",				Int_Vector_Vector_Float_Float) },
	{ V("coverradius",				Float_Void) },
	{ V("fcancheckattacks",			Int_Void) },
	{ V("checkammo",				Void_Void) },
	{ V("ignoreconditions",			Int_Void) },
	{ V("fvalidatehinttype",		Int_Short) },
	{ V("fcanactiveidle",			Int_Void) },
	{ V("isoundmask",				Int_Void) },
	{ V("hearingsensitivity",		Float_Void) },
	{ V("barnaclevictimbitten",		Void_Entvar) },
	{ V("barnaclevictimreleased",	Void_Void) },
	{ V("preschedulethink",			Void_Void) },
	{ V("deathsound",				Void_Void) },
	{ V("alertsound",				Void_Void) },
	{ V("idlesound",				Void_Void) },
	{ V("stopfollowing",			Void_Int) },

	/** Mod specific hooks **/

	/* Counter-Strike */
	{ V("cstrike_weapon_sendweaponanim",Void_Int_Int) },
	{ V("cstrike_player_resetmaxspeed",	Void_Void) },
	{ V("cstrike_player_isbot",		Int_Void) },
	{ V("cstrike_player_getautoaimvector",		Vector_Float) },
	{ V("cstrike_player_blind",		Void_Float_Float_Float_Int) },
	{ V("cstrike_player_ontouchingweapon",Void_Cbase) },

	/*
	// Day of Defeat
	{ V("dod_setscriptreset",		Void_Void) },
	{ V("dod_item_spawndeploy",		Int_Void) },
	{ V("dod_item_setdmgtime",		Void_Float) },
	{ V("dod_item_dropgren",		Void_Void) },
	{ V("dod_weapon_isuseable",		Int_Void) },
	{ V("dod_weapon_aim",			Vector_Float_Cbase_Int) },
	{ V("dod_weapon_flaim",			Float_Float_Cbase) },
	{ V("dod_weapon_removestamina", Void_Float_Cbase) },
	{ V("dod_weapon_changefov",		Int_Int) },
	{ V("dod_weapon_zoomout",		Int_Void) },
	{ V("dod_weapon_zoomin",		Int_Void) },
	{ V("dod_weapon_getfov",		Int_Void) },
	{ V("dod_weapon_playeriswatersniping", Bool_Void) },
	{ V("dod_weapon_updatezoomspeed", Void_Void) },
	{ V("dod_weapon_special",		Void_Void) },

	// Team Fortress Classic
	{ V("tfc_dbgetitemname",		Str_Void) },
	{ V("tfc_radiusdamage",			Void_Entvar_Entvar_Float_Int_Int) },
	{ V("tfc_radiusdamage2",		Void_Vector_Entvar_Entvar_Float_Int_Int) },

	// Earth's Special Forces
	{ V("esf_isfighter",			Int_Void) },
	{ V("esf_isbuddy",				Int_Void) },
	{ V("esf_emitsound",			Void_Str_Int) },
	{ V("esf_emitnullsound",		Void_Int) },
	{ V("esf_increasestrength",		Void_Cbase_Int) },
	{ V("esf_increasepl",			Void_Int) },
	{ V("esf_setpowerlevel",		Void_Int) },
	{ V("esf_setmaxpowerlevel",		Void_Int) },
	{ V("esf_stopanitrigger",		Void_Int) },
	{ V("esf_stopfly",				Void_Void) },
	{ V("esf_hideweapon",			Void_Void) },
	{ V("esf_clientremoveweapon",	Void_Int) },
	{ V("esf_sendclientcustommodel",Void_Str) },
	{ V("esf_canturbo",				Int_Void) },
	{ V("esf_canprimaryfire",		Int_Void) },
	{ V("esf_cansecondaryfire",		Int_Void) },
	{ V("esf_canstopfly",			Int_Void) },
	{ V("esf_canblock",				Int_Void) },
	{ V("esf_canraiseKi",			Int_Void) },
	{ V("esf_canraisestamina",		Int_Void) },
	{ V("esf_canteleport",			Int_Void) },
	{ V("esf_canstartfly",			Int_Void) },
	{ V("esf_canstartpowerup",		Int_Void) },
	{ V("esf_canjump",				Int_Void) },
	{ V("esf_canwalljump",			Int_Void) },
	{ V("esf_issuperjump",			Int_Void) },
	{ V("esf_ismoveback",			Int_Void) },
	{ V("esf_checkwalljump",		Int_Void) },
	{ V("esf_enablewalljump",		Void_Vector) },
	{ V("esf_disablewalljump",		Void_Void) },
	{ V("esf_resetwalljumpvars",	Void_Void) },
	{ V("esf_getwalljumpanim",		Int_Str_Vector_Str) },
	{ V("esf_getwalljumpanim2",		Int_Str_Str) },
	{ V("esf_setwalljumpanimation", Void_Void) },
	{ V("esf_setflymovetype",		Void_Void) },
	{ V("esf_isflymovetype",		Int_Void) },
	{ V("esf_iswalkmovetype",		Int_Void) },
	{ V("esf_setwalkmovetype",		Void_Void) },
	{ V("esf_drawchargebar",		Void_Int) },
	{ V("esf_startblock",			Void_Void) },
	{ V("esf_stopblock",			Void_Void) },
	{ V("esf_startfly",				Void_Void) },
	{ V("esf_getmaxspeed",			Float_Void) },
	{ V("esf_setanimation",			Void_Int) },
	{ V("esf_playanimation",		Void_Void) },
	{ V("esf_getmoveforward",		Int_Void) },
	{ V("esf_getmoveright",			Int_Void) },
	{ V("esf_getmoveup",			Void_Void) },
	{ V("esf_addblindfx",			Void_Void) },
	{ V("esf_removeblindfx",		Void_Void) },
	{ V("esf_disablepsbar",			Void_Void) },
	{ V("esf_addbeamboxcrosshair",	Void_Int) },
	{ V("esf_removebeamboxcrosshair", Void_Void) },
	{ V("esf_drawpswinbonus",		Void_Void) },
	{ V("esf_drawpsbar",			Void_Float_Float) },
	{ V("esf_lockcrosshair",		Void_Void) },
	{ V("esf_unlockcrosshair",		Void_Void) },
	{ V("esf_rotatecrosshair",		Void_Void) },
	{ V("esf_unrotatecrosshair",	Void_Void) },
	{ V("esf_watermove",			Void_Void) },
	{ V("esf_checktimebaseddamage", Void_Void) },
	{ V("esf_doessecondaryattack",	Int_Void) },
	{ V("esf_doesprimaryattack",	Int_Void) },
	{ V("esf_removespecialmodes",	Void_Void) },
	{ V("esf_stopturbo",			Void_Void) },
	{ V("esf_takebean",				Void_Void) },
	{ V("esf_getpowerlevel",		Void_Void) },
	{ V("esf_removeallotherweapons",Void_Void) },
	{ V("esf_stopswoop",			Void_Void) },
	{ V("esf_setdeathanimation",	Void_Void) },
	{ V("esf_setmodel",				Void_Void) },
	{ V("esf_addattacks",			Void_Void) },
	{ V("esf_emitclasssound",		Void_Str_Str_Int) },
	{ V("esf_checklightning",		Void_Void) },
	{ V("esf_freezecontrols",		Void_Void) },
	{ V("esf_unfreezecontrols",		Void_Void) },
	{ V("esf_updateki",				Void_Void) },
	{ V("esf_updatehealth",			Void_Void) },
	{ V("esf_getteleportdir",		Vector_Void) },
	{ V("esf_weapon_holsterwhenmeleed",	Void_Void) },

	// Natural-Selection
	{ V("ns_setbonecontroller",		Float_Int_Float) },
	{ V("ns_savedataforreset",		Void_Void) },
	{ V("ns_gethull",				Int_Void) },
	{ V("ns_getmaxwalkspeed",		Float_Void) },
	{ V("ns_setteamid",				Str_Str) },
	{ V("ns_geteffectiveplayerclass", Int_Void) },
	{ V("ns_getauthenticationmask", Int_Void) },
	{ V("ns_effectiveplayerclasschanged", Void_Void) },
	{ V("ns_needsteamupdate",		Void_Void) },
	{ V("ns_sendteamupdate",		Void_Void) },
	{ V("ns_sendweaponupdate",		Void_Void) },
	{ V("ns_initplayerfromspawn",	Void_Edict) },
	{ V("ns_packdeadplayeritems",	Void_Void) },
	{ V("ns_getanimationforactivity",Void_Int_Str_Bool) },
	{ V("ns_startobserver",			Void_Vector_Vector) },
	{ V("ns_stopobserver",			Void_Void) },
	{ V("ns_getadrenalinefactor",	Float_Void) },
	{ V("ns_givenameditem",			Void_Str_Bool) },
	{ V("ns_suicide",				Void_Void) },
	{ V("ns_getcanuseweapon",		Int_Void) },
	{ V("ns_weapon_getweaponprimetime",	Float_Void) },
	{ V("ns_weapon_primeweapon",		Void_Void) },
	{ V("ns_weapon_getisweaponprimed",	Int_Void) },
	{ V("ns_weapon_getisweaponpriming",	Int_Void) },
	{ V("ns_weapon_defaultdeploy",		Int_Str_Str_Int_Str_Int_Int) },
	{ V("ns_weapon_defaultreload",		Int_Int_Int_Float_Int) },
	{ V("ns_weapon_getdeploytime",		Float_Void) },

	// Sven co-op
	{ V("sc_getclassification",		Int_Int) },
	{ V("sc_ismonster",				Bool_Void) },
	{ V_REMOVED("sc_isphysx") },
	{ V("sc_ispointentity",			Bool_Void) },
	{ V("sc_ismachine",				Bool_Void) },
	{ V("sc_criticalremove",		Bool_Void) },
	{ V("sc_updateonremove",		Void_Void) },
	{ V("sc_fvisible",				Bool_Cbase_Bool) },
	{ V("sc_fvisiblefrompos",		Bool_pVector_pVector) },
	{ V("sc_isfacing",				Bool_Entvar_Float) },
	{ V("sc_getpointsfordamage",	Float_Float) },
	{ V("sc_getdamagepoints",		Void_Entvar_Entvar_Float) },
	{ V("sc_oncreate",				Void_Void) },
	{ V("sc_ondestroy",				Void_Void) },
	{ V_REMOVED("sc_isvalidentity") },
	{ V("sc_shouldfadeondeath",		Int_Void) },
	{ V("sc_setupfriendly",			Void_Void) },
	{ V_REMOVED("sc_revivethink") },
	{ V("sc_revive",				Void_Void) },
	{ V("sc_startmonster",			Void_Void) },
	{ V("sc_checkrangeattack1_move",Int_Float_Float) },
	{ V("sc_checkrangeattack2_move",Int_Float_Float) },
	{ V("sc_checkmeleeattack1_move",Int_Float_Float) },
	{ V("sc_checkmeleeattack2_move",Int_Float_Float) },
	{ V("sc_checktankusage",		Int_Void) },
	{ V("sc_setgaitactivity",		Int_Void) },
	{ V("sc_ftriangulate",			Int_pVector_pVector_Float_Cbase_pVector_pVector_Bool) },
	{ V("sc_ftriangulateextension",	Int_pVector_pVector_Float_Cbase_pVector) },
	{ V("sc_findcovergrenade",		Int_Vector_Vector_Float_Float) },
	{ V("sc_findcoverdistance",		Int_Vector_Vector_Float_Float) },
	{ V("sc_findattackpoint",		Int_Vector_Vector_Float_Float) },
	{ V("sc_fvalidatecover",		Int_pVector) },
	{ V("sc_nofriendlyfire1",		Int_Void) },
	{ V("sc_nofriendlyfire2",		Int_Vector) },
	{ V("sc_nofriendlyfire3",		Int_Vector_Cbase) },
	{ V("sc_nofriendlyfiretopos",	Int_Vector) },
	{ V("sc_fvisiblegunpos",		Int_Cbase_pVector) },
	{ V("sc_finbulletcone",			Int_Cbase_pVector) },
	{ V("sc_callgibmonster",		Void_Void) },
	{ V("sc_checktimebaseddamage",	Void_Void) },
	{ V("sc_ismoving",				Bool_Void) },
	{ V("sc_isplayerfollowing",		Int_Void) },
	{ V("sc_startplayerfollowing",	Void_Cbase_Bool) },
	{ V("sc_stopplayerfollowing",	Void_Bool_Bool) },
	{ V("sc_usesound",				Void_Void) },
	{ V("sc_unusesound",			Void_Void) },
	{ V("sc_ridemonster",			Void_Cbase) },
	{ V("sc_checkandapplygenericattacks",	Void_Void) },
	{ V("sc_checkscared",			Bool_Void) },
	{ V("sc_checkcreaturedanger",	Void_Void) },
	{ V("sc_checkfalldamage",		Void_Void) },
	{ V("sc_checkrevival",			Void_Void) },
	{ V("sc_mediccallsound",		Void_Void) },

	{ V_REMOVED("sc_player_menuinputperformed") },
	{ V_REMOVED("sc_player_ismenuinputdone") },
	{ V("sc_player_specialspawn",	Void_Void) },
	{ V("sc_player_isvalidinfoentity",	Bool_Void) },
	{ V("sc_player_levelend",		Void_Void) },
	{ V("sc_player_votestarted",	Void_Int) },
	{ V("sc_player_canstartnextvote",	Bool_Int) },
	{ V("sc_player_vote",			Void_Int) },
	{ V("sc_player_hasvoted",		Bool_Void) },
	{ V("sc_player_resetvote",		Void_Void) },
	{ V("sc_player_lastvoteinput",	Int_Void) },
	{ V("sc_player_initvote",		Void_Void) },
	{ V("sc_player_timetostartnextvote", Float_Void) },
	{ V("sc_player_resetview",		Void_Void) },
	{ V("sc_player_getlogfrequency",Float_Void) },
	{ V("sc_player_logplayerstats",	Bool_Void) },
	{ V_REMOVED("sc_player_disablecollisionwithplayer") },
	{ V_REMOVED("sc_player_enablecollisionwithplayer") },
	{ V_REMOVED("sc_player_cantouchplayer") },

	{ V("sc_item_materialize",		Void_Void) },

	{ V("sc_weapon_bulletaccuracy",	Vector_Vector_Vector_Vector) },
	{ V("sc_weapon_tertiaryattack",	Void_Void) },
	{ V("sc_weapon_burstsupplement",Void_Void) },
	{ V("sc_weapon_getp_model",		Str_Str) },
	{ V("sc_weapon_getw_model",		Str_Str) },
	{ V("sc_weapon_getv_model",		Str_Str) },
	{ V("sc_weapon_precachecustommodels",	Void_Void) },
	{ V("sc_weapon_ismultiplayer",	Bool_Void) },
	{ V("sc_weapon_frunfuncs",		Bool_Void) },
	{ V("sc_weapon_setfov",			Void_Int) },
	{ V("sc_weapon_fcanrun",		Bool_Void) },
	{ V("sc_weapon_customdecrement",Void_Float) },
	{ V("sc_weapon_setv_model",		Void_Str) },
	{ V("sc_weapon_setp_model",		Void_Str) },
	{ V("sc_weapon_changeweaponskin",Void_Short) },

	// New Additions (2013)

	{ V("tfc_killed",Void_Entvar_Entvar_Int) },
	{ V("tfc_istriggered", Int_Void) },
	{ V("tfc_weapon_sendweaponanim", Void_Int_Int) },
	{ V("tfc_weapon_getnextattackdelay", Float_Float) },

	{ V("sc_takehealth",Bool_Float_Int_Int) },
	{ V("sc_takearmor", Bool_Float_Int_Int) },
	{ V("sc_giveammo", Int_Int_Str_Int_Bool) },
	{ V("sc_checkattacker", Int_Cbase) },
	{ V("sc_player_isconnected", Bool_Void) },

	{ V("dod_weapon_sendweaponanim", Void_Int_Int) },
	*/
	{ V("cstrike_item_isweapon", Int_Void) },

	/*
	{ V("gearbox_mysquadtalkmonsterpointer", Cbase_Void) },
	{ V("gearbox_weapontimebase", Float_Void) },

	{ V("ts_weapon_alternateattack", Void_Void) },
*/
	{ V("item_getiteminfo", Int_ItemInfo) },
/*
	{ V("sc_prespawn",				Void_Void) },
	{ V("sc_postspawn",				Void_Void) },
	{ V("sc_onkeyvalueupdate",		Void_Str) },
	{ V("sc_setclassification",		Void_Int) },
	{ V("sc_istriggered",			Bool_Void) },
	{ V("sc_mycustompointer",		Cbase_Void) },
	{ V("sc_myitempointer",			Cbase_Void) },
	{ V("sc_addpoints",				Void_Int_Bool) },
	{ V("sc_addpointstoteam",		Void_Int_Bool) },
	{ V("sc_removeplayeritem",		Bool_Cbase) },
	{ V("sc_oncontrols",			Bool_Entvar) },
	{ V("sc_issneaking",			Bool_Void) },
	{ V("sc_isalive",				Bool_Void) },
	{ V("sc_isbspmodel",			Bool_Void) },
	{ V("sc_reflectgauss",			Bool_Void) },
	{ V("sc_hastarget",				Bool_Int) },
	{ V("sc_isinworld",				Bool_Void) },
	{ V("sc_isplayer",				Bool_Void) },
	{ V("sc_isnetclient",			Bool_Void) },
	{ V("sc_isbreakable",			Bool_Void) },
	{ V("sc_subusetargets",			Void_Cbase_Int_Float) },
	{ V("sc_islockedbymaster",		Bool_Void) },
	{ V("sc_fbecomeprone",			Bool_Cbase) },
	{ V("sc_fvecvisible",			Bool_pVector) },
	{ V("sc_setplayerally",			Void_Bool) },
	{ V("sc_onsetoriginbymap",		Void_Void) },
	{ V("sc_isrevivable",			Bool_Void) },
	{ V("sc_beginrevive",			Void_Float) },
	{ V("sc_endrevive",				Void_Float) },
	{ V("sc_canplaysequence",		Bool_Bool_Int) },
	{ V("sc_canplaysentence2",		Bool_Bool) },
	{ V("sc_playscriptedsentence",	Void_Str_Float_Float_Float_Bool_Cbase) },
	{ V("sc_item_addtoplayer",		Bool_Cbase) },
	{ V("sc_item_addduplicate",		Bool_Cbase) },
	{ V("sc_item_addammofromitem",	Bool_Cbase) },
	{ V("sc_item_getpickupsound",	Str_Void) },
	{ V("sc_item_cancollect",		Bool_Cbase_Int) },
	{ V("sc_item_collect",			Void_Cbase_Int) },
	{ V("sc_item_getiteminfo",		Bool_ItemInfo) },
	{ V("sc_item_candeploy",		Bool_Void) },
	{ V("sc_item_deploy",			Bool_Void) },
	{ V("sc_item_canholster",		Bool_Void) },
	{ V("sc_item_inactiveitempreframe",	Void_Void) },
	{ V("sc_item_inactiveitempostframe",Void_Void) },
	{ V("sc_item_detachfromplayer",	Void_Void) },
	{ V("sc_item_updateclientdata", Bool_Cbase) },
	{ V("sc_item_getrespawntime",	Float_Void) },
	{ V("sc_item_canhaveduplicates",Bool_Void) },
	{ V("sc_weapon_extractammofromitem",Bool_Cbase) },
	{ V("sc_weapon_addweapon",		Bool_Void) },
	{ V("sc_weapon_getammo1drop",	Int_Void) },
	{ V("sc_weapon_getammo2drop",	Int_Void) },
	{ V("sc_weapon_playemptysound",	Bool_Void) },
	{ V("sc_weapon_isusable",		Bool_Void) },
	{ V("sc_weapon_finishreload",	Void_Void) },
	{ V("sc_weapon_shouldreload",	Bool_Void) },
	{ V("sc_weapon_shouldweaponidle",Bool_Void) },
	{ V("sc_weapon_usedecrement",	Bool_Void) },
	{ V("sc_player_enteredobserver",Void_Void) },
	{ V("sc_player_leftobserver",	Void_Void) },
	{ V("sc_player_isobserver",		Bool_Void) },
	*/
};

static ke::Vector<HamHook*> s_Hooks[HAM_LAST_ENTRY_DONT_USE_ME_LOL];

inline void** EdictToVTable(edict_t* ent)
{
	char* btbl = (char*)ent->pvPrivateData;
	btbl += GetOffset().GetBase();
	return *((void***)btbl);
};

inline void** GetVTable(void* pthis, int size)
{
	return *((void***)(((char*)pthis) + size));
}

inline void* GetVTableEntry(void* pthis, int ventry, int size)
{
	void** vtbl = GetVTable(pthis, size);

	return vtbl[ventry];
}

void* GetFunction(void* pthis, int id, bool& istramp)
{
	istramp = false;
	void* func = GetVTableEntry(pthis, ham_hooklist[id].vtid, GetOffset().GetBase());

	// Check to see if it's a trampoline
	for (size_t i = 0; i < s_Hooks[id].length(); ++i)
	{
		if (func == s_Hooks[id].at(i)->tramp)
		{
			istramp = true;
			return func;
		}
	}

	return func;
}

void* GetHamFunction(void* pthis, int id)
{
	void** vtbl = GetVTable(pthis, GetOffset().GetBase());

	int** ivtbl = (int**)vtbl;
	void* func = ivtbl[ham_hooklist[id].vtid];

	// Iterate through the hooks for the id, see if the function is found
	for (size_t i = 0; i < s_Hooks[id].length(); ++i)
	{
		// If the function points to a trampoline, then return the original
		// function.
		if (func == s_Hooks[id].at(i)->tramp)
		{
			return s_Hooks[id].at(i)->func;
		}
	}

	// this is an original function
	return func;
}

int RegisterHam(int func, const char* classname, void* targetfunc)
{
	if (!ham_hooklist[func].isset)
	{
		MF_Log("Failed to retrieve vtable id for \"%s\".", ham_hooklist[func].name);
		return 0;
	}

	edict_t* pEntity = CREATE_ENTITY();
	CALL_GAME_ENTITY(PLID, classname, &pEntity->v);

	if (pEntity->pvPrivateData == NULL)
	{
		REMOVE_ENTITY(pEntity);
		MF_Log("Failed to retrieve classtype for \"%s\".", classname);
		return 0;
	}

	void** vtable = GetVTable(pEntity->pvPrivateData, GetOffset().GetBase());
	REMOVE_ENTITY(pEntity);

	if (vtable == NULL)
	{
		MF_Log("Failed to retrieve vtable for \"%s\".", classname);
		return 0;
	}


	int** ivtable = (int**)vtable;
	void* vfunction = (void*)ivtable[ham_hooklist[func].vtid];

	// Check the list of this function's hooks, see if the function we have is a hook

	for (size_t i = 0; i < s_Hooks[func].length(); ++i)
	{
		if (s_Hooks[func].at(i)->tramp == vfunction)
		{
			return 1;
		}
	}

	HamHook* pHook = new HamHook(vtable, ham_hooklist[func].vtid, targetfunc, ham_hooklist[func].isvoid, ham_hooklist[func].needsretbuf, ham_hooklist[func].paramcount, classname);
	s_Hooks[func].append(pHook);

	return 1;
}

void ClearHamHooks()
{
	for (size_t i = 0; i < HAM_LAST_ENTRY_DONT_USE_ME_LOL; i++)
	{
		for (size_t j = 0; j < s_Hooks[i].length(); ++j)
		{
			delete s_Hooks[i].at(j);
		}
		s_Hooks[i].clear();
	}
}

std::stack<HamHookData>& GetHamHookData()
{
	static std::stack<HamHookData> s_stack;
	return s_stack;
}