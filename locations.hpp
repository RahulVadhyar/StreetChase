//location for all the textures that we are going to use
#define CONTAINER_TEX_DIR "Textures/container.jpg"
#define GUN_TEX_DIR "Textures/gun.png"
#define BULLET_TEX_DIR "Textures/bullet.png"

#define TEXTURE_DIR(dir) "Textures/" dir
#define BACKGROUND_DIR(dir) TEXTURE_DIR("Backgrounds/" dir)
#define OBJECT_DIR(dir) TEXTURE_DIR("Objects/" dir)
#define ANIMATION_DIR(dir) TEXTURE_DIR("Animations/" dir)
namespace texture_dirs{
    namespace background{
        const char* moon_and_stars = BACKGROUND_DIR("moon and stars.png");
        const char* background_buildings = BACKGROUND_DIR("background buildings.png");
        const char* foreground_buildings = BACKGROUND_DIR("foreground buildings.png");
        const char* middle_buildings = BACKGROUND_DIR("middle buildings.png");
        const char* foreground = BACKGROUND_DIR("foreground.png");
        const char* ground = BACKGROUND_DIR("ground.png");
    };

    namespace objects{
        const char* first_aid_kit = OBJECT_DIR("first aid kit.png");
        const char* frag_granade = OBJECT_DIR("frag granade.png");
        const char* med_kit = OBJECT_DIR("med kit.png");
        const char* pistol = OBJECT_DIR("pistol.png");
        const char* shotgun = OBJECT_DIR("shotgun.png");
        const char* sniper = OBJECT_DIR("sniper.png");
        const char* SMG = OBJECT_DIR("SMG.png");
        const char* smoke_granade = OBJECT_DIR("smoke granade.png");
    };

    namespace animations{
        namespace Police{
            const char* walking = ANIMATION_DIR("Police/walking.png");
        };
        namespace FinalBoss{};
        namespace GangMember1{};
        namespace GangMember2{};
        namespace GangMember3{};
    };
};
//location where all the shaders are located
namespace shader_dir{
    namespace vs{
        const char* shader = "Shaders/shader.vs";
        const char* healthbar = "Shaders/healthbar.vs";
        const char* text = "Shaders/TextShader.vs";
        const char* animation = "Shaders/AnimationShader.vs";
    };
    namespace fs{
        const char* shader = "Shaders/shader.fs";
        const char* healthbar = "Shaders/healthbar.fs";
        const char* text = "Shaders/TextShader.fs";
        const char* animation = "Shaders/AnimationShader.fs";
    };
};