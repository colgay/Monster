#include "amxxmodule.h"
#include "WaypointApi.h"

Node *g_pStartNode;
Node *g_pEndNode;

short g_SpriteBeam4;

char* UTIL_VarArgs(const char* format, ...)
{
	va_list argptr;
	static char string[1024];

	va_start(argptr, format);
	vsprintf(string, format, argptr);
	va_end(argptr);

	return string;
}

void UTIL_BeamPoints(edict_t* pEntity, const Vector& pos1, const Vector& pos2, short sprite, int startFrame, int frameRate, int life, int width, int noise, int r, int g, int b, int brightness, int speed)
{
	MESSAGE_BEGIN(pEntity == nullptr ? MSG_BROADCAST : MSG_ONE_UNRELIABLE, SVC_TEMPENTITY, NULL, pEntity);
	WRITE_BYTE(TE_BEAMPOINTS);
	WRITE_COORD(pos1.x);
	WRITE_COORD(pos1.y);
	WRITE_COORD(pos1.z);
	WRITE_COORD(pos2.x);
	WRITE_COORD(pos2.y);
	WRITE_COORD(pos2.z);
	WRITE_SHORT(sprite);
	WRITE_BYTE(startFrame);		// startframe
	WRITE_BYTE(frameRate);		// framerate
	WRITE_BYTE(life);		// life
	WRITE_BYTE(width);		// width
	WRITE_BYTE(noise);		// noise
	WRITE_BYTE(r);	// r
	WRITE_BYTE(g);		// g
	WRITE_BYTE(b);		// b
	WRITE_BYTE(brightness);	// brightness
	WRITE_BYTE(speed);		// speed
	MESSAGE_END();
}

void OnAmxxAttach()
{
	WP_RequestFunctions();
}

void OnPluginsLoaded()
{
	g_SpriteBeam4 = PRECACHE_MODEL("sprites/zbeam4.spr");

	g_pStartNode = nullptr;
	g_pEndNode = nullptr;

	SERVER_PRINT("Monster module loaded...\n");
}

void ClientCommand(edict_t* pEntity)
{
	const char* pszCmd = CMD_ARGV(0);

	if (strcmp(pszCmd, "mo_s") == 0)
	{
		g_pStartNode = WP_FindClosestNode(pEntity->v.origin, 500.0f, true);
		if (g_pStartNode != nullptr)
		{
			Vector pos = g_pStartNode->GetPosition();
			CLIENT_PRINTF(pEntity, print_console, UTIL_VarArgs("[MO] start node set. {%.f, %.f, %.f}\n", pos.x, pos.y, pos.z));
		}

		RETURN_META(MRES_SUPERCEDE);
	}
	else if (strcmp(pszCmd, "mo_e") == 0)
	{
		g_pEndNode = WP_FindClosestNode(pEntity->v.origin, 500.0f, true);
		if (g_pEndNode != nullptr)
		{
			Vector pos = g_pEndNode->GetPosition();
			CLIENT_PRINTF(pEntity, print_console, UTIL_VarArgs("[MO] start node set. {%.f, %.f, %.f}\n", pos.x, pos.y, pos.z));
		}

		RETURN_META(MRES_SUPERCEDE);
	}
	else if (strcmp(pszCmd, "mo_go") == 0)
	{
		if (g_pStartNode != nullptr && g_pEndNode != nullptr)
		{
			std::vector<Node*> result;
			if (WP_FindShortestPath(g_pStartNode, g_pEndNode, result))
			{
				if (result.size() > 1)
				{
					for (auto it1 = result.begin(), it2 = ++result.begin(); it2 != result.end(); ++it1, ++it2)
					{
						UTIL_BeamPoints(pEntity, (*it1)->GetPosition(), (*it2)->GetPosition(), g_SpriteBeam4, 0, 0, 100, 10, 3, 0, 50, 250, 255, 0);
					}
				}

				CLIENT_PRINTF(pEntity, print_console, UTIL_VarArgs("[MO] found (%d) paths.\n", result.size()));
				RETURN_META(MRES_SUPERCEDE);
			}
		}

		CLIENT_PRINTF(pEntity, print_console, "[MO] path not found.\n");
		RETURN_META(MRES_SUPERCEDE);
	}

	RETURN_META(MRES_IGNORED);
}

void DispatchThink(edict_t* pEntity)
{
	RETURN_META(MRES_IGNORED);
}