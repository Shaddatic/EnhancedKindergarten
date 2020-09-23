#include "stdafx.h"
#include "LandTableInfo.h"
#include "SA2ModLoader.h"
#include "TextConv.h"
#include "IniFile.h"

#pragma pack(push, 8)
struct __declspec(align(4)) KinderDoorThing
{
	int nameID;
	int nameIDThing;
	int doorIndex;
	NJS_VECTOR position;
	int rotation;
};
#pragma pack(pop)

struct KinderSpawnThing { float posX; float posZ; int rotY; };

extern "C"
{

	NJS_TEXNAME dckindergartena[57];
	NJS_TEXLIST dckind = { arrayptrandlength(dckindergartena) };

	NJS_TEXNAME dckindexttxcount[43];
	NJS_TEXLIST dckindextlist = { arrayptrandlength(dckindexttxcount) };

	__declspec(dllexport) void Init(const char* path, const HelperFunctions& helperFunctions)
	{
		const IniFile* config = new IniFile(std::string(path) + "\\config.ini");

		HMODULE hmodule = GetModuleHandle(__TEXT("Data_DLL_orig"));

		LandTable* DCKindExt = (LandTable*)GetProcAddress(hmodule, "objLandTableKinderHo");
		*DCKindExt = *(new LandTableInfo(std::string(path) + "\\Landtables\\EKindExt.sa2blvl"))->getlandtable();
		DCKindExt->TextureList = &dckindextlist;
		DCKindExt->TextureName = (char*)"al_stg_kinder_ho_tex";

		bool enable = config->getBool("GenSet", "Enable", false);
		if (enable == false)
		{
			LandTable* DCKind = (LandTable*)GetProcAddress(hmodule, "objLandTableKinderCo");
			*DCKind = *(new LandTableInfo(std::string(path) + "\\Landtables\\EKind.sa2blvl"))->getlandtable();
			DCKind->TextureList = &dckind;
			DCKind->TextureName = (char*)"al_stg_kinder_co_tex_dc";

			DataArray(KinderDoorThing, doors, 0x008A1A50, 6);

			//doors[0].position.x = 0.0; //Principle
			//doors[1].position.x = 0.0; //HealthCenter
			//doors[2].position.x = 0.0; //Classroom
			//doors[4].position.x = 0.0; //FortuneTeller
			//doors[5].position.x = 0.0; //Exit

			doors[3].position.x = 87.43; //BlackMarket
			doors[3].position.z = 15.25;
			doors[3].rotation = 47325;

			DataArray(KinderSpawnThing, spawns, 0x008A19F0, 7);

			spawns[0].posX = 0.0; //FirstEnter
			spawns[0].posZ = 3.0;
			spawns[1].posX = 71.59; //BlackMarket
			spawns[1].posZ = 11.91;
			spawns[1].rotY = 1800;
			//spawns[2].posX = 0.0; //Billboard
			spawns[3].posX = 48.90; //Classroom
			spawns[3].posZ = 58.545;
			spawns[3].rotY = 9092;
			spawns[4].posX = 0.0; //Principal
			spawns[4].posZ = 76.36;
			spawns[4].rotY = 16384;
			spawns[5].posX = -49.44; //HealthCenter
			spawns[5].posZ = 58.475;
			spawns[5].rotY = 23664;
			spawns[6].posX = -75.19; //FortuneTeller
			spawns[6].posZ = 13.37;
			spawns[6].rotY = 31029;

			DataPointer(double, billocX, 0x9080E8); //Moves Billboard Interaction Location
			DataPointer(double, billocZ, 0x9080E0);

			billocX = 41.408;
			billocZ = -32.0;

			bool wind = config->getBool("GenSet", "Wind", false);
			if (wind == true)
			{
				DCKind->COLList[0].Flags &= ~0x80000000;
				DCKind->COLList[1].Flags &= ~0x80000000;
			}

			bool tabl = config->getBool("GenSet", "Table", false);
			if (tabl == true)
			{
				DCKind->COLList[2].Flags &= ~0x80000000;
				DCKind->COLList[3].Flags &= ~0x80000000;
				DCKind->COLList[4].Flags &= ~0x80000000;
				DCKind->COLList[29].Flags &= ~1;
			}

			bool billbo = config->getBool("GenSet", "Billbo", false);
			if (billbo == true)
			{
				DCKind->COLList[5].Flags &= ~0x80000000;
				billocX = 200.0;
				billocZ = 200.0;
			}
		}
	}

	__declspec(dllexport) ModInfo SA2ModInfo = { ModLoaderVer };
}
