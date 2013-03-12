#include "main.hpp"
const sf::Color HighLight(32,32,32);
maze Maze;
int GameTime=0;
pairi Adj[8]={pairi(-1,0),pairi(1,0),pairi(0,-1),pairi(0,1),pairi(1,1),pairi(-1,1),pairi(-1,-1),pairi(1,-1)};

int main(){
    srand(time(NULL));
    // Main Variables
    int X=0;
    int Y=0;
    int Tick=30;
    sf::Shape Squares[TileNum];
    sf::RenderWindow App({(int)(float(WinHeight)*(Width/Height)),WinHeight,32},"Zombie Squares");
    sf::View Cam(sf::FloatRect(0.f,0.f,SWidth,SHeight));
    sf::Event Events;
    sf::Color Tiles[TileNum]={sf::Color::Black,sf::Color::Blue,sf::Color(0,128,0),sf::Color(255,255,255,0),sf::Color(64,64,128)};
    sf::Font Font;
    sf::String ScoreTxt("Score: 0",Font,TileSize);
    sf::String Bullets(" Bullets: 3",Font,TileSize);
    sf::String HighScore(" HighScore: 0",Font,TileSize);
    Player Character(ScoreTxt,Bullets,HighScore);
    bool Draw=false;
    map<sf::Key::Code,pairi> MoveKeys={{sf::Key::W,pairi(0,-1)},{sf::Key::S,pairi(0,1)},{sf::Key::A,pairi(-1,0)},{sf::Key::D,pairi(1,0)}};
    map<sf::Key::Code,pairi> ShootKeys={{sf::Key::Up,pairi(0,-1)},{sf::Key::Down,pairi(0,1)},{sf::Key::Left,pairi(-1,0)},{sf::Key::Right,pairi(1,0)}};
    // Setup
    sf::IntRect StartArea(-3,-3,3,3);
    for(int a=StartArea.Left;a<=StartArea.Right;a++){ // Create Starting Area
        for(int b=StartArea.Top;b<=StartArea.Bottom;b++){
            Maze[pairi(a,b)]=a==StartArea.Left||a==StartArea.Right||b==StartArea.Top||b==StartArea.Bottom?4:0;
        }
    }
    App.SetView(Cam);
    App.SetFramerateLimit(30);
    for(unsigned int i=0u;i<TileNum;i++)Squares[i]=sf::Shape::Rectangle({1,1},{TileSize-2,TileSize-2},Tiles[i],1.f,Tiles[i]+HighLight);
    if(!Font.LoadFromFile("Font.ttf",(unsigned int)TileSize)){
        cout<<"Unable to load font \'Font.ttf\'!"<<endl;
        exit(11);
    }
    ((sf::Image&)Font.GetImage()).SetSmooth(false);
    // Game Loop
    while(App.IsOpened()){
        // Update Time
        Tick--;
        if(!Tick){
            Tick=30;
            GameTime++;
        }
        // Clear Old Scene;
        App.Clear();
        while(App.GetEvent(Events)){ // EVENTS
            switch(Events.Type){
                case sf::Event::Closed:{
                    App.Close();
                    break;
                }case sf::Event::KeyPressed:{
                    sf::Key::Code Key=Events.Key.Code;
                    if(MoveKeys.count(Key)){
                        Character.Move(MoveKeys[Key]);
                    }else if(ShootKeys.count(Key)){
                        Character.Shoot(ShootKeys[Key]);
                    }else{
                        switch(Key){
                            case sf::Key::Escape:{
                                App.Close();
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
        // Update View
        X=Character.X-CWidth;
        Y=Character.Y-CHeight;
        Cam.SetFromRect(sf::FloatRect(X*TileSize,Y*TileSize,X*TileSize+SWidth,Y*TileSize+SHeight));
        // Draw Terrain
        sf::IntRect Screen(X-1,Y-1,X+Width+1,Y+Height+1);
        for(int x=Screen.Left;x<Screen.Right;x++){
            for(int y=Screen.Top;y<Screen.Bottom;y++){
                pairi C(x,y);
                bool D=Draw;
                if(!Draw){
                    D=Character.InSight(C);
                }
                unsigned int Index=GetTile(Maze,x,y);
                if(D){
                    Squares[Index].SetPosition(x*TileSize,y*TileSize);
                    App.Draw(Squares[Index]);
                }
            }
        }
        set<Entity*> Remove;
        // Update Lazers
        for(Lazer& L:Lazer::Lazers){
            if(L.Color.a==0)Remove.insert(&L);
            else L.Update();
        }
        for(const Entity* E:Remove){
            for(unsigned int i=0u;i<Lazer::Lazers.size();i++){
                if(&Lazer::Lazers[i]==E){
                    Lazer::Lazers.erase(Lazer::Lazers.begin()+i);
                    break;
                }
            }
        }
        Remove.clear();
        // Update Enemies
        for(Enemy& E:Enemy::Enemies){
            char C=GetTile(Maze,E.X,E.Y);
            bool Keep=true;
            if(C==1)Keep=false;
            if(E.Life<=0){
                Keep=false;
                Character.Point(1);
            }
            if(E.X==Character.X&&E.Y==Character.Y){
                Keep=false;
                Character.Point(-E.Power);
            }
            if(Keep)E.Update();
            else Remove.insert(&E);
        }
        // Remove Enemies in invalid locations
        if(Remove.size()){
            for(const Entity* E:Remove){
                for(unsigned int i=0u;i<Enemy::Enemies.size();i++){
                    if(&Enemy::Enemies[i]==E){
                        Enemy::Enemies.erase(Enemy::Enemies.begin()+i);
                        break;
                    }
                }
            }
        }
        // Update Player
        Character.Update();
        // Draw Enemies
        for(Enemy& E:Enemy::Enemies){
            pairi L(E.X,E.Y);
            if(Draw||Character.InSight(L))E.Draw(App);
        }
        // Draw Lazers
        for(Lazer& E:Lazer::Lazers){
            pairi L(E.X,E.Y);
            if(Draw||Character.InSight(L))E.Draw(App);
        }
        // Draw Player
        Character.Draw(App);
        // Interface
        Cam.SetFromRect(sf::FloatRect(0,0,SWidth,SHeight));
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

void EvalMaze(maze& Tiles,pairi Tile,int Size,set<char> Blockers){
    int Step=1;
    Tiles.clear();
    set<pairi> Added={Tile},Old;
    Tiles[Tile]=0;
    pairi P[4]={pairi(-1,0),pairi(1,0),pairi(0,-1),pairi(0,1)};
    while(Step<Size&&Added.size()){
        Old=Added;
        Added.clear();
        for(const pairi& p:Old){
            for(unsigned int i=0u;i<4u;i++){
                pairi PP=P[i]+p;
                if(!Blockers.count(GetTile(Maze,PP.first,PP.second))){
                    if(Tiles.count(PP)){
                        if(Tiles[PP]>Step){
                            Tiles[PP]=Step;
                            Added.insert(PP);
                        }
                    }else{
                        Tiles[PP]=Step;
                        Added.insert(PP);
                    }
                }
            }
        }
        Step++;
    }
}

char MakeTile(maze& Tiles,int x,int y){
    char& Ret=(Tiles[pairi(x,y)]=0);
    bool X=(x%2==0);
    bool Y=(y%2==0);
    int R=rand()%10;
    if(R==0)Ret=2;
    else if(R<=1+GameTime/50)Ret=99;
    if(X&&Y){
        Ret=1;
        int rnd=rand()%2;
        if(((y%4)+x)%4==0){
            pairi Loc(x-1,y);
            if(rnd){
                Loc.first+=2;
            }
            char& NT=Tiles[Loc];
            if(NT!=4)NT=1;
        }else{
            pairi Loc(x,y-1);
            if(rnd){
                Loc.second+=2;
            }
            char& NT=Tiles[Loc];
            if(NT!=4)NT=1;
        }
    }
    if(Ret==99){
        Ret=0;
        Enemy::New(x,y,1+GameTime/50);
    }
    return Ret;
}

maze LoadMaze(string FileName){
    ifstream File(FileName.c_str());
    maze Ret;
    map<char,char> Cypher={{'X',1},{' ',0},{'*',5}};
    string Line;
    pairi Pt(0,0);
    while(File.good()){
        Pt.first=0;
        getline(File,Line);
        if(Line!="")for(char C:Line){
            Ret[Pt]=(Cypher.count(C)?Cypher[C]:C);
            Pt.first++;
        }
        Pt.second++;
    }
    File.close();
    return Ret;
}
