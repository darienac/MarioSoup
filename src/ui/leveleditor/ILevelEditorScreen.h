#pragma once

#include "game/GameLevel.h"
#include "screens/IScreen.h"

class ConfirmPopup;
class TextInputPopup;

class ILevelEditorScreen: public IScreen {
    public:
    enum UIState {
        EDITOR, SAVE_DIALOG, OPEN_DIALOG, INFO_POPUP, CONFIRM_DIALOG, RESIZE_DIALOG, TEXT_INPUT_DIALOG
    };

    virtual void initLevel() = 0;

    virtual UIState getState() = 0;
    virtual void setState(UIState state) = 0;
    virtual GameLevel* getLevel() = 0;
    virtual void setLevel(GameLevel* level) = 0;
    virtual void setSavePath(std::string path) = 0;
    virtual std::string& getSavePath() = 0;
    virtual void setChangesSaved(bool value) = 0;
    virtual bool getChangesSaved() = 0;
    virtual void setAutosave(bool value) = 0;
    virtual void attemptAutosave() = 0;

    virtual void setInfoMessage(const char* message) = 0;
    virtual char* getFileInputBuffer() = 0;
    virtual ConfirmPopup* getConfirmPopup() = 0;
    virtual TextInputPopup* getTextInputPopup() = 0;

    virtual bool isFullscreen() = 0;
    virtual void setFullscreen(bool value) = 0;

    virtual void runLevel() = 0;
    virtual void closeWindow() = 0;
};