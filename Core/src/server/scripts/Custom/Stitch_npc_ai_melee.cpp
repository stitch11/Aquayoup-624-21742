////#########################################################################################################################################################################################################################################
// Copyright (C) Juillet 2020 Stitch pour https:\\Aquayoup.123.fr
// AI generique npc par classe : Mélée Ver 2022-05-15 (warrior simple, combat au corp a corp)
// 
// ScriptName = Stitch_npc_ai_melee : npc d'exemple : 15100012
// spell1 : Attaque principale
// spell2 : Dot
// spell3 : spell lancé a l'agro
// spell4 : spell lancé a l'évade ou respawn
// spell5 : Buf
//
// Si spell1 = 0 : tirage aléatoire des spells
//
// Il est possible d'influencer le temp entre 2 cast avec `BaseAttackTime` & `RangeAttackTime` 
// UPDATE `creature_template` SET `spell1` = ATTAQUE , `spell2` =  DOT , `spell3` =  AGRO, `spell4` = EVADE , `spell5` = BUF , `ScriptName` = 'Stitch_npc_ai_melee',`AIName` = '' WHERE (entry = 2237);
// REPLACE INTO `creature_equip_template` (`CreatureID`, `ID`, `ItemID1`, `ItemID2`, `ItemID3`, `VerifiedBuild`) VALUES
// (15100012, 1, 88857, 0, 0, 0);	## Lance
//###########################################################################################################################################################################################################################################

#include "CreatureTextMgr.h"

//################################################################################################
//StitchAI AI Mélée
//################################################################################################

class Stitch_npc_ai_melee : public CreatureScript
{
public: Stitch_npc_ai_melee() : CreatureScript("Stitch_npc_ai_melee") { }

		struct Stitch_npc_ai_meleeAI : public ScriptedAI
		{
			Stitch_npc_ai_meleeAI(Creature* creature) : ScriptedAI(creature) { }

			uint32 Npc_Type = me->GetCreatureTemplate()->type;
			uint32 Npc_Family = me->GetCreatureTemplate()->family;
			uint32 Npc_Model = me->GetDisplayId();
			uint32 Random = 0;
			uint32 DistanceDeCast = 40;												// Distance max a laquelle un npc attaquera , au dela il quite le combat
			uint32 ResteADistance = 5;												// Distance max a laquelle un npc s'approchera
			uint32 Dist = 0;														// Distance entre le npc et sa cible
			Unit* victim = me->GetVictim();										 
			uint32 MessageAlagro = 0;
			uint32 Spell_ContreAttaque = 0;

			// Definitions des variables Cooldown et le 1er lancement
			uint32 Cooldown_Spell1 = 500;
			uint32 Cooldown_Spell2 = 2000;
			uint32 Cooldown_ResteADistance = 4000;									// Test si en contact
			uint32 Cooldown_ResteADistance_defaut = urand(5000, 6000);
			uint32 Cooldown_Anti_Bug_Figer = 2000;
			uint32 Cooldown_Npc_Emotes = urand(5000, 8000);
			uint32 Cooldown_Trop_Loin = 3000;
			uint32 Cooldown_Spell_Heal = 5000;
			uint32 Cooldown_Spell_Heal_defaut = 8000;
			uint32 Spell_Heal = 0;
			uint32 Cooldown_Spell_ContreAttaque = 4000;
			uint32 Cooldown_Spell_ContreAttaque_defaut = 8000;

			// Spells
			uint32 Buf_1 = 12712;																			// Soldat aguerri
			uint32 liste_Buf[5] = { 12712, 6673, 1160, 159362, 8599 };										// Soldat aguerri 12712 (2 mains= dmg+15%), Cri de guerre 6673, Cri démoralisant 1160 (8s 10m Soi-même), Folie sanguinaire 159362 (pv 1%/3s), Enrager 8599
			uint32 Spell_agro = 0;
			uint32 liste_agro[3] = { 100, 355, 145763 };													// Charge 100, Provocation 355, Bondir 8-40m 145763
			uint32 Spell_1 = 0;
			uint32 liste_spell_1[8] = { 29426, 126799, 118326, 172851, 38742, 99409, 100431, 115530 };		// Frappe héroïque 29426, Frappe tranchante 126799, Attaque vicieuse 118326, Enchaînement 172851, Enchaînement gangrené 38742, Enchaînement noir 99409, Enchaînement enflammé 100431, Fendoir spirituel 115530
			uint32 Spell_2 = 0;
			uint32 liste_spell_2[9] = { 127171, 131662, 8147, 118532, 125436, 772, 772, 8147, 8147 };		// Fendoir vicieux 15/lvl + 2/lvl/1s cumulable 5 fois 127171, Coups de couteau 131662, Entaille infectée 118532, Découpe d'os 125436, Pourfendre 772, Coup de tonnerre 8147
			uint32 Spell_evade = 137573;																	// vitesse (+70%/4s) 
			uint32 Spell_trop_Loin = 100;																	// Charge 100

			uint32 Spell_changeform_felin = 768;
			uint32 Spell_changeform_ours = 5487;
			uint32 Spell_changeform_selenien = 24858;
			uint32 Spell_changeform_arbre_de_vie = 33891;
			uint32 Spell_changeform_treant = 114282;


			// Emotes
			uint32 Npc_Emotes[22] = { 1,3,7,11,15,16,19,21,22,23,24,53,66,71,70,153,254,274,381,401,462,482 };

			uint32 Start_Agro = 0;


			void InitializeAI() override
			{
				// Buf au lancement du serveur (Pet, armure de givre, etc)
				uint32 Tmp = me->m_spells[3];
				if (Tmp != 0)
				{
					me->CastSpell(me, Tmp, true);
				}
				Arme_rangé();
				Retire_changeform();
			}

			void Init_AI()
			{
				// Message a l'agro forcé par spell(8)
				if (me->m_spells[7] == 1) { MessageAlagro = 1; }

				// Spell contre attaque si PV bas
				if (me->m_spells[6] != 0) { Spell_ContreAttaque = me->m_spells[6]; }

				// ################################################################################################################################################
				// Tirages aléatoires des spells
				// ################################################################################################################################################
				// Si m_spells[0] != 0 alors affectation aléatoire de tous les spells (prédéfinis dans le core) , sinon utilisera les spell défini dans creature_template spells[1 a 5]
				// m_spells[0] : Attaque principale						- Correspond a spell1 de creature_template
				// m_spells[1] : Dot									- Correspond a spell2 de creature_template
				// m_spells[2] : spell lancé a l'agro					- Correspond a spell3 de creature_template
				// m_spells[3] : spell lancé a l'évade ou respawn		- Correspond a spell4 de creature_template
				// m_spells[4] : Buf									- Correspond a spell5 de creature_template

				if (me->m_spells[0] == 0) 
				{ 
					Spell_1 = liste_spell_1[urand(0, 7)];
					Spell_2 = liste_spell_2[urand(0, 8)];
					Spell_agro = liste_agro[urand(0, 2)];
					Buf_1 = liste_Buf[urand(0, 4)];

					// Pas de DOT ou Heal si level <15 sinon trop dur
					if (me->getLevel() < 15)
					{
						Spell_2 = 0;
						Spell_Heal = 0;
					}

				} else 
				{ 
					Spell_1 = me->m_spells[0]; 
					if (me->m_spells[1] != 0) { Spell_2 = me->m_spells[1]; } else Spell_2 = 0;
					if (me->m_spells[2] != 0) { Spell_agro = me->m_spells[2]; } else Spell_agro = 0;
					if (me->m_spells[3] != 0) { Spell_evade = me->m_spells[3]; } else Spell_evade = 0;
					if (me->m_spells[4] != 0) { Buf_1 = me->m_spells[4]; } else Buf_1 = 0;
					if (me->m_spells[5] != 0) { Spell_Heal = me->m_spells[5]; } else Spell_Heal = 0;
					
				}

				// ################################################################################################################################################
				// Divers
				// ################################################################################################################################################
				me->SetSheath(SHEATH_STATE_MELEE);												// S'equipe de l'arme au contact
				me->SetReactState(REACT_AGGRESSIVE);
				if (Spell_evade != 0) { me->CastSpell(me, Spell_evade, true); }
				// ################################################################################################################################################
			}
			void JustRespawned() override
			{
				me->GetMotionMaster()->MoveTargetedHome();							// Retour home pour rafraichir client
				//me->SetSpeedRate(MOVE_RUN, 1.01f);
				me->SetReactState(REACT_AGGRESSIVE);

				Init_AI();
				//Random = urand(1, 2);
				if (/*Random == 1 && */Spell_evade != 0) { me->CastSpell(me, Spell_evade, true); }		// 1/2 Chance de lancer le sort au respawn ou evade

			}

			void EnterCombat(Unit* /*who*/) override
			{
				ResteADistance = urand(4, 6);
				Init_AI();

				me->SetSheath(SHEATH_STATE_MELEE);									//Arme sortie
				if (me->GetMotionMaster()->GetCurrentMovementGeneratorType() == WAYPOINT_MOTION_TYPE)
				{
					me->StopMoving();
					me->GetMotionMaster()->MoveIdle();
				}

				//Retire certaines Aura, emotes & Bytes a l'agro
				me->RemoveAura(80264);	// Retire Marche a pas de loup
				me->RemoveAura(152891);	// Retire Avance a pas de loup invisible
				me->RemoveAura(104015);	// Retire afaissé / Stun
				me->RemoveAura(153964);	// Retire agenouillé, avec evade
				me->RemoveAura(42648);	// Retire Dort allongé + zzz
				me->RemoveAura(18795);	// Retire Dort allongé + zzz 
				me->RemoveAura(43905);	// Retire Ivre
				me->RemoveAura(101090);	// Retire Danse
				me->HandleEmoteCommand(0);
				me->SetUInt32Value(UNIT_NPC_EMOTESTATE, 0); // Autre façon de retirer des émotes pour les cas particuliers
				me->SetByteValue(UNIT_FIELD_BYTES_1, 0, 0);
				me->SetByteValue(UNIT_FIELD_BYTES_2, 0, 0);

			}
			void EnterEvadeMode(EvadeReason /*why*/) override
			{
				Start_Agro = 0;
				RetireBugDeCombat();
				me->AddUnitState(UNIT_STATE_EVADE);
				//me->SetSpeedRate(MOVE_RUN, 1.5f);													// Vitesse de déplacement
				me->GetMotionMaster()->MoveTargetedHome();											// Retour home
				me->RemoveAllControlled();															// renvois pet

				me->RemoveAura(Buf_1);

				//Random = urand(1, 3);
				//if (Random == 1 && Spell_evade != 0) { me->CastSpell(me, Spell_evade, true); }		// 1/3 Chance de lancer le sort au respawn ou evade

			}
			void JustReachedHome() override
			{
				me->SetReactState(REACT_AGGRESSIVE);
				Arme_rangé();
				//me->SetSpeedRate(MOVE_RUN, 1.01f);								// Vitesse par defaut définit a 1.01f puisque le patch modification par type,famille test si 1.0f
				Retire_changeform();
				if (Spell_evade != 0) { me->CastSpell(me, Spell_evade, true); }
			}
			void UpdateAI(uint32 diff) override
			{
				if (me->HasUnitState(UNIT_STATE_CONFUSED) || me->HasUnitState(UNIT_STATE_STUNNED) || me->HasUnitState(UNIT_STATE_DISTRACTED) || me->HasUnitState(UNIT_STATE_CANNOT_TURN) || me->HasUnitState(UNIT_STATE_CONTROLLED) || me->HasUnitState(UNIT_STATE_POSSESSED) || me->HasUnitState(UNIT_STATE_CONFUSED_MOVE))
					return;
				// ################################################################################################################################################
				// Emotes hors combat & mouvement & humanoides ou mort-vivants ####################################################################################
				// ################################################################################################################################################
				if ((Cooldown_Npc_Emotes <= diff) && (!me->isMoving()) && (!me->IsInCombat()) && (Npc_Type == CREATURE_TYPE_HUMANOID || Npc_Type == CREATURE_TYPE_UNDEAD))
				{
					uint32 Npc_Play_Emotes = Npc_Emotes[urand(0, 21)];
					me->HandleEmoteCommand(Npc_Play_Emotes);
					Cooldown_Npc_Emotes = urand(10000, 20000);
				}
				else
					Cooldown_Npc_Emotes -= diff;

				// ################################################################################################################################################
				// En Combat ######################################################################################################################################
				// ################################################################################################################################################
				if (UpdateVictim() /*&& !me->HasUnitState(UNIT_STATE_MOVE) || me->HasUnitState(UNIT_STATE_CASTING)*/)
				{
					Unit* victim = me->GetVictim();
					Dist = me->GetDistance(victim);

					if (Start_Agro == 0)
					{
						Start_Agro = 1;

						// Message a l'agro , ci le mob a plusieurs lignes (creature_text groupid>0) il y a de forte chance que ce soit pour un dialogue
						// et non un simple message a l'agro. Donc on l'ignore.
						Random = urand(1, 5);
						if ((sCreatureTextMgr->TextExist(me->GetEntry(), 0)) && (!sCreatureTextMgr->TextExist(me->GetEntry(), 1) && Random == 1) || MessageAlagro == 1)
						{
							Talk(0);
						}

						// Tourne au tour de la cible au contact plus rapide 
						if (me->GetCreatureTemplate()->pickpocketLootId == 1)
						{
							Cooldown_ResteADistance_defaut = urand(3000,4000);
						}

						// ########################################################################################################################################
						// Spell a lancer a l'agro 
						// ########################################################################################################################################

						if (Random == 1 && Buf_1 != 0)
						{
							me->CastSpell(me, Buf_1, true); 			// 1/2 Chance de lancer le Buf : Soldat aguerri
						}
						
						//Random = urand(1, 3);
						//if (Random == 1 && Spell_evade == 137573 && Spell_agro != 100 && Spell_agro != 145763)
						//{
						if (Spell_evade != 0) { me->CastSpell(me, Spell_evade, true); }	// vitesse_4s si spell_agro n'est pas charge ou Bondir
						//}


					
					}

					// Spell agro au contact
					if (Start_Agro == 1 && (Dist < 6) && isSpell_agro_charge() == false)
					{
						Start_Agro = 2;
						Random = urand(1, 2);
						if (Random == 1 && Spell_agro != 0)
						{
							me->CastSpell(victim, Spell_agro, true);
						}	// 1/2 Chance de lancer le sort d'agro

					}
					if (Start_Agro == 1 && (Dist >= 10) && isSpell_agro_charge() == true)
					{
						Start_Agro = 2;
						Random = urand(1, 2);
						if (Random == 1 && Spell_agro != 0)
						{
							me->CastSpell(victim, Spell_agro, true);
						}	// 1/2 Chance de lancer le sort d'agro

					}
						// ####################################################################################################################################

					// Ce heal s'il a un sort de heal -------------------------------------------------------------------------------------------------------------
					Heal_En_Combat_Caster(diff);

					// ############################################################################################################################################
					// Combat 

					if (Dist < 6)
					{ 

						// Spell1 sur la cible
						if (Cooldown_Spell1 <= diff && Spell_1 !=0)
						{
							me->CastSpell(victim, Spell_1, true);
							DoMeleeAttackIfReady();																// Combat en mélée
							Cooldown_Spell1 = 3000;
						}
						else Cooldown_Spell1 -= diff;

						// Spell2 sur la cible
						if (Cooldown_Spell2 <= diff && Spell_2 != 0)
						{
							if (!victim->HasAura(Spell_2))
							{
								me->CastSpell(victim, Spell_2, true);
								Cooldown_Spell2 = urand(6000, 8000);
							}
						}
						else Cooldown_Spell2 -= diff;
					}

					ContreAttaque(diff);
					Mouvement_Contact(diff);

					// ############################################################################################################################################
				}
				// ################################################################################################################################################
				Mouvement_All();
			}

			void RetireBugDeCombat()
			{
				me->CombatStop(true);
				me->RemoveAllControlled();												// renvois pet
				me->DeleteThreatList();
				me->LoadCreaturesAddon();
				me->SetLootRecipient(NULL);
				me->ResetPlayerDamageReq();
				me->SetLastDamagedTime(0);
				me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);				// UNROOT
				me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IN_COMBAT);					// Retire flag "en combat"
				me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);				// Retire flag "non attaquable"
			}
			void Mouvement_All()
			{
				// Anti bug de combat
				if (me->IsAlive() && (me->GetDistance(me->GetHomePosition()) >  50))
				{
					RetireBugDeCombat();

					if (me->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE) && me->IsInCombat())
					{
						me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
					}

					EnterEvadeMode(EVADE_REASON_SEQUENCE_BREAK);

					if (me->IsInCombat() && me->getAttackers().empty())
					{
						EnterEvadeMode(EVADE_REASON_NO_HOSTILES);
					}
				}




				if (!UpdateVictim())
					return;

				Dist = me->GetDistance(me->GetVictim());
				if ((Dist > DistanceDeCast) || (me->GetDistance2d(me->GetHomePosition().GetPositionX(), me->GetHomePosition().GetPositionY()) > 40))
				{
					RetireBugDeCombat();
					me->AddUnitState(UNIT_STATE_EVADE);
					EnterEvadeMode(EVADE_REASON_SEQUENCE_BREAK);						// Quite le combat si la cible > 30m (Caster & Mélée) ou > 40m de home
				}
			}
			void Mouvement_Contact(uint32 diff)
			{
				if (!UpdateVictim() || AuraFigé())
					return;

				Unit* victim = me->GetVictim();
				Dist = me->GetDistance(victim);

				//DoMeleeAttackIfReady();													// Combat en mélée

				// Si la cible >= 6m (pour éviter bug de rester figé) ---------------------------------------------------------------------------------------------
				if (Cooldown_Anti_Bug_Figer <= diff)
				{
					if (Dist >= 6)
					{
						float x = 0.0f, y = 0.0f, z = 0.0f;
						uint32 mapid = 0;

					x = (victim->GetPositionX() + urand(0, 2) - 1);
					y = (victim->GetPositionY() + urand(0, 2) - 1);
					if (me->GetMap()->IsOutdoors(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ()) && !me->IsUnderWater())
					{
						z = victim->GetMap()->GetHeight(me->GetPhaseMask(), x, y, MAX_HEIGHT, true);
					}
					else
					{
						z = victim->GetPositionZ();	// Sinon bug en interieur
					}
					mapid = victim->GetMapId();
					//me->GetClosePoint(x, y, z, me->GetObjectSize() / 3, 3);
					me->GetMotionMaster()->MovePoint(mapid, x, y, z);
					me->GetMotionMaster()->MoveChase(victim, 1, frand(0, 6.2836f));			// Pour suivre la cible avec un angle
					Cooldown_Anti_Bug_Figer = 1000;
					}
				}
				else Cooldown_Anti_Bug_Figer -= diff;

				// Si la cible < 8m --------------------------------------------------------------------------------------------------------------------------------
				if (Cooldown_ResteADistance <= diff)
				{
					if (Dist < 8)
					{
					Random = urand(1, 4);
					if (Random == 1 || Random == 2)
					{
						Tourne_Au_Tour_En_Combat();											// 2 chances sur 4 tourne au tour de sa victime
					}
					else if (Random == 3)
					{
						Avance_3m_En_Combat();												// 1 chances sur 4 avance
					}
					Cooldown_ResteADistance = Cooldown_ResteADistance_defaut;
					}
				}
				else Cooldown_ResteADistance -= diff;

				// Si la cible >= 10m <25m : Charge ---------------------------------------------------------------------------------------------------------------
				if (Cooldown_Trop_Loin <= diff)
				{
					if (Dist >= 10 && Dist <= 25)
					{
					Random = urand(1, 4);
						if (Random == 1 && Spell_trop_Loin !=0)
						{
							DoCastVictim(Spell_trop_Loin);
						}
					}
					Cooldown_Trop_Loin = urand(6000, 10000);
				}
				else Cooldown_Trop_Loin -= diff;
			}
			void Tourne_Au_Tour_En_Combat()
			{
				if (!UpdateVictim())
					return;

				Unit* victim = me->GetVictim();
				Dist = me->GetDistance(victim);

				float x = 0.0f, y = 0.0f, z = 0.0f;
				uint32 mapid = 0;

				x = (victim->GetPositionX() + urand(0, 4) - 2);
				y = (victim->GetPositionY() + urand(0, 4) - 2);
				if (me->GetMap()->IsOutdoors(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ()) && !me->IsUnderWater())
				{
					z = victim->GetMap()->GetHeight(me->GetPhaseMask(), x, y, MAX_HEIGHT, true);
				}
				else
				{
					z = victim->GetPositionZ();	// Sinon bug en interieur
				}
				mapid = victim->GetMapId();
				//me->GetClosePoint(x, y, z, me->GetObjectSize() / 3, 3);
				me->GetMotionMaster()->MovePoint(mapid, x, y, z);
			}
			void Avance_3m_En_Combat()
			{
				if (!UpdateVictim())
					return;

				Unit* victim = me->GetVictim();
				Dist = me->GetDistance(victim);

				float x = 0.0f, y = 0.0f, z = 0.0f;
				uint32 mapid = 0;

				x = (victim->GetPositionX());
				y = (victim->GetPositionY());
				z = victim->GetMap()->GetHeight(me->GetPhaseMask(), x, y, MAX_HEIGHT, true);
				mapid = victim->GetMapId();
				me->GetClosePoint(x, y, z, me->GetObjectSize() / 3, 3);
				me->GetMotionMaster()->MovePoint(mapid, x, y, z);
			}
			void Heal_En_Combat_Caster(uint32 diff)
			{
				//if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_MOVE))
				//return;

				if (Cooldown_Spell_Heal <= diff && Spell_Heal != 0)
				{
					// heal sur lui meme-------------------------------------------------------------------------------------------------------------------------------
					if ((me->GetHealth() < (me->GetMaxHealth()*0.60)))								// Si PV < 60%
					{
						//DoCast(me, Spell_Heal);
						DoCastAOE(Spell_Heal);
						Cooldown_Spell_Heal = Cooldown_Spell_Heal_defaut;
					}
				}
				else Cooldown_Spell_Heal -= diff;
			}
			void Bonus_Degat_Arme_Done(int val) // 
			{
				// +- Bonus en % de degat des armes infligées a victim
				me->HandleStatModifier(UNIT_MOD_ATTACK_POWER, TOTAL_PCT, val, true);
				me->HandleStatModifier(UNIT_MOD_DAMAGE_OFFHAND, TOTAL_PCT, val, true);
				me->HandleStatModifier(UNIT_MOD_DAMAGE_RANGED, TOTAL_PCT, val, true);
			}
			void Bonus_Armure(int val) // 
			{
				// +- Bonus d'armure 100% = pas de bonus/malus   ex : Bonus_Armure(115); // Bonus d'armure +15%
				me->SetModifierValue(UNIT_MOD_ARMOR, BASE_VALUE, me->GetArmor() * (val / 100));
				me->SetCanModifyStats(true);
				me->UpdateAllStats();
			}
			void ContreAttaque(uint32 diff)
			{
				if (!UpdateVictim())
					return;
				Unit* victim = me->GetVictim();
				if (victim->HasAura(Spell_ContreAttaque) || Spell_ContreAttaque == 0)
					return;

				// Contre attaque sur la cible (2 chance sur 3) -------------------------------------------------------------------------------------------------
				if (Cooldown_Spell_ContreAttaque <= diff && Spell_ContreAttaque != 0)
				{
					if ((me->GetHealth() < (me->GetMaxHealth()*0.40)))									// Si PV =< 40%
					{
						Random = urand(1, 3);
						if (Random != 1)
						{
							DoCastVictim(Spell_ContreAttaque);
						}
					}
					Cooldown_Spell_ContreAttaque = Cooldown_Spell_ContreAttaque_defaut;
				}
				else Cooldown_Spell_ContreAttaque -= diff;
			}
			bool AuraLenteur()
			{
				if (me->HasAura(116)		// Eclair_de_givre 116 
					|| me->HasAura(71318)	// Eclair_de_givre 71318
					|| me->HasAura(31589)	// Lenteur 31589
					|| me->HasAura(6136) 	// Armure_de_givre 6136
					|| me->HasAura(8056) 	// Horion_de_givre 8056
					|| me->HasAura(12548) 	// Horion_de_givre 12548
					|| me->HasAura(9080) 	// Brise_genou 9080
					|| me->HasAura(1715) 	// Brise_genou 1715
					|| me->HasAura(69917) 	// Fievre_de_givre 69917
					|| me->HasAura(67719) 	// Fievre_de_givre 67719
					|| me->HasAura(45477) 	// Toucher_de_glace 45477
					|| me->HasAura(300051) 	// Javelot_de_givre 300051
					|| me->HasAura(300237) 	// Javelot_de_givre 300237
					|| me->HasAura(60192) 	// Gel_de_zone 60192
					|| me->HasAura(116095) 	// Handicap 116095
					|| me->HasAura(300197) 	// Toucher_de_glace 300197
					|| me->HasAura(20170)	// Sceau de justice 20170
					) return true;
				else return false;
			}
			bool AuraFigé()
			{
				if (me->HasAura(122)		// Nova de givre
					|| me->HasAura(3600)	// Totem de lien terrestre
					|| me->HasAura(6474)	// Totem de lien terrestre passif
					) return true;
				else return false;
			}
			bool isSpell_agro_charge()
			{
				if (Spell_agro == 100 
					|| Spell_agro ==  6268 || Spell_agro == 119517 || Spell_agro == 32323 || Spell_agro == 19196 
					|| Spell_agro == 52311 || Spell_agro == 20508  || Spell_agro == 82635 || Spell_agro == 82797 
					|| Spell_agro == 89712 || Spell_agro == 75002  || Spell_agro == 64431 || Spell_agro == 49576 
					|| Spell_agro == 30010 || Spell_agro == 93515  || Spell_agro == 75002 || Spell_agro == 82635 
					|| Spell_agro == 145763|| Spell_agro == 163036 || Spell_agro ==  6544 || Spell_agro == 47482 
					|| Spell_agro == 300131) return true;
				else return false;
			}
			void Retire_changeform()
			{
				// Retire changeform druide
				me->RemoveAura(Spell_changeform_felin);
				me->RemoveAura(Spell_changeform_ours);
				me->RemoveAura(Spell_changeform_selenien);
				me->RemoveAura(Spell_changeform_arbre_de_vie);
				me->RemoveAura(Spell_changeform_treant);
			}
			void Arme_rangé()
			{
				// Certains Modelid posent probleme et seront donc ignorés
				if (Npc_Model == 6824 || Npc_Model == 6825 || Npc_Model == 6821 || Npc_Model == 5773 || Npc_Model == 937 || Npc_Model == 16861)
					return;

				// Uniquement pour les humanoides, mort-vivants + family 0
				if ((Npc_Type != CREATURE_TYPE_HUMANOID && Npc_Type != CREATURE_TYPE_UNDEAD) || Npc_Family != 0)
					return;

				me->SetSheath(SHEATH_STATE_UNARMED);								//Arme rangée
			}
		};

		


		CreatureAI* GetAI(Creature* creature) const override
		{
			return new Stitch_npc_ai_meleeAI(creature);
		}
};

//#################################################################################################################################################################
void AddSC_npc_ai_melee()
{
	new Stitch_npc_ai_melee();
}















