#pragma once

#include "ui/IUIElement.h"
#include "ui/playlevel/IPlayLevelScreen.h"
#include "controls/KeyboardControls.h"
#include "audio/AudioManager.h"
#include "audio/AudioCache.h"

class PlayLevelUI: public IUIElement {
    private:
    IPlayLevelScreen* screen;

    AudioManager* audio;
    AudioSource* skidSound;

    IControls* controls;
    bool* keys;

    void updateScroll() {
        int scrollX = screen->getScrollX();
        int scrollY = screen->getScrollY();
        GameLevelZone* zone = screen->getLevel()->getCurrentZone();
        Mario* mario = &zone->getMario();
        if (mario->getX() + scrollX > 128) {
            scrollX = 128 - mario->getX();
        } else if (mario->getX() + scrollX < 112) {
            scrollX = 112 - mario->getX();
        }
        if (mario->getY() + scrollY > 160) {
            scrollY = 160 - mario->getY();
        } else if (mario->getY() + scrollY < 80) {
            scrollY = 80 - mario->getY();
        }

        if (scrollX < IPlayLevelScreen::WINDOW_WIDTH - zone->getWidth() * 16) {
            scrollX = IPlayLevelScreen::WINDOW_WIDTH - zone->getWidth() * 16;
        }
        if (scrollX > 0) {
            scrollX = 0;
        }

        if (scrollY < IPlayLevelScreen::WINDOW_HEIGHT - zone->getHeight() * 16) {
            scrollY = IPlayLevelScreen::WINDOW_HEIGHT - zone->getHeight() * 16;
        }
        if (scrollY > 0) {
            scrollY = 0;
        }

        screen->setScrollX(scrollX);
        screen->setScrollY(scrollY);
    }

    void updateAudioListener() {
        int scrollX = screen->getScrollX();
        int scrollY = screen->getScrollY();

        GameLevelZone* zone = screen->getLevel()->getCurrentZone();
        Mario* mario = &zone->getMario();
        if (mario->isSkidding()) {
            skidSound->setPos(mario->getX(), mario->getY());
            skidSound->play();
        } else {
            skidSound->stop();
        }
        
        float pos[3] = {(float) -scrollX + IPlayLevelScreen::WINDOW_WIDTH / 2, (float) -scrollY + IPlayLevelScreen::WINDOW_HEIGHT / 2, 128};
        audio->setPos(pos);
    }

    public:
    PlayLevelUI(IPlayLevelScreen* screen, AudioManager* audio, bool* keys): screen(screen), audio(audio), keys(keys) {
        controls = new KeyboardControls(keys);

        skidSound = new AudioSource(true, false);
        skidSound->setBuffer(AudioCache::audio["smas:skid"]);
    }

    void loadLevelZone() {
        GameLevelZone* zone = screen->getLevel()->getCurrentZone();
        for (int i = 0; i < GameObject::NUM_LAYERS; i++) {
            IGameLevelRegion* region = zone->getRegions()[i];
            for (int x = 0; x < region->getWidth(); x++) {
                for (int y = 0; y < region->getHeight(); y++) {
                    GameObject* obj = region->getGridObject(x, y);
                    if (obj->isFlag(GameObject::ENTITY)) {
                        region->setGridObject(GameObjectCache::objects["air"], x, y);
                        obj->onEntityReplace(x, y, *region);
                    }
                }
            }
        }
    }

    void tick() {
        GameLevelZone* zone = screen->getLevel()->getCurrentZone();
        Mario* mario = &zone->getMario();
        IMario::PlayState playState = mario->getPlayState();
        for (int i = 0; i < GameObject::NUM_LAYERS; i++) {
            std::vector<IEntity*> shouldDelete;
            for (IEntity* entity : zone->getRegions()[i]->getEntities()) {
                if (playState == IMario::PLAY) {
                    entity->tick(*zone, *audio, *controls);
                }
                if (entity->shouldDelete()) {
                    shouldDelete.push_back(entity);
                }
            }
            for (IEntity* entity : shouldDelete) {
                zone->getRegions()[i]->removeEntity(entity);
            }
        }

        mario->tick(*zone, *audio, *controls);

        if (playState == IMario::PLAY) {
            for (int i = 0; i < GameObject::NUM_LAYERS; i++) {
                IGameLevelRegion* region = zone->getRegions()[i];
                for (IEntity* entity : region->getEntities()) {
                    if (!entity->isSolid()) {
                        for (IEntity* entityTouched : region->getEntities()) {
                            if (entity->getCollisionBox().collidesWith(entityTouched->getCollisionBox(), entityTouched->getX() - entity->getX(), entityTouched->getY() - entity->getY())) {
                                entity->onCollideEntity(*entityTouched);
                            }
                        }
                        if (entity->getCollisionBox().collidesWith(mario->getCollisionBox(), mario->getX() - entity->getX(), mario->getY() - entity->getY())) {
                            entity->onCollideMario(*mario);
                        }
                        continue;
                    }
                    for (IEntity* entityPushed : region->getEntities()) {
                        if (!entityPushed->isPushable()) {
                            continue;
                        }
                        int eX = entityPushed->getX();
                        int eY = entityPushed->getY();
                        if (entity->getCollisionBox().pushBoxAway(entity->getX(), entity->getY(), entityPushed->getCollisionBox(), eX, eY)) {
                            int eXPrev = entityPushed->getX();
                            int eYPrev = entityPushed->getY();
                            entityPushed->setX(eX);
                            entityPushed->setY(eY);
                            entityPushed->onPushed(*entity, eX - eXPrev, eY - eYPrev);
                        }
                    }
                    if (mario->getZoneLayer() != i) {
                        continue;
                    }
                    int mX = mario->getX();
                    int mY = mario->getY();
                    if (entity->getCollisionBox().pushBoxAway(entity->getX(), entity->getY(), mario->getCollisionBox(), mX, mY)) {
                        mario->setX(mX);
                        mario->setY(mY);
                    }
                }
            }
        }

        updateScroll();
        updateAudioListener();
    }

    virtual void charInput(int codepoint) override {
        if (codepoint == GLFW_KEY_ESCAPE) {
            screen->exitToEditor();
        }
    }

    ~PlayLevelUI() {
        delete controls;

        delete skidSound;
    }
};