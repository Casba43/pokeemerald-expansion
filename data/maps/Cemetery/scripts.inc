Cemetery_MapScripts::
	map_script MAP_SCRIPT_ON_FRAME_TABLE, Cemetery_OnFrame
	.byte 0

Cemetery_OnFrame:
	map_script_2 VAR_CEMETERY_STATE, 0, Cemetery_EventScript_Intro
	.2byte 0

@ Runs once, the moment a new game drops the player in front of the shrine.
Cemetery_EventScript_Intro::
	lockall
	setvar VAR_CEMETERY_STATE, 1
	call Cemetery_EventScript_SetIntroFlags
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	delay 40
	msgbox Cemetery_Text_Intro, MSGBOX_DEFAULT
	closemessage
	delay 20
	msgbox Cemetery_Text_ThreeBallsWait, MSGBOX_DEFAULT
	closemessage
	releaseall
	end

@ The intro state the moving-truck scene would normally have set up, so that
@ Littleroot Town and the player's house still behave once the player leaves.
Cemetery_EventScript_SetIntroFlags::
	checkplayergender
	call_if_eq VAR_RESULT, MALE, Cemetery_EventScript_SetIntroFlagsMale
	call_if_eq VAR_RESULT, FEMALE, Cemetery_EventScript_SetIntroFlagsFemale
	return

Cemetery_EventScript_SetIntroFlagsMale::
	setflag FLAG_HIDE_LITTLEROOT_TOWN_PLAYERS_HOUSE_VIGOROTH_1
	setflag FLAG_HIDE_LITTLEROOT_TOWN_PLAYERS_HOUSE_VIGOROTH_2
	setflag FLAG_HIDE_LITTLEROOT_TOWN_BRENDANS_HOUSE_TRUCK
	setflag FLAG_HIDE_LITTLEROOT_TOWN_MAYS_HOUSE_TRUCK
	setrespawn HEAL_LOCATION_LITTLEROOT_TOWN_BRENDANS_HOUSE_2F
	setvar VAR_LITTLEROOT_INTRO_STATE, 1
	setflag FLAG_HIDE_LITTLEROOT_TOWN_MAYS_HOUSE_MOM
	setflag FLAG_HIDE_LITTLEROOT_TOWN_MAYS_HOUSE_TRUCK
	setflag FLAG_HIDE_LITTLEROOT_TOWN_BRENDANS_HOUSE_RIVAL_MOM
	setflag FLAG_HIDE_LITTLEROOT_TOWN_BRENDANS_HOUSE_RIVAL_SIBLING
	setflag FLAG_HIDE_LITTLEROOT_TOWN_BRENDANS_HOUSE_2F_POKE_BALL
	setvar VAR_LITTLEROOT_HOUSES_STATE_BRENDAN, 1
	setdynamicwarp MAP_LITTLEROOT_TOWN, 3, 10
	return

Cemetery_EventScript_SetIntroFlagsFemale::
	setflag FLAG_HIDE_LITTLEROOT_TOWN_PLAYERS_HOUSE_VIGOROTH_1
	setflag FLAG_HIDE_LITTLEROOT_TOWN_PLAYERS_HOUSE_VIGOROTH_2
	setflag FLAG_HIDE_LITTLEROOT_TOWN_BRENDANS_HOUSE_TRUCK
	setflag FLAG_HIDE_LITTLEROOT_TOWN_MAYS_HOUSE_TRUCK
	setrespawn HEAL_LOCATION_LITTLEROOT_TOWN_MAYS_HOUSE_2F
	setvar VAR_LITTLEROOT_INTRO_STATE, 2
	setflag FLAG_HIDE_LITTLEROOT_TOWN_BRENDANS_HOUSE_MOM
	setflag FLAG_HIDE_LITTLEROOT_TOWN_BRENDANS_HOUSE_TRUCK
	setflag FLAG_HIDE_LITTLEROOT_TOWN_MAYS_HOUSE_RIVAL_MOM
	setflag FLAG_HIDE_LITTLEROOT_TOWN_MAYS_HOUSE_RIVAL_SIBLING
	setflag FLAG_HIDE_LITTLEROOT_TOWN_MAYS_HOUSE_2F_POKE_BALL
	setvar VAR_LITTLEROOT_HOUSES_STATE_MAY, 1
	setdynamicwarp MAP_LITTLEROOT_TOWN, 12, 10
	return

@ All three Poké Balls open the same selection screen; the other two are taken
@ away once a choice has been made.
Cemetery_EventScript_StarterBall::
	lock
	msgbox Cemetery_Text_BallsLaidBeforeShrine, MSGBOX_DEFAULT
	closemessage
	setflag FLAG_SYS_POKEMON_GET
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_CEMETERY_STARTER_BALL_LEFT
	removeobject LOCALID_CEMETERY_STARTER_BALL_MIDDLE
	removeobject LOCALID_CEMETERY_STARTER_BALL_RIGHT
	setflag FLAG_HIDE_CEMETERY_STARTER_BALLS
	special ChooseStarterNoBattle
	delay 30
	setvar VAR_CEMETERY_STATE, 2
	clearflag FLAG_HIDE_CEMETERY_KEEPER
	call Cemetery_EventScript_SyncStoryState
	msgbox Cemetery_Text_TookStarter, MSGBOX_DEFAULT
	closemessage
	goto Cemetery_EventScript_Ambush
	end

@ Whatever left the Poke Balls here does not intend to let you walk off with one.
@ Duskull is held at Lv 3 on purpose: it learns Disable at Lv 4, which could lock
@ the single damaging move a Lv 5 starter has.
Cemetery_EventScript_Ambush::
	delay 40
	playse SE_M_CONFUSE_RAY
	msgbox Cemetery_Text_SomethingRises, MSGBOX_DEFAULT
	closemessage
	playmoncry SPECIES_DUSKULL, CRY_MODE_ENCOUNTER
	delay 40
	waitmoncry
	setwildbattle SPECIES_DUSKULL, 3
	dowildbattle
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, Cemetery_EventScript_AmbushFled
	msgbox Cemetery_Text_AmbushOver, MSGBOX_DEFAULT
	closemessage
	release
	end

Cemetery_EventScript_AmbushFled::
	msgbox Cemetery_Text_AmbushFled, MSGBOX_DEFAULT
	closemessage
	release
	end

@ Puts the rest of the game where Route 101's rescue scene would have left it,
@ so Birch, his lab and the Pokédex hand-off still work normally.
Cemetery_EventScript_SyncStoryState::
	setflag FLAG_RESCUED_BIRCH
	setflag FLAG_HIDE_ROUTE_101_BIRCH_ZIGZAGOON_BATTLE
	setflag FLAG_HIDE_ROUTE_101_BIRCH_STARTERS_BAG
	setflag FLAG_HIDE_ROUTE_101_ZIGZAGOON
	clearflag FLAG_HIDE_LITTLEROOT_TOWN_BIRCHS_LAB_BIRCH
	setvar VAR_ROUTE101_STATE, 3
	setvar VAR_BIRCH_LAB_STATE, 2
	@ Littleroot's north exit has coord events for VAR_LITTLEROOT_TOWN_STATE 0
	@ ("you need a POKéMON") and 1 ("go see what is happening"). Both order the
	@ Twin to walk over and push the player back, but she is only moved to the
	@ exit by SetTwinPos, which is skipped once FLAG_RESCUED_BIRCH is set. The
	@ result was an invisible NPC blocking the way out of town, so put the town
	@ past every one of those gates.
	setvar VAR_LITTLEROOT_TOWN_STATE, 4
	@ State 4 means the running shoes were already given, and that scene needs
	@ Mom outside for a moment that no longer happens, so grant them here.
	setflag FLAG_SYS_B_DASH
	checkplayergender
	call_if_eq VAR_RESULT, MALE, Cemetery_EventScript_HideMayInBedroom
	call_if_eq VAR_RESULT, FEMALE, Cemetery_EventScript_HideBrendanInBedroom
	return

Cemetery_EventScript_HideMayInBedroom::
	setflag FLAG_HIDE_LITTLEROOT_TOWN_MAYS_HOUSE_RIVAL_BEDROOM
	return

Cemetery_EventScript_HideBrendanInBedroom::
	setflag FLAG_HIDE_LITTLEROOT_TOWN_BRENDANS_HOUSE_RIVAL_BEDROOM
	return

Cemetery_EventScript_Shrine::
	msgbox Cemetery_Text_Shrine, MSGBOX_SIGN
	end

Cemetery_EventScript_BlockGate::
	lockall
	msgbox Cemetery_Text_NotWithoutAPokemon, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_PLAYER, Cemetery_Movement_BackFromGate
	waitmovement 0
	releaseall
	end

Cemetery_EventScript_LeaveThroughGate::
	lockall
	warp MAP_LITTLEROOT_TOWN, 13, 18
	waitstate
	releaseall
	end

Cemetery_Movement_BackFromGate:
	walk_up
	step_end

@ ---------------------------------------------------------------- headstones
Cemetery_EventScript_FathersGrave::
	lockall
	msgbox Cemetery_Text_FathersGrave, MSGBOX_DEFAULT
	closemessage
	releaseall
	end

Cemetery_EventScript_GraveWorn::
	msgbox Cemetery_Text_GraveWorn, MSGBOX_SIGN
	end

Cemetery_EventScript_GraveMoss::
	msgbox Cemetery_Text_GraveMoss, MSGBOX_SIGN
	end

Cemetery_EventScript_GraveTilted::
	msgbox Cemetery_Text_GraveTilted, MSGBOX_SIGN
	end

Cemetery_EventScript_GraveDated::
	msgbox Cemetery_Text_GraveDated, MSGBOX_SIGN
	end

Cemetery_EventScript_GraveOldest::
	msgbox Cemetery_Text_GraveOldest, MSGBOX_SIGN
	end

Cemetery_EventScript_GraveTrainerAndPartner::
	msgbox Cemetery_Text_GraveTrainerAndPartner, MSGBOX_SIGN
	end

Cemetery_EventScript_GraveNoName::
	msgbox Cemetery_Text_GraveNoName, MSGBOX_SIGN
	end

Cemetery_EventScript_GraveScratchedOut::
	msgbox Cemetery_Text_GraveScratchedOut, MSGBOX_SIGN
	end

Cemetery_EventScript_GraveNorthMarker::
	msgbox Cemetery_Text_GraveNorthMarker, MSGBOX_SIGN
	end

Cemetery_EventScript_GraveFoundHere::
	msgbox Cemetery_Text_GraveFoundHere, MSGBOX_SIGN
	end

Cemetery_EventScript_GraveKeeper::
	msgbox Cemetery_Text_GraveKeeper, MSGBOX_SIGN
	end


@ The keeper tends the ground, and left the flowers on the new stone. He found
@ the body. He will not say what else he found until the player has beaten YEW.
Cemetery_EventScript_Keeper::
	lock
	faceplayer
	goto_if_set FLAG_BADGE05_GET, Cemetery_EventScript_KeeperTellsAll
	msgbox Cemetery_Text_KeeperGuarded, MSGBOX_DEFAULT
	closemessage
	release
	end

Cemetery_EventScript_KeeperTellsAll::
	goto_if_set FLAG_CEMETERY_KEEPER_TOLD, Cemetery_EventScript_KeeperRepeat
	setflag FLAG_CEMETERY_KEEPER_TOLD
	msgbox Cemetery_Text_KeeperTellsAll, MSGBOX_DEFAULT
	closemessage
	release
	end

Cemetery_EventScript_KeeperRepeat::
	msgbox Cemetery_Text_KeeperRepeat, MSGBOX_DEFAULT
	closemessage
	release
	end


Cemetery_Text_Intro:
	.string "…\p"
	.string "Cold air. Wet grass. Rows and rows\n"
	.string "of headstones, fading into the fog.\p"
	.string "You have no idea how you got here.$"

Cemetery_Text_ThreeBallsWait:
	.string "At the top of the path, three POKé\n"
	.string "BALLS have been laid before a shrine.\p"
	.string "They look like they are waiting for\n"
	.string "someone.$"

Cemetery_Text_BallsLaidBeforeShrine:
	.string "Three POKé BALLS, side by side on\n"
	.string "the cold stone.\p"
	.string "Something inside them stirs as you\n"
	.string "reach out…$"

Cemetery_Text_TookStarter:
	.string "The moment your fingers close around\n"
	.string "it, fog rolls up over the shrine.\p"
	.string "When it thins, the stone is bare.\n"
	.string "You never saw who took the rest.$"

Cemetery_Text_SomethingRises:
	.string "The fog draws back from one of the\n"
	.string "graves.\p"
	.string "Something is climbing out of it.$"

Cemetery_Text_AmbushOver:
	.string "The thing sinks back into the earth\n"
	.string "without a sound.\p"
	.string "Your POKéMON stays close to you now.$"

Cemetery_Text_AmbushFled:
	.string "You back away, and the fog closes\n"
	.string "over the grave again.$"

Cemetery_Text_Shrine:
	.string "A weathered stone shrine.\p"
	.string "The inscription has worn away, but\n"
	.string "someone still tends the urns.$"

Cemetery_Text_NotWithoutAPokemon:
	.string "The fog beyond the gate is thick\n"
	.string "enough to swallow the path whole.\p"
	.string "Walking out there without a POKéMON\n"
	.string "would be madness.$"

Cemetery_Text_FathersGrave:
	.string "The stone is new. The edges are\n"
	.string "still sharp.\p"
	.string "You know the name on it better than\n"
	.string "your own.\p"
	.string "There are flowers in the urn above,\n"
	.string "and they have not wilted yet.\p"
	.string "Someone was here this week.$"

Cemetery_Text_GraveWorn:
	.string "The name has worn away entirely.\n"
	.string "Only the shape of the letters is left.$"

Cemetery_Text_GraveMoss:
	.string "Moss has filled every cut in the\n"
	.string "stone. You could scrape it clear.\p"
	.string "You decide not to.$"

Cemetery_Text_GraveTilted:
	.string "The stone leans badly.\p"
	.string "The ground beneath it has been\n"
	.string "settling for a long time.$"

Cemetery_Text_GraveDated:
	.string "A date, some ninety years past.\p"
	.string "No name. No family. Nothing else.$"

Cemetery_Text_GraveOldest:
	.string "The oldest stone in the row.\p"
	.string "“HERE LIES ONE WHO WOULD NOT\n"
	.string "LEAVE THE HILL.”$"

Cemetery_Text_GraveTrainerAndPartner:
	.string "Two names share the stone.\p"
	.string "A TRAINER and their POKéMON, buried\n"
	.string "on the same day.$"

Cemetery_Text_GraveNoName:
	.string "No name at all. Only a number,\n"
	.string "cut deep and neat.\p"
	.string "Somebody keeps a list somewhere.$"

Cemetery_Text_GraveScratchedOut:
	.string "There was a name here.\p"
	.string "Someone has taken a chisel to it\n"
	.string "and taken it off.$"

Cemetery_Text_GraveKeeper:
	.string "“KEEPER OF THIS GROUND, FORTY\n"
	.string "YEARS.”\p"
	.string "Whoever tends the place now has\n"
	.string "not put their own name up.$"

Cemetery_Text_KeeperGuarded:
	.string "I keep this ground. Forty-one years\n"
	.string "now, since my father had it.\p"
	.string "…You will be his child. You have\n"
	.string "the same way of standing.\p"
	.string "I put the flowers there. Somebody\n"
	.string "ought to.\p"
	.string "As for the rest of it, no.\n"
	.string "Not to a TRAINER with nothing to show.\p"
	.string "Go and earn something first.$"

Cemetery_Text_KeeperTellsAll:
	.string "So. YEW's BADGE.\p"
	.string "Then you will have heard her say she\n"
	.string "does not believe it either.\p"
	.string "I found him at first light, in the\n"
	.string "north row. No mark on him.\p"
	.string "His POKé BALLS were on his belt,\n"
	.string "every one of them shut.\p"
	.string "A man does not meet something out\n"
	.string "here and never reach for his team.\p"
	.string "And he was the fourth. Four, in six\n"
	.string "years, all in the north row.\p"
	.string "The council had the names taken off\n"
	.string "two of those stones.\p"
	.string "I did not do the scratching, and\n"
	.string "nobody asked me for the keys.$"

Cemetery_Text_KeeperRepeat:
	.string "The north row. Four of them.\p"
	.string "Ask YEW what she means to do about\n"
	.string "it. She has been waiting for someone\l"
	.string "to ask.$"

Cemetery_Text_GraveNorthMarker:
	.string "A slab set flush with the ground at\n"
	.string "the head of the row.\p"
	.string "“NORTH ROW. PLOTS 1-40.”\p"
	.string "Eight are filled. The rest of the\n"
	.string "cemetery took a century to fill.$"

Cemetery_Text_GraveFoundHere:
	.string "The stone is newer than the ones\n"
	.string "behind it.\p"
	.string "“FOUND AT THIS PLACE.”\p"
	.string "No cause. No age. Just the year,\n"
	.string "and where they were lying.$"
