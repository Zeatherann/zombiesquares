#include "main.hpp"

Settings Settings::LoadSettings(const string& SettingsFileName){
    ifstream SettingsFile(SettingsFileName.c_str());
    Settings Ret;
    if(!SettingsFile)return Ret;
    YAML::Parser SettingsParser(SettingsFile);
    YAML::Node SettingsNode;
    if(SettingsParser.GetNextDocument(SettingsNode)){
        if(const YAML::Node* Node=SettingsNode.FindValue("Fullscreen"))(*Node)>>Ret.Fullscreen;
        if(const YAML::Node* Node=SettingsNode.FindValue("WindowWidth"))(*Node)>>Ret.WindowWidth;
        if(const YAML::Node* Node=SettingsNode.FindValue("WindowHeight"))(*Node)>>Ret.WindowHeight;
        if(const YAML::Node* Node=SettingsNode.FindValue("ClassicControls"))(*Node)>>Ret.ClassicControls;
        if(const YAML::Node* ControlNode=SettingsNode.FindValue("Controls")){
            if(ControlNode->Type()==YAML::NodeType::Map){
                for(YAML::Iterator Iter=ControlNode->begin();Iter!=ControlNode->end();++Iter){
                    string Key,Value;
                    Iter.first()>>Key;
                    if(Ret.Controls.count(Key)){
                        Iter.second()>>Value;
                        sf::Key::Code FoundKey=Keys.Find(Value);
                        if(FoundKey!=sf::Key::Count){
                            Ret.Controls[Key].first=FoundKey;
                        }
                    }
                }
            }
        }
    }
    SettingsFile.close();
    return Ret;
}

Settings::Settings():Fullscreen(false),WindowWidth(800u),WindowHeight(640u),ClassicControls(false),Controls({{"Move Up",{sf::Key::W,NULL}},{"Move Down",{sf::Key::S,NULL}},{"Move Left",{sf::Key::A,NULL}},{"Move Right",{sf::Key::D,NULL}},{"Shoot Up",{sf::Key::Up,NULL}},{"Shoot Down",{sf::Key::Down,NULL}},{"Shoot Left",{sf::Key::Left,NULL}},{"Shoot Right",{sf::Key::Right,NULL}},{"Pause",{sf::Key::Escape,NULL}}}){}

Settings::Settings(const Settings& Copy):Fullscreen(Copy.Fullscreen),WindowWidth(Copy.WindowWidth),WindowHeight(Copy.WindowHeight),ClassicControls(Copy.ClassicControls),Controls(Copy.Controls){}

Settings::~Settings(){}

void Settings::SaveSettings(const string& SettingsFileName)const{
    ofstream SettingsFile(SettingsFileName.c_str());
    YAML::Emitter SettingsEmitter;
    SettingsEmitter<<YAML::BeginDoc<<YAML::BeginMap<<
    YAML::Key<<"Fullscreen"<<       YAML::Value<<Fullscreen<<
    YAML::Key<<"WindowWidth"<<      YAML::Value<<WindowWidth<<
    YAML::Key<<"WindowHeight"<<     YAML::Value<<WindowHeight<<
    YAML::Key<<"ClassicControls"<<  YAML::Value<<ClassicControls<<
    YAML::Key<<"Controls"<<         YAML::Value<<YAML::BeginMap;
    for(const pair<const string,pair<sf::Key::Code,Button*>>& Iter:Controls){
        SettingsEmitter<<YAML::Key<<Iter.first<<YAML::Value<<Keys.Find(Iter.second.first);
    }
    SettingsEmitter<<YAML::EndMap<<YAML::EndMap<<YAML::EndDoc;
    SettingsFile<<SettingsEmitter.c_str();
    SettingsFile.close();
}

Settings& Settings::operator=(const Settings& Copy){
    if(&Copy!=this){
        Fullscreen=Copy.Fullscreen;
        WindowWidth=Copy.WindowWidth;
        WindowHeight=Copy.WindowHeight;
        ClassicControls=Copy.ClassicControls;
        Controls=Copy.Controls;
    }
    return *this;
}
