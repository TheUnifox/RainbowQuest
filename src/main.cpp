#include "main.hpp"
#include "ModConfig.hpp"

static ModInfo modInfo; // Stores the ID and version of our mod, and is sent to the modloader upon startup
DEFINE_CONFIG(RainbowConfig);

// Returns a logger, useful for printing debug messages
Logger& getLogger() {
    static Logger* logger = new Logger(modInfo);
    return *logger;
}

float mainColor1Hue = 0;
float mainColor2Hue = 180;
float obstacleColorHue = 0;

UnityEngine::Color mainColor1;
UnityEngine::Color mainColor2;
UnityEngine::Color obstacleColor;

// Called at the early stages of game loading
extern "C" void setup(ModInfo& info) {
    info.id = ID;
    info.version = VERSION;
    modInfo = info;
	
    getLogger().info("Completed setup!");
}


UnityEngine::Color ColorFromHSV(float h, float s, float v){
    h/=360.0f;
    s/=100.0f;
    v/=100.0f;
    UnityEngine::Color color;
    color = UnityEngine::Color::HSVToRGB(h, s, v);
    return color;
}

UnityEngine::Color GetLinearColor(UnityEngine::Color color){
    UnityEngine::Color linearColor;
    linearColor.r = UnityEngine::Mathf::GammaToLinearSpace(color.r);
    linearColor.g = UnityEngine::Mathf::GammaToLinearSpace(color.g);
    linearColor.b = UnityEngine::Mathf::GammaToLinearSpace(color.b);
    linearColor.a = color.a;
    return linearColor;
}

bool first = false;
//getting an update function thats only in gameplay scenes, and always in gameplay scenes
//using something called every frame to update the colors every frame
MAKE_HOOK_MATCH(BeatEffectSpawner_Update, &GlobalNamespace::BeatEffectSpawner::Update, void, GlobalNamespace::BeatEffectSpawner* self){
    if(getRainbowConfig().RainbowEnabled.GetValue()){
        if(first){
            //seed rand and get a random hue for obstacles ig
            std::srand(std::time(0));
            obstacleColorHue = (rand() % 360);
        }
        ArrayW<GlobalNamespace::SaberModelController*> saberModelControllers =  UnityEngine::Resources::FindObjectsOfTypeAll<GlobalNamespace::SaberModelController*>();
        GlobalNamespace::SaberModelController* selectedController = saberModelControllers[0];
        GlobalNamespace::ColorManager* colorManager = selectedController->colorManager;
        GlobalNamespace::ColorScheme* colorScheme = colorManager->colorScheme;
        float incrementval = getRainbowConfig().RainbowSpeed.GetValue();
        (mainColor1Hue < 360) ? (mainColor1Hue += incrementval) : (mainColor1Hue = 0);
        (mainColor2Hue < 360) ? (mainColor2Hue += incrementval) : (mainColor2Hue = 0);
        (obstacleColorHue < 360) ? (obstacleColorHue += incrementval) : (obstacleColorHue = 0);
        getLogger().info("color hues are: %f, %f, and %f", mainColor1Hue, mainColor2Hue, obstacleColorHue);
        UnityEngine::Color mainColor1 = GetLinearColor(ColorFromHSV(mainColor1Hue , 100, 100));
        UnityEngine::Color mainColor2 = GetLinearColor(ColorFromHSV(mainColor2Hue , 100, 100));
        UnityEngine::Color obstacleColor =  GetLinearColor(ColorFromHSV(obstacleColorHue , 100, 100));
        getLogger().info("colors are: main color 1: %f, %f, %f, %f main color 2: %f, %f, %f, %f obstacle color:, %f, %f, %f, %f", mainColor1.r, mainColor1.g, mainColor1.b, mainColor1.a, mainColor2.r, mainColor2.g, mainColor2.b, mainColor2.a, obstacleColor.r, obstacleColor.g, obstacleColor.b, obstacleColor.a);

        mainColor1.a = colorScheme->saberAColor.a;
        mainColor2.a = colorScheme->saberBColor.a;
        obstacleColor.a = colorScheme->obstaclesColor.a;
        getLogger().info("alpha adjusted colors are: main color 1: %f, %f, %f, %f main color 2: %f, %f, %f, %f obstacle color:, %f, %f, %f, %f", mainColor1.r, mainColor1.g, mainColor1.b, mainColor1.a, mainColor2.r, mainColor2.g, mainColor2.b, mainColor2.a, obstacleColor.r, obstacleColor.g, obstacleColor.b, obstacleColor.a);

        getLogger().info("before colors are:  main color 1: %f, %f, %f, %f main color 2: %f, %f, %f, %f obstacle color:, %f, %f, %f, %f", colorScheme->saberAColor.r, colorScheme->saberAColor.g, colorScheme->saberAColor.b, colorScheme->saberAColor.a, colorScheme->saberBColor.r, colorScheme->saberBColor.g, colorScheme->saberBColor.b, colorScheme->saberBColor.a, colorScheme->obstaclesColor.r, colorScheme->obstaclesColor.g, colorScheme->obstaclesColor.b, colorScheme->obstaclesColor.a);
        colorScheme->saberAColor = mainColor1;
        colorScheme->saberBColor = mainColor2;
        colorScheme->obstaclesColor = obstacleColor;
        getLogger().info("after colors are:  main color 1: %f, %f, %f, %f main color 2: %f, %f, %f, %f obstacle color:, %f, %f, %f, %f", colorScheme->saberAColor.r, colorScheme->saberAColor.g, colorScheme->saberAColor.b, colorScheme->saberAColor.a, colorScheme->saberBColor.r, colorScheme->saberBColor.g, colorScheme->saberBColor.b, colorScheme->saberBColor.a, colorScheme->obstaclesColor.r, colorScheme->obstaclesColor.g, colorScheme->obstaclesColor.b, colorScheme->obstaclesColor.a);
        //might as well try to set environment colors, 99% sure it doesnt change them tho
        mainColor1.a = colorScheme->environmentColor0.a;
        mainColor2.a = colorScheme->environmentColor1.a;

        colorScheme->environmentColor0 = mainColor1;
        colorScheme->environmentColor1 = mainColor2;

        colorManager->colorScheme = colorScheme;

        BeatEffectSpawner_Update(self);
    }
}

void DidActivate(HMUI::ViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    getLogger().info("settings menu");
    // Create our UI elements only when shown for the first time.
    if(firstActivation) {
        getLogger().info("creating container");
        // Create a container that has a scroll bar.
        self->get_gameObject()->AddComponent<HMUI::Touchable*>();
        UnityEngine::GameObject* settings = QuestUI::BeatSaberUI::CreateScrollableSettingsContainer(self->get_transform());

        getLogger().info("creating modal");
        HMUI::ModalView* settingsModal = QuestUI::BeatSaberUI::CreateModal(settings->get_transform(), UnityEngine::Vector2(120.0f, 80.0f), [](HMUI::ModalView* modal){}, true);
        UnityEngine::GameObject* settingsm = QuestUI::BeatSaberUI::CreateScrollableModalContainer(settingsModal);
        
        getLogger().info("creating text");
        QuestUI::BeatSaberUI::CreateText(settingsm->get_transform(), "Rainbow Settings");

        getLogger().info("creating toggle");
        QuestUI::BeatSaberUI::CreateToggle(settingsm->get_transform(), "Rainbow Enabled", getRainbowConfig().RainbowEnabled.GetValue(),
            [](bool value) { 
                getRainbowConfig().RainbowEnabled.SetValue(value);
            });

        getLogger().info("creating slider");
        QuestUI::BeatSaberUI::CreateSliderSetting(settingsm->get_transform(), "Rainbow Speed", 0.1f, getRainbowConfig().RainbowSpeed.GetValue(), 0.0f, 1.0f,
            [](float value) { 
                getRainbowConfig().RainbowSpeed.SetValue(value);
            });

        getLogger().info("creating settings toggle");
        QuestUI::BeatSaberUI::CreateToggle(settings->get_transform(), "Settings menu", false,
            [settingsModal](bool value) { 
                getLogger().info("toggling settings");
                if (value){
                    settingsModal->Show(true, true, nullptr);
                }
                else{
                    settingsModal->Hide(true, nullptr);
                }
            });
        
        getLogger().info("done settings menu");
    }
}

// Called later on in the game loading - a good time to install function hooks
extern "C" void load() {
    il2cpp_functions::Init();
    
    getLogger().info("initing config");
    getRainbowConfig().Init(modInfo);

    getLogger().info("initing questui");
    QuestUI::Init();
    getLogger().info("registering settings ui");
	QuestUI::Register::RegisterModSettingsViewController(modInfo, DidActivate);
    QuestUI::Register::RegisterMainMenuModSettingsViewController(modInfo, DidActivate);
    getLogger().info("Successfully installed Settings UI!");

    getLogger().info("Starting RainbowQuest installation...");
    INSTALL_HOOK(getLogger(), BeatEffectSpawner_Update);
    getLogger().info("Successfully installed RainbowQuest!");
}