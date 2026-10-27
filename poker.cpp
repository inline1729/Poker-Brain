#include <deque>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <random>
#include <string>
#include <algorithm>
#include <fstream>
#include <thread>
#include <chrono>
using namespace std;
string flyonehand;
string flytwohand;
string flythreehand;
string hand;
vector<int> p1rank;
vector<int> p2rank;
vector<int> p3rank;
vector<int> p4rank;
string action;
int bb=50;
int sb=25;
int c=0;
int meet=bb;
int pot=0;
string river;
string replay="y";
string winner="you<3";
vector<int> money={1500,1500,1500,1500};
vector<int> bet={0,0,0,0};
vector<string> players={ "player","flyone","flytwo","flythree"};
vector <int> betin={1,1,1,1};
string blind=players[c];
string sblind=players[c+1];
deque<string> shuffledeck(){
     deque<string> deck;
    const string cardValue[] = {
        "2", "3", "4", "5", "6", "7",
        "8", "9", "T", "J", "Q", "K", "A"
    };
    const string cardSuit[] = {"H", "D", "S", "C"};

    for (const string& suit : cardSuit) {
        for (const string& value : cardValue) {
            deck.push_back(value + suit);
        }
    }

    static mt19937 rng(random_device{}());
    shuffle(deck.begin(), deck.end(), rng);

    return deck;
}
vector<int> hands(string player,int bet,string hand,int betin){
    bool sm=false;
    int spade=0;
    int diamond=0;
    int hearts=0;
    int club=0;
    string card1=hand.substr(0,2);
    string card2=hand.substr(2,2);
    string card3=hand.substr(4,2);
    string card4=hand.substr(6,2);
    string card5=hand.substr(8,2);
    string card6=hand.substr(10,2);
    string card7=hand.substr(12,2);
    vector <string> cards={card1,card2,card3,card4,card5,card6,card7};
    vector<int> numbers={0,0,0,0,0,0,0,0,0,0,0,0,0};
    vector<int> rank(8, 0);
    if (betin==-1){
        cout<<player<<"folded"<< '\n';
        rank[0]=-1;
        return rank;
    }
        //check suits
    for (char& c: hand){
            if (c=='S'){
                spade+=1;
            }else if(c=='D'){
                diamond+=1;
            }else if(c=='H'){
                hearts+=1;
            }else if(c=='C'){
                club+=1;
            }
            if(c=='2'){
                numbers[0]+=1;
            }else if (c=='3'){
                numbers[1]+=1;
            }else if(c=='4'){
                numbers[2]+=1;
            }else if(c=='5'){
                numbers[3]+=1;
            }else if(c=='6'){
                numbers[4]+=1;
            }else if(c=='7'){
                numbers[5]+=1;
            }else if(c=='8'){
                numbers[6]+=1;
            }else if(c=='9'){
                numbers[7]+=1;
            }else if(c=='T'){
                numbers[8]+=1;
            }else if(c=='J'){
                numbers[9]+=1;
            }else if(c=='Q'){
                numbers[10]+=1;
            }else if(c=='K'){
                numbers[11]+=1;
            }else if(c=='A'){
                numbers[12]+=1;
            }
    }
    //if suites match
    while(betin>=0){
    if (spade>=5 ||diamond>=5|| hearts>=5 ||club>=5){
        //find find striaght or royal flush
        if (numbers[12]>0 && numbers[11]>0 && numbers[10]>0 && numbers[9]>0 && numbers[8]>0){
            hand="royal flush";
            rank[0]=1;
            break;
        }
        for(int i=0;i<=7;i++){
            if (numbers[i+4]>0 && numbers[i+3]>0 && numbers[i+2]>0 && numbers[i+1]>0 && numbers[i]>0){
            hand="straight flush";
            rank[0]=2;
            int temp=1;
            for(int i=12;i>=0;i--){
                if (numbers[i]>0){
                    rank[temp]=i;
                    temp++;
                }
            }
            break;
        }
        }
        sm=true;
    }
        if((find(numbers.begin(), numbers.end(), 4))!=numbers.end()){
       hand="four of a kind";
       rank[0]=3;
       int temp=1;
            for(int i=12;i>=0;i--){
                if (numbers[i]==4){
                    rank[temp]=i;
                    temp++;
                }else{
                    temp++;
                    rank[temp]=i;
                    temp--;
                }
            }
    }else if(((find(numbers.begin(), numbers.end(), 3))!=numbers.end())&&((find(numbers.begin(), numbers.end(), 2))!=numbers.end())){
        hand="full house";
         int temp=1;
            for(int i=12;i>=0;i--){
                if (numbers[i]==3){
                    rank[temp]=i;
                    temp++;
                }else if(numbers[i]==2){
                    rank[temp]=i;
                    temp++;
                }else{
                    temp++;
                    rank[temp]=i;
                    temp--;
                }
            }
       rank[0]=4;
       break;
    }else if(sm==true){
        hand="flush";
       rank[0]=5;
       int temp=1;
            for(int i=12;i>=0;i--){
                if (numbers[i]==2){
                    rank[temp]=i;
                    temp++;
                }else{
                    temp++;
                    rank[temp]=i;
                    temp--;
                }
            }
       break;
    }
    for(int i=0;i<=8;i++){
            if (numbers[i+4]>0 && numbers[i+3]>0 && numbers[i+2]>0 && numbers[i+1]>0 && numbers[i]>0){
            hand="straight";
            rank[0]=6;
             int temp=1;
            for(int i=12;i>=0;i--){
                if (numbers[i]>0){
                    rank[temp]=i;
                    temp++;
                }
            }
            break;
        }
    }
    if(((find(numbers.begin(), numbers.end(), 3))!=numbers.end())){
        hand="three of a kind";
        rank[0]=7;
         int temp=1;
            for(int i=12;i>=0;i--){
                if (numbers[i]==3){
                    rank[temp]=i;
                    temp++;
                }else{
                    temp++;
                    rank[temp]=i;
                    temp--;
                }
            }
        break;
    }else if(((find(numbers.begin(), numbers.end(), 2))!=numbers.end())&& (find((find(numbers.begin(), numbers.end(), 2))+1, numbers.end(), 2))!=numbers.end()){
        hand="two pair";
        rank[0]=8;
         int temp=1;
            for(int i=12;i>=0;i--){
                if (numbers[i]==2){
                    rank[temp]=i;
                    temp++;
                }else{
                    temp++;
                    rank[temp]=i;
                    temp--;
                }
            }
        break;
    }else if(((find(numbers.begin(), numbers.end(), 2))!=numbers.end())){
       hand="pair";
        rank[0]=9;
         int temp=1;
            for(int i=12;i>=0;i--){
                if (numbers[i]==2){
                    rank[temp]=i;
                    temp++;
                }else{
                    temp++;
                    rank[temp]=i;
                    temp--;
                }
            }
        break;
    }else{
        hand="high card";
        rank[0]=10;
        int temp=1;
            for(int i=12;i>=0;i--){
            if (temp < static_cast<int>(rank.size())) {
                rank[temp] = i;
                temp++;
            }
                }
            }
        break;
   
}
    cout<<player<<"had a"<<hand<<"\n";

    return rank;
//print
}
string askBot(const string& hand, int meet, int money) {
    const char* home = getenv("USERPROFILE");
    string path = string(home ? home : ".") + "\\text.txt";   // same file as Python

    string cards = hand.substr(0,2) + " " + hand.substr(2,2) + ", ";
    for (size_t i = 4; i + 1 < hand.size(); i += 2) {
        cards += hand.substr(i,2);
        if (i + 2 < hand.size()) cards += " ";
    }

    {
        ofstream out(path, ios::trunc);
        out << cards << ", " << meet << ", " << money;
    }

    for (int i = 0; i < 600; i++) {                 // wait up to 30 seconds
        this_thread::sleep_for(chrono::milliseconds(50));
        ifstream in(path);
        stringstream ss;
        ss << in.rdbuf();
        string text = ss.str();
        if (!text.empty() && text.back() == '\n' &&
            (text.rfind("FOLD", 0) == 0 || text.rfind("CALL", 0) == 0 || text.rfind("RAISE", 0) == 0)) {
            return text;
        }
    }
    cerr << "Bot timed out. Is pokerbot.py running?\n";
    return "";
}
void betting(int& flymoneys, int& flybet, string player,int& betin,string hand) {
    while (betin==1) {
        //bot betting
        if (player!="player"){
            if (player != "player") {
    int meet = *max_element(bet.begin(), bet.end());
    string action = askBot(hand, meet, flymoneys);

    if (action.rfind("FOLD", 0) == 0) {
        flybet = -1; betin = -1; return;
    }
    if (action.rfind("CALL", 0) == 0) {
        int callAmount = max(0, meet - flybet);
        int paid = min(callAmount, flymoneys);
        flymoneys -= paid; flybet += paid; pot += paid;
        return;
    }
    if (action.rfind("RAISE", 0) == 0) {
        auto index = action.find_first_of("0123456789");
        if (index != string::npos) {
            int target = stoi(action.substr(index));
            int additional = min(target - flybet, flymoneys);
            flymoneys -= additional; pot += additional; flybet += additional;
            return;
        }
    }
    // unrecognized reply: fold so the game doesn't hang
    cerr << "Bad bot reply: " << action << "\n";
    flybet = -1; betin = -1; return;
}
    }else{
    int meet = *max_element(bet.begin(), bet.end());
        string action;

        cout << player << " enter RAISE, CALL or FOLD. "
             << "Current BET: " << meet << ": ";

        cin >> action;

        if (action == "FOLD") {
            flybet = -1;
            betin=-1;
            return;
        }

        if (action == "CALL") {
            if (flybet == -1) return;

            int callAmount = meet - flybet;

            if (callAmount < 0) callAmount = 0;

            int paid = min(callAmount, flymoneys);
            flymoneys -= paid;
            flybet += paid;
            pot += paid;
            return;
        }

        if (action == "RAISE") {
            int target;
            cout << "Enter total bet amount (greater than "
                 << meet << "): ";
            cin >> target;

            if (target <= meet || target <= flybet) {
                cout << "Bet must exceed the current bet.\n";
                continue;
            }

            int additional = target - flybet;

            if (additional > flymoneys) {
                cout << "Not enough money. Try again.\n";
                continue;
            }

            flymoneys -= additional;
            pot += additional;
            flybet = target;
            return;
        }

        if (action == "allin") {
            int additional = flymoneys;
            flymoneys = 0;
            flybet += additional;
            pot += additional;
            betin=0;
            return;
        }

        cout << "Invalid action.  RAISE, STAND or FOLD. .\n";
    }
}
}

string playhand(deque<string>deck,vector<string>& players){
//deals hands
cout<<"dealing hands..."<< '\n';
flyonehand=deck[0]+deck[1];
deck.pop_front();
deck.pop_front();
flytwohand=deck[0]+deck[1];
deck.pop_front();
deck.pop_front();
flythreehand=deck[0]+deck[1];
deck.pop_front();
deck.pop_front();
hand=deck[0]+deck[1];
deck.pop_front();
deck.pop_front();
river=deck[0]+deck[1]+deck[2];
deck.pop_front();
deck.pop_front();
deck.pop_front();
cout<<"your hand is: "<<hand<< '\n';
//1st betting round
cout<<"Big Blind is: "<<blind<< '\n';
cout<<"the small blind is: "<<sblind<< '\n';
//bb money
int bbIndex = (c + 1) % 4;
int sbIndex = (c + 2) % 4;

money[bbIndex] -= bb;
bet[bbIndex] += bb;

money[sbIndex] -= sb;
bet[sbIndex] += sb;

pot += bb + sb; 
if (c==0){
    betting(money[c],bet[c],players[c],betin[c],hand);
    betting(money[c+1],bet[c+1],players[c+1],betin[c+1],flyonehand);
    betting(money[c+2],bet[c+2],players[c+2],betin[c+2],flytwohand);
    betting(money[c+3],bet[c+3],players[c+3],betin[c+3],flythreehand);
    }
    else if(c==1){
    betting(money[c],bet[c],players[c],betin[c],flyonehand);
    betting(money[c+1],bet[c+1],players[c+1],betin[c+1],flytwohand);
    betting(money[c+2],bet[c+2],players[c+2],betin[c+2],flythreehand);
    betting(money[c-1],bet[c-1],players[c-1],betin[c-1],hand);
    }
    else if(c==2){
    betting(money[c],bet[c],players[c],betin[c],flytwohand);
    betting(money[c+1],bet[c+1],players[c+1],betin[c+1],flythreehand);
    betting(money[c-2],bet[c-2],players[c-2],betin[c-2],hand);
    betting(money[c-1],bet[c-1],players[c-1],betin[c-1],flyonehand);
    }
    else if (c==3){
    betting(money[c],bet[c],players[c],betin[c],flythreehand);
    betting(money[c-1],bet[c-1],players[c-1],betin[c-1],flytwohand);
    betting(money[c-2],bet[c-2],players[c-2],betin[c-2],flyonehand);
    betting(money[c-3],bet[c-3],players[c-3],betin[c-3],hand);
    }
//flop
bet={0,0,0,0};
cout<<"the flop is: "<<river<< '\n';
//2nd betting round
if (c==0){
    betting(money[c],bet[c],players[c],betin[c],hand+river);
    betting(money[c+1],bet[c+1],players[c+1],betin[c+1],flyonehand+river);
    betting(money[c+2],bet[c+2],players[c+2],betin[c+2],flytwohand+river);
    betting(money[c+3],bet[c+3],players[c+3],betin[c+3],flythreehand+river);
    }
    else if(c==1){
    betting(money[c],bet[c],players[c],betin[c],flyonehand+river);
    betting(money[c+1],bet[c+1],players[c+1],betin[c+1],flytwohand+river);
    betting(money[c+2],bet[c+2],players[c+2],betin[c+2],flythreehand+river);
    betting(money[c-1],bet[c-1],players[c-1],betin[c-1],hand+river);
    }
    else if(c==2){
    betting(money[c],bet[c],players[c],betin[c],flytwohand+river);
    betting(money[c+1],bet[c+1],players[c+1],betin[c+1],flythreehand+river);
    betting(money[c-2],bet[c-2],players[c-2],betin[c-2],hand+river);
    betting(money[c-1],bet[c-1],players[c-1],betin[c-1],flyonehand+river);
    }
    else if (c==3){
    betting(money[c],bet[c],players[c],betin[c],flythreehand+river);
    betting(money[c-1],bet[c-1],players[c-1],betin[c-1],flytwohand+river);
    betting(money[c-2],bet[c-2],players[c-2],betin[c-2],flyonehand+river);
    betting(money[c-3],bet[c-3],players[c-3],betin[c-3],hand+river);
    }
//turn
    bet={0,0,0,0};
river+=deck[0];
deck.pop_front();
cout<<"the turn is: "<<river<< '\n';
//3rd betting round
if (c==0){
    betting(money[c],bet[c],players[c],betin[c],hand+river);
    betting(money[c+1],bet[c+1],players[c+1],betin[c+1],flyonehand+river);
    betting(money[c+2],bet[c+2],players[c+2],betin[c+2],flytwohand+river);
    betting(money[c+3],bet[c+3],players[c+3],betin[c+3],flythreehand+river);
    }
    else if(c==1){
    betting(money[c],bet[c],players[c],betin[c],flyonehand+river);
    betting(money[c+1],bet[c+1],players[c+1],betin[c+1],flytwohand+river);
    betting(money[c+2],bet[c+2],players[c+2],betin[c+2],flythreehand+river);
    betting(money[c-1],bet[c-1],players[c-1],betin[c-1],hand+river);
    }
    else if(c==2){
    betting(money[c],bet[c],players[c],betin[c],flytwohand+river);
    betting(money[c+1],bet[c+1],players[c+1],betin[c+1],flythreehand+river);
    betting(money[c-2],bet[c-2],players[c-2],betin[c-2],hand+river);
    betting(money[c-1],bet[c-1],players[c-1],betin[c-1],flyonehand+river);
    }
    else if (c==3){
    betting(money[c],bet[c],players[c],betin[c],flythreehand+river);
    betting(money[c-1],bet[c-1],players[c-1],betin[c-1],flytwohand+river);
    betting(money[c-2],bet[c-2],players[c-2],betin[c-2],flyonehand+river);
    betting(money[c-3],bet[c-3],players[c-3],betin[c-3],hand+river);
    }
//river
bet={0,0,0,0};

river+=deck[0];
deck.pop_front();
cout<<"the river is: "<<river<< '\n';
//last betting round
cout<<"final betting round!"<< '\n';
if (c==0){
    betting(money[c],bet[c],players[c],betin[c],hand+river);
    betting(money[c+1],bet[c+1],players[c+1],betin[c+1],flyonehand+river);
    betting(money[c+2],bet[c+2],players[c+2],betin[c+2],flytwohand+river);
    betting(money[c+3],bet[c+3],players[c+3],betin[c+3],flythreehand+river);
    }
    else if(c==1){
    betting(money[c],bet[c],players[c],betin[c],flyonehand+river);
    betting(money[c+1],bet[c+1],players[c+1],betin[c+1],flytwohand+river);
    betting(money[c+2],bet[c+2],players[c+2],betin[c+2],flythreehand+river);
    betting(money[c-1],bet[c-1],players[c-1],betin[c-1],hand+river);
    }
    else if(c==2){
    betting(money[c],bet[c],players[c],betin[c],flytwohand+river);
    betting(money[c+1],bet[c+1],players[c+1],betin[c+1],flythreehand+river);
    betting(money[c-2],bet[c-2],players[c-2],betin[c-2],hand+river);
    betting(money[c-1],bet[c-1],players[c-1],betin[c-1],flyonehand+river);
    }
    else if (c==3){
    betting(money[c],bet[c],players[c],betin[c],flythreehand+river);
    betting(money[c-1],bet[c-1],players[c-1],betin[c-1],flytwohand+river);
    betting(money[c-2],bet[c-2],players[c-2],betin[c-2],flyonehand+river);
    betting(money[c-3],bet[c-3],players[c-3],betin[c-3],hand+river);
    }
    bet={0,0,0,0};
    hand+=river;
    flyonehand+=river;
    flytwohand+=river;
    flythreehand+=river;
    //see what hands people have
    p1rank=hands(players[0],bet[0],hand,betin[0]);
    p2rank=hands(players[1],bet[1],flyonehand,betin[1]);
    p3rank=hands(players[2],bet[2],flytwohand,betin[2]);
    p4rank=hands(players[3],bet[3],flythreehand,betin[3]);
    //winners
    vector<vector<int>> ranks = {
    p1rank, p2rank, p3rank, p4rank
};

int bestPlayer = -1;

for (int i = 0; i < 4; i++) {
    if (betin[i] == -1) continue; // Skip folded players

    if (bestPlayer == -1) {
        bestPlayer = i;
        continue;
    }

   for (int j = 0;
     j < min(ranks[i].size(), ranks[bestPlayer].size());
     j++) {

    if (ranks[i][j] == ranks[bestPlayer][j]) {
        continue;
    }

    if (j == 0) {
        // Hand category: lower is better
        if (ranks[i][j] < ranks[bestPlayer][j]) {
            bestPlayer = i;
        }
    } else {
        // Kicker: higher is better
        if (ranks[i][j] > ranks[bestPlayer][j]) {
            bestPlayer = i;
        }
    }

    break;
}
}
if (bestPlayer != -1) {
    money[bestPlayer] += pot;
    return players[bestPlayer];
}

return "No winner";
}
int main(){
srand(time(NULL));
while (replay=="y"){
//blinds
deque<string> deck =shuffledeck();
bet = {0, 0, 0, 0};
betin = {1,1,1,1};
pot = 0;
river = "";
winner=playhand(deck,players);
cout<<"winner is"<<winner<<'\n';
cout<<"they won"<<pot<<'\n';
blind=players[(c + 1) % 4];
sblind=players[(c + 2) % 4];
cout<<"do you want to play again y/n:";
cin>>replay;
if (replay=="n"){
    cout<<"thanks for playing, your take is "<<money[0]<< '\n';
    cout<<"the winner is "<<winner<< '\n';
    break;
}
else{
    c = (c + 1) % 4;
}
}
return(0);
}