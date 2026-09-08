PetalburgCity_House2_MapScripts::
	.byte 0

PetalburgCity_House2_EventScript_Woman::
	msgbox PetalburgCity_House2_Text_YewBecameGymLeader, MSGBOX_NPC
	end

PetalburgCity_House2_EventScript_SchoolKid::
	msgbox PetalburgCity_House2_Text_BattledYewOnce, MSGBOX_NPC
	end

PetalburgCity_House2_Text_YewBecameGymLeader:
	.string "YEW is our town's new GYM LEADER.\p"
	.string "The last one died out at the old\n"
	.string "cemetery. Nobody will say how.\p"
	.string "She took the post the same week.\n"
	.string "Some say too quickly.$"

PetalburgCity_House2_Text_BattledYewOnce:
	.string "I battled YEW once, but, whew,\n"
	.string "she was way too strong.\p"
	.string "How would I put it?\p"
	.string "I got the feeling she was somewhere\n"
	.string "else the whole time.$"

