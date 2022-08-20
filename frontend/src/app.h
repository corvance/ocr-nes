#include <SFML/Graphics.hpp>

#include "emulator.h"
#include "gui/gui.h"
#include "input/input.h"
#include "config.h"

class App
{
public:
    const int NES_SCREEN_WIDTH = 256;
    const int NES_SCREEN_HEIGHT = 240;

    void Start(int argc, char** argv);

    // Location of the binary derived from argv[0],
    // to avoid having to hardcode relative paths.
    std::string binaryDir;

    Emulator emulator;
    sf::RenderWindow window;
    Input input;

    bool emuPaused = false;

    // Current and previous window scale to avoid unneccesary window resize calls.
    // This is set by ImGui and is therefore 0-indexed, so a value of 0 is 1x scale.
    int windowScale = 2;
    int windowScalePrev = -1;

    // The currently selected save state slot.
    int stateSlot = 0;
    // The current screenshot ID.
    int screenshotID = 0;
    // For triggering a screenshot scheduled via the pause menu.
    bool pendingScreenshot = false;

    // The number of frames to skip rendering for every frame (0 - 9).
    // This can increase FPS, especially when playing without a speed cap.
    int frameSkip = 0;
    int frameSkipCounter = -1;

    /**
     * @brief Saves a save state in the given slot.
     *
     * @param slot The save slot to save in.
     */
    void saveState(u8 slot);

    /**
     * @brief Loads a save state from the given slot.
     *
     * @param slot The save slot to load from.
     *
     * @return true Load was successful.
     * @return true Load was unsuccessful, e.g., no save in this slot.
     */
    bool loadState(u8 slot);

    /**
     * @brief Saves a screenshot with the current screenshot ID, and increments the screenshot ID ready for the next.
     */
    void saveScreenshot(sf::RenderWindow& window);
};