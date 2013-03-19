#include "main.hpp"
const sf::Color HighLight(32,32,32);
maze Maze;
int GameTime=0;
pairi Adj[8]={pairi(-1,0),pairi(1,0),pairi(0,-1),pairi(0,1),pairi(1,1),pairi(-1,1),pairi(-1,-1),pairi(1,-1)};
bool MenuMode=false;
float WinWidth=1024;
float WinHeight=768;

int main(){
    srand(time(NULL));
    // Main Variables
    int X=0;
    int Y=0;
    int Tick=30;
    sf::Shape Squares[TileNum];
    sf::RenderWindow App(sf::VideoMode(WinWidth,WinHeight),"Zombie Squares");
    sf::View Cam(sf::FloatRect(0.f,0.f,WinWidth,WinHeight));
    sf::Event Events;
    sf::Color Tiles[TileNum]={sf::Color::Black,sf::Color(0,0,128),sf::Color(0,128,0),sf::Color(64,64,128)};
    sf::Font Font;
    sf::String ScoreTxt("Score: 0",Font,TileSize);
    sf::String Bullets(" Bullets: 3",Font,TileSize);
    sf::String HighScore(" HighScore: 0",Font,TileSize);
    sf::PostFX MenuGray;
    new Player(ScoreTxt,Bullets,HighScore);
    map<sf::Key::Code,pairi> MoveKeys={{sf::Key::W,pairi(0,-1)},{sf::Key::S,pairi(0,1)},{sf::Key::A,pairi(-1,0)},{sf::Key::D,pairi(1,0)}};
    map<sf::Key::Code,pairi> ShootKeys={{sf::Key::Up,pairi(0,-1)},{sf::Key::Down,pairi(0,1)},{sf::Key::Left,pairi(-1,0)},{sf::Key::Right,pairi(1,0)}};
    // Setup
    { // Create Starting Area
        int Area=3;
        for(int a=-Area;a<=Area;a++)for(int b=-Area;b<=Area;b++)Maze[pairi(a,b)]=abs(a)==Area||abs(b)==Area?3:0;
    }
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
    {
        sf::Vector2f Size(WinWidth*0.5f,TileSize);
        sf::Vector2f Location(WinWidth*0.25f,WinHeight*0.25f);
        MenuPause=new Menu(Location,Size,0.f,sf::String("Paused",Font),sf::Shape::Rectangle({},{},sf::Color(0,0,0,64),1.f,sf::Color(0,0,255)));
        Size.x-=2.f;
        Size.y-=2.f;
        MenuPause->AddButton(ButtonStyle(new Button({},Size,sf::String("Resume Game",Font),[&]{MenuMode=false;}),sf::Color(0,0,255,128)),1,1);
        MenuPause->AddButton(ButtonStyle(new Button({},Size,sf::String("Save and Exit",Font),[&]{Save("SaveGame.zs");App.Close();}),sf::Color(0,0,255,128)),1);
    }
    // Game Loop
    if(FileExists("SaveGame.zs")){
        Load("SaveGame.zs");
        MenuMode=true;
    }
    while(App.IsOpened()){
        // Update Time
        if(!MenuMode){
            Tick--;
            if(!Tick){
                Tick=30;
                GameTime++;
            }
        }
        // Clear Old Scene;
        App.Clear();
        while(App.GetEvent(Events)){ // EVENTS
            if(MenuMode)UIGroup::GetAllInput(Events,App.GetInput());
            switch(Events.Type){
                case sf::Event::Closed:{
                    App.Close();
                    break;
                }case sf::Event::Resized:{
                    WinWidth=Events.Size.Width;
                    WinHeight=Events.Size.Height;
                    // Move Interfaces
                    MenuPause->Move(sf::Vector2f((WinWidth-MenuPause->Size.x)*0.5f,(WinHeight-MenuPause->GetHeight())*0.5f));
                    break;
                }case sf::Event::MouseWheelMoved:{
                    int Old=Player::SightRadius+Events.MouseWheel.Delta;
                    if(Old>=0)Player::SightRadius=Old;
                    break;
                }case sf::Event::KeyPressed:{
                    sf::Key::Code Key=Events.Key.Code;
                    if(!MenuMode&&MoveKeys.count(Key)){
                        Player::Character->Move(MoveKeys[Key]);
                    }else if(!MenuMode&&ShootKeys.count(Key)){
                        Player::Character->Shoot(ShootKeys[Key]);
                    }else{
                        switch(Key){
                            case sf::Key::Escape:{
                                MenuMode=!MenuMode;
                                break;
                            }default:{break;}
                        }
                    }
                    break;
                }default:{break;}
            }
        }// END EVENTS
        // Check Window
        if(!App.IsOpened())break;
        // Update Game View
        X=Player::Character->X;
        Y=Player::Character->Y;
        {
            float Ratio=WinHeight/WinWidth;
            float S=(Player::SightRadius+1)*TileSize;
            float W=S*(Ratio>1.f?1:1/Ratio);
            float H=S*(Ratio<1.f?1:Ratio);
            Cam.SetFromRect(sf::FloatRect(X*TileSize-W,Y*TileSize-H,(X+1)*TileSize+W,(Y+1)*TileSize+H));
        }
        // Draw Terrain
        int SR=Player::SightRadius+1;
        for(int x=X-SR;x<=X+SR;x++){
            for(int y=Y-SR;y<=Y+SR;y++){
                pairi Loc(x,y);
                if(Player::Character->InSight(Loc)){
                    unsigned int Index=GetTile(Maze,x,y);
                    sf::Shape& Sq=Squares[Index];
                    char Sight=Player::Character->GetSight(Loc);
                    float Alpha=255.f*(1.f-(float(Sight)/float(Player::SightRadius+1)));
                    if(Alpha>255.f)Alpha=255.f;
                    if(Alpha<0.f)Alpha=0.f;
                    Sq.SetColor(sf::Color(255,255,255,Alpha));
                    Sq.SetPosition(x*TileSize,y*TileSize);
                    App.Draw(Sq);
                }
            }
        }
        // Update and Draw Entities
        Entity::Tick(App);
        if(MenuMode)App.Draw(MenuGray);
        // Interface
        Cam.SetFromRect(sf::FloatRect(0,0,WinWidth,WinHeight));
        if(MenuMode)UIGroup::DrawAll(App);
        float W=ScoreTxt.GetRect().GetWidth();
        // Draw HighScore
        HighScore.SetPosition(W+Bullets.GetRect().GetWidth(),0);
        App.Draw(HighScore);
        // Draw Bullet Count
        Bullets.SetPosition(W,0);
        App.Draw(Bullets);
        // Draw Score
        App.Draw(ScoreTxt);
        // Display Scene
        App.Display();
    }
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
