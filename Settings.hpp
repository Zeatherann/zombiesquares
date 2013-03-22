#ifndef SETTINGS_HPP
#define SETTINGS_HPP
class Settings{
public:
    // Variables
    bool Fullscreen;
    unsigned int WindowWidth;
    unsigned int WindowHeight;
    bool ClassicControls;
    map<string,pair<sf::Key::Code,Button*>> Controls;
    // Constructor
    static Settings LoadSettings(const string& SettingsFileName);
    Settings();
    Settings(const Settings& Copy);
    // Destructor
    ~Settings();
    // Functions
    void SaveSettings(const string& SettingsFileName)const;
    // Operators
    Settings& operator=(const Settings& Copy);
};
#endif // SETTINGS_HPP
