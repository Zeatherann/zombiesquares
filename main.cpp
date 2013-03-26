#include "main.hpp"
const sf::Color HighLight(32,32,32);
maze Maze;
int GameTime=0;
pairi Adj[8]={pairi(-1,0),pairi(1,0),pairi(0,-1),pairi(0,1),pairi(1,1),pairi(-1,1),pairi(-1,-1),pairi(1,-1)};
char MenuMode=2;
float Awake=1.f;
set<Menu*> Menus;
KeyMap Keys;
Settings Config=Settings::LoadSettings("Settings.yml");
float WinWidth=Config.WindowWidth;
float WinHeight=Config.WindowHeight;
string BindKey="";
sf::Font Font;

int main(){
    float UIScale=1.f;
    srand(time(NULL));
    // Main Variables
    int X=0;
    int Y=0;
    int Tick=30;
    sf::Shape Squares[TileNum];
    sf::RenderWindow App(sf::VideoMode(WinWidth,WinHeight),"Zombie Squares",Config.Fullscreen?sf::Style::Fullscreen:sf::Style::Titlebar|sf::Style::Resize|sf::Style::Close);
    sf::View Cam(sf::FloatRect(0.f,0.f,WinWidth,WinHeight));
    sf::Event Events;
    ColorsInit();
    sf::Color Tiles[TileNum]={Colors["floor"],Colors["wall"],Colors["points"],Colors["starting wall"]};
    vector<sf::Shape> UIShapes;
    const sf::Input& Input=App.GetInput();
    UIElement::State CurState(Input);
    float Ratio;
    float Sight;
    float SightWidth;
    float SightHeight;
    sf::FloatRect GameView;
    bool UpdateUI=true;
    bool UpdateView=true;
    { /// Create UI Shapes
        sf::Shape S;
        sf::Color C = Colors["lazer"]+HighLight;
        S.SetOutlineWidth(1.f);
        S.AddPoint({TileSize*0.125f,TileSize*0.125f},Colors["lazer"],C);
        S.AddPoint({TileSize*0.875f,TileSize*0.125f},Colors["lazer"],C);
        S.AddPoint({TileSize*0.875f,TileSize*0.875f},Colors["lazer"],C);
        S.AddPoint({TileSize*0.125f,TileSize*0.875f},Colors["lazer"],C);
        UIShapes.push_back(S);
        S=sf::Shape();
    }
    sf::String ScoreTxt("Score: 0",Font,TileSize);
    sf::String HighScore("HighScore: 0",Font,TileSize);
    sf::PostFX MenuGray;
    map<string,pairi> MoveKeys={{"Move Up",pairi(0,-1)},{"Move Down",pairi(0,1)},{"Move Left",pairi(-1,0)},{"Move Right",pairi(1,0)}};
    map<string,pairi> ShootKeys={{"Shoot Up",pairi(0,-1)},{"Shoot Down",pairi(0,1)},{"Shoot Left",pairi(-1,0)},{"Shoot Right",pairi(1,0)}};
    App.SetView(Cam);
    App.SetFramerateLimit(30);
    for(unsigned int i=0u;i<TileNum;i++)Squares[i]=sf::Shape::Rectangle({1,1},{TileSize-2,TileSize-2},Tiles[i],1.f,Tiles[i]+HighLight);
    if(!Font.LoadFromFile("Font.ttf",(unsigned int)TileSize)){
        cout<<"Unable to load font \'Font.ttf\'!"<<endl;
        exit(11);
    }
    ((sf::Image&)Font.GetImage()).SetSmooth(false);
    if(!MenuGray.LoadFromFile("MenuEffect.sfx")){
        cout<<"Unable to load file \'MenuEffect.sfx\'!"<<endl;
        exit(10);
    }
    MenuGray.SetTexture("framebuffer",NULL);
    // Create Interface
    Menu* MenuPause;
    Menu* MenuMain;
    Menu* MenuNewGame;
    Menu* MenuSaveGame;
    Menu* MenuGameOver;
    Menu* MenuSettings;
    {
        sf::Vector2f Size(TileSize*10,TileSize);
        sf::Vector2f Location(WinWidth*0.25f,WinHeight*0.25f);
        MenuPause=new Menu(Location,Size,0.f,sf::String("Paused",Font),sf::Shape::Rectangle({},{},sf::Color::Black,1.f,sf::Color(0,0,255)));
        MenuPause->Buffer=TileSize*0.25f;
        Size.x-=2.f;
        Size.y-=2.f;
        MenuPause->AddButton(ButtonStyle(new Button({},Size,sf::String("Resume Game",Font),[&]{MenuMode=0;HideMenus();},sf::Key::R,'R'),sf::Color(0,0,255,128)),1);
        MenuPause->AddButton(ButtonStyle(new Button({},Size,sf::String("Save Game",Font),[&]{Save("SaveGame.zs");},sf::Key::S,'S'),sf::Color(0,0,255,128)),1);
        MenuPause->AddButton(ButtonStyle(new Button({},Size,sf::String("Exit to Menu",Font),[&]{MenuMode=-2;ShowMenu(MenuSaveGame);},sf::Key::E,'E'),sf::Color(0,0,255,128)),1);
        MenuPause->AddButton(ButtonStyle(new Button({},Size,sf::String("Exit Game",Font),[&]{MenuMode=-1;ShowMenu(MenuSaveGame);},sf::Key::X,'x'),sf::Color(0,0,255,128)),1);
        Size.x+=2.f;
        Size.y+=2.f;
        MenuMain=new Menu(Location,Size,0.f,sf::String("Zombie Squares",Font),sf::Shape::Rectangle({},{},sf::Color::Black,1.f,sf::Color(0,0,255)),false);
        MenuMain->Buffer=TileSize*0.25f;
        Size.x-=2.f;
        Size.y-=2.f;
        MenuMain->AddButton(ButtonStyle(new Button({},Size,sf::String("New Game",Font),[&]{ShowMenu(MenuNewGame);},sf::Key::N,'N'),sf::Color(0,0,255,128)),1);
        MenuMain->AddButton(ButtonStyle(new Button({},Size,sf::String("Load Game",Font),[&]{Entity::Clear();new Player(ScoreTxt,HighScore);Load("SaveGame.zs");MenuMode=1;ShowMenu(MenuPause);},sf::Key::L,'L'),sf::Color(0,0,255,128)),1);
        MenuMain->AddButton(ButtonStyle(new Button({},Size,sf::String("Create Maze",Font),NULL,sf::Key::C,'C'),sf::Color(255,0,0,128)),1);
        MenuMain->AddButton(ButtonStyle(new Button({},Size,sf::String("Highscores",Font),NULL,sf::Key::H,'H'),sf::Color(255,0,0,128)),1);
        MenuMain->AddButton(ButtonStyle(new Button({},Size,sf::String("Settings",Font),[&]{ShowMenu(MenuSettings);MenuMode=4;},sf::Key::S,'S'),sf::Color(0,0,255,128)),1,1);
        MenuMain->AddButton(ButtonStyle(new Button({},Size,sf::String("Exit Game",Font),[&]{App.Close();},sf::Key::E,'E'),sf::Color(0,0,255,128)),1);
        Size.x+=2.f;
        Size.y+=2.f;
        MenuNewGame=new Menu(Location,Size,0.f,sf::String("New Game",Font),sf::Shape::Rectangle({},{},sf::Color::Black,1.f,sf::Color(0,0,255)),false);
        MenuNewGame->Buffer=TileSize*0.25f;
        Size.x-=2.f;
        Size.y-=2.f;
        MenuNewGame->AddButton(ButtonStyle(new Button({},Size,sf::String("Random Maze",Font),[&]{UpdateView=UpdateUI=true;NewMaze();Awake=0.f;new Player(ScoreTxt,HighScore);MenuMode=0;HideMenus();},sf::Key::R,'R'),sf::Color(0,0,255,128)),1);
        MenuNewGame->AddButton(ButtonStyle(new Button({},Size,sf::String("Load Maze",Font),NULL,sf::Key::L,'L'),sf::Color(255,0,0,128)),1);
        MenuNewGame->AddButton(ButtonStyle(new Button({},Size,sf::String("Cancel",Font),[&]{MenuMode=2;ShowMenu(MenuMain);},sf::Key::C,'C'),sf::Color(0,0,255,128)),1);
        Size.x+=2.f;
        Size.y+=2.f;
        MenuSaveGame=new Menu(Location,Size,0.f,sf::String("Save Before Exiting",Font),sf::Shape::Rectangle({},{},sf::Color::Black,1.f,sf::Color(0,0,255)),false);
        MenuSaveGame->Buffer=TileSize*0.25f;
        Size.x-=2.f;
        Size.y-=2.f;
        MenuSaveGame->AddButton(ButtonStyle(new Button({},Size,sf::String("Save Maze",Font),[&]{Save("SaveGame.zs");Maze.clear();Entity::Clear();if(MenuMode==-1)App.Close();else ShowMenu(MenuMain);},sf::Key::S,'S'),sf::Color(0,0,255,128)),1);
        MenuSaveGame->AddButton(ButtonStyle(new Button({},Size,sf::String("Don't Save Maze",Font),[&]{Maze.clear();Entity::Clear();if(MenuMode==-1)App.Close();else ShowMenu(MenuMain);},sf::Key::D,'D'),sf::Color(0,0,255,128)),1);
        MenuSaveGame->AddButton(ButtonStyle(new Button({},Size,sf::String("Cancel",Font),[&]{MenuMode=1;ShowMenu(MenuPause);},sf::Key::C,'C'),sf::Color(0,0,255,128)),1);
        Size.x+=2.f;
        Size.y+=2.f;
        MenuGameOver=new Menu(Location,Size,0.f,sf::String("Game Over",Font),sf::Shape::Rectangle({},{},sf::Color::Black,1.f,sf::Color(0,0,255)),false);
        MenuGameOver->Buffer=TileSize*0.25f;
        Size.x-=2.f;
        Size.y-=2.f;
        MenuGameOver->AddButton(ButtonStyle(new Button({},Size,sf::String("Play Again",Font),[&]{ShowMenu(MenuMain);MenuMode=2;},sf::Key::P,'P'),sf::Color(0,0,255,128)),1);
        MenuGameOver->AddButton(ButtonStyle(new Button({},Size,sf::String("Exit",Font),[&]{App.Close();},sf::Key::E,'E'),sf::Color(0,0,255,128)),1);        Size.x+=2.f;
        Size.x+=2.f;
        Size.y+=2.f;
        MenuSettings=new Menu(Location,Size,0.f,sf::String("Settings",Font),sf::Shape::Rectangle({},{},sf::Color::Black,1.f,sf::Color(0,0,255)),false);
        MenuSettings->Buffer=TileSize*0.25f;
        Size.x=Size.x*0.5f-2.f;
        Size.y-=2.f;
        AddKeyBinding("Move Up",0,MenuSettings,Size);
        AddKeyBinding("Move Down",0,MenuSettings,Size);
        AddKeyBinding("Move Left",0,MenuSettings,Size);
        AddKeyBinding("Move Right",0,MenuSettings,Size);
        AddKeyBinding("Shoot Up",2,MenuSettings,Size);
        AddKeyBinding("Shoot Down",2,MenuSettings,Size);
        AddKeyBinding("Shoot Left",2,MenuSettings,Size);
        AddKeyBinding("Shoot Right",2,MenuSettings,Size);
        AddKeyBinding("Pause",0,MenuSettings,Size);
        Size.x=2.f+Size.x*2.f;
        MenuSettings->AddButton(ButtonStyle(new Button({},Size,sf::String("Back to Main Menu",Font),[&]{MenuMode=2;ShowMenu(MenuMain);}),sf::Color(0,0,255,128)),0);
    }
    Menus={MenuPause,MenuMain,MenuNewGame,MenuSaveGame,MenuGameOver,MenuSettings};
    ShowMenu(MenuMain);
    // Game Loop
    while(App.IsOpened()){
        // Update Time
        if(MenuMode==0){
            if(Awake<1.f)Awake+=0.025f;
            if(--Tick<=0){
                Tick=30;
                GameTime++;
            }
        }
        // Clear Old Scene;
        App.Clear();
        while(App.GetEvent(Events)){ // EVENTS
            EventSubscriptions::process(Events, CurState);
            if(MenuMode!=0&&UIElement::TrackedEvents.count(Events.Type)){
                UpdateUI=true;
            }
            if(BindKey!=""&&Events.Type==sf::Event::KeyPressed){
                if(Config.Controls.count(BindKey)){
                    pair<sf::Key::Code,Button*>& KeyBinding=Config.Controls.find(BindKey)->second;
                    KeyBinding.first=Events.Key.Code;
                    if(KeyBinding.second){
                        stringstream Stream(ios::in|ios::out);
                        Stream<<(string)KeyBinding.second->Text.GetText();
                        string Base;
                        getline(Stream,Base,':');
                        KeyBinding.second->SetText(Base+": "+Keys.Find(Events.Key.Code));
                    }
                }
                BindKey="";
            }else{
                switch(Events.Type){
                    case sf::Event::Closed:{
                        App.Close();
                        break;
                    }case sf::Event::Resized:{
                        WinWidth=Events.Size.Width;
                        WinHeight=Events.Size.Height;
                        UpdateView=true;
                        break;
                    }case sf::Event::MouseWheelMoved:{
                        int Old=Player::SightRadius+Events.MouseWheel.Delta;
                        if(Old>=0)Player::SightRadius=Old;
                        UpdateView=true;
                        break;
                    }case sf::Event::KeyPressed:{
                        sf::Key::Code Key=Events.Key.Code;
                        if(Key==Config.Controls["Pause"].first){
                            if(MenuMode==0){
                                MenuMode=1;
                                ShowMenu(MenuPause);
                            }else if(MenuMode==1){
                                MenuMode=0;
                                HideMenus();
                            }
                        }else if(MenuMode==0){
                            for(pair<const string,pair<sf::Key::Code,Button*>>& Iter:Config.Controls){
                                if(Iter.second.first==Key){
                                    if(Iter.first[0]=='S'){
                                        Player::Character->Shoot(ShootKeys[Iter.first]);
                                    }else if(Iter.first[0]=='M'&&Player::Character->Move(MoveKeys[Iter.first])){
                                        UpdateView=true;
                                    }
                                }
                            }
                        }
                        break;
                    }default:{break;}
                }
            }
        }// END EVENTS
        if(UpdateView){
            UpdateView=false;
            if(Player::Character){
                X=Player::Character->X;
                Y=Player::Character->Y;
            }
            Ratio=WinHeight/WinWidth;
            Sight=(Player::SightRadius+1)*TileSize;
            SightWidth=Sight*(Ratio>1.f?1.f:1/Ratio);
            SightHeight=Sight*(Ratio>1.f?Ratio:1.f);
            UIScale=(Ratio>1.f?WinWidth/(Sight*2):WinHeight/(Sight*2));
            // Move Interfaces
            GameView=sf::FloatRect(X*TileSize-SightWidth,Y*TileSize-SightHeight,(X+1)*TileSize+SightWidth,(Y+1)*TileSize+SightHeight);
            sf::Vector2f NewLoc((WinWidth-MenuPause->Size.x)*0.5f,(WinHeight-MenuPause->GetHeight())*0.5f);
            for(Menu* Iter:Menus)Iter->Move(sf::Vector2f(-Sight+TileSize,-Sight+TileSize*2.f));
        }
        // Check Window
        if(!App.IsOpened())break;
        // Update Game View
        if(Player::Character){
            Cam.SetFromRect(GameView);
            // Draw Terrain
            int SR=Player::SightRadius+1;
            for(int x=X-SR;x<=X+SR;x++){
                for(int y=Y-SR;y<=Y+SR;y++){
                    pairi Loc(x,y);
                    if(Player::Character->InSight(Loc)){
                        unsigned int Index=GetTile(Maze,x,y);
                        sf::Shape& Sq=Squares[Index];
                        char SightValue=Player::Character->GetSight(Loc);
                        float Alpha=Awake*(1.f-(float(SightValue)/float(Player::SightRadius+1)));
                        if(Alpha>1.f)Alpha=1.f;
                        if(Alpha<0.f)Alpha=0.f;
                        Sq.SetColor(sf::Color(255,255,255,255.f*Alpha));
                        Sq.SetPosition(x*TileSize,y*TileSize);
                        App.Draw(Sq);
                    }
                }
            }
        }
        // Update and Draw Entities
        Entity::Tick(App);
        if(!Player::Character&&MenuMode==0){
            MenuMode=3;
            ShowMenu(MenuGameOver);
        }
        Cam.SetFromRect(sf::FloatRect(-SightWidth,-SightHeight,SightWidth,SightHeight));
        if(UpdateUI){
            UpdateUI=false;
            CurState.Mouse=App.ConvertCoords(Input.GetMouseX(),Input.GetMouseY());
        }
        // Interface
        if(MenuMode!=0){
            App.Draw(MenuGray);
            UIGroup::UpdateAll(CurState,App);
        }
        // Draw Bullets
        if(Player::Character){
            int MaxBullets=3;
            int Shots=Player::Character->Shots;
            if(Shots>MaxBullets)MaxBullets=Shots;
            float W=MaxBullets*TileSize-Sight;
            ScoreTxt.SetPosition(W,-Sight);
            W+=ScoreTxt.GetRect().GetWidth();
            HighScore.SetPosition(W+2,-Sight);
            App.Draw(sf::Shape::Rectangle(-Sight,-Sight,Sight,TileSize-Sight,sf::Color::Black));
            App.Draw(sf::Shape::Rectangle(1-Sight,1-Sight,Sight-1,Sight-1,sf::Color(0,0,0,0),1.f,sf::Color::White));
            for(int a=0;a<Shots;a++){
                UIShapes[0].SetPosition(a*TileSize-Sight,-Sight);
                App.Draw(UIShapes[0]);
            }
            // Draw HighScore
            App.Draw(HighScore);
            // Draw Score
            App.Draw(ScoreTxt);
        }
        // Display Scene
        App.Display();
    }
    Config.SaveSettings("Settings.yml");
    return 0;
}

Button* ButtonStyle(Button* B,sf::Color Base){
    sf::Color Tints[3]={Base/2,Base*2,Base};
    sf::Shape* Rects[3]={&B->Pressed,&B->Hovering,&B->Normal};
    for(unsigned int j=0u;j<3u;j++){
        unsigned int i;
        for(i=0u;i<4u;i++)Rects[j]->SetPointColor(i,Tints[j]);
        Tints[j].a=128;
        for(i=0u;i<4u;i++)Rects[j]->SetPointOutlineColor(i,Tints[j]);
        Tints[j].a=255;
        Rects[j]->SetOutlineWidth(1.f);
    }
    B->Text.SetColor(sf::Color(128,128,128));
    return B;
}

void ShowMenu(Menu* M){
    for(Menu* Iter:Menus){
        Iter->Visible=Iter==M;
    }
}

void HideMenus(){
    for(Menu* Iter:Menus){
        Iter->Visible=false;
    }
}

void AddKeyBinding(string KeyName,int Side,Menu* MenuToAdd,sf::Vector2f Size){
    if(Config.Controls.count(KeyName)==0)return;
    sf::Key::Code Key=Config.Controls[KeyName].first;
    //Button* B=ButtonStyle(Config.Controls[KeyName].second=new Button(sf::Vector2f(0,0),Size,sf::String(KeyName+": "+Keys.Find(Key),Font),[KeyName]{BindKey=KeyName;}),sf::Color(0,0,255,128));
    MenuToAdd->AddButton(ButtonStyle(Config.Controls[KeyName].second=new Button(sf::Vector2f(0,0),Size,sf::String(KeyName+": "+Keys.Find(Key),Font),[KeyName]{BindKey=KeyName;}),sf::Color(0,0,255,128)),Side);
}
