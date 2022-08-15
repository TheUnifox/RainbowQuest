#pragma once

#include <map>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <string>
#include <optional>
#include <functional>
#include <unordered_map>
#include <unordered_set>

// Include the modloader header, which allows us to tell the modloader which mod this is, and the version etc.
#include "modloader/shared/modloader.hpp"

// beatsaber-hook is a modding framework that lets us call functions and fetch field values from in the game
// It also allows creating objects, configuration, and importantly, hooking methods to modify their values
#include "beatsaber-hook/shared/utils/logging.hpp"
#include "beatsaber-hook/shared/config/config-utils.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-functions.hpp"
#include "beatsaber-hook/shared/utils/hooking.hpp"
#include "beatsaber-hook/shared/utils/utils.h"

#include "UnityEngine/Mathf.hpp"
#include "UnityEngine/Random.hpp"
#include "UnityEngine/Color.hpp"
#include "UnityEngine/Resources.hpp"

#include "GlobalNamespace/SaberModelController.hpp"
#include "GlobalNamespace/ColorManager.hpp"
#include "GlobalNamespace/ColorScheme.hpp"
#include "GlobalNamespace/BeatEffectSpawner.hpp"
#include "GlobalNamespace/BeatmapEventTypeExtensions.hpp"

#include "HMUI/Touchable.hpp"
#include "HMUI/CurvedCanvasSettings.hpp"
#include "HMUI/CurvedTextMeshPro.hpp"

#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/QuestUI.hpp"

#include "custom-types/shared/types.hpp"
#include "custom-types/shared/macros.hpp"
#include "custom-types/shared/coroutine.hpp"

// Define these functions here so that we can easily read configuration and log information from other files
Configuration& getConfig();
Logger& getLogger();

static struct Config_t {
    bool Lights = true;
    bool Walls = false;
    bool Sabers = false;
    bool Trails = false;
    bool Notes = false;
    bool QSabers = false;
    double SaberASpeed = 1.0;
    double SaberBSpeed = 1.0;
    double SabersStartDiff = 180.0;
    double LightASpeed = 2.0;
    double LightBSpeed = 2.0;
    double LightsStartDiff = 180.0;
    double WallsSpeed = 2.0;
} Config;

enum ColorType
{
    ColorA,
    ColorB,
    None = -1
};

enum NoteType
{
	NoteA,
	NoteB,
	GhostNote,
	Bomb
};