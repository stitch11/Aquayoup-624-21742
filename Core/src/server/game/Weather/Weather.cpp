/*
 * Copyright (C) 2008-2016 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/** \file
    \ingroup world
*/

#include "Weather.h"
#include "WorldPacket.h"
#include "Player.h"
#include "World.h"
#include "Log.h"
#include "Util.h"
#include "ScriptMgr.h"
#include "WorldSession.h"
#include "MiscPackets.h"

//Stitch meteo : area plutot que zone
#include"World.h"
#include "DBCStores.h"
#include "WeatherMgr.h"
//Stitch TMP
//#include "Map.h"

/// Create the Weather object
Weather::Weather(uint32 zone, WeatherData const* weatherChances)
    : m_zone(zone), m_weatherChances(weatherChances)
{
    m_timer.SetInterval(sWorld->getIntConfig(CONFIG_INTERVAL_CHANGEWEATHER));
    m_type = WEATHER_TYPE_FINE;
	m_rainCycles = 0;	//Stitch meteo : duree de la pluie
    m_grade = 0;

    TC_LOG_INFO("misc", "WORLD: Starting weather system for zone %u (change every %u minutes).", m_zone, (uint32)(m_timer.GetInterval() / (MINUTE*IN_MILLISECONDS)));
}

/// Launch a weather update
bool Weather::Update(uint32 diff)
{
    if (m_timer.GetCurrent() >= 0)
        m_timer.Update(diff);
    else
        m_timer.SetCurrent(0);

    ///- If the timer has passed, ReGenerate the weather
    if (m_timer.Passed())
    {
        m_timer.Reset();
        // update only if Regenerate has changed the weather
        if (ReGenerate())
        {
            ///- Weather will be removed if not updated (no players in zone anymore)
            if (!UpdateWeather())
                return false;
        }
    }

    sScriptMgr->OnWeatherUpdate(this, diff);
    return true;
}

/// Calculate the new weather
bool Weather::ReGenerate()
{

	//Stitch Météo forcée par zone
	// WEATHER_TYPE_FINE = 0, WEATHER_TYPE_RAIN (Pluie) = 1, WEATHER_TYPE_SNOW (Neige) = 2,
	//  WEATHER_TYPE_STORM (TEMPÊTE) = 3,WEATHER_TYPE_THUNDERS (TONNERRE)= 86, WEATHER_TYPE_BLACKRAIN (PLUIE NOIRE) = 90
	
	
	uint32 forcedZone = m_zone;
	if (AreaTableEntry const* areaEntry = sAreaTableStore.LookupEntry(m_zone))
		if (areaEntry->ParentAreaID)
			forcedZone = areaEntry->ParentAreaID;
	switch (forcedZone)
	//switch (m_zone)

	{
	case 1:    // Dun Morogh
	{
		uint8 Tmp = urand(1,10);
		if (Tmp < 8)
		{
			m_type = WEATHER_TYPE_SNOW;
			m_grade = 1.0f;
			return true;
		}
		break;
	}
	case 4:    // Terres foudroyées
	{
		uint8 Tmp = urand(1, 10);
		if (Tmp < 8)
		{
			m_type = WEATHER_TYPE_THUNDERS;
			m_grade = 1.0f;
			return true;
		}
		break;
	}
	case 65:    // Desolation des dragon 
	{
		uint8 Tmp = urand(1, 10);
		if (Tmp < 8)
		{
			m_type = WEATHER_TYPE_SNOW;
			m_grade = 1.0f;
			return true;
		}
		break;
	}
	case 67:    // Les pics Foudroyés 
	{
		uint8 Tmp = urand(1, 10);
		if (Tmp < 8)
		{
			m_type = WEATHER_TYPE_SNOW;
			m_grade = 1.0f;
			return true;
		}
		break;
	}
	case 210:    // La couronne de glace  
	{
		uint8 Tmp = urand(1, 10);
		if (Tmp < 6)
		{
			m_type = WEATHER_TYPE_SNOW;
			m_grade = 1.0f;
			return true;
		}
		break;
	}
	case 440:    // Tanaris
	{
		uint8 Tmp = urand(1, 10);
		if (Tmp < 8)
		{
			m_type = WEATHER_TYPE_STORM;
			m_grade = 1.0f;
			return true;
		}
		break;
	}
	case 618:    // Berceau-de-l'hiver
	{
		uint8 Tmp = urand(1, 10);
		if (Tmp < 8)
		{
			m_type = WEATHER_TYPE_SNOW;
			m_grade = 1.0f;
			return true;
		}
		break;
	}
	case 1377:    // Silitus
	{
		uint8 Tmp = urand(1, 10);
		if (Tmp < 4)
		{
			m_type = WEATHER_TYPE_STORM;
			m_grade = 1.0f;
			return true;
		}
		break;
	}
	case 3520:    // Vallée d'ombrelune
	{
		uint8 Tmp = urand(1, 10);
		if (Tmp < 5)
		{
			m_type = WEATHER_TYPE_STORM /* WEATHER_TYPE_THUNDERS*/;
			m_grade = 1.0f;
			return true;
		}
		break;
	}
	case 3521:    // Marecage de Zangar
	{
		uint8 Tmp = urand(1, 10);
		if (Tmp < 3)
		{
			m_type = WEATHER_TYPE_RAIN;
			m_grade = 1.0f;
			return true;
		}
		break;
	}
	case 3523:    // Raz-de-Néant 
	{
		uint8 Tmp = urand(1,10);
		if (Tmp < 3)
		{
			m_type = WEATHER_TYPE_STORM /*WEATHER_TYPE_THUNDERS*/;
			m_grade = 1.0f;
			return true;
		}
		else if (Tmp < 6)
		{
			m_type = WEATHER_TYPE_STORM;
			m_grade = 1.0f;
			return true;
		}
		break;
	}
	case 3711:    // Bassin de Sholazar 
	{
		uint8 Tmp = urand(1, 10);
		if (Tmp < 3)
		{
			m_type = WEATHER_TYPE_RAIN;
			m_grade = 1.0f;
			return true;
		}
		break;
	}
	case 4197:    //  Joug-d'Hiver 
	{
		uint8 Tmp = urand(1, 10);
		if (Tmp < 5)
		{
			m_type = WEATHER_TYPE_SNOW;
			m_grade = 1.0f;
			return true;
		}
		break;
	}
	case 5841:    //  Sommet de kun-Lai
	{
		uint8 Tmp = urand(1, 10);
		if (Tmp < 7)
		{
			m_type = WEATHER_TYPE_SNOW;
			m_grade = 1.0f;
			return true;
		}
		break;
	}
	case 6134:    //  Etendues Sauvage de Krasarang
	{
		uint8 Tmp = urand(1, 10);
		if (Tmp < 4)
		{
			m_type = WEATHER_TYPE_RAIN;
			m_grade = 1.0f;
			return true;
		}
		break;
	}
	case 6720:    //  Crete de GivreFeu
	{
		uint8 Tmp = urand(1, 10);
		if (Tmp < 6)
		{
			m_type = WEATHER_TYPE_SNOW;
			m_grade = 1.0f;
			return true;
		}
		break;
	}



	default:
		break;
	}



    if (!m_weatherChances)
    {
        m_type = WEATHER_TYPE_FINE;
        m_grade = 0.0f;
        return false;
    }

    /// Weather statistics:
    ///- 30% - no change
    ///- 30% - weather gets better (if not fine) or change weather type
    ///- 30% - weather worsens (if not fine)
    ///- 10% - radical change (if not fine)

	//Stitch meteo : duree de la pluie
	if (m_type == WEATHER_TYPE_RAIN)
	{
		uint32 maxCycles = (sWorld->getIntConfig(CONFIG_WEATHER_RAIN_MAX_DURATION) + 4) / 5;
		if (maxCycles && ++m_rainCycles >= maxCycles)
		{
			m_rainCycles = 0;
			m_type = WEATHER_TYPE_FINE;
			m_grade = 0.0f;
			return true;
		}
	}
	else
		m_rainCycles = 0;


    uint32 u = urand(0, 99);

	//Stitch meteo change plus souvent
    //if (u < 30)
	if (u < 10)
        return false;

    // remember old values
    WeatherType old_type = m_type;
    float old_grade = m_grade;

    //78 days between January 1st and March 20nd; 365/4=91 days by season
    // season source http://aa.usno.navy.mil/data/docs/EarthSeasons.html
    time_t gtime = sWorld->GetGameTime();
    struct tm ltime;
    localtime_r(&gtime, &ltime);
    uint32 season = ((ltime.tm_yday - 78 + 365) / 91) % 4;

    static char const* seasonName[WEATHER_SEASONS] = { "spring", "summer", "fall", "winter" };

    TC_LOG_INFO("misc", "Generating a change in %s weather for zone %u.", seasonName[season], m_zone);

    if ((u < 60) && (m_grade < 0.33333334f))                // Get fair
    {
        m_type = WEATHER_TYPE_FINE;
        m_grade = 0.0f;
    }

    if ((u < 60) && (m_type != WEATHER_TYPE_FINE))          // Get better
    {
        m_grade -= 0.33333334f;
        return true;
    }

    if ((u < 90) && (m_type != WEATHER_TYPE_FINE))          // Get worse
    {
        m_grade += 0.33333334f;
        return true;
    }

    if (m_type != WEATHER_TYPE_FINE)
    {
        /// Radical change:
        ///- if light -> heavy
        ///- if medium -> change weather type
        ///- if heavy -> 50% light, 50% change weather type

        if (m_grade < 0.33333334f)
        {
            m_grade = 0.9999f;                              // go nuts
            return true;
        }
        else
        {
            if (m_grade > 0.6666667f)
            {
                                                            // Severe change, but how severe?
                uint32 rnd = urand(0, 99);
                if (rnd < 50)
                {
                    m_grade -= 0.6666667f;
                    return true;
                }
            }
            m_type = WEATHER_TYPE_FINE;                     // clear up
            m_grade = 0;
        }
    }

    // At this point, only weather that isn't doing anything remains but that have weather data
    uint32 chance1 = m_weatherChances->data[season].rainChance;
    uint32 chance2 = chance1 + m_weatherChances->data[season].snowChance;
    uint32 chance3 = chance2 + m_weatherChances->data[season].stormChance;

    uint32 rnd = urand(1, 100);
    if (rnd <= chance1)
        m_type = WEATHER_TYPE_RAIN;
    else if (rnd <= chance2)
        m_type = WEATHER_TYPE_SNOW;
    else if (rnd <= chance3)
        m_type = WEATHER_TYPE_STORM;
    else
        m_type = WEATHER_TYPE_FINE;

    /// New weather statistics (if not fine):
    ///- 85% light
    ///- 7% medium
    ///- 7% heavy
    /// If fine 100% sun (no fog)

    if (m_type == WEATHER_TYPE_FINE)
    {
        m_grade = 0.0f;
    }
    else if (u < 90)
    {
		//Stitch meteo faible : plus forte
        //m_grade = (float)rand_norm() * 0.3333f;
		m_grade = (float)rand_norm() * 0.3333f + 0.2222f;
    }
    else
    {
        // Severe change, but how severe?
        rnd = urand(0, 99);
        if (rnd < 50)
            m_grade = (float)rand_norm() * 0.3333f + 0.3334f;
        else
            m_grade = (float)rand_norm() * 0.3333f + 0.6667f;
    }

    // return true only in case weather changes
    return m_type != old_type || m_grade != old_grade;
}

void Weather::SendWeatherUpdateToPlayer(Player* player)
{
	//Stitch meteo : effet au maximun
    WorldPackets::Misc::Weather weather(GetWeatherState(), m_grade);
	//WorldPackets::Misc::Weather weather(GetWeatherState(), 1.0f);

    player->GetSession()->SendPacket(weather.Write());
}

/// Send the new weather to all players in the zone
bool Weather::UpdateWeather()
{
    ///- Send the weather packet to all players in this zone
    if (m_grade >= 1)
        m_grade = 0.9999f;
    else if (m_grade < 0)
        m_grade = 0.0001f;

    WeatherState state = GetWeatherState();

	TC_LOG_ERROR("server", "Weather zone %u type %u grade %f state %u", m_zone, m_type, m_grade, uint32(state));

	//Stitch meteo : effet au maximun
    WorldPackets::Misc::Weather weather(state, m_grade);
	///WorldPackets::Misc::Weather weather(state, 1.0f);

    //- Returns false if there were no players found to update



	//Stitch meteo : area plutot que zone
	/*
   if (!sWorld->SendZoneMessage(m_zone, weather.Write()))

        return false;
		*/
	WorldPacket const* weatherPacket = weather.Write();
	bool playersFound = false;
	for (auto const& itr : sWorld->GetAllSessions())
	{
		Player* p = itr.second->GetPlayer();
		if (p && p->IsInWorld() && IsInWeatherArea(p))
		{
			p->GetSession()->SendPacket(weatherPacket);
			playersFound = true;
		}
	}
	if (!playersFound)
		return false;



   //Stitch meteo : override lumiere neige (0 = retour à la lumiere normale)
   uint32 lightId = 0;
   if (state != WEATHER_STATE_FINE)
   {
	   if (m_type == WEATHER_TYPE_SNOW)
		   lightId = 6586;
	   else if (m_type == WEATHER_TYPE_STORM)
		   lightId = 6587;
   }
   TC_LOG_ERROR("server", "Override zone %u type %u lightId %u", m_zone, uint32(m_type), lightId);
   for (auto const& itr : sWorld->GetAllSessions())
   {
	   Player* p = itr.second->GetPlayer();
	   
	   //Stitch meteo : area plutot que zone
	   //if (p && p->IsInWorld() && p->GetZoneId() == m_zone)
	   if (p && p->IsInWorld() && IsInWeatherArea(p))


	   {
		   p->GetMap()->SendOverrideLightToPlayer(p, lightId, 2000);
		   //Stitch meteo : area plutot que zone
		   //p->GetMap()->SetZoneOverrideLight(m_zone, lightId, 2000);
		   //break;
	   }
   }



   //Stitch Meteo : rafraîchir la vitesse + messages (ralentissement neige/tempête)
   static std::unordered_map<uint32, bool> s_slowByZone;
   bool slowNow = IsSlowingWeather();
   bool slowBefore = s_slowByZone[m_zone];
   s_slowByZone[m_zone] = slowNow;

   for (auto const& itr : sWorld->GetAllSessions())
   {
	   Player* pl = itr.second->GetPlayer();
	   
	   //Stitch meteo : area plutot que zone
	   if (!pl || !pl->IsInWorld() || !IsInWeatherArea(pl))
		   //if (!pl || !pl->IsInWorld() || pl->GetZoneId() != m_zone)


		   continue;

	   for (uint8 i = MOVE_WALK; i < MAX_MOVE_TYPE; ++i)
		   pl->UpdateSpeed(UnitMoveType(i));

	   if (sWorld->getIntConfig(CONFIG_WEATHER_SLOW_PCT) >= 100 || slowNow == slowBefore)
		   continue;

	   std::string msg;
	   LocaleConstant loc = pl->GetSession()->GetSessionDbcLocale();
	   if (slowNow)
	   {
		   switch (loc)
		   {
		   case LOCALE_frFR: msg = "D\xC3\xA9" "placement r\xC3\xA9" "duit par le mauvais temps"; break;
		   case LOCALE_esES: msg = "Movimiento reducido por el mal tiempo"; break;
		   case LOCALE_deDE: msg = "Bewegung durch das schlechte Wetter verringert"; break;
		   default:          msg = "Movement reduced by bad weather"; break;
		   }
	   }
	   else
	   {
		   switch (loc)
		   {
		   case LOCALE_frFR: msg = "Vitesse de d\xC3\xA9" "placement restaur\xC3\xA9" "e"; break;
		   case LOCALE_esES: msg = "Velocidad de movimiento restaurada"; break;
		   case LOCALE_deDE: msg = "Bewegungsgeschwindigkeit wiederhergestellt"; break;
		   default:          msg = "Movement speed restored"; break;
		   }
	   }
	   pl->GetSession()->SendNotification("%s", msg.c_str());
   }


    ///- Log the event
    char const* wthstr;
    switch (state)
    {
        case WEATHER_STATE_FOG:
            wthstr = "fog";
            break;
        case WEATHER_STATE_LIGHT_RAIN:
            wthstr = "light rain";
            break;
        case WEATHER_STATE_MEDIUM_RAIN:
            wthstr = "medium rain";
            break;
        case WEATHER_STATE_HEAVY_RAIN:
            wthstr = "heavy rain";
            break;
        case WEATHER_STATE_LIGHT_SNOW:
            wthstr = "light snow";
            break;
        case WEATHER_STATE_MEDIUM_SNOW:
            wthstr = "medium snow";
            break;
        case WEATHER_STATE_HEAVY_SNOW:
            wthstr = "heavy snow";
            break;
        case WEATHER_STATE_LIGHT_SANDSTORM:
            wthstr = "light sandstorm";
            break;
        case WEATHER_STATE_MEDIUM_SANDSTORM:
            wthstr = "medium sandstorm";
            break;
        case WEATHER_STATE_HEAVY_SANDSTORM:
            wthstr = "heavy sandstorm";
            break;
        case WEATHER_STATE_THUNDERS:
            wthstr = "thunders";
            break;
        case WEATHER_STATE_BLACKRAIN:
            wthstr = "blackrain";
            break;
        case WEATHER_STATE_FINE:
        default:
            wthstr = "fine";
            break;
    }

    TC_LOG_INFO("misc", "Change the weather of zone %u to %s.", m_zone, wthstr);
    sScriptMgr->OnWeatherChange(this, state, m_grade);
    return true;
}

/// Set the weather
void Weather::SetWeather(WeatherType type, float grade)
{
    if (m_type == type && m_grade == grade)
        return;

    m_type = type;
    m_grade = grade;
    UpdateWeather();
}

/// Get the sound number associated with the current weather
WeatherState Weather::GetWeatherState() const
{
    if (m_grade<0.27f)
        return WEATHER_STATE_FINE;

    switch (m_type)
    {
        case WEATHER_TYPE_RAIN:
            if (m_grade<0.40f)
                return WEATHER_STATE_LIGHT_RAIN;
            else if (m_grade<0.70f)
                return WEATHER_STATE_MEDIUM_RAIN;
            else
                return WEATHER_STATE_HEAVY_RAIN;
        case WEATHER_TYPE_SNOW:
            if (m_grade<0.40f)
                return WEATHER_STATE_LIGHT_SNOW;
            else if (m_grade<0.70f)
                return WEATHER_STATE_MEDIUM_SNOW;
            else
                return WEATHER_STATE_HEAVY_SNOW;
        case WEATHER_TYPE_STORM:
            if (m_grade<0.40f)
                return WEATHER_STATE_LIGHT_SANDSTORM;
            else if (m_grade<0.70f)
                return WEATHER_STATE_MEDIUM_SANDSTORM;
            else
                return WEATHER_STATE_HEAVY_SANDSTORM;
        case WEATHER_TYPE_BLACKRAIN:
            return WEATHER_STATE_BLACKRAIN;
        case WEATHER_TYPE_THUNDERS:
            return WEATHER_STATE_THUNDERS;
        case WEATHER_TYPE_FINE:
        default:
            return WEATHER_STATE_FINE;
    }
}

bool Weather::IsSlowingWeather() const
{
	return (m_type == WEATHER_TYPE_SNOW || m_type == WEATHER_TYPE_STORM) && GetWeatherState() != WEATHER_STATE_FINE;
}


//Stitch meteo : area plutot que zone : le joueur est-il concerne par cette meteo ?
bool Weather::IsInWeatherArea(Player* player) const
{
	return WeatherMgr::ResolveWeatherId(player->GetAreaId(), player->GetZoneId()) == m_zone;
}
