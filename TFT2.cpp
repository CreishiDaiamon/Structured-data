#include <iostream>
#include<string>
#include <stdlib.h>
#include <iomanip>
struct champ{
        //Nombre
        std::string nombre;
        //Vida
        int hp[4];
        //Defense
        int def[4];
        //Damage
        int dam[4];
    };
//Estadisticas de las habilidades
    struct skills{
        //Pasiva
        int p_dam[4];
        int p_cd[4];
        int p_cc[4];
        //Q
        int q_dam[4];
        int q_cd[4];
        bool q_airbone;
        //W
        int w_dam[4];
        int w_cd[4];
        bool w_airbone;
        int w_cc[4];
        //E
        int e_dam[4];
        int e_cd[4];
        int e_heal[4];
        int e_dur;
        int e_shield[4];
        //R
        int r_dam[3];
        int r_dur;
        int r_heal[3];
    };
//Estadisticas de los enemigos
    struct enemy{
        std::string enemy_name;
        int hp;
        int dam;
        //cc
        bool status_cc;
        //skills
        int p_dam,
            p_cd,
            q_dam,
            q_cd,
            w_dam,
            w_cd,
            e_dam,
            e_cd,
            r_dam,
            r_cd;
    };
//Definiendo estructura Champ
    champ champions[3]{
        {"Aatrox",{713,936,1370,2110},{39,44,55,74},{68,80,104,145}},
        {"Annie",{654,871,1297,2020},{28,34,45,63},{54,61,73,95}},
        {"Jhin",{723,956,1402,2166},{30,35,46,61},{66,78,100,139}}
    };
//Definiendo estructura Skills
    skills champskills[3]{
        //Aatrox
        {{6,7,9,12},{7,6,4,3},{0,0,0,0},//P(dam;cd;stun)
        {10,50,70,90},{4,3,2,1},1,//Q(dam;cd;airbone)
        {30,40,50,70},{8,7,5,4},1,{0,0,0,0},//W(dam;cd;airbone;root)
        {0,0,0,0},{5,4,3,2},{20,23,27,30},0,{0,0,0,0},//E(dam;cd;heal;dur;shield)
        {20,30,40},5,{50,75,100}},//R(dam;dur;heal)
        //Annie
        {{0,0,0,0},{0,0,0,0},{1,1,2,2},//P(dam;cd;stun)
        {80,160,205,250},{2,2,2,2},0,//Q(dam;cd;airbone)
        {70,115,205,250},{2,2,2,2},0,{0,0,0,0},//W(dam;cd;airbone;root)
        {20,30,50,60},{7,6,5,4},{0,0,0,0},3,{40,90,205,250},//E(dam;cd;heal;dur;shield)
        {150,275,400},0,{0,0,0}},//R(dam;dur;heal)
        //Jhin
        {{15,20,20,25},{1,1,1,1},{0,0,0,0},//P(dam;cd;stun)
        {40,95,120,145},{6,5,4,3},0,//Q(dam;cd;airbone)
        {20,85,155,190},{7,7,7,7},0,{1,1,2,2},//W(dam;cd;airbone;root)
        {20,80,200,260},{4,4,4,4},{0,0,0,0},1,{0,0,0,0},//E(dam;cd;heal;dur;shield)
        {50,125,200},3,{0,0,0}},//R(dam;dur;heal)
    };
//Definiendo enemigos
    enemy enemies[2]{
        {"Minion",1036,44},
        {"Gromp",2000,60},
    };
//prototipos de funciones
int Selec();
void MsjSel(champ&, int, skills&);
void Rondas(champ&, skills&, enemy&, int);
int FightWindow(champ&, skills&, enemy&, int);
int Attack(champ&, skills&, enemy&, int);
int Aatrox();
int Annie(champ&, skills&, enemy&, int);
int Jhin();

//Variable globales
int pcd=0 ,qcd=0 ,wcd=0 ,ecd=0 , rcd=0;
int champion;
char habilidad;
//función main
int main (){
    int num,//num para elegir personaje
    ronda=0,//indica la ronda
    lvl=0;//indica el nivel de habilidad
    std::cout<<"RPG DE LOL\n";
    num= Selec()-1;
    champion=num+1;
    std::system ("cls");
    champ playerc = champions[num];//se definen las estadisiticas base el campeón
    skills players = champskills[num];//se definen las skills del campeón
        enemy myenemy = enemies[ronda];//se define por primera vez al enemigo a enfrentar
        MsjSel(playerc , lvl, players);
        Rondas(playerc, players, myenemy, lvl);
}

int Selec (){
    int champ;
    std::cout<<"Seleccione el campeon que desea"<<std::endl;
    std::cout<<"Aatrox[1]\tAnnie[2]\tJhin[3]"<<std::endl;
    std::cin>>champ;
    while (champ !=1 && champ !=2 && champ !=3){
        std::cout<<"No seas maje, volve a intentar.";
        std::cin>>champ;
    }
    return champ;
}
void MsjSel(champ &p,int l, skills &s){
    std::cout<<"Usted ha seleccionado a "<<p.nombre<<std::endl;
    //estadisticas
    std::cout<<std::left<<std::setw(8)<<"\nVida"<<std::left<<
    std::setw(8)<<"Defensa"<<std::left<<std::setw(8)<<"Damage"<<std::endl;
    std::cout<<std::left<<std::setw(8)<<p.hp[l]<<std::left<<
    std::setw(8)<<p.def[l]<<std::left<<std::setw(8)<<p.dam[l]<<std::endl;
    //SKills
    std::cout<<"\nHabilidad\t"<<std::left<<std::setw(5)<<"Damage\t"<<std::left<<
    std::setw(5)<<"CD"<<std::left<<std::setw(5)<<"\tCC"<<std::setw(5)<<"\t"<<
    std::setw(5)<<"Shield"<<std::endl;
    std::cout<<"Pasiva\t\t"<<s.p_dam[l]<<"\t"<<s.p_cd[l]<<"\t"<<s.p_cc[l]<<std::endl;
    std::cout<<"Q\t\t"<<s.q_dam[l]<<"\t"<<s.q_cd[l]<<"\t"<<"-"<<std::endl;
    std::cout<<"W\t\t"<<s.w_dam[l]<<"\t"<<s.w_cd[l]<<"\t"<<s.w_cc[l]<<"\t\t-"<<std::endl;
    std::cout<<"E\t\t"<<s.e_dam[l]<<"\t"<<s.e_cd[l]<<"\t"<<"-"<<
    "\t\t"<<s.e_shield[l]<<std::endl;
    std::cout<<"R\t\t"<<s.r_dam[l]<<"\t"<<"xd"<<"\t"<<"-"<<"\t\t-"<<std::endl;
}
void Rondas (champ &p, skills &s, enemy &e, int l){
    int turno=1, ronda = 1;
    bool stun;


    //std::cout<<"Ronda "<<ronda<<std::endl;
    std::cout<<"El "<<e.enemy_name <<" ha aparecido!"<<std::endl;
    while (e.hp >0 && p.hp>0)//esta secuencia de instrucciones se repetirá hasta que uno muera
    {
        std::cout<<"Ronda "<<ronda<<std::endl;
        std::cout<<"Turno "<<turno<<std::endl;
        //Stats del enemigo
        FightWindow(p, s, e, l);
        //Skills enemigo
        //std::cout<<std::left<<std::setw(8)<<"Vida"<<std::left<<std::setw(8)<<"Damage"<<std::endl;
        //std::cout<<std::left<<std::setw(8)<<e.hp<<std::left<<std::setw(8)<<e.dam<<std::endl;
        std::cout<<"Que habilidad desea usar?(Q,w,E,R)"<<std::endl;
        std::cin>>habilidad;
        
        Attack(p, s, e, l);

        std::cout<<"\nHP enemigo: "<<e.hp<<"/"<<enemies->hp<<std::endl;
        turno++;

        std::cin.ignore();
        //std::system("cls");
    }
}

int FightWindow(champ &p, skills &s, enemy &e, int l){
    std::cout<<"Nombre del enemigo: "<<e.enemy_name<<std::endl;
    std::cout<<std::left<<std::setw(8)<<"Vida"<<std::left<<std::setw(8)<<"Damage"<<std::endl;
    std::cout<<std::left<<std::setw(8)<<e.hp<<"/"<<enemies->hp<<std::left<<std::setw(8)<<e.dam<<std::endl;
    //SkillWindow
    std::cout<<"\nHabilidad\t"<<std::left<<std::setw(5)<<"Damage\t"<<std::left<<
    std::setw(5)<<"CD"<<std::left<<std::setw(5)<<"\tCC"<<std::setw(5)<<"\t"<<
    std::setw(5)<<"Shield"<<std::endl;
    std::cout<<"Pasiva\t\t"<<s.p_dam[l]<<"\t"<<pcd<<"\t"<<s.p_cc[l]<<std::endl;
    std::cout<<"Q\t\t"<<s.q_dam[l]<<"\t"<<qcd<<"\t"<<"-"<<std::endl;
    std::cout<<"W\t\t"<<s.w_dam[l]<<"\t"<<wcd<<"\t"<<s.w_cc[l]<<"\t\t-"<<std::endl;
    std::cout<<"E\t\t"<<s.e_dam[l]<<"\t"<<ecd<<"\t"<<"-"<<
    "\t\t"<<s.e_shield[l]<<std::endl;
    std::cout<<"R\t\t"<<s.r_dam[l]<<"\t"<<"xd"<<"\t"<<"-"<<"\t\t-"<<std::endl;
    return 0;
}

int Attack(champ &p, skills &s, enemy &e, int l){
    //std::cout<<"XDDDDDDDDDDDDDDDDDDDD";
    switch(champion){
        case 1:
            Aatrox();
            break;
        case 2:
            Annie(p, s, e, l);
            break;
        case 3:
            Jhin();
            break;//¿Por que no servía con return 0? 
    }
    return 0;
}

int Aatrox(){

    return 0;
}

int Annie(champ &p, skills &s, enemy &e, int l){
    //Pasiva
    int cont=0;
    if(cont=4){
        e.status_cc=1;
        cont=0;
    }
    switch(habilidad){
//Usando q
        case 'q':
            std::cout<<"Has utilizado 'Desintegrar'. Haces "<<s.q_dam[l]<<" de damage.";
            e.hp-=s.q_dam[l];
            qcd=s.q_cd[l];
            //std::cout<<"XDDDDDDDDDDDDDDDDDDDDDDDDDDDDD"<<e.hp<<"   "<<s.q_dam[l];
            std::cin.get();
            break;
//Usando w
        case 'w':
            std::cout<<"Has utilizado 'Incinerar'. Haces "<<s.w_dam[l]<<" de damage.";
            e.hp=e.hp-s.w_dam[l];
            wcd=s.w_cd[l];
            break;
//usando e
        case 'e':
            std::cout<<"Has utilizado 'Escudo Fundido'. Haces "<<s.e_dam[l]<<" de damage.";
            e.hp=e.hp-s.e_dam[l];
            ecd=s.e_cd[l];
            break;

        case 'r':
            std::cout<<"Has utilizado 'Tibers'. Haces "<<s.r_dam[l]<<" de damage. ";
            e.hp=e.hp-s.r_dam[l];
            
            break;
//en caso de no tener cd
        default:
        break;
    }
    std::cout<<"Das un ataque basico. Haces "<<p.dam[l]<<" de damage.";
    e.hp-=p.dam[l];
    cont++;
    return 0;
}

int Jhin(){

return 0;
}